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

#include <iomanip>      // setw, set::precision, ...
#include <cmath>

#include "musicXMLOptions.h"
#include "lilypondOptions.h"

#include "lpsr2LilypondTranslator.h"


using namespace std;

namespace MusicXML2
{

// useful shortcut macros
#define idtr indenter::gIndenter
#define tab  indenter::gIndenter.getSpacer ()

// for comments highlighting in the generated Lilypond code
const int commentFieldWidth = 30;

//________________________________________________________________________
lpsr2LilypondTranslator::lpsr2LilypondTranslator (
  S_msrOptions&         msrOpts,
  S_lpsrOptions&        lpsrOpts,
  indentedOutputStream& ios,
  S_lpsrScore           lpsrScore)
    : fIndentedOutputStream (ios)
{
  fMsrOptions  = msrOpts;
  fLpsrOptions = lpsrOpts;
  
  // the LPSR score we're visiting
  fVisitedLpsrScore = lpsrScore;

  // inhibit the browsing of measure repeats replicas,
  // since Lilypond only needs the repeat measure
  fVisitedLpsrScore->
    getMsrScore ()->
      setInhibitMeasureRepeatReplicasBrowsing ();

  // inhibit the browsing of multiple rest replicas,
  // since Lilypond only needs the measure number
  fVisitedLpsrScore->
    getMsrScore ()->
      setInhibitMultipleRestMeasuresBrowsing ();

  // header handling
  fOnGoingHeader = false;

  // time
  fVoiceIsCurrentlySenzaMisura = false;
  fOnGoingVoiceCadenza = false;
  
  // staves
  fOnGoingStaff = false;

  // voices
  fOnGoingVoice = false;

  // harmonies
  fOnGoingHarmonyVoice = false;
  fPowerChordHaveAlreadyBeenGenerated = false;

  // figured bass
  fOnGoingFiguredBassVoice = false;
  fCurrentFiguredBassFiguresCounter = 0;

  // repeats
  fCurrentRepeatEndingsNumber = 0;

  // measures
  fMeasuresCounter = 0;
  
  // durations
  fLastMetWholeNotes = rational (0, 1);

  // notes
  fOnGoingNote = false;

  // articulations
  fCurrentArpeggioDirection = k_NoDirection;

  // stems
  fCurrentStemKind = msrStem::k_NoStem;
//  fOnGoingStemNone = false; JMI

  // double tremolos
  fCurrentDoubleTremoloElementsLpsrDuration = -39;

  // chords
  fOnGoingChord = false; // JMI

  // stanzas
  fOngoingNonEmptyStanza = false;

  // score blocks
  fOnGoingScoreBlock = false;
};
  
lpsr2LilypondTranslator::~lpsr2LilypondTranslator ()
{}

//________________________________________________________________________
void lpsr2LilypondTranslator::generateLilypondCodeFromLpsrScore ()
{
  if (fVisitedLpsrScore) {    
    // browse a msrScore browser
    msrBrowser<lpsrScore> browser (this);
    browser.browse (*fVisitedLpsrScore);
  }
}

//________________________________________________________________________
string lpsr2LilypondTranslator::absoluteOctaveAsLilypondString (
  int absoluteOctave)
{
  string result;

  if (gGeneralOptions->fTraceNotes) {
    fIndentedOutputStream <<
      endl <<
      "%{ absoluteOctave = " << absoluteOctave << " %}" <<
      endl;
  }

  // generate LilyPond absolute octave
  switch (absoluteOctave) {
    case 0:
      result = ",,,";
      break;
    case 1:
      result = ",,";
      break;
    case 2:
      result = ",";
      break;
    case 3:
      result = "";
      break;
    case 4:
      result = "'";
      break;
    case 5:
      result = "''";
      break;
    case 6:
      result = "'''";
      break;
    case 7:
      result = "''''";
      break;
    case 8:
      result = "'''''";
      break;
    default:
      {
        stringstream s;

        s <<
          "%{" << absoluteOctave << "???%}";

        result = s.str ();
      }
  } // switch

  return result;
}

//________________________________________________________________________
string lpsr2LilypondTranslator::alterationAsLilypondString (
  msrAlteration alteration)
{
  string result;
  
  switch (alteration) {
    case kDoubleFlat:
      result = "DOUBLE-FLAT";
      break;
    case kSesquiFlat:
      result = "THREE-Q-FLAT";
      break;
    case kFlat:
      result = "FLAT";
      break;
    case kSemiFlat:
      result = "SEMI-FLAT";
      break;
    case kNatural:
      result = "NATURAL";
      break;
    case kSemiSharp:
      result = "SEMI-SHARP";
      break;
    case kSharp:
      result = "SHARP";
      break;
    case kSesquiSharp:
      result = "THREE-Q-SHARP";
      break;
    case kDoubleSharp:
      result = "DOUBLE-SHARP";
      break;
    case k_NoAlteration:
      result = "alteration???";
      break;
  } // switch

  return result;
}

//________________________________________________________________________
string lpsr2LilypondTranslator::alterationAsLilypondAccidentalMark (
  msrAlteration alteration)
{
  string result;
  
  switch (alteration) {
    case kDoubleFlat:
      result = " ^\\markup { \\doubleflat } ";
      break;
    case kSesquiFlat:
      result = " ^\\markup { \\sesquiflat } ";
      break;
    case kFlat:
      result = " ^\\markup { \\flat } ";
      break;
    case kSemiFlat:
      result = " ^\\markup { \\semiflat } ";
      break;
    case kNatural:
      result = " ^\\markup { \\natural } ";
      break;
    case kSemiSharp:
      result = " ^\\markup { \\semisharp } ";
      break;
    case kSharp:
      result = " ^\\markup { \\sharp } ";
      break;
    case kSesquiSharp:
      result = " ^\\markup { \\sesquisharp } ";
      break;
    case kDoubleSharp:
      result = " ^\\markup { \\doublesharp } ";
      break;
    case k_NoAlteration:
      result = " ^\\markup {%{k_NoAlteration???%} } ";
      break;
  } // switch

  return result;
}

//________________________________________________________________________
string lpsr2LilypondTranslator::lilypondRelativeOctave (
  S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();

  // generate LilyPond octave relative to fRelativeOctaveReference

  // in MusicXML, octave number is 4 for the octave starting with middle C
  int noteAbsoluteOctave =
    note->getNoteOctave ();

  msrDiatonicPitch
    noteDiatonicPitch =
      note->
        noteDiatonicPitch (
          inputLineNumber);

  msrDiatonicPitch
    referenceDiatonicPitch =
      fRelativeOctaveReference->
        noteDiatonicPitch (
          inputLineNumber);

  string
    referenceDiatonicPitchAsString =
      fRelativeOctaveReference->
        noteDiatonicPitchAsString (
          inputLineNumber);
      
  int
    referenceAbsoluteOctave =
      fRelativeOctaveReference->
        getNoteOctave ();

  /*
    If no octave changing mark is used on a pitch, its octave is calculated
    so that the interval with the previous note is less than a fifth.
    This interval is determined without considering accidentals.
  */
    
  int
    noteAboluteDiatonicOrdinal =
      noteAbsoluteOctave * 7
        +
      noteDiatonicPitch - kC,
      
    referenceAboluteDiatonicOrdinal =
      referenceAbsoluteOctave * 7
        +
      referenceDiatonicPitch - kC;

  if (gGeneralOptions->fTraceNotes) {
    const int fieldWidth = 28;

    fIndentedOutputStream << left <<
/*
      setw (fieldWidth) <<
      "% referenceDiatonicPitch" <<
      " = " <<
      referenceDiatonicPitch <<
      endl <<
*/
      setw (fieldWidth) <<
      "% referenceDiatonicPitchAsString" <<
      " = " <<
      referenceDiatonicPitchAsString <<
      endl <<
      setw (fieldWidth) <<
      "% referenceAbsoluteOctave" <<
       " = " <<
      referenceAbsoluteOctave <<
      endl <<
      endl <<
      setw (fieldWidth) <<
      "% referenceAboluteDiatonicOrdinal" <<
      " = " <<
      referenceAboluteDiatonicOrdinal <<
      endl <<
      setw (fieldWidth) <<
      "% noteAboluteDiatonicOrdinal" <<
      " = " <<
      noteAboluteDiatonicOrdinal <<
      endl <<
      endl;
  }

  stringstream s;
  
  // generate the octaves as needed
  if (noteAboluteDiatonicOrdinal >= referenceAboluteDiatonicOrdinal) {
    noteAboluteDiatonicOrdinal -= 4;
    while (noteAboluteDiatonicOrdinal >= referenceAboluteDiatonicOrdinal) {
      s << "'";
      noteAboluteDiatonicOrdinal -= 7;
    } // while
  }
  
  else {
    noteAboluteDiatonicOrdinal += 4;
    while (noteAboluteDiatonicOrdinal <= referenceAboluteDiatonicOrdinal) {
      s << ",";
      noteAboluteDiatonicOrdinal += 7;
    } // while
  }

  return s.str ();
}

void lpsr2LilypondTranslator::printNoteAsLilypondString ( // JMI
  S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();
    
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
          fIndentedOutputStream << "\\[ ";
          break;
        case msrLigature::kContinueLigature:
          break;
        case msrLigature::kStopLigature:
   // JMI       fIndentedOutputStream << "\\] ";
          break;
      } // switch
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
          fIndentedOutputStream <<
            endl <<
            "\\stemNeutral "; // JMI ""\\once\\omit Stem ";
        }

        // should stem direction be generated?
        if (gLilypondOptions->fStems) {
      
          if (fCurrentStem) {
            // should stem direction be generated?
            if (stemKind != fCurrentStemKind) {
              switch (stemKind) {
                case msrStem::k_NoStem:
                  fIndentedOutputStream << "\\stemNeutral ";
                  break;
                case msrStem::kStemUp:
                  fIndentedOutputStream << "\\stemUp ";
                  break;
                case msrStem::kStemDown:
                  fIndentedOutputStream << "\\stemDown ";
                  break;
                case msrStem::kStemNone:
                  break;
                case msrStem::kStemDouble: // JMI ???
                  break;
              } // switch
            }
      
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
          fIndentedOutputStream <<
            endl <<
            "\\stemNeutral"; // JMI ""\\once\\omit Stem ";
        }

        // should stem direction be generated?
        if (gLilypondOptions->fStems) {
      
          if (fCurrentStem) {
            // should stem direction be generated?
            if (stemKind != fCurrentStemKind) {
              switch (stemKind) {
                case msrStem::k_NoStem:
                  fIndentedOutputStream << "\\stemNeutral ";
                  break;
                case msrStem::kStemUp:
                  fIndentedOutputStream << "\\stemUp ";
                  break;
                case msrStem::kStemDown:
                  fIndentedOutputStream << "\\stemDown ";
                  break;
                case msrStem::kStemNone:
                  break;
                case msrStem::kStemDouble: // JMI ???
                  break;
              } // switch
            }
      
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
          fIndentedOutputStream <<
            pitchedRestAsLilypondString (note);
        }
  
        else {
          // print the rest name
          fIndentedOutputStream <<
            string (
              note->getNoteOccupiesAFullMeasure ()
                ? "R"
                : "r");
        }
          
        // print the rest duration
        fIndentedOutputStream <<
          durationAsLilypondString (
            inputLineNumber,
            note->
              getNoteSoundingWholeNotes ());
  
        // is the rest pitched?
        if (noteIsAPitchedRest) {
          fIndentedOutputStream <<
            " \\rest";

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
      fIndentedOutputStream << "s";
      
      // print the skip duration
      fIndentedOutputStream <<
        durationAsLilypondString (
          inputLineNumber,
          note->
            getNoteSoundingWholeNotes ());

      // a rest is no relative octave reference,
      // the preceding one is kept
      break;
      
    case msrNote::kStandaloneNote:
      // print the note name
      fIndentedOutputStream <<
        notePitchAsLilypondString (note);
      
      // print the note duration
      fIndentedOutputStream <<
        durationAsLilypondString (
          inputLineNumber,
          note->
            getNoteSoundingWholeNotes ());

      // handle delayed ornaments if any
      if (note->getNoteHasADelayedOrnament ())
        // c2*2/3 ( s2*1/3\turn JMI
        fIndentedOutputStream <<
          "*" <<
          gLilypondOptions->
            fDelayedOrnamentsFraction;
      
      // print the tie if any
      {
        S_msrTie noteTie = note->getNoteTie ();
      
        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kStartTie) {
            fIndentedOutputStream << " ~";
          }
        }
      }

      // this note is the new relative octave reference
      fRelativeOctaveReference = note;
      break;

    case msrNote::kDoubleTremoloMemberNote:
      // print the note name
      fIndentedOutputStream <<
        notePitchAsLilypondString (note);
      
      // print the note duration, i.e. the double tremolo elements duration
      fIndentedOutputStream <<
        fCurrentDoubleTremoloElementsLpsrDuration; // JMI

      // handle delayed ornaments if any
      if (note->getNoteHasADelayedOrnament ())
        // c2*2/3 ( s2*1/3\turn JMI
        fIndentedOutputStream <<
          "*" <<
          gLilypondOptions->fDelayedOrnamentsFraction;
      
      // print the tie if any
      {
        S_msrTie noteTie = note->getNoteTie ();
      
        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kStartTie) {
            fIndentedOutputStream << " ~";
          }
        }
      }

      // this note is the new relative octave reference
      fRelativeOctaveReference = note;
      break;

    case msrNote::kGraceNote:
      // print the note name
      fIndentedOutputStream <<
        notePitchAsLilypondString (note);
      
      // print the grace note's graphic duration
      fIndentedOutputStream <<
        msrDurationAsString (
          note->
            getNoteGraphicDuration ());

      // print the dots if any JMI ???
      for (int i = 0; i < note->getNoteDotsNumber (); i++) {
        fIndentedOutputStream << ".";
      } // for
      
      // print the tie if any
      {
        S_msrTie noteTie = note->getNoteTie ();
      
        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kStartTie) {
            fIndentedOutputStream << "~ ";
          }
        }
      }

      // this note is the new relative octave reference
      fRelativeOctaveReference = note;
      break;
      
    case msrNote::kChordMemberNote:
      // print the note name
      fIndentedOutputStream <<
        notePitchAsLilypondString (note);
      
      // don't print the note duration,
      // it will be printed for the chord itself

      // inside chords, a note is relative to the preceding one
      fRelativeOctaveReference = note;
      break;
      
    case msrNote::kTupletMemberNote:
      if (! gLilypondOptions->fTupletsOnALine) {
        fIndentedOutputStream <<
          endl;
      }
        
      // print the note name
      if (note->getNoteIsARest ()) {
        fIndentedOutputStream <<
          string (
            note->getNoteOccupiesAFullMeasure ()
              ? "R"
              : "r");
      }
      else {
        fIndentedOutputStream <<
          notePitchAsLilypondString (note);
      }
      
      // print the note (display) duration
      fIndentedOutputStream <<
        durationAsLilypondString (
          inputLineNumber,
          note->
            getNoteDisplayWholeNotes ());

      // print the tie if any
      {
        S_msrTie noteTie = note->getNoteTie ();
      
        if (noteTie) {
          if (noteTie->getTieKind () == msrTie::kStartTie) {
            fIndentedOutputStream << "~ ";
          }
        }
      }

      // a rest is no relative octave reference,
      if (! note->getNoteIsARest ())
        // this note is the new relative octave reference
        fRelativeOctaveReference = note;
      break;
  } // switch

  fIndentedOutputStream << " ";

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

//________________________________________________________________________
string lpsr2LilypondTranslator::notePitchAsLilypondString (
  S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();
    
  stringstream s;

  // is the note unpitched?
  if (note->getNoteIsUnpitched ())
    s <<
      "\\once \\override NoteHead #'style = #'cross ";

  // should an editorial accidental be generated?
  switch (note->getNoteEditorialAccidentalKind ()) {
    case msrNote::kNoteEditorialAccidentalYes:
      s <<
        "\\editorialAccidental ";
      break;
    case msrNote::kNoteEditorialAccidentalNo:
      break;
  } // switch
  
  // get the note quarter tones pitch
  msrQuarterTonesPitch
    noteQuarterTonesPitch =
      note->getNoteQuarterTonesPitch ();

  // fetch the quarter tones pitch as string
  string
    quarterTonesPitchAsString =
      msrQuarterTonesPitchAsString (
        gLpsrOptions->fLpsrQuarterTonesPitchesLanguage,
        noteQuarterTonesPitch);

  // get the note quarter tones display pitch
  msrQuarterTonesPitch
    noteQuarterTonesDisplayPitch =
      note->getNoteQuarterTonesDisplayPitch ();

  // fetch the quarter tones display pitch as string
  string
    quarterTonesDisplayPitchAsString =
      msrQuarterTonesPitchAsString (
        gLpsrOptions->fLpsrQuarterTonesPitchesLanguage,
        noteQuarterTonesDisplayPitch);
      
  // generate the pitch
  s <<
    quarterTonesPitchAsString;
    
  // in MusicXML, octave number is 4 for the octave starting with middle C
  int noteAbsoluteOctave =
    note->getNoteOctave ();
    
  int noteAbsoluteDisplayOctave =
    note->getNoteDisplayOctave ();

  // should an absolute octave be generated?
  bool generateAbsoluteOctave =
    gLilypondOptions->fAbsoluteOctaves
      ||
    ! fRelativeOctaveReference;

  if (gGeneralOptions->fTraceNotes) {
    const int fieldWidth = 28;

    fIndentedOutputStream << left <<
      endl <<
      setw (fieldWidth) <<
      "% line" <<
      " = " <<
      note->getInputLineNumber () <<
      endl <<
      setw (fieldWidth) <<
      "% msrQuarterTonesPitch" <<
      " = " <<
      quarterTonesPitchAsString <<
      endl <<
      setw (fieldWidth) <<
      "% quarterTonesDisplayPitch" <<
      " = " <<
      quarterTonesDisplayPitchAsString <<
      endl <<
      setw (fieldWidth) <<
      "% noteAbsoluteOctave" <<
      " = " <<
      noteAbsoluteOctave <<
      endl <<
      setw (fieldWidth) <<
      "% noteAbsoluteDisplayOctave" <<
      " = " <<
      noteAbsoluteDisplayOctave <<
      endl <<
    endl;
  }

  if (generateAbsoluteOctave) {
    // generate LilyPond absolute octave
    s <<
      absoluteOctaveAsLilypondString (
        noteAbsoluteOctave);
  }

  else {
    // generate LilyPond octave relative to fRelativeOctaveReference
    s <<
      lilypondRelativeOctave (note);
  }

  // should a cautionary accidental be generated?
  switch (note->getNoteCautionaryAccidentalKind ()) {
    case msrNote::kNoteCautionaryAccidentalYes:
      s <<
        "?";
      break;
    case msrNote::kNoteCautionaryAccidentalNo:
      break;
  } // switch

  return s.str ();
}

//________________________________________________________________________
string lpsr2LilypondTranslator::durationAsLilypondString (
  int      inputLineNumber,
  rational wholeNotes)
{
  string result;
  
  bool explicitDuration;
  
  if (wholeNotes != fLastMetWholeNotes) {
    explicitDuration = true;
    fLastMetWholeNotes = wholeNotes;
  }
  else {
    explicitDuration =
      gLilypondOptions->fAllDurations;
  }
  
  if (explicitDuration)
    result =
      wholeNotesAsLilypondString (
        inputLineNumber,
        wholeNotes);

  return result;
}

//________________________________________________________________________
string lpsr2LilypondTranslator::pitchedRestAsLilypondString (
  S_msrNote note)
{
  int inputLineNumber =
    note->getInputLineNumber ();
    
  stringstream s;

  // get the note quarter tones pitch
  msrQuarterTonesPitch
    noteQuarterTonesPitch =
      note->getNoteQuarterTonesPitch ();

  // fetch the quarter tones pitch as string
  string
    quarterTonesPitchAsString =
      msrQuarterTonesPitchAsString (
        gLpsrOptions->fLpsrQuarterTonesPitchesLanguage,
        noteQuarterTonesPitch);

  // get the note quarter tones display pitch
  msrQuarterTonesPitch
    noteQuarterTonesDisplayPitch =
      note->getNoteQuarterTonesDisplayPitch ();

  // fetch the quarter tones display pitch as string
  string
    quarterTonesDisplayPitchAsString =
      msrQuarterTonesPitchAsString (
        gLpsrOptions->fLpsrQuarterTonesPitchesLanguage,
        noteQuarterTonesDisplayPitch);
      
  // generate the display pitch
  s <<
    note->noteDisplayPitchAsString ();
//    note->notePitchAsString (); JMI
//    quarterTonesDisplayPitchAsString;
    
  // in MusicXML, octave number is 4 for the octave starting with middle C
  int noteAbsoluteOctave =
    note->getNoteOctave ();
    
  int noteAbsoluteDisplayOctave =
    note->getNoteDisplayOctave ();

  // should an absolute octave be generated?
  bool generateAbsoluteOctave =
    gLilypondOptions->fAbsoluteOctaves
      ||
    ! fRelativeOctaveReference;

  if (gGeneralOptions->fTraceNotes) {
    const int fieldWidth = 28;

    fIndentedOutputStream << left <<
      endl <<
      setw (fieldWidth) <<
      "% line" <<
      " = " <<
      note->getInputLineNumber () <<
      endl <<
      setw (fieldWidth) <<
      "% msrQuarterTonesPitch" <<
      " = " <<
      quarterTonesPitchAsString <<
      endl <<
      setw (fieldWidth) <<
      "% quarterTonesDisplayPitch" <<
      " = " <<
      quarterTonesDisplayPitchAsString <<
      endl <<
      setw (fieldWidth) <<
      "% noteAbsoluteOctave" <<
      " = " <<
      noteAbsoluteOctave <<
      endl <<
      setw (fieldWidth) <<
      "% noteAbsoluteDisplayOctave" <<
      " = " <<
      noteAbsoluteDisplayOctave <<
      endl <<
      endl;
  }

  if (generateAbsoluteOctave) {
    // generate LilyPond absolute octave
    s <<
      absoluteOctaveAsLilypondString (
        noteAbsoluteDisplayOctave);
  }

  else {
    // generate LilyPond octave relative to fRelativeOctaveReference
    s <<
      lilypondRelativeOctave (note);
  }

  return s.str ();
}

//________________________________________________________________________
string lpsr2LilypondTranslator::noteArticulationAsLilyponString (
  S_msrArticulation articulation)
{
  stringstream s;
  
  switch (articulation->getArticulationPlacement ()) {
    case k_NoPlacement:
      break;
    case kAbovePlacement:
      fIndentedOutputStream << "^";
      break;
    case kBelowPlacement:
      fIndentedOutputStream << "_";
      break;
  } // switch

  switch (articulation->getArticulationKind ()) {

    case msrArticulation::kAccent:
      s << ">";
      break;
    case msrArticulation::kBreathMark:
      s << "\\breathe";
      break;
    case msrArticulation::kCaesura:
    /* JMI
          fIndentedOutputStream <<
            endl <<
            R"(\once\override BreathingSign.text = \markup {\musicglyph #"scripts.caesura.straight"} \breathe)" <<
            endl;
     */
      s <<
        endl <<
        "\\override BreathingSign.text = \\markup {"
        "\\musicglyph #\"scripts.caesura.curved\"}" <<
        endl <<
        "\\breathe" <<
        endl;
      break;
    case msrArticulation::kSpiccato:
      s << "%{spiccato???%}";
      break;
    case msrArticulation::kStaccato:
      s << ".";
      break;
    case msrArticulation::kStaccatissimo:
      s << "!";
      break;
    case msrArticulation::kStress:
      s << "%{stress???%}";
      break;
    case msrArticulation::kUnstress:
      s << "%{unstress???%}";
      break;
    case msrArticulation::kDetachedLegato:
      s << "_"; // portato
      break;
    case msrArticulation::kStrongAccent:
      s << "^"; // marcato
      break;
    case msrArticulation::kTenuto:
      s << "-";
      break;
      
    case msrArticulation::kFermata:
      // this is handled in visitStart (S_msrFermata&)
      break;
      
    case msrArticulation::kArpeggiato:
      // this is handled in chordArticulationAsLilyponString ()
      break;
    case msrArticulation::kNonArpeggiato:
      // this is handled in chordArticulationAsLilyponString ()
      break;
    case msrArticulation::kDoit:
      s << "\\bendAfter #+4";
      break;
    case msrArticulation::kFalloff:
      s << "\\bendAfter #-4";
      break;
    case msrArticulation::kPlop:
      s << "%{plop???%}";
      break;
    case msrArticulation::kScoop:
      s << "%{scoop???%}";
      break;
  } // switch
  
  return s.str ();
}

//________________________________________________________________________
string lpsr2LilypondTranslator::chordArticulationAsLilyponString (
  S_msrArticulation articulation)
{
  stringstream s;
  
  switch (articulation->getArticulationPlacement ()) {
    case k_NoPlacement:
      break;
    case kAbovePlacement:
      fIndentedOutputStream << "^";
      break;
    case kBelowPlacement:
      fIndentedOutputStream << "_";
      break;
  } // switch

  switch (articulation->getArticulationKind ()) {
    case msrArticulation::kAccent:
      s << ">";
      break;
    case msrArticulation::kBreathMark:
      s << "\\breathe";
      break;
    case msrArticulation::kCaesura:
    /* JMI
          fIndentedOutputStream <<
            endl <<
            R"(\once\override BreathingSign.text = \markup {\musicglyph #"scripts.caesura.straight"} \breathe)" <<
            endl;
     */
      s <<
        endl <<
        "\\override BreathingSign.text = \\markup {"
        "\\musicglyph #\"scripts.caesura.curved\"}" <<
        endl <<
      "\\breathe" <<
        endl;
      break;
    case msrArticulation::kSpiccato:
      s << "%{spiccato???%}";
      break;
    case msrArticulation::kStaccato:
      s << "\\staccato"; // JMI "-.";
      break;
    case msrArticulation::kStaccatissimo:
      s << "!";
      break;
    case msrArticulation::kStress:
      s << "%{stress???%}";
      break;
    case msrArticulation::kUnstress:
      s << "%{unstress%}";
      break;
    case msrArticulation::kDetachedLegato:
      s << "_"; // portato
      break;
    case msrArticulation::kStrongAccent:
      s << "^"; // marcato
      break;
    case msrArticulation::kTenuto:
      s << "-";
      break;
      
    case msrArticulation::kFermata:
      // this is handled in visitStart (S_msrFermata&)
      break;
      
    case msrArticulation::kArpeggiato:
      s << "\\arpeggio";
      break;
    case msrArticulation::kNonArpeggiato:
      s << "\\arpeggio";
      break;
    case msrArticulation::kDoit:
      s << "\\bendAfter #+4";
      break;
    case msrArticulation::kFalloff:
      s << "\\bendAfter #-4";
      break;
    case msrArticulation::kPlop:
      s << "%{plop%}";
      break;
    case msrArticulation::kScoop:
      s << "%{scoop%}";
      break;
  } // switch
  
  return s.str ();
}

//________________________________________________________________________
string lpsr2LilypondTranslator::technicalAsLilypondString (
  S_msrTechnical technical)
{
  string result;

  switch (technical->getTechnicalKind ()) {
    case msrTechnical::kArrow:
      result = "%{\\Arrow???%}";
      break;
    case msrTechnical::kDoubleTongue:
      result = "-\\tongue #2";
      break;
    case msrTechnical::kDownBow:
      result = "\\downbow";
      break;
    case msrTechnical::kFingernails:
      result = "%{\\Fingernails???%}";
      break;
    case msrTechnical::kHarmonic:
      result = "\\flageolet"; // JMI "\\once\\override Staff.NoteHead.style = #'harmonic-mixed";
      break;
    case msrTechnical::kHeel:
      result = "\\lheel"; // rheel??? JMI
      break;
    case msrTechnical::kHole:
      result = "%{\\Hole???%}";
      break;
    case msrTechnical::kOpenString:
      result = "\\open"; // halfopen ??? JMI
      break;
    case msrTechnical::kSnapPizzicato:
      result = "\\snappizzicato";
      break;
    case msrTechnical::kStopped:
      result = "\\stopped"; // or -+ JMI
      break;
    case msrTechnical::kTap:
      result = "%{\\Tap???%}";
      break;
    case msrTechnical::kThumbPosition:
      result = "\\thumb";
      break;
    case msrTechnical::kToe:
      result = "\\ltoe"; // rtoe ??? JMI
      break;
    case msrTechnical::kTripleTongue:
      result = "-\\tongue #3";
      break;
    case msrTechnical::kUpBow:
      result = "\\upbow";
      break;
  } // switch

  return result;
}

//________________________________________________________________________
string lpsr2LilypondTranslator::technicalWithIntegerAsLilypondString (
  S_msrTechnicalWithInteger technicalWithInteger)
{
  stringstream s;

  switch (technicalWithInteger->getTechnicalWithIntegerKind ()) {
    case msrTechnicalWithInteger::kBend:
      s <<
        "\\bendAfter " <<
       technicalWithInteger->
          getTechnicalWithIntegerValue ();
      break;
    case msrTechnicalWithInteger::kFingering:
      s <<
        "- " <<
       technicalWithInteger->
          getTechnicalWithIntegerValue ();
      break;
    case msrTechnicalWithInteger::kFret:
      // LilyPond will take care of that JMI
      break;
    case msrTechnicalWithInteger::kString:
      s << // no space is allowed between the backslash and the number
        "\\" <<
       technicalWithInteger->
          getTechnicalWithIntegerValue ();
      break;
  } // switch
     
  return s.str ();
}

//________________________________________________________________________
string lpsr2LilypondTranslator::technicalWithStringAsLilypondString (
  S_msrTechnicalWithString technicalWithString)
{
  string result;
  
  switch (technicalWithString->getTechnicalWithStringKind ()) {
    case msrTechnicalWithString::kHammerOn:
      result = "%{\\HammerOn???%}";
      break;
    case msrTechnicalWithString::kHandbell:
      result = "%{\\Handbell???%}";
      break;
    case msrTechnicalWithString::kOtherTechnical:
      result = "%{\\OtherTechnical???%}";
      break;
    case msrTechnicalWithString::kPluck:
      result = "%{Pluck???%}";
      break;
    case msrTechnicalWithString::kPullOff:
      result = "%{\\PullOff???%}";
      break;
  } // switch

  result +=
    string (" ") +
    "-\\markup {\"" + technicalWithString->
      getTechnicalWithStringValue () +
      "\"}";
    
  return result;
}

//________________________________________________________________________
string lpsr2LilypondTranslator::ornamentAsLilypondString (
  S_msrOrnament ornament)
{
  string result;
  
  string
    noteUplinkDuration =
      ornament->
        getOrnamentNoteUplink ()->
          noteSoundingWholeNotesAsMsrString ();

  switch (ornament->getOrnamentKind ()) {
    case msrOrnament::kTrillMark:
      result = "\\trill ";
      break;
      
    case msrOrnament::kWavyLine:
      result = "%{\\wavy line???%} ";
      break;
      
    case msrOrnament::kTurn:
      result = "\\turn ";
      break;
      
    case msrOrnament::kInvertedTurn:
      result = "\\reverseturn ";
      break;
      
    case msrOrnament::kDelayedTurn:
      {
        // c2*2/3 ( s2*1/3\turn
        int
          numerator =
            gLilypondOptions->
              fDelayedOrnamentsFraction.
                getDenominator (),
          denominator =
            gLilypondOptions->
              fDelayedOrnamentsFraction.
                getDenominator ();
              
        stringstream s;

        s <<
          "s" <<
          noteUplinkDuration <<
          "*" <<
            denominator
            -
            numerator <<
          "/" <<
            denominator <<
          "\\turn ";
          
        result = s.str ();
      }
      break;
      
    case msrOrnament::kDelayedInvertedTurn:
      {
/* JMI
        stringstream s;

        s <<
          "delayed inverted turn is not supported, replaced by inverted turn," <<
          endl <<
          "see http://lilypond.org/doc/v2.18/Documentation/snippets/expressive-marks";
        
        lpsrMusicXMLWarning (
          inputLineNumber,
          s.str ());
          
        result = "\\reverseturn %{ " + s.str () + " %}";
*/
        // c2*2/3 ( s2*1/3\turn
        stringstream s;

        s <<
          "s" <<
          noteUplinkDuration <<
          "*1/3" "\\reverseturn ";
          
        result = s.str ();
      }
      break;
      
    case msrOrnament::kVerticalTurn:
      result =
        "^\\markup { \\rotate #90 \\musicglyph #\"scripts.turn\" } ";
          /* JMI
      {
        string message =
          "delayed vertical turn is not supported, ignored";
        
        lpsrMusicXMLWarning (
          inputLineNumber,
          message);
          
        result = "%{ " + message + " %}";
      }
        */
      break;
      
    case msrOrnament::kMordent:
      result = "\\mordent ";
      break;
      
    case msrOrnament::kInvertedMordent:
      result = "%{\\inverted mordent???%} ";
      break;
      \
    case msrOrnament::kSchleifer:
      result = "%{\\schleifer???%} ";
      break;
      
    case msrOrnament::kShake:
      result = "%{\\shake???%} ";
      break;
      
    case msrOrnament::kAccidentalMark:
      result =
        alterationAsLilypondAccidentalMark (
          ornament->getOrnamentAccidentalMark ());
      break;
  } // switch

  return result;
}

//________________________________________________________________________
string lpsr2LilypondTranslator::dynamicsAsLilypondString (
  S_msrDynamics dynamics)
{
  string result =
    "\\" + dynamics->dynamicsKindAsString ();
    
  return result;
}

//________________________________________________________________________
string lpsr2LilypondTranslator::harpPedalTuningAsLilypondString (
  msrAlteration alteration)
{
  string result;
  
  switch (alteration) {
    case kDoubleFlat:
      result = "?";
      break;
    case kSesquiFlat:
      result = "?";
      break;
    case kFlat:
      result = "^";
      break;
    case kSemiFlat:
      result = "?";
      break;
    case kNatural:
      result = "-";
      break;
    case kSemiSharp:
      result = "?";
      break;
    case kSharp:
      result = "v";
      break;
    case kSesquiSharp:
      result = "?";
      break;
    case kDoubleSharp:
      result = "?";
      break;
    case k_NoAlteration:
      result = "alteration???";
      break;
  } // switch

  return result;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::transposeDiatonicError (
  int inputLineNumber,
  int transposeDiatonic,
  int transposeChromatic)
{
  stringstream s;

  s <<
    "diatonic '" << transposeDiatonic <<
    "' is not consistent with " <<
    "chromaticic '" << transposeChromatic <<
    "'";
    
  msrMusicXMLError (
    inputLineNumber,
    s.str ());
}

//________________________________________________________________________
string lpsr2LilypondTranslator::singleTremoloDurationAsLilypondString (
  S_msrSingleTremolo singleTremolo)
{  
  int
    inputLineNumber =
      singleTremolo->
        getInputLineNumber ();
    
  int
    singleTremoloMarksNumber =
      singleTremolo->
        getSingleTremoloMarksNumber ();

  S_msrNote
    singleTremoloNoteUplink =
      singleTremolo->
        getSingleTremoloNoteUplink ();

  msrDuration
    singleTremoloNoteDuration =
      singleTremoloNoteUplink->
        getNoteGraphicDuration ();
  
  /*
  The same output can be obtained by adding :N after the note,
  where N indicates the duration of the subdivision (it must be at least 8).
  If N is 8, one beam is added to the note’s stem.
  */
  
  int durationToUse =
    singleTremoloMarksNumber; // JMI / singleTremoloNoteSoundingWholeNotes;
        
  if (singleTremoloNoteDuration >= kEighth)
    durationToUse +=
      1 + (singleTremoloNoteDuration - kEighth);
  
  stringstream s;

  s <<
    ":" <<
    int (pow (2, durationToUse + 2)) <<
    " ";
    
  return s.str ();
}

//________________________________________________________________________
string lpsr2LilypondTranslator::harmonyDegreeAlterationAsLilypondString (
  msrAlteration harmonyDegreeAlteration)
{
  string result;
  
  switch (harmonyDegreeAlteration) {
    case k_NoAlteration:
      result = "?";
      break;
    case kDoubleFlat:
      result = "?";
      break;
    case kSesquiFlat:
      result = "?";
      break;
    case kFlat:
      result = "-";
      break;
    case kSemiFlat:
      result = "?";
      break;
    case kNatural:
      result = "";
      break;
    case kSemiSharp:
      result = "?";
      break;
    case kSharp:
      result = "+";
      break;
    case kSesquiSharp:
      result = "?";
      break;
    case kDoubleSharp:
      result = "?";
      break;
  } // switch

  return result;
}

string lpsr2LilypondTranslator::harmonyAsLilypondString (
  S_msrHarmony harmony)
{
  int inputLineNumber =
    harmony->getInputLineNumber ();
    
  stringstream s;

  // should '\powerChords' be generated?
  switch (harmony->getHarmonyKind ()) {
    case kPowerHarmony:
      if (! fPowerChordHaveAlreadyBeenGenerated) {
        s << "\\powerChords ";
        fPowerChordHaveAlreadyBeenGenerated = true;
      }
      break;
    default:
      ;
  } // switch
  
  // print harmony pitch and duration
  s <<
    msrQuarterTonesPitchAsString (
      gMsrOptions->
        fMsrQuarterTonesPitchesLanguage,
      harmony->
        getHarmonyRootQuarterTonesPitch ()) <<
    wholeNotesAsLilypondString (
      inputLineNumber,
      harmony->
        getHarmonySoundingWholeNotes ());
    
  // print harmony kind
  switch (harmony->getHarmonyKind ()) {
    case k_NoHarmony:
      s << "Harmony???";
      break;

    case kMajorHarmony:
      s << ":5.3";
      break;
    case kMinorHarmony:
      s << ":m";
      break;
    case kAugmentedHarmony:
      s << ":aug";
      break;
    case kDiminishedHarmony:
      s << ":dim";
      break;

    case kDominantHarmony:
      s << ":7";
      break;
    case kMajorSeventhHarmony:
      s << ":maj7";
      break;
    case kMinorSeventhHarmony:
      s << ":m7";
      break;
    case kDiminishedSeventhHarmony:
      s << ":dim7";
      break;
    case kAugmentedSeventhHarmony:
      s << ":aug7";
      break;
    case kHalfDiminishedHarmony:
      s << ":m7.5-";
      break;
    case kMajorMinorHarmony:
      s << ":m7+";
      break;

    case kMajorSixthHarmony:
      s << ":6";
      break;
    case kMinorSixthHarmony:
      s << ":m6";
      break;

    case kDominantNinthHarmony:
      s << ":9";
      break;
    case kMajorNinthHarmony:
      s << ":maj7.9";
      break;
    case kMinorNinthHarmony:
      s << ":m7.9";
      break;

    case kDominantEleventhHarmony:
      s << ":11";
      break;
    case kMajorEleventhHarmony:
      s << ":maj7.11";
      break;
    case kMinorEleventhHarmony:
      s << ":m7.11";
      break;

    case kDominantThirteenthHarmony:
      s << ":13";
      break;
    case kMajorThirteenthHarmony:
      s << ":maj7.13";
      break;
    case kMinorThirteenthHarmony:
      s << ":m7.13";
      break;

    case kSuspendedSecondHarmony:
      s << ":sus2";
      break;
    case kSuspendedFourthHarmony:
      s << ":sus4";
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
      s << ":3-.6-%{:Neapolitan%}";
      break;
    case kItalianHarmony:
      s << ":3.6+%{:Italian%}";
      break;
    case kFrenchHarmony:
      s << ":3.5+.6+%{:French%}";
      break;
    case kGermanHarmony:
      s << ":3.5.6+%{:German%}";
      break;

    case kPedalHarmony:
      s << "%{:Pedal%}";
      break;
    case kPowerHarmony:
      s << ":5 %{power%}";
      break;
    case kTristanHarmony:
      s << ":2+.5+.6+%{:Tristan%}";
      break;
      
    case kOtherHarmony:
      s << "%{:Other%}";
      break;
    case kNoneHarmony:
      s << "%{:None%}";
      break;
  } // switch

  msrQuarterTonesPitch
    harmonyBassQuarterTonesPitch =
      harmony->
        getHarmonyBassQuarterTonesPitch ();
      
  if (harmonyBassQuarterTonesPitch != k_NoQuarterTonesPitch)
    s <<
      "/" <<
      msrQuarterTonesPitchAsString (
        gMsrOptions->fMsrQuarterTonesPitchesLanguage,
        harmonyBassQuarterTonesPitch);


  // print harmony degrees if any
  list<S_msrHarmonyDegree>
    harmonyDegreesList =
      harmony->getHarmonyDegreesList ();

  if (harmonyDegreesList.size ()) {
    bool thereAreDegreesToBeRemoved = false;
    
    // print degrees to be added first
    for (
      list<S_msrHarmonyDegree>::const_iterator i = harmonyDegreesList.begin();
      i != harmonyDegreesList.end();
      i++) {
      S_msrHarmonyDegree harmonyDegree = (*i);

      // get harmony degree information
      int
        harmonyDegreeValue =
          harmonyDegree->getHarmonyDegreeValue ();
          
      msrAlteration
        harmonyDegreeAlteration =
          harmonyDegree->getHarmonyDegreeAlteration ();
      
      msrHarmonyDegree::msrHarmonyDegreeTypeKind
        harmonyDegreeTypeKind =
          harmonyDegree->getHarmonyDegreeTypeKind ();
                            
      // print the harmony degree
      switch (harmonyDegreeTypeKind) {
        case msrHarmonyDegree::kHarmonyDegreeAddType:
          {
            s <<
              "." <<
              harmonyDegreeValue <<
              harmonyDegreeAlterationAsLilypondString (
                harmonyDegreeAlteration);
          }
          break;
          
        case msrHarmonyDegree::kHarmonyDegreeAlterType:
          s <<
            "." <<
            harmonyDegreeValue <<
            harmonyDegreeAlterationAsLilypondString (
              harmonyDegreeAlteration);
          break;
          
        case msrHarmonyDegree::kHarmonyDegreeSubtractType:
          thereAreDegreesToBeRemoved = true;
          break;
      } // switch
    } // for
    
    // then print degrees to be removed if any
    if (thereAreDegreesToBeRemoved) {
      s << "^";

      int counter = 0;
      for (
        list<S_msrHarmonyDegree>::const_iterator i = harmonyDegreesList.begin();
        i != harmonyDegreesList.end();
        i++) {
        counter++;
        
        S_msrHarmonyDegree harmonyDegree = (*i);
  
        // get harmony degree information
        int
          harmonyDegreeValue =
            harmonyDegree->getHarmonyDegreeValue ();
            
        msrAlteration
          harmonyDegreeAlteration =
            harmonyDegree->getHarmonyDegreeAlteration ();
        
        msrHarmonyDegree::msrHarmonyDegreeTypeKind
          harmonyDegreeTypeKind =
            harmonyDegree->getHarmonyDegreeTypeKind ();
                              
        // print the harmony degree
        switch (harmonyDegreeTypeKind) {
          case msrHarmonyDegree::kHarmonyDegreeAddType:
          case msrHarmonyDegree::kHarmonyDegreeAlterType:
            break;
            
          case msrHarmonyDegree::kHarmonyDegreeSubtractType:
            if (counter > 1)
              s << ".";
    
            s <<
              harmonyDegreeValue <<
              harmonyDegreeAlterationAsLilypondString (
                harmonyDegreeAlteration);
            break;
        } // switch
      } // for
    }
  }

  int harmonyInversion =
    harmony->getHarmonyInversion ();
    
  if ( harmonyInversion!= K_HARMONY_NO_INVERSION) {
    s <<
      "%{ inversion: " << harmonyInversion << " %}";
  }
    
  return s.str ();
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrScore& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting lpsrScore" <<
      endl;

  // initial empty line in LilyPond code
  // to help copy/paste it
// JMI  fIndentedOutputStream << endl;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrScore& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting lpsrScore" <<
      endl;

  // final empty line in LilyPond code
  // to help copy/paste it
  fIndentedOutputStream <<
    endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrLilypondVarValAssoc& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting lpsrLilypondVarValAssoc" <<
      endl;

  if (elt->getComment ().size())
    fIndentedOutputStream <<
      "% " << elt->getComment () <<
      endl;

  if (elt->getCommentedKind () == lpsrLilypondVarValAssoc::kCommented)
    fIndentedOutputStream << "\%";
  
  switch (elt->getBackslashKind ()) {
    case lpsrLilypondVarValAssoc::kWithBackslash:
      fIndentedOutputStream << "\\";
      break;
    case lpsrLilypondVarValAssoc::kWithoutBackslash:
      break;
  } // switch

  // the largest variable name length in a header is 18
  int fieldWidth;
  
  if (fOnGoingHeader)
    fieldWidth = 18;
  else
    fieldWidth = elt->getVariableName ().size();
    
  fIndentedOutputStream <<
    left<< setw (fieldWidth) << elt->getVariableName ();
  
  if (elt->getVarValSeparator () == lpsrLilypondVarValAssoc::kEqualSign)
    fIndentedOutputStream << " = ";
  else
    fIndentedOutputStream << " ";
  
  if (elt->getQuotesKind () == lpsrLilypondVarValAssoc::kQuotesAroundValue)
    fIndentedOutputStream << "\"";
    
  fIndentedOutputStream <<
    elt->getVariableValue ();

  if (elt->getUnit ().size())
    fIndentedOutputStream <<
      "\\" <<
      elt->getUnit ();
  
  if (elt->getQuotesKind () == lpsrLilypondVarValAssoc::kQuotesAroundValue)
    fIndentedOutputStream << "\"";
  
  fIndentedOutputStream << endl;

  switch (elt->getEndlKind ()) {
    case lpsrLilypondVarValAssoc::kWithEndl:
      fIndentedOutputStream << endl;
      break;
      
    case lpsrLilypondVarValAssoc::kWithEndlTwice:
      fIndentedOutputStream << endl << endl;
      break;
      
    case lpsrLilypondVarValAssoc::kWithoutEndl:
      break;
  } // switch
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrLilypondVarValAssoc& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting lpsrLilypondVarValAssoc" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrSchemeVarValAssoc& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting lpsrSchemeVarValAssoc" <<
      endl;

  if (elt->getComment ().size())
    fIndentedOutputStream <<
      "% " << elt->getComment () <<
      endl;

  if (elt->getCommentedKind () == lpsrSchemeVarValAssoc::kCommented)
    fIndentedOutputStream << "\% ";
  
  fIndentedOutputStream <<
    "#(" <<
    elt->getVariableName () <<
    " " <<
    elt->getVariableValue () <<
    ")";
    
  switch (elt->getEndlKind ()) {
    case lpsrSchemeVarValAssoc::kWithEndl:
      fIndentedOutputStream << endl;
      break;
    case lpsrSchemeVarValAssoc::kWithEndlTwice:
      fIndentedOutputStream << endl << endl;
      break;
    case lpsrSchemeVarValAssoc::kWithoutEndl:
      break;
  } // switch
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrSchemeVarValAssoc& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting lpsrSchemeVarValAssoc" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrHeader& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting lpsrHeader" <<
      endl;

  fIndentedOutputStream <<
    "\\header" << " {" <<
    endl;

  idtr++;

  fOnGoingHeader = true;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrHeader& elt)
{
  fOnGoingHeader = false;

  idtr--;

  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting lpsrHeader" <<
      endl;

  fIndentedOutputStream <<
    "}" <<
    endl << endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrPaper& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting lpsrPaper" <<
      endl;

  fIndentedOutputStream <<
    "\\paper" << " {" <<
    endl;

  idtr++;
  
  const int fieldWidth = 20;

  // page width, height and margins

  {
    float paperWidth =
      elt->getPaperWidth ();
      
    if (paperWidth > 0) {
      fIndentedOutputStream <<
        left <<
          setw (fieldWidth) <<
          "paper-width" << " = " <<
          setprecision(4) << paperWidth << "\\cm" <<
          endl;
    }
  }
  
  {
    float paperHeight =
      elt->getPaperHeight ();
      
    if (paperHeight > 0) {
      fIndentedOutputStream <<
        left <<
          setw (fieldWidth) <<
          "paper-height" << " = " <<
          setprecision(4) << paperHeight << "\\cm" <<
          endl;
    }
  }
  
  {
    float topMargin =
      elt->getTopMargin ();
      
    if (topMargin > 0) {
      fIndentedOutputStream <<
        left <<
          setw (fieldWidth) <<
          "top-margin" << " = " <<
          setprecision(4) << topMargin << "\\cm" <<
          endl;
    }
  }

  {
    float bottomMargin =
      elt->getBottomMargin ();
      
    if (bottomMargin > 0) {
      fIndentedOutputStream <<
        left <<
          setw (fieldWidth) <<
          "bottom-margin" << " = " <<
          setprecision(4) << bottomMargin << "\\cm" <<
          endl;
    }
  }

  {
    float leftMargin =
      elt->getLeftMargin ();
      
    if (leftMargin > 0) {
      fIndentedOutputStream <<
        left <<
          setw (fieldWidth) <<
          "left-margin" << " = " <<
          setprecision(4) << leftMargin << "\\cm" <<
          endl;
    }
  }

  {
    float rightMargin =
      elt->getRightMargin ();
      
    if (rightMargin > 0) {
      fIndentedOutputStream <<
        left <<
          setw (fieldWidth) <<
          "right-margin" << " = " <<
        setprecision(4) << rightMargin << "\\cm" <<
        endl;
    }
  }

  // spaces

  {
    float betweenSystemSpace =
      elt->getBetweenSystemSpace ();
      
    if (betweenSystemSpace > 0) {
      fIndentedOutputStream <<
        left <<
        setw (fieldWidth) <<
        "between-system-space" << " = " <<
        setprecision(4) << betweenSystemSpace << "\\cm" <<
        endl;
    }
  }

  {
    float pageTopSpace =
      elt->getPageTopSpace ();
      
    if (pageTopSpace > 0) {
      fIndentedOutputStream <<
        left <<
        setw (fieldWidth) <<
        "page-top-space" << " = " <<
        setprecision(4) << pageTopSpace << "\\cm" <<
        endl;
    }
  }

  // headers and footers

  {
    string oddHeaderMarkup =
      elt->getOddHeaderMarkup ();
      
    if (oddHeaderMarkup.size ()) {
      fIndentedOutputStream <<
        left <<
        setw (fieldWidth) <<
        "oddHeaderMarkup" << " = " <<
        oddHeaderMarkup <<
        endl;
    }
  }

  {
    string evenHeaderMarkup =
      elt->getEvenHeaderMarkup ();
      
    if (evenHeaderMarkup.size ()) {
      fIndentedOutputStream <<
        left <<
        setw (fieldWidth) <<
        "evenHeaderMarkup" << " = " <<
        evenHeaderMarkup <<
        endl;
    }
  }

  {
    string oddFooterMarkup =
      elt->getOddFooterMarkup ();
      
    if (oddFooterMarkup.size ()) {
      fIndentedOutputStream <<
        left <<
        setw (fieldWidth) <<
        "oddFooterMarkup" << " = " <<
        oddFooterMarkup <<
        endl;
    }
  }

  {
    string evenFooterMarkup =
      elt->getEvenFooterMarkup ();
      
    if (evenFooterMarkup.size ()) {
      fIndentedOutputStream <<
        left <<
        setw (fieldWidth) <<
        "evenFooterMarkup" << " = " <<
        evenFooterMarkup <<
        endl;
    }
  }

  // generate the default 'indent' setting ready for the user
  fIndentedOutputStream <<
    left <<
    setw (fieldWidth) <<
    "indent" << " = " <<
    setprecision(4) << 1.5 << "\\cm" <<
    endl;

  // generate the default 'short-indent' setting ready for the user
  fIndentedOutputStream <<
    left <<
    setw (fieldWidth) <<
    "short-indent" << " = " <<
    setprecision(4) << 1.0 << "\\cm" <<
    endl;

  fIndentedOutputStream << endl;

  // generate a 'page-count' comment ready for the user
  fIndentedOutputStream <<
    left <<
    setw (fieldWidth) <<
    "%" "page-count" << " = " <<
    setprecision(4) << 1 <<
    endl;

  // generate a 'system-count' comment ready for the user
  fIndentedOutputStream <<
    left <<
    setw (fieldWidth) <<
    "%" "system-count" << " = " <<
    setprecision(4) << 1 <<
    endl;

  // fonts
  if (gLilypondOptions->fJazzFonts) {
    fIndentedOutputStream <<
R"(
  #(define fonts
     (set-global-fonts
      #:music "lilyjazz"
      #:brace "lilyjazz"
      #:roman "lilyjazz-text"
      #:sans "lilyjazz-chord"
      #:factor (/ staff-height pt 20)
      ))
)";
  }
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrPaper& elt)
{
  idtr--;

  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting lpsrPaper" <<
      endl;
      
  fIndentedOutputStream <<
    "}" <<
    endl << endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrLayout& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting lpsrLayout" <<
      endl;

  fIndentedOutputStream <<
    "\\layout" << " {" <<
    endl;

  idtr++;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrLayout& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting lpsrLayout" <<
      endl;

  if (gLilypondOptions->fRepeatBrackets) {
    fIndentedOutputStream <<
      "\\context " "{" <<
      endl;
    
    idtr++;

    fIndentedOutputStream <<
      "\\Score" <<
      endl <<
      "% defaultBarType = #\"!\"" <<
      endl <<
      "startRepeatType = #\"[|:\"" <<
      endl <<
      "endRepeatType = #\":|]\"" <<
      endl <<
      "doubleRepeatType = #\":|][|:\"" <<
      endl;

    idtr--;
        
    fIndentedOutputStream <<
      "}" <<
      endl;
  }

  if (true) { // JMI XXL
    fIndentedOutputStream <<
      "\\context {" <<
      endl;
  
    idtr++;
  
    fIndentedOutputStream <<
      "\\Staff" <<
      endl <<
      "\\consists \"Span_arpeggio_engraver\"" <<
      endl;

    idtr--;

    fIndentedOutputStream <<
      "}" <<
      endl;
  }
    
  idtr--;

  fIndentedOutputStream <<
    "}" <<
    endl <<
    endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrScoreBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting lpsrScoreBlock" <<
      endl;

  fIndentedOutputStream <<
    "\\score {" <<
    endl;

  idtr++;
/* JMI
  if (elt->getScoreBlockElements ().size()) {
    fIndentedOutputStream <<
      "<<" <<
      endl;
  
    idtr++;
  }
*/
  fOnGoingScoreBlock = true;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrScoreBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting lpsrScoreBlock" <<
      endl;
/* JMI
  if (elt->getScoreBlockElements ().size()) {
    idtr--;
    
    fIndentedOutputStream <<
      ">>" <<
      endl <<
      * endl;
  }
*/
  idtr--;
  
  fIndentedOutputStream <<
    "}" <<
    endl;

  fOnGoingScoreBlock = false;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrParallelMusic& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting lpsrParallelMusic" <<
      endl;

  if (elt->getParallelMusicElements ().size()) { // JMI
    if (gLilypondOptions->fComments) {
      fIndentedOutputStream << left <<
        setw (commentFieldWidth) <<
        "<<" <<
        "% parallel music";
    }
    
    else {
      fIndentedOutputStream <<
        "<<";
    }
    
    fIndentedOutputStream <<
      endl;
  
    idtr++;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrParallelMusic& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting lpsrParallelMusic" <<
      endl;

  if (elt->getParallelMusicElements ().size()) { // JMI
    idtr--;
    
    if (gLilypondOptions->fComments) {
      fIndentedOutputStream << left <<
        setw (commentFieldWidth) <<
        ">>" <<
        "% parallel music";
    }

    else {
      fIndentedOutputStream <<
        ">>";
    }
    
    fIndentedOutputStream <<
      endl <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrPartGroupBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting lpsrPartGroupBlock" <<
      endl;

  // fetch part group
  S_msrPartGroup
    partGroup =
      elt->getPartGroup ();
      
  msrPartGroup::msrPartGroupSymbolKind
    partGroupSymbolKind =
      partGroup->getPartGroupSymbolKind ();

  bool
    partGroupBarline =
      partGroup->getPartGroupBarline ();
      
  string
    partGroupInstrumentName =
      partGroup->getPartGroupInstrumentName ();
      
  string partGroupContextName;

  // LPNR, page 567

  // don't generate code for a for the top-most part group block
  if (partGroup->getPartGroupPartGroupUplink ()) {
    switch (partGroupSymbolKind) {
      case msrPartGroup::k_NoPartGroupSymbol:
        partGroupContextName = "";
        break;
        
      case msrPartGroup::kBracePartGroupSymbol: // JMI
      /*
       *
       * check whether individual part have instrument names
       * 
        if (partGroupInstrumentName.size ())
          partGroupContextName = "\\new PianoStaff";
        else
          partGroupContextName = "\\new GrandStaff";
          */
        partGroupContextName =
          partGroupBarline // only partGroup has an instrument name
            ? "\\new PianoStaff"
            : "\\new GrandStaff";
        break;
        
      case msrPartGroup::kBracketPartGroupSymbol:
        partGroupContextName =
          partGroupBarline
            ? "\\new StaffGroup"
            : "\\new ChoirStaff";
        break;
        
      case msrPartGroup::kLinePartGroupSymbol:
        partGroupContextName = "\\new StaffGroup";
        break;
        
      case msrPartGroup::kSquarePartGroupSymbol:
        partGroupContextName = "\\new StaffGroup";
        break;
    } // switch
  
    fIndentedOutputStream <<
      setw (commentFieldWidth) <<
      partGroupContextName + " <<";
      
    if (gLilypondOptions->fComments)
      fIndentedOutputStream <<
        "% part group " <<
        partGroup->getPartGroupCombinedName ();
        
    fIndentedOutputStream <<
      endl;
  }

  if (partGroupInstrumentName.size ())
    fIndentedOutputStream <<
      "\\set PianoStaff.instrumentName = \"" <<
      partGroupInstrumentName <<
      "\"" <<
      endl;

    bool doConnectArpeggios = true; // JMI
    
    if (doConnectArpeggios)
      fIndentedOutputStream <<
        "\\set PianoStaff.connectArpeggios = ##t" <<
        endl;
  
    if (partGroupSymbolKind == msrPartGroup::kSquarePartGroupSymbol) {
      idtr++;
      fIndentedOutputStream <<
        "\\set StaffGroup.systemStartDelimiter = #'SystemStartSquare" <<
        endl;
      idtr--;
    }
       
  fIndentedOutputStream <<
    endl;

  if (elt->getPartGroupBlockElements ().size () > 1)
    idtr++;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrPartGroupBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting lpsrPartGroupBlock" <<
      endl;

  // fetch part group
  S_msrPartGroup
    partGroup =
      elt->getPartGroup ();
      
  if (elt->getPartGroupBlockElements ().size () > 1)
    idtr--;

  // don't generate code for a for the top-most part group block
  if (partGroup->getPartGroupPartGroupUplink ()) {
    fIndentedOutputStream <<
      setw (commentFieldWidth) << ">>";
      
    if (gLilypondOptions->fComments)
      fIndentedOutputStream <<
        "% part group " <<
        partGroup->getPartGroupCombinedName ();
        
    fIndentedOutputStream <<
      endl <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrPartBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting lpsrPartBlock" <<
      endl;

  // fetch part block's part
  S_msrPart
    part =
      elt->getPart ();
      
  if (part->getPartStavesMap ().size () > 1) {
    // don't generate code for a part with only one stave

    string
      partName =
        part->getPartName (),
      partAbbreviation =
        part->getPartAbbreviation ();
        /*
      partInstrumentName =  // JMI
        part->getPartInstrumentName (),
      partInstrumentAbbreviation =  // JMI
        part->getPartInstrumentAbbreviation ();
        */
  
    if (gLilypondOptions->fComments) {
      fIndentedOutputStream << left <<
        setw (commentFieldWidth) <<
        "\\new PianoStaff <<" <<
        "% part " << part->getPartCombinedName ();
    }
    else {
      fIndentedOutputStream <<
        "\\new PianoStaff <<";      
    }
    fIndentedOutputStream <<
      endl;
    
    idtr++;

    if (partName.size ()) {
      fIndentedOutputStream <<
        "\\set PianoStaff.instrumentName = \"" <<
        partName <<
        "\"" <<
        endl;
    }
    
    if (partAbbreviation.size ()) {
      fIndentedOutputStream <<
        "\\set PianoStaff.shortInstrumentName = \"" <<
        partAbbreviation <<
        "\"" <<
        endl;
    }

    bool doConnectArpeggios = true; // JMI
    
    if (doConnectArpeggios) {
      fIndentedOutputStream <<
        "\\set PianoStaff.connectArpeggios = ##t" <<
        endl;
    }
       
    fIndentedOutputStream <<
      endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrPartBlock& elt)
{
  // fetch current part block's part
  S_msrPart
    part =
      elt->getPart ();
      
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting lpsrPartBlock" <<
      endl;

  if (part->getPartStavesMap ().size () > 1) {
    // don't generate code for a part with only one stave

    idtr--;
  
    if (gLilypondOptions->fComments) {
      fIndentedOutputStream <<
        setw (commentFieldWidth) << ">>" <<    
        "% part " <<
        part->getPartCombinedName ();
    }
    else {
      fIndentedOutputStream <<
        ">>";
    }
    
    fIndentedOutputStream <<
      endl <<
      endl;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrStaffBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting lpsrStaffBlock" <<
      endl;

  S_msrStaff
    staff =
      elt->getStaff ();
      
  string staffContextName;
  
  switch (staff->getStaffKind ()) {
    case msrStaff::kMasterStaff:
      staffContextName = "\\new Staff";
      break;
      
    case msrStaff::kRegularStaff:
      staffContextName = "\\new Staff";
      break;
      
    case msrStaff::kTablatureStaff:
      staffContextName = "\\new TabStaff";
      break;
      
    case msrStaff::kPercussionStaff:
      staffContextName = "\\new DrumStaff";
      break;
      
    case msrStaff::kHarmonyStaff:
      staffContextName = "\\new kHarmonyStaff???";
      break;
      
    case msrStaff::kFiguredBassStaff:
      staffContextName = "\\new FiguredBassStaff???";
      break;
  } // switch

  stringstream s;

  s <<
    staffContextName <<
    " = \"" <<
    staff->getStaffName () <<
    "\"";

  string newContext = s.str ();
      
  if (gLilypondOptions->fComments) {
    fIndentedOutputStream << left <<
        setw (commentFieldWidth) <<
        newContext + " <<" <<
        " % staff \"" << staff->getStaffName () << "\"";
  }
  else {
    fIndentedOutputStream <<
      newContext << " <<";    
  }

  fIndentedOutputStream <<
    endl;
 
  idtr++;

/* JMI
  // fetch part
  S_msrPart
    part =
      staff->getStaffDirectPartUplink ();
*/

  string
    staffBlockInstrumentName =
      elt->getStaffBlockInstrumentName (),
    staffBlockShortInstrumentName =
      elt->getStaffBlockShortInstrumentName ();

  if (staffBlockInstrumentName.size ()) {
    fIndentedOutputStream <<
      "\\set Staff.instrumentName = \"" <<
      staffBlockInstrumentName <<
      "\"" <<
      endl;
  }

  if (staffBlockShortInstrumentName.size ()) {
    fIndentedOutputStream <<
      "\\set Staff.shortInstrumentName = \"" <<
      staffBlockShortInstrumentName <<
      "\"" <<
      endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrStaffBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting lpsrStaffBlock" <<
      endl;

  idtr--;

  if (gLilypondOptions->fComments) {
    fIndentedOutputStream <<
      setw (commentFieldWidth) << ">>" <<    
      "% staff " <<
      elt->getStaff ()->getStaffName ();
  }
  else {
    fIndentedOutputStream <<
      ">>";
  }

  fIndentedOutputStream <<
    endl <<
    endl;
}

/*
//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrNewStaffgroupBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting lpsrNewStaffgroupBlock" <<
      endl;

   fIndentedOutputStream <<
     "\\new StaffGroup" << " " << "{" <<
      endl;

  idtr++;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrNewStaffgroupBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting lpsrNewStaffgroupBlock" <<
      endl;

  idtr--;

  fIndentedOutputStream <<
    " }" <<
    endl << endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrNewStaffBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting lpsrNewStaffBlock" <<
      endl;

  idtr++;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrNewStaffBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting lpsrNewStaffBlock" <<
      endl;

  idtr--;
}
*/

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrUseVoiceCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting lpsrUseVoiceCommand" <<
      endl;

  S_msrVoice
    voice = elt->getVoice ();

  S_msrStaff
    staff = voice-> getVoiceStaffUplink ();

  msrStaff::msrStaffKind
    staffKind = staff->getStaffKind ();
  
  string staffContextName;
  string voiceContextName;
  
  switch (staffKind) {
    case msrStaff::kMasterStaff:
      staffContextName = "\\context Staff";
      voiceContextName = "Voice";
      break;
      
    case msrStaff::kRegularStaff:
      staffContextName = "\\context Staff";
      voiceContextName = "Voice";
      break;
      
    case msrStaff::kTablatureStaff:
      staffContextName = "\\context TabStaff";
      voiceContextName = "TabVoice";
      break;
      
    case msrStaff::kPercussionStaff:
      staffContextName = "\\context DrumStaff";
      voiceContextName = "DrumVoice";
      break;
      
    case msrStaff::kHarmonyStaff:
      staffContextName = "\\context ChordNames2";
      voiceContextName = "???"; // JMI
      break;
      
    case msrStaff::kFiguredBassStaff:
      staffContextName = "\\context FiguredBass";
      voiceContextName = "???"; // JMI
      break;
  } // switch

 // if (voice->getStaffRelativeVoiceNumber () > 0) { JMI
    // don't include the silent voice in the staff by default
    
    fIndentedOutputStream <<
      "\\context " << voiceContextName << " = " "\"" <<
      voice->getVoiceName () << "\"" << " <<" <<
       endl;
  
    idtr++;
  
    if (gLilypondOptions->fNoAutoBeaming)
      fIndentedOutputStream <<
        "\\set " << staffContextName << ".autoBeaming = ##f" <<
        endl;
  
    if (staffKind == msrStaff::kRegularStaff) {
      if (staff->getStaffNumberOfMusicVoices () > 1) {
        switch (voice->getVoiceStaffRelativeNumber ()) {
          case 1:
            fIndentedOutputStream << "\\voiceOne ";
            break;
          case 2:
            fIndentedOutputStream << "\\voiceTwo ";
            break;
          case 3:
            fIndentedOutputStream << "\\voiceThree ";
            break;
          case 4:
            fIndentedOutputStream << "\\voiceFour ";
            break;
          default:
            {}
        } // switch

        fIndentedOutputStream <<
          "% " <<
          staff->getStaffNumberOfMusicVoices () <<
          " music voices" <<
          endl <<
          endl;
      }
    }
  
    fIndentedOutputStream <<
      "\\" << voice->getVoiceName () << endl;
  
    idtr--;
    
    fIndentedOutputStream <<
      ">>" <<
      endl;
 // }
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrUseVoiceCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting lpsrUseVoiceCommand" <<
      endl;
}
  
//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrNewLyricsBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting lpsrNewLyricsBlock" <<
      endl;

  if (! gLilypondOptions->fNoLilypondLyrics) {
    fIndentedOutputStream <<
      "\\new Lyrics" <<
      endl;

    idtr++;
    
    fIndentedOutputStream <<
      /* JMI
        "\\lyricsto " <<
        "\""  << elt->getVoice ()->getVoiceName () << "\""  <<
        endl <<
        */
      "\\with { associatedVoice = " <<
      "\""  << elt->getVoice ()->getVoiceName () << "\""  <<
      "}" <<
      endl <<

      "\\" << elt->getStanza ()->getStanzaName () <<
      endl;

    idtr--;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrNewLyricsBlock& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting lpsrNewLyricsBlock" <<
      endl;

  if (! gLilypondOptions->fNoLilypondLyrics) {
    // JMI
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrVariableUseCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting lpsrVariableUseCommand" <<
      endl;

  idtr++;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrVariableUseCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting lpsrVariableUseCommand" <<
      endl;

  idtr--;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrContext& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting lpsrContext" <<
      endl;

  string
    contextTypeAsString =
      elt->getContextTypeAsString (),
    contextName =
      elt->getContextName ();
    
  fIndentedOutputStream <<
    "\\context " << contextTypeAsString <<
    " = \"" << contextName << "\"" <<
    endl;

  if (false) { //option JMI
    fIndentedOutputStream <<
      "\\with {" <<
      endl;

    idtr++;
    
    fIndentedOutputStream <<
      "\\override BarLine.bar-extent = #'(-2 . 2)" <<
      endl <<
      "\\consists \"Bar_engraver\"" <<
      endl;
        
    idtr--;
        
    fIndentedOutputStream <<
      "}" <<
      endl;
  }
        
  fIndentedOutputStream <<
    "\\" << contextName <<
    endl <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrContext& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting lpsrContext" <<
      endl;
}
  
//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrBarCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting lpsrBarCommand" <<
      endl;

  idtr++;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrBarCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting lpsrBarCommand" <<
      endl;

  idtr--;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrMelismaCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting lpsrMelismaCommand" <<
      endl;

  switch (elt->getMelismaKind ()) {
    case lpsrMelismaCommand::kMelismaStart:
// JMI      fIndentedOutputStream << "\\melisma ";
      break;
    case lpsrMelismaCommand::kMelismaEnd:
// JMI      fIndentedOutputStream << "\\melismaEnd ";
      break;
  } // switch
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrMelismaCommand& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting lpsrMelismaCommand" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrScore& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrScore" <<
      endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrScore& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrScore" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrCredit& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrCredit" <<
      endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrCredit& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrCredit" <<
      endl;
}

void lpsr2LilypondTranslator::visitStart (S_msrCreditWords& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrCreditWords" <<
      endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrCreditWords& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrCreditWords" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrPartGroup& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrPartGroup" <<
      elt->getPartGroupCombinedName () <<
      endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrPartGroup& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrPartGroup" <<
      elt->getPartGroupCombinedName () <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrPart& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrPart" <<
      elt->getPartCombinedName () <<
      endl;

  fCurrentPart = elt;
}

void lpsr2LilypondTranslator::visitEnd (S_msrPart& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrPart" <<
      elt->getPartCombinedName () <<
      endl;

  fCurrentPart = 0;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrStaff& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrStaff \"" <<
      elt->getStaffName () <<
      "\"" <<
      endl;
  
  fOnGoingStaff = true;
}

void lpsr2LilypondTranslator::visitEnd (S_msrStaff& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrStaff \"" <<
      elt->getStaffName () <<
      "\"" <<
      endl;
  
  fOnGoingStaff = false;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrStaffLinesNumber& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "%--> Start visiting msrStaffLinesNumber" <<
      endl;

  // fetch staff lines number
  int
    linesNumber =
      elt->getLinesNumber ();

  if (linesNumber != 5) { // default value
    fIndentedOutputStream <<
      endl <<
      "\\stopStaff " <<
      "\\override Staff.StaffSymbol.line-count = " <<
      linesNumber <<
      " \\startStaff" <<
      endl;
  }
}

void lpsr2LilypondTranslator::visitStart (S_msrStaffTuning& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "%--> Start visiting msrStaffTuning" <<
      endl;

/* JMI
  list<S_msrStaffTuning>
    staffTuningsList =
      staff->getStaffTuningsList ();
      
  if (staffTuningsList.size ()) {
    // \set TabStaff.stringTunings = \stringTuning <c' g' d'' a''>

    fIndentedOutputStream <<
      "\\set TabStaff.stringTunings = \\stringTuning <";

    list<S_msrStaffTuning>::const_iterator
      iBegin = staffTuningsList.begin(),
      iEnd   = staffTuningsList.end(),
      i      = iBegin;
      
    for ( ; ; ) {
      fIndentedOutputStream <<
        msrQuarterTonesPitchAsString (
          gLpsrOptions->fLpsrQuarterTonesPitchesLanguage,
 // JMI            elt->getInputLineNumber (),
          ((*i)->getStaffTuningQuarterTonesPitch ())) <<        
 // JMI       char (tolower ((*i)->getStaffTuningStep ())) <<
        absoluteOctaveAsLilypondString (
          (*i)->getStaffTuningOctave ());
      if (++i == iEnd) break;
      fIndentedOutputStream << " ";
    } // for

    fIndentedOutputStream <<
      ">" <<
      endl;
  }
 */
}

void lpsr2LilypondTranslator::visitStart (S_msrStaffDetails& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "%--> Start visiting msrStaffDetails" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrVoice& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrVoice \"" <<
      elt->getVoiceName () <<
      "\"" <<
      endl;

  fCurrentVoice = elt;
  
  fIndentedOutputStream <<
    fCurrentVoice->getVoiceName () << " = ";

  // generate the beginning of the voice definition
  switch (fCurrentVoice->getVoiceKind ()) {
    
    case msrVoice::kMasterVoice:
      msrInternalError (
        fCurrentVoice->getInputLineNumber (),
        "a master voice is not expected in lpsr2LilypondTranslator"); // JMI
      break;      
    
    case msrVoice::kRegularVoice:
      if (gLilypondOptions->fAbsoluteOctaves)
        fIndentedOutputStream <<
          "{" <<
          endl;
      else
        fIndentedOutputStream <<
          "\\relative {" <<
          endl;
      break;
      
    case msrVoice::kHarmonyVoice:
      fIndentedOutputStream <<
        "\\chordmode {" <<
        endl;
      break;
      
    case msrVoice::kFiguredBassVoice:
      fIndentedOutputStream <<
        "\\figuremode {" <<
        endl;
      break;
      
    case msrVoice::kSilentVoice:
      fIndentedOutputStream <<
        "{" <<
        endl;
      break;
  } // switch

  idtr++;

  if (gLilypondOptions->fGlobal) {
    fIndentedOutputStream <<
      "\\global" <<
      endl <<
      endl;
  }
    
  if (gLilypondOptions->fDisplayMusic) {
    fIndentedOutputStream <<
      "\\displayMusic {" <<
      endl;

    idtr++;
  }
    
  fIndentedOutputStream <<
    "\\language \"" <<
    msrQuarterTonesPitchesLanguageAsString (
      gLpsrOptions->fLpsrQuarterTonesPitchesLanguage) <<
    "\"" <<
    endl;

  if (gLpsrOptions->fLpsrChordsLanguage != k_IgnatzekChords)
    fIndentedOutputStream <<
      "\\" <<
      lpsrChordsLanguageAsString (
        gLpsrOptions->fLpsrChordsLanguage) <<
      "Chords" <<
      endl;

  if (gLilypondOptions->fShowAllBarNumbers)
    fIndentedOutputStream <<
      "\\set Score.barNumberVisibility = #all-bar-numbers-visible" <<
      endl <<
      "\\override Score.BarNumber.break-visibility = ##(#f #t #t)" <<
      endl <<
      endl;

// JMI   \set Score.alternativeNumberingStyle = #'numbers-with-letters


  if (
    fCurrentVoice->getVoiceContainsMultipleRests ()
      ||
    gLilypondOptions->fCompressMultiMeasureRests)
    fIndentedOutputStream <<
      "\\compressMMRests" <<
      endl <<
      endl;

  if (gLilypondOptions->fAccidentalStyle != kDefaultStyle)
    fIndentedOutputStream <<
      "\\accidentalStyle Score." <<
      lpsrAccidentalStyleAsString (
        gLilypondOptions->fAccidentalStyle) <<
      endl <<
      endl;

  fRelativeOctaveReference = 0;

  fVoiceIsCurrentlySenzaMisura = false;
  
  fOnGoingVoice = true;

  switch (fCurrentVoice->getVoiceKind ()) {
    case msrVoice::kMasterVoice:
      msrInternalError (
        elt->getInputLineNumber (),
        "a master voice is not expected in lpsr2LilypondTranslator"); // JMI
      break;      
    
    case msrVoice::kRegularVoice:
      break;
      
    case msrVoice::kHarmonyVoice:
      fOnGoingHarmonyVoice = true;
      break;
      
    case msrVoice::kFiguredBassVoice:
      fOnGoingFiguredBassVoice = true;
      break;
      
    case msrVoice::kSilentVoice:
      break;
  } // switch

  // force durations to be displayed explicitly
  fLastMetWholeNotes = rational (0, 1);
}

void lpsr2LilypondTranslator::visitEnd (S_msrVoice& elt)
{
  idtr--;

  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrVoice \"" <<
      elt->getVoiceName () <<
      "\"" <<
      endl;
  
  if (gLilypondOptions->fDisplayMusic) {
    fIndentedOutputStream <<
      "}" <<
      endl;

    idtr--;
  }
    
  // generate the end of the voice definition
  switch (elt->getVoiceKind ()) {
    case msrVoice::kMasterVoice:
      msrInternalError (
        elt->getInputLineNumber (),
        "a master voice is not expected in lpsr2LilypondTranslator"); // JMI
      break;      
    
    case msrVoice::kRegularVoice:
      fIndentedOutputStream <<
        "}" <<
        endl <<
        endl;
      break;
      
    case msrVoice::kHarmonyVoice:
      fIndentedOutputStream <<
        "}" <<
        endl <<
        endl;
      break;
      
    case msrVoice::kFiguredBassVoice:
      fIndentedOutputStream <<
        "}" <<
        endl <<
        endl;
      break;
      
    case msrVoice::kSilentVoice:
      fIndentedOutputStream <<
        "}" <<
        endl <<
        endl;
      break;
  } // switch

  switch (elt->getVoiceKind ()) {
    case msrVoice::kMasterVoice:
      msrInternalError (
        elt->getInputLineNumber (),
        "a master voice is not expected in lpsr2LilypondTranslator"); // JMI
      break;
      
    case msrVoice::kRegularVoice:
      break;
      
    case msrVoice::kHarmonyVoice:
      fOnGoingHarmonyVoice = false;
      break;
      
    case msrVoice::kFiguredBassVoice:
      fOnGoingFiguredBassVoice = false;
      break;
      
    case msrVoice::kSilentVoice:
      break;
  } // switch

  fCurrentVoice = 0;
  fOnGoingVoice = false;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrVoiceStaffChange& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    cerr <<
      "% --> Start visiting msrVoiceStaffChange '" <<
      elt->voiceStaffChangeAsString () << "'" <<
      endl;

  fIndentedOutputStream <<
    endl <<
    "\\change Staff=\"" <<
    elt->getNewStaff ()->getStaffNumber () <<
    "\"" <<
    endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrHarmony& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrHarmony '" <<
      elt->harmonyAsString () <<
      "'" <<
      endl;

  if (fOnGoingNote) {
    if (gGeneralOptions->fTraceHarmonies) {
      fIndentedOutputStream <<
        "%{ " << elt->harmonyAsString () << " %}" <<
        endl;
    }
  }
  
  else if (fOnGoingChord) { // JMI
    /*
    // register the harmony in the current chord clone
    fCurrentChord->
      setChordHarmony (elt); // JMI
      */
  }

  else if (fOnGoingHarmonyVoice) {
    // indent before the fist harmony of the msrSegment if needed
    if (++ fSegmentNotesAndChordsCountersStack.top () == 1)
      fIndentedOutputStream <<
        idtr;

    fIndentedOutputStream <<
      harmonyAsLilypondString (elt) <<
      " ";
      
    if (gLilypondOptions->fNoteInputLineNumbers)
      // print the harmony line number as a comment
      fIndentedOutputStream <<
        "%{ " << elt->getInputLineNumber () << " %} ";  
  }

/* JMI
  else if (fOnGoingChord) {
    // don't generate code for the code harmony,
    // this will be done after the chord itself JMI
  }
  */
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrFiguredBass& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrFiguredBass '" <<
      elt->figuredBassAsString () <<
      "'" <<
      endl;

  fCurrentFiguredBass = elt;
  
  if (fOnGoingNote) {
    fIndentedOutputStream <<
      "%{ " << fCurrentFiguredBass->figuredBassAsString () << " %}" <<
      endl;
  }
  
  else if (fOnGoingChord) { // JMI
    /*
    // register the figured bass in the current chord clone
    fCurrentChord->
      setChordFiguredBass (elt); // JMI
      */
  }

  else if (fOnGoingFiguredBassVoice) {
    fIndentedOutputStream <<
      "<";
      
    if (gLilypondOptions->fNoteInputLineNumbers)
      // print the figured bass line number as a comment
      fIndentedOutputStream <<
        "%{ " << fCurrentFiguredBass->getInputLineNumber () << " %} ";  
  }

/* JMI
  else if (fOnGoingChord) {
    // don't generate code for the code figured bass,
    // this will be done after the chord itself JMI
  }
  */

  fCurrentFiguredBassFiguresCounter = 0;
}

void lpsr2LilypondTranslator::visitStart (S_msrFigure& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrFigure '" <<
      elt->figureAsString () <<
      "'" <<
      endl;

  fCurrentFiguredBassFiguresCounter++;
  
  // is the figured bass parenthesized?
  msrFiguredBass::msrFiguredBassParenthesesKind
    figuredBassParenthesesKind =
      fCurrentFiguredBass->
        getFiguredBassParenthesesKind ();
        
  // generate the figure number
  switch (figuredBassParenthesesKind) {
    case msrFiguredBass::kFiguredBassParenthesesYes:
      fIndentedOutputStream << "[";
      break;
    case msrFiguredBass::kFiguredBassParenthesesNo:
      break;
  } // switch

  fIndentedOutputStream <<
    elt->getFigureNumber ();
    
  switch (figuredBassParenthesesKind) {
    case msrFiguredBass::kFiguredBassParenthesesYes:
      fIndentedOutputStream << "]";
      break;
    case msrFiguredBass::kFiguredBassParenthesesNo:
      break;
  } // switch

  // handle the figure prefix
  switch (elt->getFigurePrefixKind ()) {
    case msrFigure::k_NoFigurePrefix:
      break;
    case msrFigure::kDoubleFlatPrefix:
      fIndentedOutputStream << "--";
      break;
    case msrFigure::kFlatPrefix:
      fIndentedOutputStream << "-";
      break;
    case msrFigure::kFlatFlatPrefix:
      fIndentedOutputStream << "flat flat";
      break;
    case msrFigure::kNaturalPrefix:
      fIndentedOutputStream << "!";
      break;
    case msrFigure::kSharpSharpPrefix:
      fIndentedOutputStream << "sharp sharp";
      break;
    case msrFigure::kSharpPrefix:
      fIndentedOutputStream << "+";
      break;
    case msrFigure::kDoubleSharpPrefix:
      fIndentedOutputStream << "++";
      break;
  } // switch

  // handle the figure suffix
  switch (elt->getFigureSuffixKind ()) {
    case msrFigure::k_NoFigureSuffix:
      break;
    case msrFigure::kDoubleFlatSuffix:
      fIndentedOutputStream << "double flat";
      break;
    case msrFigure::kFlatSuffix:
      fIndentedOutputStream << "flat";
      break;
    case msrFigure::kFlatFlatSuffix:
      fIndentedOutputStream << "flat flat";
      break;
    case msrFigure::kNaturalSuffix:
      fIndentedOutputStream << "natural";
      break;
    case msrFigure::kSharpSharpSuffix:
      fIndentedOutputStream << "sharp sharp";
      break;
    case msrFigure::kSharpSuffix:
      fIndentedOutputStream << "sharp";
      break;
    case msrFigure::kDoubleSharpSuffix:
      fIndentedOutputStream << "souble sharp";
      break;
    case msrFigure::kSlashSuffix:
      fIndentedOutputStream << "/";
      break;
  } // switch

  // generate a space if not last figure in figured bass
  if (
    fCurrentFiguredBassFiguresCounter
      <
    fCurrentFiguredBass->getFiguredBassFiguresList ().size ()) {
    fIndentedOutputStream << " ";
  }
  }

void lpsr2LilypondTranslator::visitEnd (S_msrFiguredBass& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrFiguredBass '" <<
      elt->figuredBassAsString () <<
      "'" <<
      endl;

  fIndentedOutputStream <<
    ">" <<
    wholeNotesAsLilypondString (
      elt->getInputLineNumber (),
      elt->
        getFiguredBassSoundingWholeNotes ()) <<
    " ";
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrSegment& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "%--> Start visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << "'" <<
      endl;

  if (gLilypondOptions->fComments) {
    fIndentedOutputStream <<
      setw (commentFieldWidth) <<
      "% start of segment" <<
      endl;

    idtr++;
  }

  fSegmentNotesAndChordsCountersStack.push (0);
}

void lpsr2LilypondTranslator::visitEnd (S_msrSegment& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "%--> End visiting msrSegment '" <<
      elt->getSegmentAbsoluteNumber () << "'" <<
      endl;

  if (gLilypondOptions->fComments) {
    idtr--;
    
    fIndentedOutputStream <<
      setw (commentFieldWidth) <<
      "% end of segment" <<
      endl;
  }
  /* JMI
  else
    fIndentedOutputStream <<
      endl;
*/

  fSegmentNotesAndChordsCountersStack.pop ();
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrMeasure& elt)
{    
  int
    inputLineNumber =
      elt->getInputLineNumber ();

  string
    measureNumber =
      elt->getMeasureNumber ();

  // take this measure into account
  fMeasuresCounter++;
  
  // get measure kind
  msrMeasure::msrMeasureKind
    measureKind =
      elt->getMeasureKind ();
      
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fIndentedOutputStream <<
      "% --> Start visiting msrMeasure " <<
      measureNumber <<
      ", measureKind:" <<
      msrMeasure::measureKindAsString (measureKind) <<
      ", line " << inputLineNumber <<
      endl;
  }

  if (gLilypondOptions->fComments) {
    fIndentedOutputStream <<
      setw (commentFieldWidth) <<
      "% start of measure \"" << measureNumber << "\"" <<
      ", line " << inputLineNumber <<
      endl;

    idtr++;
  }

  // is this the end of a cadenza?
  if (
    fOnGoingVoiceCadenza
      &&
    measureKind != msrMeasure::kOverfullMeasureKind) {
    fIndentedOutputStream <<
      "\\cadenzaOff";

    if (gLilypondOptions->fComments)
      fIndentedOutputStream <<
        " % kOverfullMeasureKind End";

    fIndentedOutputStream<<
      endl;

    fOnGoingVoiceCadenza = false;
  }

  fSegmentNotesAndChordsCountersStack.push (0);

  switch (measureKind) {
    case msrMeasure::kUnknownMeasureKind:
      {
        stringstream s;

        s <<
          "measure '" << measureNumber <<
          "' is of unknown kind";

    // JMI     msrInternalError (
        msrInternalWarning (
          inputLineNumber, s.str ());
      }
      break;
      
    case msrMeasure::kFullMeasureKind:
      break;
      
    case msrMeasure::kUpbeatMeasureKind:
      {
        rational
          measureLength =
            elt->getMeasureLength ();

/* JMI
        fIndentedOutputStream <<
          "%{ measureLength: " << measureLength << " %}";
*/

        string
          upbeatDuration =
            wholeNotesAsLilypondString (
              inputLineNumber,
              elt->getMeasureLength ());

        fIndentedOutputStream <<
          "\\partial " << upbeatDuration <<
          endl;
      }
      break;
      
    case msrMeasure::kUnderfullMeasureKind:
      {
        rational
          measureLength =
            elt->getMeasureLength ();

        rational
          measureFullMeasureLength =
            elt->getMeasureFullMeasureLength ();

        // we should set the score measure length in this case
        rational
          ratioToFullLength =
            measureLength / measureFullMeasureLength;
        ratioToFullLength.rationalise ();
  
        if (gGeneralOptions->fTraceMeasures) {
          const int fieldWidth = 27;
          
          fIndentedOutputStream <<
            "% Setting the measure length for measure " <<
            measureNumber <<
            ", line = " << inputLineNumber <<
            endl <<
            "% measureLength" << " = " << measureLength <<
            endl <<
            "% measureFullMeasureLength" << " = " << measureFullMeasureLength <<
            endl <<
            "% ratioToFullLength" << " = " << ratioToFullLength <<
            endl <<
            endl;
        }

        if (ratioToFullLength == rational (1, 1)) {
          msrInternalError (
            inputLineNumber,
            "underfull measure is actuall the full measure length");
        }

        else {
          fIndentedOutputStream <<
            "\\set Score.measureLength = #(ly:make-moment " <<
            ratioToFullLength.toString () <<
            ")" <<
            endl;
    
          // should we generate a break?
          if (gLilypondOptions->fBreakLinesAtIncompleteRightMeasures)
            fIndentedOutputStream <<
              "\\break" <<
              endl;
        }
      }
      break;

    case msrMeasure::kOverfullMeasureKind:
      if (! fOnGoingVoiceCadenza) {
        fIndentedOutputStream <<
          "\\cadenzaOn";

        if (gLilypondOptions->fComments) {
          fIndentedOutputStream << " % kOverfullMeasureKind Start";
        }

        fIndentedOutputStream <<
          endl;

        fOnGoingVoiceCadenza = true;
      }
      break;
      
    case msrMeasure::kSenzaMisuraMeasureKind:
      if (! fOnGoingVoiceCadenza) {
        fIndentedOutputStream <<
          "\\cadenzaOn";

        if (gLilypondOptions->fComments) {
          fIndentedOutputStream << " % kSenzaMisuraMeasureKind Start";
        }

        fIndentedOutputStream <<
          endl;

        fOnGoingVoiceCadenza = true;
      }
      break;
      
    case msrMeasure::kEmptyMeasureKind:
      {
        // generate a rest the duration of the measure
        // followed by a bar check
        fIndentedOutputStream <<
          "R" <<
          wholeNotesAsLilypondString (
            inputLineNumber,
            elt->
              getMeasureFullMeasureLength ()) <<
          " | " <<
          endl;
      }
      break;
  } // switch
}

void lpsr2LilypondTranslator::visitEnd (S_msrMeasure& elt)
{
  int
    inputLineNumber =
      elt->getInputLineNumber ();

  string
    measureNumber =
      elt->getMeasureNumber ();
    
  // get measure kind
  msrMeasure::msrMeasureKind
    measureKind =
      elt->getMeasureKind ();
      
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrMeasure " <<
      measureNumber <<
      ", measureKind:" <<
      msrMeasure::measureKindAsString (measureKind) <<
      ", line " << elt->getInputLineNumber () <<
      endl;

  // handle the measure
  switch (measureKind) {
    case msrMeasure::kUnknownMeasureKind:
      break;
      
    case msrMeasure::kFullMeasureKind:
      break;
      
    case msrMeasure::kUpbeatMeasureKind:
      break;
      
    case msrMeasure::kUnderfullMeasureKind:
      fIndentedOutputStream <<
        "\\unset Score.measureLength" <<
        endl;
      break;

    case msrMeasure::kOverfullMeasureKind:
      fIndentedOutputStream <<
        endl <<
        "\\cadenzaOff" <<
        endl;

      fOnGoingVoiceCadenza = false;
      
/* JMI
      idtr--;
      fIndentedOutputStream <<
        "}" <<
        endl <<
        "\\bar \"|\"" <<
        endl;
        */
      break;

    case msrMeasure::kSenzaMisuraMeasureKind:
      fIndentedOutputStream <<
          "\\cadenzaOff" <<
          endl <<
          "\\bar \"|\"" <<
          endl;

      fOnGoingVoiceCadenza = false;
      break;

    case msrMeasure::kEmptyMeasureKind:
      break;
  } // switch
    
  if (gLilypondOptions->fComments) {
    idtr--;

    fIndentedOutputStream <<
      setw (commentFieldWidth) <<
      "% end of measure " <<
      measureNumber <<
      ", line " << inputLineNumber <<
      endl <<
      endl;      
  }

  if (gLilypondOptions->fSeparatorLineEveryNMeasures > 0) {
    if (
      fMeasuresCounter
        %
      gLilypondOptions->fSeparatorLineEveryNMeasures
        ==
      0)
      fIndentedOutputStream <<
        endl <<
        "% ============================= " <<
        endl <<
        endl;
  }
  
  fSegmentNotesAndChordsCountersStack.pop ();
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrStanza& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrStanza \"" <<
      elt->getStanzaName () <<
      "\"" <<
      endl;

  if (! gLilypondOptions->fNoLilypondLyrics) {
    // don't generate code for the stanza inside the code for the voice
    fOngoingNonEmptyStanza =
      ! fOnGoingVoice && elt->getStanzaTextPresent ();

    if (fOngoingNonEmptyStanza) {
      fIndentedOutputStream <<
        elt->getStanzaName () << " = " << "\\lyricmode" << " {" <<
        endl;
        
      idtr++;
    }
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrStanza& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrStanza \"" <<
      elt->getStanzaName () <<
      "\"" <<
      endl;

  if (! gLilypondOptions->fNoLilypondLyrics) {
    if (fOngoingNonEmptyStanza) {
      idtr--;
    
      fIndentedOutputStream <<
        endl <<
        "}" <<
        endl <<
        endl;
    }

    fOngoingNonEmptyStanza = false;
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrSyllable& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrSyllable '" <<
      elt->syllableAsString () <<
      "'" <<
      endl;

  if (! gLilypondOptions->fNoLilypondLyrics) {
    if (
    (gGeneralOptions->fTraceLyrics || fOngoingNonEmptyStanza)
  && ! fOnGoingVoice    ) { // JMI
      
      switch (elt->getSyllableKind ()) {
        case msrSyllable::kSingleSyllable:
          writeTextsListAsLilypondString (
            elt->getSyllableTextsList (),
            fIndentedOutputStream);
          fIndentedOutputStream <<
            elt->syllableWholeNotesAsMsrString () <<
            " ";
          break;
          
        case msrSyllable::kBeginSyllable:
          writeTextsListAsLilypondString (
            elt->getSyllableTextsList (),
            fIndentedOutputStream);
          fIndentedOutputStream <<
            elt->syllableWholeNotesAsMsrString () <<
            " ";
          break;
          
        case msrSyllable::kMiddleSyllable:
          fIndentedOutputStream <<
            "-- ";
          writeTextsListAsLilypondString (
            elt->getSyllableTextsList (),
            fIndentedOutputStream);
          fIndentedOutputStream <<
            elt->syllableWholeNotesAsMsrString () <<
            " ";
          break;
          
        case msrSyllable::kEndSyllable:
          fIndentedOutputStream <<
            "-- ";
          writeTextsListAsLilypondString (
            elt->getSyllableTextsList (),
            fIndentedOutputStream);
          fIndentedOutputStream <<
            elt->syllableWholeNotesAsMsrString () <<
            " ";
          break;
          
        case msrSyllable::kRestSyllable:
          writeTextsListAsLilypondString (
            elt->getSyllableTextsList (),
            fIndentedOutputStream);
          fIndentedOutputStream <<
            elt->syllableWholeNotesAsMsrString () <<
            " ";
/* JMI
          // LilyPond ignores the skip duration
          // when \lyricsto is used
          fIndentedOutputStream <<
 //           "%{rest" << elt->syllableWholeNotesAsMsrString () << "%} ";
            "\\skip" <<
            elt->syllableWholeNotesAsMsrString () <<
            " %{rest%} ";
          */ 
          break;
          
        case msrSyllable::kSkipSyllable:
          // LilyPond ignores the skip duration
          // when \lyricsto is used
          fIndentedOutputStream <<
            "\\skip" <<
            elt->syllableWholeNotesAsMsrString () <<
            " ";
          break;
          
        case msrSyllable::kMelismaFirstSyllable:
          /*
            A melisma can be defined entirely in the lyrics by entering
            a single underscore character, _, for every extra note
            that has to be added to the melisma.
          */
          fIndentedOutputStream <<
            "__ _ " << // JMI "%{" << elt->syllableWholeNotesAsMsrString () << "%} ";
            elt->syllableWholeNotesAsMsrString () << " ";
           break;
          
        case msrSyllable::kMelismaOtherSyllable:
          /*
            A melisma can be defined entirely in the lyrics by entering
            a single underscore character, _, for every extra note
            that has to be added to the melisma.
          */
          fIndentedOutputStream <<
            "_ " << // JMI "%{" << elt->syllableWholeNotesAsMsrString () << "%} ";
            elt->syllableWholeNotesAsMsrString () << " ";
          break;
          
        case msrSyllable::kTiedSyllable:
          fIndentedOutputStream <<
            "%{ ~ " << "\"";
          writeTextsListAsLilypondString (
            elt->getSyllableTextsList (),
            fIndentedOutputStream);
          fIndentedOutputStream <<
            "\"" << " %}" <<
            endl;
          break;
          
        case msrSyllable::kSlurSyllable:
          fIndentedOutputStream <<
            "%{ slur " << "\"";
          writeTextsListAsLilypondString (
            elt->getSyllableTextsList (),
            fIndentedOutputStream);
          fIndentedOutputStream <<
            "\"" << " %}" <<
            endl;
          break;
  
        case msrSyllable::kSlurBeyondEndSyllable:
  // JMI       fIndentedOutputStream <<
   //         "__ " << " ";
          break;
  
        case msrSyllable::kLigatureSyllable:
          fIndentedOutputStream <<
            "%{ ligature " << "\"";
          writeTextsListAsLilypondString (
            elt->getSyllableTextsList (),
            fIndentedOutputStream);
          fIndentedOutputStream <<
            "\"" << " %}" <<
            endl;
          break;
  
        case msrSyllable::kLigatureBeyondEndSyllable:
  // JMI       fIndentedOutputStream <<
   //         "__ " << " ";
          break;
  
        case msrSyllable::kBarcheckSyllable:
          fIndentedOutputStream <<
            "| %{ ";
          writeTextsListAsLilypondString (
            elt->getSyllableTextsList (),
            fIndentedOutputStream);
          fIndentedOutputStream <<
            " %}" <<
            endl;
          break;
    
        case msrSyllable::kBarNumberCheckSyllable:
          fIndentedOutputStream <<
            "%{ | ";
          writeTextsListAsLilypondString (
            elt->getSyllableTextsList (),
            fIndentedOutputStream);
          fIndentedOutputStream <<
            " %}" <<
            endl;
          break;
    
        case msrSyllable::kLineBreakSyllable:
          fIndentedOutputStream <<
            "%{ break " << "\"";
          writeTextsListAsLilypondString (
            elt->getSyllableTextsList (),
            fIndentedOutputStream);
          fIndentedOutputStream <<
            "\"" << " %}" <<
            endl;
          break;
    
        case msrSyllable::kPageBreakSyllable:
          fIndentedOutputStream <<
            "%{ pageBreak " << "\"";
          writeTextsListAsLilypondString (
            elt->getSyllableTextsList (),
            fIndentedOutputStream);
          fIndentedOutputStream <<
            "\"" << " %}" <<
            endl;
          break;
    
        case msrSyllable::k_NoSyllable: // JMI
          break;
      } // switch

      switch (elt->getSyllableExtendKind ()) {
        case msrSyllable::kStandaloneSyllableExtend:
          // generate a lyric extender after this syllable
          fIndentedOutputStream <<
            "__ ";
          break;
        case msrSyllable::kStartSyllableExtend:
          break;
        case msrSyllable::kContinueSyllableExtend:
          break;
        case msrSyllable::kStopSyllableExtend:
          break;
        case msrSyllable::k_NoSyllableExtend:
          break;
      } // switch
      
      if (gLilypondOptions->fNoteInputLineNumbers)
        // print the note line number as a comment
        fIndentedOutputStream <<
          "%{ " << elt->getInputLineNumber () << " %} ";  
    }
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrSyllable& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrSyllable '" <<
      elt->syllableAsString () <<
      "'" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrClef& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrClef '" <<
      elt->clefAsString () <<
      "'" <<
      endl;

  msrClef::msrClefKind
    clefKind =
      elt->getClefKind ();

  if (clefKind != msrClef::k_NoClef) {
    fIndentedOutputStream <<
      "\\clef" " \"";
  
    switch (clefKind) {
      case msrClef::k_NoClef:
        break;
      case msrClef::kTrebleClef:
        fIndentedOutputStream << "treble";
        break;
      case msrClef::kSopranoClef:
        fIndentedOutputStream << "soprano";
        break;
      case msrClef::kMezzoSopranoClef:
        fIndentedOutputStream << "mezzosoprano";
        break;
      case msrClef::kAltoClef:
        fIndentedOutputStream << "alto";
        break;
      case msrClef::kTenorClef:
        fIndentedOutputStream << "tenor";
        break;
      case msrClef::kBaritoneClef:
        fIndentedOutputStream << "baritone";
        break;
      case msrClef::kBassClef:
        fIndentedOutputStream << "bass";
        break;
      case msrClef::kTrebleLine1Clef:
        fIndentedOutputStream << "french";
        break;
      case msrClef::kTrebleMinus15Clef:
        fIndentedOutputStream << "treble_15";
        break;
      case msrClef::kTrebleMinus8Clef:
        fIndentedOutputStream << "treble_8";
        break;
      case msrClef::kTreblePlus8Clef:
        fIndentedOutputStream << "treble^8";
        break;
      case msrClef::kTreblePlus15Clef:
        fIndentedOutputStream << "treble^15";
        break;
      case msrClef::kBassMinus15Clef:
        fIndentedOutputStream << "bass_15";
        break;
      case msrClef::kBassMinus8Clef:
        fIndentedOutputStream << "bass_8";
        break;
      case msrClef::kBassPlus8Clef:
        fIndentedOutputStream << "bass^8";
        break;
      case msrClef::kBassPlus15Clef:
        fIndentedOutputStream << "bass^15";
        break;
      case msrClef::kVarbaritoneClef:
        fIndentedOutputStream << "varbaritone";
        break;
      case msrClef::kTablature4Clef:
        if (gLilypondOptions->fModernTab)
          fIndentedOutputStream << "moderntab";
        else
          fIndentedOutputStream << "tab";
        break;
      case msrClef::kTablature5Clef:
        if (gLilypondOptions->fModernTab)
          fIndentedOutputStream << "moderntab";
        else
          fIndentedOutputStream << "tab";
        break;
      case msrClef::kTablature6Clef:
        if (gLilypondOptions->fModernTab)
          fIndentedOutputStream << "moderntab";
        else
          fIndentedOutputStream << "tab";
        break;
      case msrClef::kTablature7Clef:
        if (gLilypondOptions->fModernTab)
          fIndentedOutputStream << "moderntab";
        else
          fIndentedOutputStream << "tab";
        break;
      case msrClef::kPercussionClef:
        fIndentedOutputStream << "percussion";
        break;
    } // switch
  
  fIndentedOutputStream <<
    "\"" <<
    endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrClef& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrClef '" <<
      elt->clefAsString () <<
      "'" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrKey& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrKey '" <<
      elt->keyAsString () <<
      "'" <<
      endl;

  switch (elt->getKeyKind ()) {
    case msrKey::kTraditionalKind:
      fIndentedOutputStream <<
        "\\key " <<
        msrQuarterTonesPitchAsString (
          gLpsrOptions->fLpsrQuarterTonesPitchesLanguage,
          elt->getKeyTonicQuarterTonesPitch ()) <<
        " \\" <<
        msrKey::keyModeKindAsString (
          elt->getKeyModeKind ()) <<
        endl;
      break;
      
    case msrKey::kHumdrumScotKind:
      {
        const vector<S_msrHumdrumScotKeyItem>&
          humdrumScotKeyItemsVector =
            elt->getHumdrumScotKeyItemsVector ();
        
        if (humdrumScotKeyItemsVector.size ()) {
          fIndentedOutputStream <<
            endl <<
            "\\set Staff.keyAlterations = #`(";

          vector<S_msrHumdrumScotKeyItem>::const_iterator
            iBegin = humdrumScotKeyItemsVector.begin(),
            iEnd   = humdrumScotKeyItemsVector.end(),
            i      = iBegin;
            
          for ( ; ; ) {
            S_msrHumdrumScotKeyItem item = (*i);
              
            if (elt->getKeyItemsOctavesAreSpecified ()) {
              //   JMI   "((octave . step) . alter) ((octave . step) . alter) ...)";
              //\set Staff.keyAlterations = #`(((3 . 3) . 7) ((3 . 5) . 3) ((3 . 6) . 3))"  \time 2/4
    
                                      
                fIndentedOutputStream <<
                  "(" <<
                    "(" <<
                    item->getKeyItemOctave () - 3 <<
                      // in MusicXML, octave number is 4 for the octave
                      // starting with middle C,
                      // and the latter is c' in LilyPond
                    " . " <<
                    item->getKeyItemDiatonicPitch () <<
                    ")" <<
                  " . ," <<
                  alterationAsLilypondString (
                    item->getKeyItemAlteration ()) <<                
                  ")";
            }
  
            else {
              // Alternatively, for each item in the list, using the more concise format (step . alter) specifies that the same alteration should hold in all octaves.
                                      
                fIndentedOutputStream <<
                  "(" <<
                  item->getKeyItemDiatonicPitch () <<
                  " . ," <<
                  alterationAsLilypondString (
                    item->getKeyItemAlteration ()) <<                
                  ")";
            }
                
            if (++i == iEnd) break;
            
            fIndentedOutputStream << " ";
          } // for
  
          fIndentedOutputStream <<
            ")";
        }
        
        else {
            msrInternalError (
              elt->getInputLineNumber (),
              "Humdrum/Scot key items vector is empty");
        }
      }
      break;
  } // switch
}

void lpsr2LilypondTranslator::visitEnd (S_msrKey& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrKey '" <<
      elt->keyAsString () <<
      "'"  <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTime& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrTime " <<
      elt->timeAsString () <<
      endl;

  msrTime::msrTimeSymbolKind
    timeSymbolKind =
      elt->getTimeSymbolKind ();
      
  const vector<S_msrTimeItem>&
    timeItemsVector =
      elt->getTimeItemsVector ();

  // is this the end of a senza misura fragment?
  if (
    fVoiceIsCurrentlySenzaMisura
      &&
    timeSymbolKind != msrTime::kTimeSymbolSenzaMisura) {
    fIndentedOutputStream <<
      "\\undo\\omit Staff.TimeSignature" <<
      endl;

    fVoiceIsCurrentlySenzaMisura = false;
  }

  // handle the time
  if (timeSymbolKind == msrTime::kTimeSymbolSenzaMisura) {

    // senza misura time
    
    fIndentedOutputStream <<
      "\\omit Staff.TimeSignature" <<
      endl;

    fVoiceIsCurrentlySenzaMisura = true;
  }

  else {

    // con misura time
    
    int timesItemsNumber =
      timeItemsVector.size ();
  
    if (timesItemsNumber) {
  
      if (! elt->getTimeIsCompound ()) {
  
        // simple time
        // \time "3/4" for 3/4
        // or senza misura
        
        S_msrTimeItem
          timeItem =
            timeItemsVector [0]; // the only element;
    
        // fetch the time item beat numbers vector
        const vector<int>&
          beatsNumbersVector =
            timeItem->
              getTimeBeatsNumbersVector ();
    
        // should the time be numeric?
        if (
          timeSymbolKind == msrTime::k_NoTimeSymbol
            ||
          gLilypondOptions->fNumericalTime) {
          fIndentedOutputStream <<
            "\\numericTimeSignature ";
        }
  
        // should there be a single number?
        switch (timeSymbolKind) {
          case msrTime::kTimeSymbolCommon:
            break;
          case msrTime::kTimeSymbolCut:
            break;
          case msrTime::kTimeSymbolNote:
            break;
          case msrTime::kTimeSymbolDottedNote:
            break;
          case msrTime::kTimeSymbolSingleNumber:
            fIndentedOutputStream <<
              "\\once\\override Staff.TimeSignature.style = #'single-digit" <<
              endl;
            break;
          case msrTime::kTimeSymbolSenzaMisura:
            break;
          case msrTime::k_NoTimeSymbol:
            break;
        } // switch
  
        /* dotted-note: JMI
  \relative {
  \override Staff.TimeSignature.stencil = #(lambda (grob)
                                             (grob-interpret-markup grob #{
                                               \markup\override #'(baseline-skip . 0.5) {
                                                 \column { \number 2 \tiny\note #"4." #-1 }
                                                 \vcenter "="
                                                 \column { \number 6 \tiny\note #"8" #-1 }
                                                 \vcenter "="
                                                 \column\number { 6 8 }
                                               }
                                               #}))
                                               *
     * ou plus simle
     * #(define-public (format-time-sig-note grob)
  (let* ((frac (ly:grob-property grob 'fraction))
  (num (if (pair? frac) (car frac) 4))
  (den (if (pair? frac) (cdr frac) 4))
  (m (markup #:override '(baseline-skip . 0.5)
  #:center-column (#:number (number->string num)
  #'" "
  #:override '(style . default)
  #:note (number->string den) DOWN))))
  (grob-interpret-markup grob m)))
  
         */
        fIndentedOutputStream <<
          "\\time " <<
          beatsNumbersVector [0] << // the only element
          "/" <<
          timeItem->getTimeBeatValue () <<
          endl;
      }
        
      else {
  
        // compound time
        // \compoundMeter #'(3 2 8) for 3+2/8
        // \compoundMeter #'((3 8) (2 8) (3 4)) for 3/8+2/8+3/4  
        // \compoundMeter #'((3 2 8) (3 4)) for 3+2/8+3/4
    
        fIndentedOutputStream <<
          "\\compoundMeter #`(";
        
        // handle all the time items in the vector
        for (int i = 0; i < timesItemsNumber; i++) {
          S_msrTimeItem
            timeItem =
              timeItemsVector [i];
      
          // fetch the time item beat numbers vector
          const vector<int>&
            beatsNumbersVector =
              timeItem->
                getTimeBeatsNumbersVector ();
      
          int beatsNumbersNumber =
            beatsNumbersVector.size ();
            
          // first generate the opening parenthesis
          fIndentedOutputStream <<
            "(";
  
          // then generate all beats numbers in the vector
          for (int j = 0; j < beatsNumbersNumber; j++) {
            fIndentedOutputStream <<
              beatsNumbersVector [j] <<
              " ";
          } // for
      
          // then generate the beat type
          fIndentedOutputStream <<
            timeItem->getTimeBeatValue ();
  
          // and finally generate the closing parenthesis
          fIndentedOutputStream <<
            ")";
  
          if (i != timesItemsNumber - 1)
            fIndentedOutputStream <<
              " ";
        } // for
              
      fIndentedOutputStream <<
        ")" <<
        endl;
      }
    }
    
    else {
      if (timeSymbolKind != msrTime::kTimeSymbolSenzaMisura) {
        msrInternalError (
          elt->getInputLineNumber (),
          "time items vector is empty");
      }
    }
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrTime& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrTime " <<
      elt->timeAsString () <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTranspose& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrTranspose" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  int  transposeDiatonic  = elt->getTransposeDiatonic ();
  int  transposeChromatic = elt->getTransposeChromatic ();
  int  transposeOctaveChange = elt->getTransposeOctaveChange ();
  bool transposeDouble = elt->getTransposeDouble ();

/*
  // transposition in LilyPond is relative to c',
  // i.e. the C in the middle of the piano keyboard

The diatonic element specifies the number of pitch steps needed to go from written to sounding pitch. This allows for correct spelling of enharmonic transpositions

The chromatic element represents the number of semitones needed to get from written to sounding pitch. This value does not include octave-change values; the values for both elements need to be added to the written pitch to get the correct sounding pitch.

The octave-change element indicates how many octaves to add to get from written pitch to sounding pitch.

If the double element is present, it indicates that the music is doubled one octave down from what is currently written (as is the case for mixed cello / bass parts in orchestral literature).
*/
  
  // determine transposition pitch
  msrQuarterTonesPitch transpositionPitch;
  
  switch (transposeChromatic) {
    case -11:
      switch (transposeDiatonic) {
        case -7:
          transpositionPitch = k_cSharp;
          break;

        case -6:
          transpositionPitch = k_dFlat;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;
      
    case -10:
      switch (transposeDiatonic) {
        case -6:
          transpositionPitch = k_dNatural;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;
      
    case -9:
      switch (transposeDiatonic) {
        case -6:
          transpositionPitch = k_dSharp;
          break;

        case -5:
          transpositionPitch = k_eFlat;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;
      
    case -8:
      switch (transposeDiatonic) {
        case -5:
          transpositionPitch = k_eNatural;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;
      
    case -7:
      switch (transposeDiatonic) {
        case -4:
          transpositionPitch = k_fNatural;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;
      
    case -6:
      switch (transposeDiatonic) {
        case -4:
          transpositionPitch = k_fSharp;
          break;

        case -3:
          transpositionPitch = k_gFlat;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;
      
    case -5:
      switch (transposeDiatonic) {
        case -3:
          transpositionPitch = k_gNatural;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;
      
    case -4:
      switch (transposeDiatonic) {
        case -3:
          transpositionPitch = k_gSharp;
          break;

        case -2:
          transpositionPitch = k_aFlat;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;
      
    case -3:
      switch (transposeDiatonic) {
        case -2:
          transpositionPitch = k_aNatural;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;
      
    case -2:
      switch (transposeDiatonic) {
        case -2:
          transpositionPitch = k_aSharp;
          break;

        case -1:
          transpositionPitch = k_bFlat;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;
      
    case -1:
      switch (transposeDiatonic) {
        case -1:
          transpositionPitch = k_bNatural;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;
      
    case 0:
      switch (transposeDiatonic) {
        case 0:
          transpositionPitch = k_cNatural;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    case 1:
      switch (transposeDiatonic) {
        case 0:
          transpositionPitch = k_cSharp;
          break;

        case 1:
          transpositionPitch = k_dFlat;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;
      
    case 2:
      switch (transposeDiatonic) {
        case 1:
          transpositionPitch = k_dNatural;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;
      
    case 3:
      switch (transposeDiatonic) {
        case 1:
          transpositionPitch = k_dSharp;
          break;

        case 2:
          transpositionPitch = k_eFlat;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;
      
    case 4:
      switch (transposeDiatonic) {
        case 2:
          transpositionPitch = k_eNatural;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;
      
    case 5:
      switch (transposeDiatonic) {
        case 3:
          transpositionPitch = k_fNatural;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;
      
    case 6:
      switch (transposeDiatonic) {
        case 3:
          transpositionPitch = k_fSharp;
          break;

        case 4:
          transpositionPitch = k_gFlat;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;
      
    case 7:
      switch (transposeDiatonic) {
        case 4:
          transpositionPitch = k_gNatural;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;
      
    case 8:
      switch (transposeDiatonic) {
        case 4:
          transpositionPitch = k_gSharp;
          break;

        case 5:
          transpositionPitch = k_aFlat;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;
      
    case 9:
      switch (transposeDiatonic) {
        case 5:
          transpositionPitch = k_aNatural;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;
      
    case 10:
      switch (transposeDiatonic) {
        case 5:
          transpositionPitch = k_aSharp;
          break;

        case 6:
          transpositionPitch = k_bFlat;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;
      
    case 11:
      switch (transposeDiatonic) {
        case 6:
          transpositionPitch = k_bNatural;
          break;

        default:
          transposeDiatonicError (
            inputLineNumber,
            transposeDiatonic,
            transposeChromatic);
      } // switch
      break;

    default:
      {
        stringstream s;
      
        s <<
          "transpose chromaticic '" << transposeChromatic <<
          "' is not between -11 and 11";
          
        msrMusicXMLError (
          inputLineNumber,
          s.str ());
      }
  } // switch

  // 4 is the octave starting with middle C
  int transpositionOctave;
  
  if (transposeChromatic < 0)
    transpositionOctave = 3;
  else
    transpositionOctave = 4;

  // take the transpose octave change into account
  transpositionOctave += transposeOctaveChange;

  // take the transpose double if any into account
  if (transposeDouble)
    transpositionOctave--;

  string
    transpositionPitchAsString =
      msrQuarterTonesPitchAsString (
        gLpsrOptions->fLpsrQuarterTonesPitchesLanguage,
        transpositionPitch);

  string
    transitionOctaveAsString =
      absoluteOctaveAsLilypondString (
        transpositionOctave);

  if (gGeneralOptions->fTraceTranspositions) {
    cerr << idtr << // JMI
      "Handlling transpose '" <<
      elt->transposeAsString () <<
      "' ignored because it is already present in voice \"" <<
      fCurrentVoice->getVoiceName () <<
      "\"" <<
      /* JMI
      getStaffName () <<
      "\" in part " <<
      fStaffPartUplink->getPartCombinedName () <<
      */
      endl <<
      ", transpositionPitch: " <<
      transpositionPitchAsString <<
      ", transpositionOctave: " <<
      transitionOctaveAsString <<
      "(" << transpositionOctave << ")" <<
      endl;
    }

  // now we can generate the transpostion command
  fIndentedOutputStream <<
    "\\transposition " <<
    transpositionPitchAsString <<
    transitionOctaveAsString <<
    " " <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrTranspose& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrTranspose" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTempo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrTempo" <<
      endl;

  string tempoIndication = elt->getTempoIndication ();
  
  int tempoUnit = elt->getTempoUnit ();
  int perMinute = elt->getPerMinute ();

  fIndentedOutputStream <<
    "\\tempo";

  if (tempoIndication.size ())
    fIndentedOutputStream <<
      " \"" << tempoIndication << "\"";

  if (tempoUnit)
    fIndentedOutputStream <<
      " " << tempoUnit << " = " << perMinute;

  fIndentedOutputStream <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrTempo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrTempo" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrArticulation& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrArticulation" <<
      endl;

  // don't generate the articulation here,
  // the note or chord will do it in its visitEnd() method
}

void lpsr2LilypondTranslator::visitEnd (S_msrArticulation& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrArticulation" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrFermata& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrFermata" <<
      endl;

/*
Articulations can be attached to rests as well as notes but they cannot be attached to multi-measure rests. A special predefined command, \fermataMarkup, is available for at- taching a fermata to a multi-measure rest (and only a multi-measure rest). This creates a MultiMeasureRestText object.
*/

/* JMI
  switch (elt->getArticulationPlacement ()) {
    case k_NoPlacement:
      // nothing needed
      break;
    case kAbovePlacement:
      fIndentedOutputStream << "^";
      break;
    case kBelowPlacement:
      fIndentedOutputStream << "_";
      break;
  } // switch
*/
      
  switch (elt->getFermataType ()) {
    case msrFermata::k_NoFermataType:
      // no markup needed
      break;
    case msrFermata::kUprightFermataType:
      // no markup needed
      break;
    case msrFermata::kInvertedFermataType:
      fIndentedOutputStream << "_";
      break;
  } // switch

  switch (elt->getFermataKind ()) {
    case msrFermata::kNormalFermataKind:
      fIndentedOutputStream << "\\fermata";
      break;
    case msrFermata::kAngledFermataKind:
      fIndentedOutputStream << "\\shortfermata";
      break;
    case msrFermata::kSquareFermataKind:
      fIndentedOutputStream << "\\longfermata";
      break;
  } // switch
}

void lpsr2LilypondTranslator::visitEnd (S_msrFermata& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrFermata" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrArpeggiato& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrArpeggiato" <<
      endl;

/* VIRER JMI
  */
}

void lpsr2LilypondTranslator::visitEnd (S_msrArpeggiato& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrArpeggiato" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrNonArpeggiato& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrNonArpeggiato" <<
      endl;
/* VIRER JMI
  */
}

void lpsr2LilypondTranslator::visitEnd (S_msrNonArpeggiato& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrNonArpeggiato" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTechnical& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrTechnical" <<
      endl;

  // don't generate the technical here,
  // the note or chord will do it in its visitEnd() method
}

void lpsr2LilypondTranslator::visitEnd (S_msrTechnical& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrTechnical" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTechnicalWithInteger& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrTechnicalWithInteger" <<
      endl;

  // don't generate the technicalWithInteger here,
  // the note or chord will do it in its visitEnd() method
}

void lpsr2LilypondTranslator::visitEnd (S_msrTechnicalWithInteger& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrTechnicalWithInteger" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTechnicalWithString& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrTechnicalWithString" <<
      endl;

  // don't generate the technicalWithString here,
  // the note or chord will do it in its visitEnd() method
}

void lpsr2LilypondTranslator::visitEnd (S_msrTechnicalWithString& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrTechnicalWithString" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrOrnament& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrOrnament" <<
      endl;

  // don't generate the ornament here,
  // the note or chord will do it in its visitEnd() method
}

void lpsr2LilypondTranslator::visitEnd (S_msrOrnament& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrOrnament" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrSingleTremolo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrSingleTremolo" <<
      endl;

  // don't generate the singleTremolo here,
  // the note or chord will do it in its visitEnd() method
}

void lpsr2LilypondTranslator::visitEnd (S_msrSingleTremolo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrSingleTremolo" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrDoubleTremolo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrDoubleTremolo" <<
      endl;

  // get double tremolo number of repeats
  int numberOfRepeats =
    elt->getDoubleTremoloNumberOfRepeats ();
    
  // the marks number determines the duration of the two elements:
  // '8' for 1, '16' for 2, etc
  fCurrentDoubleTremoloElementsLpsrDuration =
    int (
      pow (
        2,
        elt->getDoubleTremoloMarksNumber () + 2));

  if (gGeneralOptions->fTraceTremolos) {
    fIndentedOutputStream <<
      "% visitStart (S_msrDoubleTremolo&)" <<
      endl <<
      tab << "% doubleTremoloSoundingWholeNotes = " <<
        elt->getDoubleTremoloNumberOfRepeats () <<
        endl <<
      tab << "% fCurrentDoubleTremoloElementsLpsrDuration = " <<
        fCurrentDoubleTremoloElementsLpsrDuration <<
        endl <<
      tab << "% numberOfRepeats = " << numberOfRepeats <<
      endl;
  }
  
  fIndentedOutputStream <<
    "\\repeat tremolo " << numberOfRepeats <<
    " {" <<
    endl;

  idtr++;
}

void lpsr2LilypondTranslator::visitEnd (S_msrDoubleTremolo& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrDoubleTremolo" <<
      endl;

  idtr--;
  
  fIndentedOutputStream <<
    endl <<
    "}" <<
    endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrDynamics& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrDynamics" <<
      endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrDynamics& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrDynamics" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrOtherDynamics& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrOtherDynamics" <<
      endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrOtherDynamics& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrOtherDynamics" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrWords& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrWords" <<
      endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrWords& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrWords" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrSlur& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrSlur" <<
      endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrSlur& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrSlur" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrLigature& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrLigature" <<
      endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrLigature& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrLigature" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrWedge& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrWedge" <<
      endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrWedge& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrWedge" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrGraceNotes& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrGraceNotes" <<
      endl;

  if (elt->getGraceNotesIsSlashed ())
    fIndentedOutputStream <<
      "\\slashedGrace"; // JMI "\\grace";
  else
    fIndentedOutputStream <<
      "\\grace"; // JMI "\\acciaccatura" \\appoggiatura
  fIndentedOutputStream << " { ";
}

void lpsr2LilypondTranslator::visitEnd (S_msrGraceNotes& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrGraceNotes" <<
      endl;

  fIndentedOutputStream <<
    "} ";
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrAfterGraceNotes& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrAfterGraceNotes" <<
      endl;

 // JMI exists? if (elt->getGraceNotesIsSlashed ())
  fIndentedOutputStream <<
    "\\afterGrace ";

/* JMI
  printNoteAsLilypondString (
    elt->getAfterGraceNotesNote ());

  fIndentedOutputStream <<
    " { ";
    */
}

void lpsr2LilypondTranslator::visitEnd (S_msrAfterGraceNotes& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrAfterGraceNotes" <<
      endl;

  fIndentedOutputStream <<
    "} ";
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrNote& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fIndentedOutputStream <<
      "% --> Start visiting ";
      
    switch (elt->getNoteKind ()) {

      case msrNote::k_NoNoteKind:
        break;
        
      case msrNote::kRestNote:        
        fIndentedOutputStream << "rest";
        break;
        
      case msrNote::kSkipNote:        
        fIndentedOutputStream << "skip";
        break;
        
      case msrNote::kStandaloneNote:
        fIndentedOutputStream << "standalone";
        break;
        
      case msrNote::kDoubleTremoloMemberNote:
        fIndentedOutputStream << "double tremolo member note";
        break;
        
      case msrNote::kGraceNote:
        fIndentedOutputStream << "grace";
        break;
        
      case msrNote::kChordMemberNote:
        fIndentedOutputStream << "chord member";
        break;
        
      case msrNote::kTupletMemberNote:
        fIndentedOutputStream << "tuplet member";
        break;
    } // switch
    
    fIndentedOutputStream << " msrNote" <<
      endl;
  }

  // should the note be parenthesized?
  msrNote::msrNoteHeadParenthesesKind
    noteHeadParenthesesKind =
      elt->getNoteHeadParenthesesKind ();

  switch (noteHeadParenthesesKind) {
    case msrNote::kNoteHeadParenthesesYes:
      fIndentedOutputStream << "\\parenthesize ";
      break;
    case msrNote::kNoteHeadParenthesesNo:
      break;
  } // switch

  // handling note head
  msrNote::msrNoteHeadKind
    noteHeadKind =
      elt->getNoteHeadKind ();
 
  switch (noteHeadKind) {
    case msrNote::kNoteHeadSlash:
      fIndentedOutputStream << "\\tweak style #'slash ";
      break;
    case msrNote::kNoteHeadTriangle:
      fIndentedOutputStream << "\\tweak style #'triangle ";
      break;
    case msrNote::kNoteHeadDiamond:
      fIndentedOutputStream << "\\tweak style #'diamond ";
      break;
    case msrNote::kNoteHeadSquare:
      fIndentedOutputStream << "\\tweak style #'la ";
      break;
    case msrNote::kNoteHeadCross:
      fIndentedOutputStream << "\\tweak style #'cross ";
      break;
    case msrNote::kNoteHeadX:
      fIndentedOutputStream << "\\tweak style #'cross ";
      break;
    case msrNote::kNoteHeadCircleX:
      fIndentedOutputStream << "\\tweak style #'xcircle ";
      break;
    case msrNote::kNoteHeadInvertedTriangle:
      fIndentedOutputStream << "%{kNoteHeadInvertedTriangle%} ";
      break;
    case msrNote::kNoteHeadArrowDown:
      fIndentedOutputStream << "%{kNoteHeadArrowDown%} ";
      break;
    case msrNote::kNoteHeadArrowUp:
      fIndentedOutputStream << "%{kNoteHeadArrowUp%} ";
      break;
    case msrNote::kNoteHeadSlashed:
      fIndentedOutputStream << "%{kNoteHeadSlashed%} ";
      break;
    case msrNote::kNoteHeadBackSlashed:
      fIndentedOutputStream << "%{kNoteHeadBackSlashed%} ";
      break;
    case msrNote::kNoteHeadNormal:
 // JMI     fIndentedOutputStream << "%{kNoteHeadNormal%} ";
      break;
    case msrNote::kNoteHeadCluster:
      fIndentedOutputStream << "%{kNoteHeadCluster%} ";
      break;
    case msrNote::kNoteHeadCircleDot:
      fIndentedOutputStream << "%{kNoteHeadCircleDot%} ";
      break;
    case msrNote::kNoteHeadLeftTriangle:
      fIndentedOutputStream << "%{kNoteHeadLeftTriangle%} ";
      break;
    case msrNote::kNoteHeadRectangle:
      fIndentedOutputStream << "%{kNoteHeadRectangle%} ";
      break;
    case msrNote::kNoteHeadNone:
      fIndentedOutputStream << "\\once\\omit NoteHead ";
      break;
    case msrNote::kNoteHeadDo:
      fIndentedOutputStream << "\\tweak style #'do ";
      break;
    case msrNote::kNoteHeadRe:
      fIndentedOutputStream << "\\tweak style #'re ";
      break;
    case msrNote::kNoteHeadMi:
      fIndentedOutputStream << "\\tweak style #'mi ";
      break;
    case msrNote::kNoteHeadFa:
      fIndentedOutputStream << "\\tweak style #'fa ";
      break;
    case msrNote::kNoteHeadFaUp:
      fIndentedOutputStream << "\\tweak style #'triangle ";
      break;
    case msrNote::kNoteHeadSo:
      fIndentedOutputStream << "\\tweak style #'sol ";
      break;
    case msrNote::kNoteHeadLa:
      fIndentedOutputStream << "\\tweak style #'la ";
      break;
    case msrNote::kNoteHeadTi:
      fIndentedOutputStream << "\\tweak style #'ti ";
      break;
  } // switch

  // print the note as a LilyPond string
  printNoteAsLilypondString (elt);

  fOnGoingNote = true;
}

void lpsr2LilypondTranslator::visitEnd (S_msrNote& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    fIndentedOutputStream <<
      "% --> End visiting " <<
      msrNote::noteKindAsString (elt->getNoteKind ()) <<
      " msrNote" <<
      endl;
  }

  // fetch the note single tremolo
  S_msrSingleTremolo
    noteSingleTremolo =
      elt->getNoteSingleTremolo ();

  if (noteSingleTremolo) {
    // generate code for the single tremolo only
    // if note doesn't belong to a chord,
    // otherwise it will be generated for the chord itself
    if (! elt->getNoteBelongsToAChord ()) {
      fIndentedOutputStream <<
        singleTremoloDurationAsLilypondString (
          noteSingleTremolo);
    }
  }

  // get note stem kind 
  msrStem::msrStemKind
    stemKind = // JMI
      fCurrentStem
        ? fCurrentStem->getStemKind ()
        : msrStem::k_NoStem;

/* JMI
  // has the stem been omitted?
  if (elt->getNoteIsStemless ()) {
    fIndentedOutputStream <<
      endl;
  }
  */
  
  if (gLilypondOptions->fNoteInputLineNumbers)
    // print the note line number as a comment
    fIndentedOutputStream <<
      "%{ " << elt->getInputLineNumber () << " %} ";
  
  // print the note articulations if any
  list<S_msrArticulation>
    noteArticulations =
      elt->getNoteArticulations ();
      
  if (noteArticulations.size()) {
    list<S_msrArticulation>::const_iterator i;
    for (
      i=noteArticulations.begin();
      i!=noteArticulations.end();
      i++) {
      fIndentedOutputStream <<
        noteArticulationAsLilyponString ((*i)) <<    
        " ";
    } // for
  }

  // print the note technicals if any
  list<S_msrTechnical>
    noteTechnicals =
      elt->getNoteTechnicals ();
      
  if (noteTechnicals.size()) {
    list<S_msrTechnical>::const_iterator i;
    for (
      i=noteTechnicals.begin();
      i!=noteTechnicals.end();
      i++) {
        
      fIndentedOutputStream <<
        technicalAsLilypondString ((*i));

      switch ((*i)->getTechnicalPlacementKind ()) {
        case msrTechnical::k_NoTechnicalPlacement:
          break;
        case msrTechnical::kTechnicalPlacementAbove:
 // JMI         fIndentedOutputStream << "^";
          break;
        case msrTechnical::kTechnicalPlacementBelow:
 // JMI         fIndentedOutputStream << "_";
          break;
      } // switch

      fIndentedOutputStream << " ";
    } // for
  }

  // print the note technicals if any
  list<S_msrTechnicalWithInteger>
    noteTechnicalWithIntegers =
      elt->getNoteTechnicalWithIntegers ();
      
  if (noteTechnicalWithIntegers.size()) {
    list<S_msrTechnicalWithInteger>::const_iterator i;
    for (
      i=noteTechnicalWithIntegers.begin();
      i!=noteTechnicalWithIntegers.end();
      i++) {
        
      fIndentedOutputStream <<
        technicalWithIntegerAsLilypondString ((*i));

      switch ((*i)->getTechnicalWithIntegerPlacementKind ()) {
        case msrTechnicalWithInteger::k_NoTechnicalWithIntegerPlacement:
          break;
        case msrTechnicalWithInteger::kTechnicalWithIntegerPlacementAbove:
 // JMI         fIndentedOutputStream << "^";
          break;
        case msrTechnicalWithInteger::kTechnicalWithIntegerPlacementBelow:
 // JMI         fIndentedOutputStream << "_";
          break;
      } // switch

      fIndentedOutputStream << " ";
    } // for
  }

  // print the note technicals if any
  list<S_msrTechnicalWithString>
    noteTechnicalWithStrings =
      elt->getNoteTechnicalWithStrings ();
      
  if (noteTechnicalWithStrings.size()) {
    list<S_msrTechnicalWithString>::const_iterator i;
    for (
      i=noteTechnicalWithStrings.begin();
      i!=noteTechnicalWithStrings.end();
      i++) {
        
      fIndentedOutputStream <<
        technicalWithStringAsLilypondString ((*i));

      switch ((*i)->getTechnicalWithStringPlacementKind ()) {
        case msrTechnicalWithString::k_NoTechnicalWithStringPlacement:
          break;
        case msrTechnicalWithString::kTechnicalWithStringPlacementAbove:
 // JMI         fIndentedOutputStream << "^";
          break;
        case msrTechnicalWithString::kTechnicalWithStringPlacementBelow:
 // JMI         fIndentedOutputStream << "_";
          break;
      } // switch

      fIndentedOutputStream << " ";
    } // for
  }

  // print the note ornaments if any
  list<S_msrOrnament>
    noteOrnaments =
      elt->getNoteOrnaments ();
      
  if (noteOrnaments.size()) {
    list<S_msrOrnament>::const_iterator i;
    for (
      i=noteOrnaments.begin();
      i!=noteOrnaments.end();
      i++) {        
      fIndentedOutputStream <<
        ornamentAsLilypondString ((*i)); // some ornaments are not yet supported
          // <<
        // JMI " ";

      switch ((*i)->getOrnamentPlacementKind ()) {
        case msrOrnament::k_NoOrnamentPlacement:
          break;
        case msrOrnament::kOrnamentPlacementAbove:
 // JMI         fIndentedOutputStream << "^";
          break;
        case msrOrnament::kOrnamentPlacementBelow:
 // JMI         fIndentedOutputStream << "_";
          break;
      } // switch

      fIndentedOutputStream <<
        alterationAsLilypondAccidentalMark (
          (*i)->getOrnamentAccidentalMark ()) <<
        " ";
    } // for
  }

  // print the note dynamics if any
  list<S_msrDynamics>
    noteDynamics =
      elt->getNoteDynamics ();
      
  if (noteDynamics.size()) {
    list<S_msrDynamics>::const_iterator i;
    for (
      i=noteDynamics.begin();
      i!=noteDynamics.end();
      i++) {
      S_msrDynamics
        dynamics = (*i);
        
      switch (dynamics->getDynamicsPlacementKind ()) {
        case msrDynamics::k_NoDynamicsPlacement:
          fIndentedOutputStream << "-3";
          break;
        case msrDynamics::kDynamicsPlacementAbove:
          fIndentedOutputStream << "^";
          break;
        case msrDynamics::kDynamicsPlacementBelow:
      // JMI    fIndentedOutputStream << "_";
          break;
      } // switch

      fIndentedOutputStream <<
        dynamicsAsLilypondString (dynamics) << " ";
    } // for
  }
  
  // print the note other dynamics if any
  list<S_msrOtherDynamics>
    noteOtherDynamics =
      elt->getNoteOtherDynamics ();
      
  if (noteOtherDynamics.size()) {
    list<S_msrOtherDynamics>::const_iterator i;
    for (
      i=noteOtherDynamics.begin();
      i!=noteOtherDynamics.end();
      i++) {
      fIndentedOutputStream <<
        "-\\markup { "
        "\\dynamic \"" << (*i)->getOtherDynamicsString () << "\" } ";
    } // for
  }

  // print the note words if any
  list<S_msrWords>
    noteWords =
      elt->getNoteWords ();
      
  if (noteWords.size()) {
    list<S_msrWords>::const_iterator i;
    for (
      i=noteWords.begin();
      i!=noteWords.end();
      i++) {
      msrWords::msrWordsPlacementKind
        wordsPlacementKind =
          (*i)->getWordsPlacementKind ();
    
      string wordsContents =
        (*i)->getWordsContents ();

      msrFontStyle
        wordsFontStyle =
          (*i)->getWordsFontStyle ();
        
      msrFontSize
        wordsFontSize =
          (*i)->getWordsFontSize ();
        
      msrFontWeight
        wordsFontWeight =
          (*i)->getWordsFontWeight ();

      string markup;
      
      {
        // create markup apart to have its length available
        stringstream s;
        
        switch (wordsPlacementKind) {
          case msrWords::kWordsPlacementAbove:
            s << "^";
            break;
          case msrWords::kWordsPlacementBelow:
            s << "_";
            break;
        } // switch
      
        s <<
          "\\markup" << " { ";

        switch (wordsFontStyle) {
          case k_NoFontStyle:
            break;
          case kNormalFontStyle:
            // LilyPond produces 'normal style' text by default
            break;
          case KItalicFontStyle:
            s <<
              "\\italic ";
            break;
        } // switch

        switch (wordsFontWeight) {
          case k_NoFontWeight:
            break;
          case kNormalFontWeight:
            // LilyPond produces 'normal weight' text by default
            break;
          case kBoldFontWeight:
            s <<
              "\\bold ";
            break;
        } // switch

        switch (wordsFontSize) {
          case k_NoFontSize:
            break;
          case kNormalFontSize:
            // LilyPond produces 'normal size' text by default
            break;
          case kLargeFontSize:
            s <<
              "\\larger ";
            break;
          case kMediumFontSize:
            s <<
              "\\large ";
            break;
          case kSmallFontSize:
            s <<
              "\\small ";
            break;
        } // switch

        s <<
 // JMI         quoteStringIfNonAlpha (wordsContents) <<
          "\"" << wordsContents << "\"" <<
          " } ";

        markup = s.str ();
        }

      fIndentedOutputStream <<
        markup;
    } // for
  }

  // print the note slurs if any
  list<S_msrSlur>
    noteSlurs =
      elt->getNoteSlurs ();
      
  if (noteSlurs.size()) {
    list<S_msrSlur>::const_iterator i;
    for (
      i=noteSlurs.begin();
      i!=noteSlurs.end();
      i++) {
        
      switch ((*i)->getSlurKind ()) {
        case msrSlur::k_NoSlur:
          break;
        case msrSlur::kStartSlur:
          fIndentedOutputStream << "( ";
          break;
        case msrSlur::kContinueSlur:
          break;
        case msrSlur::kStopSlur:
          fIndentedOutputStream << ") ";
          break;
      } // switch
    } // for
  }

  // print the note ligatures if any
  list<S_msrLigature>
    noteLigatures =
      elt->getNoteLigatures ();
      
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
   // JMI       fIndentedOutputStream << "\\[ ";
          break;
        case msrLigature::kContinueLigature:
          break;
        case msrLigature::kStopLigature:
          fIndentedOutputStream << "\\] ";
          break;
      } // switch
    } // for
  }

  // print the note wedges if any
  list<S_msrWedge>
    noteWedges =
      elt->getNoteWedges ();
      
  if (noteWedges.size()) {
    list<S_msrWedge>::const_iterator i;
    for (
      i=noteWedges.begin();
      i!=noteWedges.end();
      i++) {
        
      switch ((*i)->getWedgeKind ()) {
        case msrWedge::kCrescendoWedge:
          fIndentedOutputStream << "\\< ";
          break;
        case msrWedge::kDecrescendoWedge:
          fIndentedOutputStream << "\\> ";
          break;
        case msrWedge::kStopWedge:
          fIndentedOutputStream << "\\! ";
          break;
      } // switch
    } // for
  }

  if (false && elt->getNoteIsFollowedByGraceNotes ()) { // JMI
    if (! elt->getNoteIsARest ()) {
      fIndentedOutputStream <<
       " { % NoteIsFollowedByGraceNotes" <<
        endl; // JMI ???
    }
  }

  fOnGoingNote = false;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrOctaveShift& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrOctaveShift" <<
      endl;

  int octaveShiftSize =
    elt->getOctaveShiftSize (); // 8 or 15
    
  fIndentedOutputStream <<
    "\\ottava #";
    
  switch (elt->getOctaveShiftKind ()) {
    case msrOctaveShift::kOctaveShiftUp:
      fIndentedOutputStream << "-" << (octaveShiftSize - 1) / 7; // 1 or 2
      break;
    case msrOctaveShift::kOctaveShiftDown:
      fIndentedOutputStream << (octaveShiftSize - 1) / 7; // 1 or 2
      break;
    case msrOctaveShift::kOctaveShiftStop:
      fIndentedOutputStream << 0;
      break;
  } // switch
  
  fIndentedOutputStream << " ";
}

void lpsr2LilypondTranslator::visitEnd (S_msrOctaveShift& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrOctaveShift" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrAccordionRegistration& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrAccordionRegistration" <<
      endl;

  int highDotsNumber =
    elt->getHighDotsNumber ();
  int middleDotsNumber =
    elt->getMiddleDotsNumber ();
  int lowDotsNumber =
    elt->getLowDotsNumber ();

  string numbersToBeUsed;
  bool   nonZeroNumberHasBeenIssued = false;

  // the numbers should be written in the order 'high, middle, low'
  // and 0 ahead of the specification is forbidden

  if (highDotsNumber > 0) {
    numbersToBeUsed +=
      to_string (highDotsNumber);
    nonZeroNumberHasBeenIssued = true;
  }
  
  if (middleDotsNumber > 0) {
    numbersToBeUsed +=
      to_string (middleDotsNumber);
    nonZeroNumberHasBeenIssued = true;
  }
  else {
    if (nonZeroNumberHasBeenIssued)
      numbersToBeUsed +=
        to_string (middleDotsNumber);
  }
  
  numbersToBeUsed +=
    to_string (lowDotsNumber);
  
  fIndentedOutputStream <<
    "\\discant \"" <<
    numbersToBeUsed <<
    "\" ";
}

void lpsr2LilypondTranslator::visitStart (S_msrHarpPedalsTuning& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrHarpPedalsTuning" <<
      endl;

  map<msrDiatonicPitch, msrAlteration>
    harpPedalsAlterationsMap =
      elt->getHarpPedalsAlterationsMap ();
      
  if (harpPedalsAlterationsMap.size ()) {
    idtr++;

    fIndentedOutputStream <<
      "_\\markup { \\harp-pedal #\"" <<
      harpPedalTuningAsLilypondString (
        harpPedalsAlterationsMap [kD]) <<
      harpPedalTuningAsLilypondString (
        harpPedalsAlterationsMap [kC]) <<
      harpPedalTuningAsLilypondString (
        harpPedalsAlterationsMap [kB]) <<
      "|" <<
      harpPedalTuningAsLilypondString (
        harpPedalsAlterationsMap [kE]) <<
      harpPedalTuningAsLilypondString (
        harpPedalsAlterationsMap [kF]) <<
      harpPedalTuningAsLilypondString (
        harpPedalsAlterationsMap [kG]) <<
      harpPedalTuningAsLilypondString (
        harpPedalsAlterationsMap [kA]) <<
      "\" } ";
  }
  else {
    fIndentedOutputStream <<
      "%{empty harp pedals tuning???%} "; // JMI
  }
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrStem& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrStem" <<
      endl;

  fCurrentStem = elt;
}

void lpsr2LilypondTranslator::visitEnd (S_msrStem& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrStem" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrBeam& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrBeam" <<
      endl;

  // LilyPond will take care of multiple beams automatically,
  // so we need only generate code for the first number (level)
  switch (elt->getBeamKind ()) {
    
    case msrBeam::kBeginBeam:
      if (elt->getBeamNumber () == 1)
        fIndentedOutputStream << "[ ";
      break;
      
    case msrBeam::kContinueBeam:
      break;
      
    case msrBeam::kEndBeam:
      if (elt->getBeamNumber () == 1)
        fIndentedOutputStream << "] ";
      break;
      
    case msrBeam::kForwardHookBeam:
      break;
      
    case msrBeam::kBackwardHookBeam:
      break;
      
    case msrBeam::k_NoBeam:
      break;
  } // switch      
}

void lpsr2LilypondTranslator::visitEnd (S_msrBeam& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrBeam" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrChord& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrChord" <<
      endl;

  // print the chord ligatures if any
  list<S_msrLigature>
    chordLigatures =
      elt->getChordLigatures ();
      
  if (chordLigatures.size()) {
    list<S_msrLigature>::const_iterator i;
    for (
      i=chordLigatures.begin();
      i!=chordLigatures.end();
      i++) {
        
      switch ((*i)->getLigatureKind ()) {
        case msrLigature::k_NoLigature:
          break;
        case msrLigature::kStartLigature:
          fIndentedOutputStream << "\\[ ";
          break;
        case msrLigature::kContinueLigature:
          break;
        case msrLigature::kStopLigature:
  // JMI        fIndentedOutputStream << "\\] ";
          break;
      } // switch
    } // for
  }

  // don't take the chord into account for line breaking ??? JMI
  
  // print the chord arpeggios directions if any
  list<S_msrArticulation>
    chordArticulations =
      elt->getChordArticulations ();
      
  if (chordArticulations.size()) {
    list<S_msrArticulation>::const_iterator i;
    for (
      i=chordArticulations.begin();
      i!=chordArticulations.end();
      i++) {
      S_msrArticulation articulation = (*i);
        
      if (
        // arpeggiato?
        S_msrArpeggiato
          arpeggiato =
            dynamic_cast<msrArpeggiato*>(&(*articulation))
        ) {
        msrDirection
          direction =
            arpeggiato->getArpeggiatoDirection ();
        
        switch (direction) {
          case k_NoDirection:
            if (fCurrentArpeggioDirection != k_NoDirection)
              fIndentedOutputStream << "\\arpeggioNormal";
            break;
          case kUpDirection:
            fIndentedOutputStream << "\\arpeggioArrowUp";
            break;
          case kDownDirection:
            fIndentedOutputStream << "\\arpeggioArrowDown";
            break;
        } // switch
          
        fIndentedOutputStream << " ";

        fCurrentArpeggioDirection = direction;
      }
      
      else if (
        // non arpeggiato?
        S_msrNonArpeggiato
          nonArpeggiato =
            dynamic_cast<msrNonArpeggiato*>(&(*articulation))
        ) {
        fIndentedOutputStream << "\\arpeggioBracket";
        
        switch (nonArpeggiato->getNonArpeggiatoTypeKind ()) {
          case msrNonArpeggiato::k_NoNonArpeggiatoType:
            fIndentedOutputStream << "%{\\k_NoNonArpeggiatoType???%}";
            break;
          case msrNonArpeggiato::kNonArpeggiatoTypeTop:
            fIndentedOutputStream << "%{\\kNonArpeggiatoTypeTop???%}";
            break;
          case msrNonArpeggiato::kNonArpeggiatoTypeBottom:
            fIndentedOutputStream << "%{\\kNonArpeggiatoTypeBottom???%}";
            break;
        } // switch
          
        fIndentedOutputStream << " ";
      }
   } // for
  }

  fIndentedOutputStream << "<";
}

void lpsr2LilypondTranslator::visitEnd (S_msrChord& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrChord" <<
      endl;

  int chordInputLineNumber =
    elt->getInputLineNumber ();
    
  fIndentedOutputStream <<
    ">";

  if (
    elt->getChordIsFirstChordInADoubleTremolo ()
      ||
    elt->getChordIsSecondChordInADoubleTremolo ()) {
      // print the note duration, i.e. the double tremolo elements duration
      fIndentedOutputStream <<
        fCurrentDoubleTremoloElementsLpsrDuration; // JMI
  }
  
  else {
    // print the chord duration
    fIndentedOutputStream <<
      durationAsLilypondString (
        chordInputLineNumber,
        elt->
          getChordDisplayWholeNotes ()); // JMI test wether chord is in a tuplet?
  }
   
  // fetch the chord single tremolo
  S_msrSingleTremolo
    chordSingleTremolo =
      elt->getChordSingleTremolo ();

  if (chordSingleTremolo) {
    // generate code for the chord single tremolo
    fIndentedOutputStream <<
      singleTremoloDurationAsLilypondString (
        chordSingleTremolo);
  }

  fIndentedOutputStream <<
    " ";
    
  // print the chord articulations if any
  list<S_msrArticulation>
    chordArticulations =
      elt->getChordArticulations ();
      
  if (chordArticulations.size()) {
    list<S_msrArticulation>::const_iterator i;
    for (
      i=chordArticulations.begin();
      i!=chordArticulations.end();
      i++) {
      fIndentedOutputStream <<
        chordArticulationAsLilyponString ((*i)) <<          
        " ";
    } // for
  }

  // print the chord technicals if any
  list<S_msrTechnical>
    chordTechnicals =
      elt->getChordTechnicals ();
      
  if (chordTechnicals.size()) {
    list<S_msrTechnical>::const_iterator i;
    for (
      i=chordTechnicals.begin();
      i!=chordTechnicals.end();
      i++) {
      fIndentedOutputStream <<
        technicalAsLilypondString ((*i)) <<
        " "; // JMI
    } // for
  }

  // print the chord technicals with integer if any
  list<S_msrTechnicalWithInteger>
    chordTechnicalWithIntegers =
      elt->getChordTechnicalWithIntegers ();
      
  if (chordTechnicalWithIntegers.size()) {
    list<S_msrTechnicalWithInteger>::const_iterator i;
    for (
      i=chordTechnicalWithIntegers.begin();
      i!=chordTechnicalWithIntegers.end();
      i++) {
      fIndentedOutputStream <<
        technicalWithIntegerAsLilypondString ((*i)) <<
        " "; // JMI
    } // for
  }

  // print the chord technicals with string if any
  list<S_msrTechnicalWithString>
    chordTechnicalWithStrings =
      elt->getChordTechnicalWithStrings ();
      
  if (chordTechnicalWithStrings.size()) {
    list<S_msrTechnicalWithString>::const_iterator i;
    for (
      i=chordTechnicalWithStrings.begin();
      i!=chordTechnicalWithStrings.end();
      i++) {
      fIndentedOutputStream <<
        technicalWithStringAsLilypondString ((*i)) <<
        " "; // JMI
    } // for
  }

  // print the chord ornaments if any
  list<S_msrOrnament>
    chordOrnaments =
      elt->getChordOrnaments ();
      
  if (chordOrnaments.size()) {
    list<S_msrOrnament>::const_iterator i;
    for (
      i=chordOrnaments.begin();
      i!=chordOrnaments.end();
      i++) {
      fIndentedOutputStream <<
        ornamentAsLilypondString ((*i)) << // some ornaments are not yet supported
        " ";
    } // for
  }

  // print the chord dynamics if any
  list<S_msrDynamics>
    chordDynamics =
      elt->getChordDynamics ();
      
  if (chordDynamics.size()) {
    list<S_msrDynamics>::const_iterator i;
    for (
      i=chordDynamics.begin();
      i!=chordDynamics.end();
      i++) {
      S_msrDynamics
        dynamics = (*i);
        
      switch (dynamics->getDynamicsPlacementKind ()) {
        case msrDynamics::k_NoDynamicsPlacement:
          fIndentedOutputStream << "-";
          break;
        case msrDynamics::kDynamicsPlacementAbove:
          fIndentedOutputStream << "^";
          break;
        case msrDynamics::kDynamicsPlacementBelow:
          fIndentedOutputStream << "_";
          break;
      } // switch

      fIndentedOutputStream <<
        dynamicsAsLilypondString (dynamics) << " ";
    } // for
  }
  
  // print the chord other dynamics if any
  list<S_msrOtherDynamics>
    chordOtherDynamics =
      elt->getChordOtherDynamics ();
      
  if (chordOtherDynamics.size()) {
    list<S_msrOtherDynamics>::const_iterator i;
    for (
      i=chordOtherDynamics.begin();
      i!=chordOtherDynamics.end();
      i++) {
      fIndentedOutputStream <<
        "\\" << (*i)->otherDynamicsAsString () << " ";
    } // for
  }

  // print the chord beams if any
  list<S_msrBeam>
    chordBeams =
      elt->getChordBeams ();
      
  if (chordBeams.size()) {
    list<S_msrBeam>::const_iterator i;
    for (
      i=chordBeams.begin();
      i!=chordBeams.end();
      i++) {
      fIndentedOutputStream <<
        "] ";
    } // for
  }

  // print the chord words if any
  list<S_msrWords>
    chordWords =
      elt->getChordWords ();
      
  if (chordWords.size()) {
    list<S_msrWords>::const_iterator i;
    for (
      i=chordWords.begin();
      i!=chordWords.end();
      i++) {
        
      msrWords::msrWordsPlacementKind
        wordsPlacementKind =
          (*i)->getWordsPlacementKind ();
    
      string wordsContents =
        (*i)->getWordsContents ();
    
      switch (wordsPlacementKind) {
        case msrWords::kWordsPlacementAbove:
          fIndentedOutputStream << "^";
          break;
        case msrWords::kWordsPlacementBelow:
          fIndentedOutputStream << "_";
          break;
      } // switch
    
      fIndentedOutputStream <<
        "\\markup" << " { " <<
        quoteStringIfNonAlpha (wordsContents) <<
        " } ";
    } // for
  }

  // print the chord slurs if any
  list<S_msrSlur>
    chordSlurs =
      elt->getChordSlurs ();
      
  if (chordSlurs.size()) {
    list<S_msrSlur>::const_iterator i;
    for (
      i=chordSlurs.begin();
      i!=chordSlurs.end();
      i++) {
        
      switch ((*i)->getSlurKind ()) {
        case msrSlur::k_NoSlur:
          break;
        case msrSlur::kStartSlur:
          fIndentedOutputStream << "( ";
          break;
        case msrSlur::kContinueSlur:
          break;
        case msrSlur::kStopSlur:
          fIndentedOutputStream << ") ";
          break;
      } // switch
   } // for
  }

  // print the chord ligatures if any
  list<S_msrLigature>
    chordLigatures =
      elt->getChordLigatures ();
      
  if (chordLigatures.size()) {
    list<S_msrLigature>::const_iterator i;
    for (
      i=chordLigatures.begin();
      i!=chordLigatures.end();
      i++) {
        
      switch ((*i)->getLigatureKind ()) {
        case msrLigature::k_NoLigature:
          break;
        case msrLigature::kStartLigature:
  // JMI        fIndentedOutputStream << "\\[ ";
          break;
        case msrLigature::kContinueLigature:
          break;
        case msrLigature::kStopLigature:
          fIndentedOutputStream << "\\] ";
          break;
      } // switch
   } // for
  }

  // print the chord wedges if any
  list<S_msrWedge>
    chordWedges =
      elt->getChordWedges ();
      
  if (chordWedges.size()) {
    list<S_msrWedge>::const_iterator i;
    for (
      i=chordWedges.begin();
      i!=chordWedges.end();
      i++) {
        
      switch ((*i)->getWedgeKind ()) {
        case msrWedge::kCrescendoWedge:
          fIndentedOutputStream << "\\< ";
          break;
        case msrWedge::kDecrescendoWedge:
          fIndentedOutputStream << "\\> ";
          break;
        case msrWedge::kStopWedge:
          fIndentedOutputStream << "\\! ";
          break;
      } // switch
    } // for
  }


/*
  // print the tie if any
  {
    S_msrTie chordTie = elt->getChordTie ();
  
    if (chordTie) {
      if (chordTie->getTieKind () == msrTie::kStartTie) {
        fIndentedOutputStream << "~ ";
      }
    }
  }
*/

  // if the preceding item is a chord, the first note of the chord
  // is used as the reference point for the octave placement
  // of a following note or chord
  fRelativeOctaveReference =
    elt->getChordNotes () [0];
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTuplet& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrTuplet" <<
      endl;

  if (fTupletsStack.size ()) {
    // elt is a nested tuplet

    S_msrTuplet
      containingTuplet =
        fTupletsStack.top ();

    // unapply containing tuplet factor,
    // i.e 3/2 inside 5/4 becomes 15/8 in MusicXML...
    elt->
      unapplySoundingFactorToTupletMembers (
        containingTuplet->
          getTupletActualNotes (),
        containingTuplet->
          getTupletNormalNotes ());
  }

  if (gLilypondOptions->fTupletsOnALine) {
    fIndentedOutputStream <<
      "\\tuplet " <<
      elt->getTupletActualNotes () <<
      "/" <<
      elt->getTupletNormalNotes() << " { ";
  }
  
  else {
    fIndentedOutputStream <<
      endl <<
      "\\tuplet " <<
      elt->getTupletActualNotes () <<
      "/" <<
      elt->getTupletNormalNotes() << " { ";
  }

  fTupletsStack.push (elt);

  idtr++;
}

void lpsr2LilypondTranslator::visitEnd (S_msrTuplet& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrTuplet" <<
      endl;

  idtr--;

  if (gLilypondOptions->fTupletsOnALine) {
    fIndentedOutputStream <<
      "} ";
  }

  else {
    fIndentedOutputStream <<
      endl <<
      "}" <<
      endl;
  }

  fTupletsStack.pop ();
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrTie& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrTie" <<
      endl;
  
  switch (elt->getTieKind ()) {
    case msrTie::k_NoTie:
      break;
    case msrTie::kStartTie:
      fIndentedOutputStream << "~ ";
      break;
    case msrTie::kContinueTie:
      break;
    case msrTie::kStopTie:
      break;
   } // switch
}

void lpsr2LilypondTranslator::visitEnd (S_msrTie& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrTie" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrSegno& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrSegno" <<
      endl;

  fIndentedOutputStream <<
    "\\mark \\markup { \\musicglyph #\"scripts.segno\" }" <<
    endl;
}

void lpsr2LilypondTranslator::visitStart (S_msrCoda& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrCoda" <<
      endl;

  fIndentedOutputStream <<
    "\\mark \\markup { \\musicglyph #\"scripts.coda\" }" <<
    endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrEyeGlasses& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting eyeGlasses" <<
      endl;

  fIndentedOutputStream <<
    "^\\markup {\\eyeglasses} ";
}

void lpsr2LilypondTranslator::visitStart (S_msrPedal& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting pedal" <<
      endl;
      
  switch (elt->getPedalType ()) {
    case msrPedal::kPedalStart:
      fIndentedOutputStream << "<> \\sustainOn ";
      break;
    case msrPedal::kPedalContinue:
      fIndentedOutputStream << "%{\\continue pedal???%} "; // JMI
      break;
    case msrPedal::kPedalChange:
      fIndentedOutputStream << "%{\\change pedal???%} "; // JMI
      break;
    case msrPedal::kPedalStop:
      fIndentedOutputStream << "<> \\sustainOff ";
      break;
  } // switch
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrBarline& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      endl <<
      "% --> Start visiting msrBarline" <<
      endl;

  switch (elt->getBarlineCategory ()) {
    
    case msrBarline::kStandaloneBarline:
      fIndentedOutputStream <<
        endl;
      
      switch (elt->getStyle ()) {
        case msrBarline::k_NoStyle:
          break;
        case msrBarline::kRegularStyle:
          fIndentedOutputStream << "\\bar \"|\"";
          break;
        case msrBarline::kDottedStyle:
          fIndentedOutputStream << "\\bar \";\"";
          break;
        case msrBarline::kDashedStyle:
          fIndentedOutputStream << "\\bar \"!\"";
          break;
        case msrBarline::kHeavyStyle:
          fIndentedOutputStream << "\\bar \".\"";
          break;
        case msrBarline::kLightLightStyle:
          fIndentedOutputStream << "\\bar \"||\"";
          break;
        case msrBarline::kLightHeavyStyle:
          fIndentedOutputStream << "\\bar \"|.\"";
          break;
        case msrBarline::kHeavyLightStyle:
          fIndentedOutputStream << "\\bar \".|\"";
          break;
        case msrBarline::kHeavyHeavyStyle:
          fIndentedOutputStream << "\\bar \"..\"";
          break;
        case msrBarline::kTickStyle:
          fIndentedOutputStream << "\\bar \"'\"";
          break;
        case msrBarline::kShortStyle:
          fIndentedOutputStream << "\\bar \"'\""; // JMI
          /* JMI ??? from gregorian.ly
          divisioMaior = {
            \once \override BreathingSign.stencil = #ly:breathing-sign::divisio-maior
            \once \override BreathingSign.Y-offset = #0
            \breathe
          }
          */
          break;
        case msrBarline::kNoneStyle:
          fIndentedOutputStream << "\\bar \"\"";
          break;
      } // switch

      fIndentedOutputStream <<
        endl <<
        endl;

      if (elt->getBarlineHasSegno ())
        fIndentedOutputStream <<
          "\\mark \\markup { \\musicglyph #\"scripts.segno\" } ";
      if (elt->getBarlineHasCoda ())
        fIndentedOutputStream <<
          "\\mark \\markup { \\musicglyph #\"scripts.coda\" } ";

      if (gLilypondOptions->fNoteInputLineNumbers)
        // print the barline line number as a comment
        fIndentedOutputStream <<
          "%{ " << elt->getInputLineNumber () << " %} ";
      break;

    case msrBarline::kRepeatStartBarline:
    case msrBarline::kRepeatEndBarline:
    case msrBarline::kHookedEndingStartBarline:
    case msrBarline::kHookedEndingEndBarline:
    case msrBarline::kHooklessEndingStartBarline:
    case msrBarline::kHooklessEndingEndBarline:
      // should not occur, since
      // LilyPond will take care of displaying the repeat
      break;

  } // switch
}

void lpsr2LilypondTranslator::visitEnd (S_msrBarline& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      endl <<
      "% --> End visiting msrBarline" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrBarCheck& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrBarCheck" <<
      endl;
      
  string nextBarNumber =
    elt->getNextBarNumber ();

  // don't generate a bar check before the end of measure 1 // JMI ???
  fIndentedOutputStream <<
    "| % " << nextBarNumber << " % bar check" <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrBarCheck& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrBarCheck" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrBarNumberCheck& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrBarNumberCheck" <<
      endl;

  fIndentedOutputStream <<
    "\\barNumberCheck #" << elt->getNextBarNumber () <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrBarNumberCheck& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrBarNumberCheck" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrLineBreak& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrLineBreak" <<
      endl;

  fIndentedOutputStream <<
    "\\myBreak | % " << elt->getNextBarNumber () <<
    endl <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrLineBreak& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrLineBreak" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrPageBreak& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrPageBreak" <<
      endl;

  fIndentedOutputStream <<
    "\\myPageBreak " <<
    endl <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrPageBreak& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrPageBreak" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrRepeat& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrRepeat" <<
      endl;

  fCurrentRepeatEndingsNumber =
    elt->getRepeatEndings ().size();
  
  if (fCurrentRepeatEndingsNumber == 0)
    fCurrentRepeatEndingsNumber = 2; // implicitly

  stringstream s;
  s <<
    "\\repeat volta " << fCurrentRepeatEndingsNumber << " {";
  
  if (gLilypondOptions->fComments) {
    fIndentedOutputStream <<
      setw (commentFieldWidth) <<
      s.str () << "% start of repeat" <<
      endl;
  }
  else {
    fIndentedOutputStream <<
      s.str () <<
      endl;
  }

  idtr++;
}

void lpsr2LilypondTranslator::visitEnd (S_msrRepeat& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrRepeat" <<
      endl;

  /*
    CAUTION:
      the end of the repeat has to be generated
      before the endings are handled
  */
  
  if (elt->getRepeatEndings ().size() == 0) {
    // the end of the repeat has not been generated yet

    idtr--;

    if (gLilypondOptions->fComments) {      
      fIndentedOutputStream <<
        setw (commentFieldWidth) <<
        "}" << "% end of repeat" <<
        endl;
    }
    else {
      fIndentedOutputStream <<
        endl <<
        "}" <<
        endl;
    }
  }

  fCurrentRepeatEndingsNumber = 0;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrRepeatCommonPart& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrRepeatCommonPart" <<
      endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrRepeatCommonPart& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrRepeatCommonPart" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrRepeatEnding& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrRepeatEnding" <<
      endl;

  if (elt->getRepeatEndingInternalNumber () == 1) {
    
    idtr--;
    
    // first repeat ending is in charge of
    // outputting the end of the repeat
    if (gLilypondOptions->fComments) {      
      fIndentedOutputStream <<
        setw (commentFieldWidth) <<
        "}" << "% end of repeat" <<
        endl;
    }
    else {
      fIndentedOutputStream <<
        endl <<
        "}" <<
        endl;
    }

    // first repeat ending is in charge of
    // outputting the start of the alternative
    if (gLilypondOptions->fComments) {      
      fIndentedOutputStream <<
        endl <<
        setw (commentFieldWidth) <<
        "\\alternative {" <<
        "% start of alternative" <<
        endl;
    }
    else {
      fIndentedOutputStream <<
        endl <<
        "\\alternative {" <<
        endl;
    }

    idtr++;
  }

  // output the start of the ending
  switch (elt->getRepeatEndingKind ()) {
    case msrRepeatEnding::kHookedEnding:
      if (gLilypondOptions->fComments) {  
        fIndentedOutputStream <<
          setw (commentFieldWidth) <<
          "{" << "% start of repeat hooked ending" <<
          endl;
      }
      else {
        fIndentedOutputStream <<
          endl <<
          "{" <<
          endl;
      }
      break;
      
    case msrRepeatEnding::kHooklessEnding:
      if (gLilypondOptions->fComments)    
        fIndentedOutputStream <<
          setw (commentFieldWidth) <<
          "{" << "% start of repeat hookless ending" <<
          endl;
      else
        fIndentedOutputStream <<
          endl <<
          "{" <<
          endl;
      break;
  } // switch

  idtr++;
}

void lpsr2LilypondTranslator::visitEnd (S_msrRepeatEnding& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrRepeatEnding" <<
      endl;

  // output the end of the ending
  switch (elt->getRepeatEndingKind ()) {
    case msrRepeatEnding::kHookedEnding:
      if (gLilypondOptions->fComments) {
        fIndentedOutputStream <<
          setw (commentFieldWidth) <<
          "}" << "% end of repeat hooked ending" <<
          endl;
      }
      else {
        fIndentedOutputStream <<
          endl <<
          "}" <<
          endl;
      }
      break;
      
    case msrRepeatEnding::kHooklessEnding:
      if (gLilypondOptions->fComments)   { 
        fIndentedOutputStream <<
          setw (commentFieldWidth) <<
          "}" << "% end of repeat hookless ending" <<
          endl;
      }
      else {
        fIndentedOutputStream <<
          endl <<
          "}" <<
          endl;
      }
      break;
  } // switch

  if (
    elt->getRepeatEndingInternalNumber ()
      ==
    fCurrentRepeatEndingsNumber) {
      
    idtr--;
    
    // last repeat ending is in charge of
    // outputting the end of the alternative
    if (gLilypondOptions->fComments)   { 
      fIndentedOutputStream <<
        setw (commentFieldWidth) <<
        "}" << "% end of alternative" <<
        endl;
    }
    else {
      fIndentedOutputStream <<
        endl <<
        "}" <<
        endl;
    }
  }

  idtr--;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrComment& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting lpsrComment" <<
      endl;

  fIndentedOutputStream <<
    "% " << elt->getContents () <<
    endl;
    
  if (elt->getCommentGapKind () == lpsrComment::kGapAfterwards)
    fIndentedOutputStream << endl;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrComment& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting lpsrComment" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_lpsrSchemeFunction& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting lpsrSchemeFunction" <<
      endl;

  fIndentedOutputStream <<
    endl <<
    "% Scheme function(s): \"" << elt->getFunctionName () << "\"" <<
    // endl is in the decription
    elt->getFunctionDescription () <<
    // endl is in the decription
    elt->getFunctionCode () <<
    endl <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_lpsrSchemeFunction& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting lpsrSchemeFunction" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrRehearsal& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrRehearsal" <<
      endl;

  fIndentedOutputStream <<
    endl <<
    endl <<
    "\\mark" "\\markup " "{" "\\box {"  "\""<<
    elt->getRehearsalText () <<
    "\"" "}}" <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrRehearsal& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrRehearsal" <<
      endl;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrMeasureRepeat& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrMeasureRepeat" <<
      endl;

  int repeatMeasuresNumber =
    elt->measureRepeatPatternMeasuresNumber ();

  int replicasMeasuresNumber =
    elt->measureRepeatReplicasMeasuresNumber ();
    
  int replicasNumber =
    elt->measureRepeatReplicasNumber ();

  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceRepeats) {
    fIndentedOutputStream <<
      "% measure repeat, line " << elt->getInputLineNumber () << ":" <<
      endl;

    const int fieldWidth = 24;

    fIndentedOutputStream << left <<
      setw (fieldWidth) <<
      "% repeatMeasuresNumber" << " = " << repeatMeasuresNumber <<
      endl <<
      setw (fieldWidth) <<
      "% replicasMeasuresNumber" << " = " << replicasMeasuresNumber <<
      endl <<
      setw (fieldWidth) <<
      "% replicasNumber" << " = " << replicasNumber <<
      endl;
  }
  
  if (gLilypondOptions->fComments) {
    fIndentedOutputStream <<
      setw (commentFieldWidth) <<
      "% start of measure repeat" <<
      singularOrPlural (
        elt->measureRepeatReplicasNumber (),
        "replica",
        "replicas") <<
      ", line " << elt->getInputLineNumber () <<
      endl;
  }

  fIndentedOutputStream <<
    endl <<
    endl <<
    "\\repeat" "percent " <<
    replicasNumber + 1 <<
     " { " <<
    endl;

    idtr++;
}

void lpsr2LilypondTranslator::visitEnd (S_msrMeasureRepeat& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrMeasureRepeat" <<
      endl;

  idtr--;
  
  fIndentedOutputStream <<
    endl <<
    endl <<
    idtr <<
    " }" <<
    endl <<
    idtr;

  if (gLilypondOptions->fComments) {
    fIndentedOutputStream <<
      setw (commentFieldWidth) <<
      "% end of measure repeat" <<
      singularOrPlural (
        elt->measureRepeatReplicasNumber (),
        "replica",
        "replicas") <<
      ", line " << elt->getInputLineNumber () <<
      endl <<
      endl;      
  }
}

void lpsr2LilypondTranslator::visitStart (S_msrMeasureRepeatPattern& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "%--> Start visiting msrMeasureRepeatPattern" <<
      endl;

  idtr++;

  // JMI
}

void lpsr2LilypondTranslator::visitEnd (S_msrMeasureRepeatPattern& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "%--> End visiting msrMeasureRepeatPattern" <<
      endl;

  idtr--;
}
  
void lpsr2LilypondTranslator::visitStart (S_msrMeasureRepeatReplicas& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "%--> Start visiting msrMeasureRepeatReplicas" <<
      endl;

  idtr++;

  // JMI
}

void lpsr2LilypondTranslator::visitEnd (S_msrMeasureRepeatReplicas& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "%--> End visiting msrMeasureRepeatReplicas" <<
      endl;

  idtr--;
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrMultipleRest& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrMultipleRest" <<
      endl;

  int inputLineNumber =
    elt->getInputLineNumber ();
    
  int restMeasuresNumber =
    elt->getMultipleRestMeasuresNumber ();

  int restSegmentMeasuresNumber =
    elt->multipleRestContentsMeasuresNumber ();
    
  if (gGeneralOptions->fTraceMeasures || gGeneralOptions->fTraceRepeats) {
    fIndentedOutputStream <<
      "% multiple rest, line " << inputLineNumber << ":" <<
      endl;

    const int fieldWidth = 24;

    fIndentedOutputStream << left <<
      setw (fieldWidth) <<
      "% restMeasuresNumber" << " = " << restMeasuresNumber <<
      endl <<
      setw (fieldWidth) <<
      "% restSegmentMeasuresNumber" << " = " << restSegmentMeasuresNumber <<
      endl <<
      endl;
  }
  
  if (gLilypondOptions->fComments) {
    fIndentedOutputStream <<
      setw (commentFieldWidth) <<
      "% start of multiple rest" <<
      singularOrPlural (
        elt->getMultipleRestMeasuresNumber (),
        "measure",
        "measures") <<
      ", line " << inputLineNumber <<
      endl;
  }

  fIndentedOutputStream <<
    endl <<
    endl <<
    "R1";

  if (gLilypondOptions->fNoteInputLineNumbers)
    // print the multiple rest line number as a comment
    fIndentedOutputStream <<
      " %{ " << inputLineNumber << " %} ";  

  fIndentedOutputStream <<    
    "*" <<
    restMeasuresNumber <<
    " | " <<
    endl;
}

void lpsr2LilypondTranslator::visitEnd (S_msrMultipleRest& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrMultipleRest" <<
      endl;
    
  if (gLilypondOptions->fComments) {
    idtr--;

    fIndentedOutputStream <<
      setw (commentFieldWidth) <<
      "% end of multiple rest" <<
      singularOrPlural (
        elt->getMultipleRestMeasuresNumber (),
        "measure",
        "measures") <<
      ", line " << elt->getInputLineNumber () <<
      endl <<
      endl;      
  }
}

void lpsr2LilypondTranslator::visitStart (S_msrMultipleRestContents& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "%--> Start visiting msrMultipleRestContents" <<
      endl;

  idtr++;

  // JMI
}

void lpsr2LilypondTranslator::visitEnd (S_msrMultipleRestContents& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "%--> End visiting msrMultipleRestContents" <<
      endl;

  idtr--;

  // JMI
}

//________________________________________________________________________
void lpsr2LilypondTranslator::visitStart (S_msrMidi& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> Start visiting msrMidi" <<
      endl;

  if (gLilypondOptions->fNoMidi) {
    fIndentedOutputStream <<
      idtr <<
      "%{" <<
      endl;

     idtr++;
  }

  fIndentedOutputStream <<
    "\\midi" << " {" <<
    endl;
  
  idtr++;
  
  fIndentedOutputStream << idtr;

  if (gLilypondOptions->fNoMidi)
    fIndentedOutputStream <<
      "% ";
  
  fIndentedOutputStream <<
    "\\tempo " <<
    elt->getMidiTempoDuration () <<
    " = " <<
    elt->getMidiTempoPerSecond () <<
    endl;
  
  idtr--;
  
  fIndentedOutputStream <<
    "}" <<
    endl;

  if (gLilypondOptions->fNoMidi) {
    idtr--;

    fIndentedOutputStream <<
      idtr <<
      "%}" <<
      endl;
  }
}

void lpsr2LilypondTranslator::visitEnd (S_msrMidi& elt)
{
  if (gLpsrOptions->fTraceLpsrVisitors)
    fIndentedOutputStream <<
      "% --> End visiting msrMidi" <<
      endl;
}


} // namespace




/* JMI
  // is there an unmetered (stemless) section?
  if (stemKind != fCurrentStemKind) {
    switch (stemKind) {
      case msrStem::kStemNone:
        if (! fOnGoingStemNone) {
          fIndentedOutputStream <<
            endl <<
            endl << //JMI
            idtr <<
              "\\temporary\\omit Stem" <<
              endl <<
            idtr <<
              "\\once\\omit Staff.TimeSignature" <<
              endl <<
            idtr <<
              "\\cadenzaOn" <<
              endl <<
            endl << //JMI
            idtr;

          fOnGoingStemNone = true;
        }
        else {
          fIndentedOutputStream <<
            endl <<
            endl << //JMI
            idtr <<
              "\\cadenzaOff" <<
              endl <<
            idtr <<
              "\\bar \"|\"" <<
              endl <<
            idtr <<
              "\\undo\\omit Stem" <<
              endl <<
            endl << //JMI
            idtr;
        }
        break;
        
      case msrStem::k_NoStem:
      case msrStem::kStemUp:
      case msrStem::kStemDown:
      case msrStem::kStemDouble:
        break;
    } // switch
  }
*/


  /*
    <notations>
      <ornaments>
        <tremolo type="single">3</tremolo>
      </ornaments>
    </notations>

The tremolo ornament can be used to indicate either single-note or double-note tremolos. Single-note tremolos use the single type, while double-note tremolos use the start and stop types. The default is "single" for compatibility with Version 1.1.

The text of the element indicates the number of tremolo marks and is an integer from 0 to 8.
Note that the number of attached beams is not included in this value, but is represented separately using the beam element.

When using double-note tremolos, the duration of each note in the tremolo should correspond to half of the notated type value.
A time-modification element should also be added with an actual-notes value of 2 and a normal-notes value of 1.
If used within a tuplet, this 2/1 ratio should be multiplied by the existing tuplet ratio.

Using repeater beams for indicating tremolos is deprecated as of MusicXML 3.0.

  */


  /*
   * \layout {
 \context {
   \Score
   proportionalNotationDuration = #(ly:make-moment 1/16)
 }
}
*/

