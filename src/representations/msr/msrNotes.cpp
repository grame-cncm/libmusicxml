/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <regex>

#include "waeMessagesHandling.h"

#include "msrNotes.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrNote msrNote::create (
  int                        inputLineNumber,
  string                     noteMeasureNumber,

  msrNoteKind                noteKind,

  msrQuarterTonesPitchKind   noteQuarterTonesPitchKind,
  msrOctaveKind              noteOctaveKind,

  rational                   noteSoundingWholeNotes,
  rational                   noteDisplayWholeNotes,

  int                        noteDotsNumber,

  msrDurationKind            noteGraphicDurationKind,

  msrQuarterTonesPitchKind   noteQuarterTonesDisplayPitchKind,
  msrOctaveKind              noteDisplayOctaveKind,

  msrNoteIsACueNoteKind      noteIsACueNoteKind,

  msrPrintObjectKind         notePrintObjectKind,

  msrNoteHeadKind            noteHeadKind,
  msrNoteHeadFilledKind      noteHeadFilledKind,
  msrNoteHeadParenthesesKind noteHeadParenthesesKind)
{
  msrNote * o =
    new msrNote (
      inputLineNumber,
      noteMeasureNumber,

      noteKind,

      noteQuarterTonesPitchKind,
      noteOctaveKind,

      noteSoundingWholeNotes,
      noteDisplayWholeNotes,

      noteDotsNumber,

      noteGraphicDurationKind,

      noteQuarterTonesDisplayPitchKind,
      noteDisplayOctaveKind,

      noteIsACueNoteKind,

      notePrintObjectKind,

      noteHeadKind,
      noteHeadFilledKind,
      noteHeadParenthesesKind);
  assert (o != nullptr);

  return o;
}

msrNote::msrNote (
  int                        inputLineNumber,
  string                     noteMeasureNumber,

  msrNoteKind                noteKind,

  msrQuarterTonesPitchKind   noteQuarterTonesPitchKind,
  msrOctaveKind              noteOctaveKind,

  rational                   noteSoundingWholeNotes,
  rational                   noteDisplayWholeNotes,

  int                        noteDotsNumber,

  msrDurationKind            noteGraphicDurationKind,

  msrQuarterTonesPitchKind   noteQuarterTonesDisplayPitchKind,
  msrOctaveKind              noteDisplayOctaveKind,

  msrNoteIsACueNoteKind      noteIsACueNoteKind,

  msrPrintObjectKind         notePrintObjectKind,

  msrNoteHeadKind            noteHeadKind,
  msrNoteHeadFilledKind      noteHeadFilledKind,
  msrNoteHeadParenthesesKind noteHeadParenthesesKind)
  : msrTupletElement (inputLineNumber),
    fNoteAlphaRGBColor ("", "")
{
  fMeasureElementMeasureNumber = noteMeasureNumber;

  // basic note description
  fNoteKind = noteKind;

  fNoteQuarterTonesPitchKind  = noteQuarterTonesPitchKind;
  fNoteOctaveKind = noteOctaveKind;

  fMeasureElementSoundingWholeNotes = noteSoundingWholeNotes;
  fNoteDisplayWholeNotes  = noteDisplayWholeNotes;

  fNoteDotsNumber = noteDotsNumber;

  fNoteGraphicDurationKind = noteGraphicDurationKind;

  fNoteTupletFactor = rational (1, 1);

  fNoteQuarterTonesDisplayPitchKind = noteQuarterTonesDisplayPitchKind;
  fNoteDisplayOctaveKind            = noteDisplayOctaveKind;

  fNoteIsACueNoteKind   = noteIsACueNoteKind;

  fNotePrintObjectKind = notePrintObjectKind;

  fNoteHeadKind            = noteHeadKind;
  fNoteHeadFilledKind      = noteHeadFilledKind;
  fNoteHeadParenthesesKind = noteHeadParenthesesKind;

  // do other initializations
  initializeNote ();
}

void msrNote::initializeNote ()
{
  // rests handling
  // ------------------------------------------------------

  if (getNoteIsARest () && fNoteDisplayOctaveKind != msrOctaveKind::k_NoOctave) {
    // this note is a pitched rest:
    // copy the display octave to the the note octave, // JMI
    // to be used in octave relative code generation
    fNoteQuarterTonesPitchKind = fNoteQuarterTonesDisplayPitchKind;
    fNoteOctaveKind = fNoteDisplayOctaveKind;
  }

  // note accidentals
  // ------------------------------------------------------

  fNoteAccidentalKind =
    msrAccidentalKind::kAccidentalNone; // default value

  fNoteEditorialAccidentalKind =
    msrEditorialAccidentalKind::kEditorialAccidentalNo; // default value

  fNoteCautionaryAccidentalKind =
    msrCautionaryAccidentalKind::kCautionaryAccidentalNo; // default value

  // note context
  // ------------------------------------------------------

  fNoteBelongsToAChord = false;
  fNoteBelongsToATuplet = false;
  fNoteOccupiesAFullMeasure = false;

  fNoteBelongsToARestMeasures = false;
  fNoteRestMeasuresSequenceNumber = -1;

  fNoteAlphaRGBColorHasBenSet = false;

  // note lyrics
  // ------------------------------------------------------

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceNotesDetails ()) {
    gLogStream <<
      endl <<
      "Initializing a note" <<
      ", kind: ";
    if (fNoteKind == msrNoteKind::k_NoNote)
      gLogStream <<
        "not yet known";
    else
      gLogStream <<
        noteKindAsString (fNoteKind);
    gLogStream <<
      ", line " << fInputLineNumber << ":" <<
      endl;

    ++gIndenter;

    const unsigned int fieldWidth = 30;

    gLogStream <<
      left <<
        setw (fieldWidth) <<
        "noteQuarterTonesPitch" << " = " <<
        msrQuarterTonesPitchKindAsStringInLanguage (
          fNoteQuarterTonesPitchKind,
          gGlobalMsrOahGroup->
            getMsrQuarterTonesPitchesLanguageKind ()) <<
        endl;

    gLogStream <<
      left <<
        setw (fieldWidth) <<
        "fMeasureElementSoundingWholeNotes" << " = " <<
        fMeasureElementSoundingWholeNotes <<
        endl;

    gLogStream <<
      left <<
        setw (fieldWidth) <<
        "noteDisplayWholeNotes" << " = " <<
        fNoteDisplayWholeNotes <<
        endl;

    gLogStream <<
      left <<
        setw (fieldWidth) <<
        "noteDotsNumber" << " = " <<
        fNoteDotsNumber <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "noteGraphicDuration" << " = ";
      if (fNoteGraphicDurationKind != msrDurationKind::k_NoDuration) {
        gLogStream <<
          msrDurationKindAsString (
            fNoteGraphicDurationKind);
      }
      else {
        gLogStream << "none";
      }

    gLogStream <<
      left <<
        setw (fieldWidth) <<
        "noteTupletFactor" << " = " <<
        fNoteTupletFactor <<
        endl;

    gLogStream <<
      endl <<

      left <<
        setw (fieldWidth) <<
        "noteOctaveKind" << " = " <<
        msrOctaveKindAsString (fNoteOctaveKind) <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "noteDisplayOctaveKind" << " = " <<
        msrOctaveKindAsString (fNoteDisplayOctaveKind) <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "noteIsACueNoteKind" << " = " <<
         noteIsACueNoteKindAsString (fNoteIsACueNoteKind) <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "notePrintObjectKind" << " = " <<
         msrPrintObjectKindAsString (fNotePrintObjectKind) <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "noteHeadKind" << " = " <<
         noteHeadKindAsString (fNoteHeadKind) <<
        endl <<
      left <<
        setw (fieldWidth) <<
        "noteHeadFilledKind" << " = " <<
         noteHeadFilledKindAsString (fNoteHeadFilledKind) <<
        endl <<
      left <<
        setw (fieldWidth) <<
        "noteHeadParenthesesKind" << " = " <<
         noteHeadParenthesesKindAsString (fNoteHeadParenthesesKind) <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "noteAccidentalKind" << " = " <<
        msrAccidentalKindAsString (
          fNoteAccidentalKind) <<
        endl <<
      left <<
        setw (fieldWidth) <<
        "noteEditorialAccidentalKind" << " = " <<
        msrEditorialAccidentalKindAsString (
          fNoteEditorialAccidentalKind) <<
        endl <<
      left <<
        setw (fieldWidth) <<
        "noteCautionaryAccidentalKind" << " = " <<
        msrCautionaryAccidentalKindAsString (
          fNoteCautionaryAccidentalKind) <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "noteBelongsToAChord" << " = " <<
         booleanAsString (fNoteBelongsToAChord) <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "noteBelongsToATuplet" << " = " <<
         booleanAsString (fNoteBelongsToATuplet) <<
        endl << endl <<

      left <<
        setw (fieldWidth) <<
        "noteOccupiesAFullMeasure" << " = " <<
         booleanAsString (fNoteOccupiesAFullMeasure) <<
        endl << endl;

    --gIndenter;
  }
#endif

  // note measure information
  // ------------------------------------------------------

  fNoteOccupiesAFullMeasure = false;

  // note redundant information (for speed)
  // ------------------------------------------------------

  fNoteIsStemless = false;

  fNoteIsAChordsFirstMemberNote = false;

  fNoteIsFirstNoteInADoubleTremolo  = false;
  fNoteIsSecondNoteInADoubleTremolo = false;

  fNoteIsFollowedByGraceNotesGroup = false;
}

msrNote::~msrNote ()
{}

//________________________________________________________________________
S_msrMeasure msrNote::fetchNoteMeasureUpLink () const
{
  S_msrMeasure result;

  switch (fNoteKind) {
    case msrNoteKind::k_NoNote:
      break;

    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteRestInMeasure:
    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteUnpitchedInMeasure:
      result = fNoteDirectMeasureUpLink;
      break;

    case msrNoteKind::kNoteRegularInChord:
      if (fNoteDirectChordUpLink) {
        result =
          fNoteDirectChordUpLink->
            getChordDirectMeasureUpLink ();
      }
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteUnpitchedInTuplet:
      if (fNoteDirectTupletUpLink) {
        result =
          fNoteDirectTupletUpLink->
            getTupletDirectMeasureUpLink ();
      }
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      // JMI
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    /* JMi
      if (fNoteDirectGraceNotesGroupUpLink) {
        result =
          fNoteDirectGraceNotesGroupUpLink->
            getGraceNotesGroupVoiceUpLink ();
              / * JMI
              getGraceNotesGroupNoteUpLink ()->
              fetchNoteVoiceUpLink ();
              * /
      }
    */
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      break;
  } // switch

  return result;
}

// grace notes group upLink
S_msrGraceNotesGroup msrNote::fetchNoteGraceNotesGroupUpLink () const
{
  S_msrGraceNotesGroup result;

  switch (fNoteKind) {
    case msrNoteKind::k_NoNote:
      break;

    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteRestInMeasure:
    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteRegularInChord:
      result = fNoteDirectGraceNotesGroupUpLink;
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
      if (fNoteDirectTupletUpLink) {
      /* JMI
        result =
          fNoteDirectTupletUpLink->
            getTupletDirectMeasureUpLink ()->
              fetchMeasureVoiceUpLink ();
      */
      }
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    /* JMi
      if (fNoteDirectGraceNotesGroupUpLink) {
        result =
          fNoteDirectGraceNotesGroupUpLink->
            getGraceNotesGroupVoiceUpLink ();
              / * JMI
              getGraceNotesGroupNoteUpLink ()->
              fetchNoteVoiceUpLink ();
              * /
      }
    */
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      break;
  } // switch

  return result;
}

S_msrVoice msrNote::fetchNoteVoiceUpLink () const
{
  S_msrVoice result;

  switch (fNoteKind) {
    case msrNoteKind::k_NoNote:
      break;

    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteRestInMeasure:
    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteRegularInChord:
      if (fNoteDirectMeasureUpLink) {
        result =
          fNoteDirectMeasureUpLink->
            fetchMeasureVoiceUpLink ();
      }
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
      if (fNoteDirectTupletUpLink) {
        result =
          fNoteDirectTupletUpLink->
            getTupletDirectMeasureUpLink ()->
              fetchMeasureVoiceUpLink ();
      }
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    /* JMi
      if (fNoteDirectGraceNotesGroupUpLink) {
        result =
          fNoteDirectGraceNotesGroupUpLink->
            getGraceNotesGroupVoiceUpLink ();
              / * JMI
              getGraceNotesGroupNoteUpLink ()->
              fetchNoteVoiceUpLink ();
              * /
      }
    */
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      break;
  } // switch

  return result;
}

S_msrStaff msrNote::fetchNoteStaffUpLink () const
{
  S_msrStaff result;

  if (fNoteDirectMeasureUpLink) {
    result =
      fNoteDirectMeasureUpLink->
        fetchMeasureStaffUpLink ();
  }

  return result;
}

S_msrPart msrNote::fetchNotePartUpLink () const
{
  S_msrPart result;

  if (fNoteDirectMeasureUpLink) {
    result =
      fNoteDirectMeasureUpLink->
        fetchMeasurePartUpLink ();
  }

  return result;
}

S_msrPartGroup msrNote::fetchNotePartGroupUpLink () const
{
  S_msrPartGroup result;

  if (fNoteDirectMeasureUpLink) {
    result =
      fNoteDirectMeasureUpLink->
        fetchMeasurePartGroupUpLink ();
  }

  return result;
}

S_msrScore msrNote::fetchNoteScoreUpLink () const
{
  S_msrScore result;

  if (fNoteDirectMeasureUpLink) {
    result =
        fNoteDirectMeasureUpLink->
          fetchMeasureScoreUpLink ();
  }

  return result;
}

void msrNote::setNoteKind (msrNoteKind noteKind)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Setting the kind of note '" <<
      asString () <<
      "' to '" <<
      noteKindAsString (noteKind) <<
      "'" <<
      endl;
  }
#endif

  fNoteKind = noteKind;
}

void msrNote::setNoteAlphaRGBColor (
  msrAlphaRGBColor& noteAlphaRGBColor)
{
  fNoteAlphaRGBColor          = noteAlphaRGBColor;
  fNoteAlphaRGBColorHasBenSet = true;
}

S_msrNote msrNote::createNoteNewbornClone (
  S_msrPart containingPart)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Creating a newborn clone of note '" <<
      asString () <<
      "' in part " <<
      containingPart->
        getPartCombinedName () <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    containingPart != nullptr,
    "containingPart is null");

  S_msrNote
    newbornClone =
      msrNote::create (
        fInputLineNumber,
        fMeasureElementMeasureNumber,

        fNoteKind,

        fNoteQuarterTonesPitchKind,
        fNoteOctaveKind,

        fMeasureElementSoundingWholeNotes,
        fNoteDisplayWholeNotes,

        fNoteDotsNumber,

        fNoteGraphicDurationKind,

        fNoteQuarterTonesDisplayPitchKind,
        fNoteDisplayOctaveKind,

        fNoteIsACueNoteKind,

        fNotePrintObjectKind,

        fNoteHeadKind,
        fNoteHeadFilledKind,
        fNoteHeadParenthesesKind);

  // basic note description
  // ------------------------------------------------------

  newbornClone->fNoteOctaveShift =
    fNoteOctaveShift;

  // note print kind
  // ------------------------------------------------------

  newbornClone->fNotePrintObjectKind =
    fNotePrintObjectKind;

  // note head
  // ------------------------------------------------------

  newbornClone->fNoteHeadKind =
    fNoteHeadKind;
  newbornClone->fNoteHeadFilledKind =
    fNoteHeadFilledKind;
  newbornClone->fNoteHeadParenthesesKind =
    fNoteHeadParenthesesKind;

  // accidentals
  // ------------------------------------------------------

  newbornClone->fNoteAccidentalKind =
    fNoteAccidentalKind;

  newbornClone->fNoteEditorialAccidentalKind =
    fNoteEditorialAccidentalKind;

  newbornClone->fNoteCautionaryAccidentalKind =
    fNoteCautionaryAccidentalKind;

  // chord member?
  // ------------------------------------------------------

  newbornClone->
    fNoteBelongsToAChord =
      fNoteBelongsToAChord;

  // tuplet member?
  // ------------------------------------------------------

  newbornClone->
    fNoteBelongsToATuplet =
      fNoteBelongsToATuplet;

  // note measure information
  // ------------------------------------------------------

  newbornClone->
    fNoteOccupiesAFullMeasure =
      fNoteOccupiesAFullMeasure;

  // multiple rest member?
  // ------------------------------------------------------

  newbornClone->
    fNoteBelongsToARestMeasures =
      fNoteBelongsToARestMeasures;

  newbornClone->
    fNoteRestMeasuresSequenceNumber =
      fNoteRestMeasuresSequenceNumber;

  // note lyrics
  // ------------------------------------------------------

  // stem
  // ------------------------------------------------------

  newbornClone->fNoteStem = // JMI
    // share this data
    fNoteStem;

  // beams
  // ------------------------------------------------------

  // articulations
  // ------------------------------------------------------

  // technicals
  // ------------------------------------------------------

  // ornaments
  // ------------------------------------------------------

  // single tremolo
  // ------------------------------------------------------

  // tie
  // ------------------------------------------------------

  newbornClone->fNoteTie = // JMI
    // share this data
    fNoteTie;

  // dynamics
  // ------------------------------------------------------

  // words
  // ------------------------------------------------------

  // slurs
  // ------------------------------------------------------

  // ligatures
  // ------------------------------------------------------

  // harmony
  // ------------------------------------------------------

  // note redundant information (for speed)
  // ------------------------------------------------------

  newbornClone->fNoteIsStemless =
    fNoteIsStemless;

  newbornClone->fNoteIsAChordsFirstMemberNote =
    fNoteIsAChordsFirstMemberNote;

  newbornClone->fNoteIsFirstNoteInADoubleTremolo =
    fNoteIsFirstNoteInADoubleTremolo;
  newbornClone->fNoteIsSecondNoteInADoubleTremolo =
    fNoteIsSecondNoteInADoubleTremolo;

  newbornClone->fNoteTrillOrnament =
    fNoteTrillOrnament;
  newbornClone->fNoteIsFollowedByGraceNotesGroup =
    fNoteIsFollowedByGraceNotesGroup;

  newbornClone->fNoteDashesOrnament =
    fNoteDashesOrnament;

  newbornClone->fNoteWavyLineSpannerStart =
    fNoteWavyLineSpannerStart;
  newbornClone->fNoteWavyLineSpannerStop =
    fNoteWavyLineSpannerStop;

  newbornClone->fNoteDelayedTurnOrnament =
    fNoteDelayedTurnOrnament;
  newbornClone->fNoteDelayedInvertedTurnOrnament =
    fNoteDelayedInvertedTurnOrnament;

  // note color
  // ------------------------------------------------------

  newbornClone->fNoteAlphaRGBColor =
    fNoteAlphaRGBColor;
  newbornClone->fNoteAlphaRGBColorHasBenSet =
    fNoteAlphaRGBColorHasBenSet;

  // upLinks
  // ------------------------------------------------------

  /* JMI
    S_msrTuplet           fNoteDirectTupletUpLink;

    S_msrMeasure          fNoteDirectMeasureUpLink;
*/

  return newbornClone;
}

S_msrNote msrNote::createNoteDeepCopy (
  S_msrVoice containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Creating a deep copy of note " <<
      asString () <<
      /* JMI
      " in part " <<
      containingPart->
        getPartCombinedName () <<
        */
      endl;
  }
#endif

/* JMI
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    containingVoice != nullptr,
    "containingVoice is null");
    */

  S_msrNote
    noteDeepCopy =
      msrNote::create (
        fInputLineNumber,
        fMeasureElementMeasureNumber,

        fNoteKind,

        fNoteQuarterTonesPitchKind,
        fNoteOctaveKind,

        fMeasureElementSoundingWholeNotes,
        fNoteDisplayWholeNotes,

        fNoteDotsNumber,

        fNoteGraphicDurationKind,

        fNoteQuarterTonesDisplayPitchKind,
        fNoteDisplayOctaveKind,

        fNoteIsACueNoteKind,

        fNotePrintObjectKind,

        fNoteHeadKind,
        fNoteHeadFilledKind,
        fNoteHeadParenthesesKind);

  // basic note description
  // ------------------------------------------------------

  noteDeepCopy->fNoteOctaveShift = // JMI
    fNoteOctaveShift;

  // note print kind
  // ------------------------------------------------------

  noteDeepCopy->fNotePrintObjectKind =
    fNotePrintObjectKind;

  // note head
  // ------------------------------------------------------

  noteDeepCopy->fNoteHeadKind =
    fNoteHeadKind;
  noteDeepCopy->fNoteHeadFilledKind =
    fNoteHeadFilledKind;
  noteDeepCopy->fNoteHeadParenthesesKind =
    fNoteHeadParenthesesKind;

  // accidentals
  // ------------------------------------------------------

  noteDeepCopy->fNoteAccidentalKind =
    fNoteAccidentalKind;

  noteDeepCopy->fNoteEditorialAccidentalKind =
    fNoteEditorialAccidentalKind;

  noteDeepCopy->fNoteCautionaryAccidentalKind =
    fNoteCautionaryAccidentalKind;

  // chord member?
  // ------------------------------------------------------

  noteDeepCopy->
    fNoteBelongsToAChord =
      fNoteBelongsToAChord;

  // tuplet member?
  // ------------------------------------------------------

  noteDeepCopy->
    fNoteBelongsToATuplet =
      fNoteBelongsToATuplet;

  // note measure information
  // ------------------------------------------------------

  noteDeepCopy->
    fNoteOccupiesAFullMeasure =
      fNoteOccupiesAFullMeasure;

  // multiple rest member?
  // ------------------------------------------------------

  noteDeepCopy->
    fNoteBelongsToARestMeasures =
      fNoteBelongsToARestMeasures;

  noteDeepCopy->
    fNoteRestMeasuresSequenceNumber =
      fNoteRestMeasuresSequenceNumber;

  // note lyrics
  // ------------------------------------------------------

/* JMI
  {
    list<S_msrSyllable>::const_iterator i;
    for (i=fNoteSyllables.begin (); i!=fNoteSyllables.end (); ++i) {
      // share this data
      noteDeepCopy->
        fNoteSyllables.push_back ((*i));
    } // for
  }
*/

  // stem
  // ------------------------------------------------------

  noteDeepCopy->fNoteStem = // JMI
    // share this data
    fNoteStem;

  // beams
  // ------------------------------------------------------

  {
    list<S_msrBeam>::const_iterator i;
    for (i=fNoteBeams.begin (); i!=fNoteBeams.end (); ++i) {
      // share this data
      noteDeepCopy->
        fNoteBeams.push_back ((*i));
    } // for
  }

  // articulations
  // ------------------------------------------------------

  for (
    list<S_msrArticulation>::const_iterator i=
      fNoteArticulations.begin ();
      i!=fNoteArticulations.end ();
      ++i
  ) {
    // share this data
    noteDeepCopy->
      fNoteArticulations.push_back ((*i));
  } // for

  // spanners
  // ------------------------------------------------------

  for (
    list<S_msrSpanner>::const_iterator i=
      fNoteSpanners.begin ();
      i!=fNoteSpanners.end ();
      ++i
  ) {
    // share this data
    noteDeepCopy->
      fNoteSpanners.push_back ((*i));
  } // for

  // technicals
  // ------------------------------------------------------

  {
    list<S_msrTechnical>::const_iterator i;
    for (
      i=fNoteTechnicals.begin ();
      i!=fNoteTechnicals.end ();
      ++i
  ) {
      // share this data
      noteDeepCopy->
        fNoteTechnicals.push_back ((*i));
    } // for
  }

  {
    list<S_msrTechnicalWithInteger>::const_iterator i;
    for (
      i=fNoteTechnicalWithIntegers.begin ();
      i!=fNoteTechnicalWithIntegers.end ();
      ++i
  ) {
      // share this data
      noteDeepCopy->
        fNoteTechnicalWithIntegers.push_back ((*i));
    } // for
  }

  {
    list<S_msrTechnicalWithString>::const_iterator i;
    for (
      i=fNoteTechnicalWithStrings.begin ();
      i!=fNoteTechnicalWithStrings.end ();
      ++i
  ) {
      // share this data
      noteDeepCopy->
        fNoteTechnicalWithStrings.push_back ((*i));
    } // for
  }

  // ornaments
  // ------------------------------------------------------

  {
    list<S_msrOrnament>::const_iterator i;
    for (i=fNoteOrnaments.begin (); i!=fNoteOrnaments.end (); ++i) {
      // share this data
      noteDeepCopy->
        fNoteOrnaments.push_back ((*i));
    } // for
  }

  // glissandos
  // ------------------------------------------------------

  {
    list<S_msrGlissando>::const_iterator i;
    for (i=fNoteGlissandos.begin (); i!=fNoteGlissandos.end (); ++i) {
      // share this data
      noteDeepCopy->
        fNoteGlissandos.push_back ((*i));
    } // for
  }

  // slides
  // ------------------------------------------------------

  {
    list<S_msrSlide>::const_iterator i;
    for (i=fNoteSlides.begin (); i!=fNoteSlides.end (); ++i) {
      // share this data
      noteDeepCopy->
        fNoteSlides.push_back ((*i));
    } // for
  }

  // grace notes
  // ------------------------------------------------------

  noteDeepCopy->fNoteGraceNotesGroupBefore =
    fNoteGraceNotesGroupBefore;
  noteDeepCopy->fNoteGraceNotesGroupAfter =
    fNoteGraceNotesGroupAfter;

/* JMI
  // after grace notes
  // ------------------------------------------------------

  noteDeepCopy->fNoteAfterGraceNotesGroup =
    fNoteAfterGraceNotesGroup;
*/

  // single tremolo
  // ------------------------------------------------------

  noteDeepCopy->fNoteSingleTremolo =
    fNoteSingleTremolo;

  // tie
  // ------------------------------------------------------

  noteDeepCopy->fNoteTie = // JMI
    // share this data
    fNoteTie;

  // dynamics
  // ------------------------------------------------------

  {
    list<S_msrDynamics>::const_iterator i;
    for (i=fNoteDynamics.begin (); i!=fNoteDynamics.end (); ++i) {
      // share this data
      noteDeepCopy->
        fNoteDynamics.push_back ((*i));
    } // for
  }

  {
    list<S_msrOtherDynamics>::const_iterator i;
    for (i=fNoteOtherDynamics.begin (); i!=fNoteOtherDynamics.end (); ++i) {
      // share this data
      noteDeepCopy->
        fNoteOtherDynamics.push_back ((*i));
    } // for
  }

  // slashes
  // ------------------------------------------------------

  {
    list<S_msrSlash>::const_iterator i;
    for (i=fNoteSlashes.begin (); i!=fNoteSlashes.end (); ++i) {
      // share this data
      noteDeepCopy->
        fNoteSlashes.push_back ((*i));
    } // for
  }

  // wedges
  // ------------------------------------------------------

  {
    list<S_msrWedge>::const_iterator i;
    for (i=fNoteWedges.begin (); i!=fNoteWedges.end (); ++i) {
      // share this data
      noteDeepCopy->
        fNoteWedges.push_back ((*i));
    } // for
  }

  // eyeglasses
  // ------------------------------------------------------

  {
    list<S_msrEyeGlasses>::const_iterator i;
    for (i=fNoteEyeGlasses.begin (); i!=fNoteEyeGlasses.end (); ++i) {
      // share this data
      noteDeepCopy->
        fNoteEyeGlasses.push_back ((*i));
    } // for
  }

  // damps
  // ------------------------------------------------------

  {
    list<S_msrDamp>::const_iterator i;
    for (i=fNoteDamps.begin (); i!=fNoteDamps.end (); ++i) {
      // share this data
      noteDeepCopy->
        fNoteDamps.push_back ((*i));
    } // for
  }

  // damp alls
  // ------------------------------------------------------

  {
    list<S_msrDampAll>::const_iterator i;
    for (i=fNoteDampAlls.begin (); i!=fNoteDampAlls.end (); ++i) {
      // share this data
      noteDeepCopy->
        fNoteDampAlls.push_back ((*i));
    } // for
  }

  // scordaturas
  // ------------------------------------------------------

  {
    list<S_msrScordatura>::const_iterator i;
    for (i=fNoteScordaturas.begin (); i!=fNoteScordaturas.end (); ++i) {
      // share this data
      noteDeepCopy->
        fNoteScordaturas.push_back ((*i));
    } // for
  }

  // words
  // ------------------------------------------------------

  {
    list<S_msrWords>::const_iterator i;
    for (i=fNoteWords.begin (); i!=fNoteWords.end (); ++i) {
      // share this data
      noteDeepCopy->
        fNoteWords.push_back ((*i));
    } // for
  }

  // slurs
  // ------------------------------------------------------

  {
    list<S_msrSlur>::const_iterator i;
    for (i=fNoteSlurs.begin (); i!=fNoteSlurs.end (); ++i) {
      // share this data
      noteDeepCopy->
        fNoteSlurs.push_back ((*i));
    } // for
  }

  // ligatures
  // ------------------------------------------------------

  {
    list<S_msrLigature>::const_iterator i;
    for (i=fNoteLigatures.begin (); i!=fNoteLigatures.end (); ++i) {
      // share this data
      noteDeepCopy->
        fNoteLigatures.push_back ((*i));
    } // for
  }

  // pedals
  // ------------------------------------------------------

  {
    list<S_msrPedal>::const_iterator i;
    for (i=fNotePedals.begin (); i!=fNotePedals.end (); ++i) {
      // share this data
      noteDeepCopy->
        fNotePedals.push_back ((*i));
    } // for
  }

  // harmonies
  // ------------------------------------------------------

  noteDeepCopy->fNoteHarmoniesList = // JMI ???
    fNoteHarmoniesList;

  // figured basses
  // ------------------------------------------------------

  noteDeepCopy->fNoteFiguredBassesList = // JMI ???
    fNoteFiguredBassesList;

  // note measure information
  // ------------------------------------------------------

  noteDeepCopy->
    fMeasureElementMeasureNumber =
      fMeasureElementMeasureNumber;
  noteDeepCopy->
    fMeasureElementPositionInMeasure =
      fMeasureElementPositionInMeasure;
  noteDeepCopy->
    fMeasureElementPositionInVoice =
      fMeasureElementPositionInVoice;

  noteDeepCopy->
    fNoteOccupiesAFullMeasure =
      fNoteOccupiesAFullMeasure;

  // note redundant information (for speed)
  // ------------------------------------------------------

  noteDeepCopy->fNoteIsStemless =
    fNoteIsStemless;

  noteDeepCopy->fNoteIsAChordsFirstMemberNote =
    fNoteIsAChordsFirstMemberNote;

  noteDeepCopy->fNoteIsFirstNoteInADoubleTremolo =
    fNoteIsFirstNoteInADoubleTremolo;
  noteDeepCopy->fNoteIsSecondNoteInADoubleTremolo =
    fNoteIsSecondNoteInADoubleTremolo;

  noteDeepCopy->fNoteTrillOrnament =
    fNoteTrillOrnament;
  noteDeepCopy->fNoteIsFollowedByGraceNotesGroup =
    fNoteIsFollowedByGraceNotesGroup;

  noteDeepCopy->fNoteDashesOrnament =
    fNoteDashesOrnament;

  noteDeepCopy->fNoteWavyLineSpannerStart =
    fNoteWavyLineSpannerStart;
  noteDeepCopy->fNoteWavyLineSpannerStop =
    fNoteWavyLineSpannerStop;

  noteDeepCopy->fNoteDelayedTurnOrnament =
    fNoteDelayedTurnOrnament;
  noteDeepCopy->fNoteDelayedInvertedTurnOrnament =
    fNoteDelayedInvertedTurnOrnament;

  // note color
  // ------------------------------------------------------

  noteDeepCopy->fNoteAlphaRGBColor =
    fNoteAlphaRGBColor;
  noteDeepCopy->fNoteAlphaRGBColorHasBenSet =
    fNoteAlphaRGBColorHasBenSet;

  // upLinks
  // ------------------------------------------------------

  /* JMI

    S_msrTuplet           fNoteDirectChordUpLink;

    S_msrGraceNotesGroup  fNoteGraceNoteGroupUpLink;

    S_msrTuplet           fNoteDirectTupletUpLink;

    S_msrMeasure          fNoteDirectMeasureUpLink;
*/

  return noteDeepCopy;
}

S_msrNote msrNote::createRestNote (
  int       inputLineNumber,
  string    noteMeasureNumber,
  rational  soundingWholeNotes,
  rational  displayWholeNotes,
  int       dotsNumber)
{
  msrNote * o =
    new msrNote (
      inputLineNumber,
      noteMeasureNumber,

      msrNoteKind::kNoteRestInMeasure, // noteKind

      msrQuarterTonesPitchKind::k_NoQuarterTonesPitch,
      msrOctaveKind::k_NoOctave, // noteOctave,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      msrDurationKind::k_NoDuration, // noteGraphicDuration

      msrQuarterTonesPitchKind::k_NoQuarterTonesPitch, // noteDisplayQuarterTonesPitch
      msrOctaveKind::k_NoOctave, // noteDisplayOctave,

      msrNote::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectYes, // JMI

      kNoteHeadNormal, // JMI
      kNoteHeadFilledYes, // JMI
      kNoteHeadParenthesesNo); // JMI
  assert (o != nullptr);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestNotes ()) {
    gLogStream <<
      "Creating rest note '" <<
      o->asShortString () <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  return o;
}

S_msrNote msrNote::createSkipNote (
  int       inputLineNumber,
  string    noteMeasureNumber,
  rational  soundingWholeNotes,
  rational  displayWholeNotes,
  int       dotsNumber)
{
  msrNote * o =
    new msrNote (
      inputLineNumber,
      noteMeasureNumber,

      msrNoteKind::kNoteSkipInMeasure, // noteKind

      msrQuarterTonesPitchKind::k_NoQuarterTonesPitch,
      msrOctaveKind::k_NoOctave, // noteOctave,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      msrDurationKind::k_NoDuration, // noteGraphicDuration

      msrQuarterTonesPitchKind::k_NoQuarterTonesPitch, // noteDisplayQuarterTonesPitch
      msrOctaveKind::k_NoOctave, // noteDisplayOctave,

      msrNote::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectYes, // JMI

      kNoteHeadNormal, // JMI
      kNoteHeadFilledYes, // JMI
      kNoteHeadParenthesesNo); // JMI
  assert (o != nullptr);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceSkipNotes () || gGlobalTraceOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Creating skip note '" <<
      o->asString () <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  return o;
}

S_msrNote msrNote::createGraceSkipNote (
  int      inputLineNumber,
  string   noteMeasureNumber,
  rational soundingWholeNotes,
  rational displayWholeNotes,
  int      dotsNumber)
{
  msrNote * o =
    new msrNote (
      inputLineNumber,
      noteMeasureNumber,

      msrNoteKind::kNoteSkipInGraceNotesGroup, // noteKind

      msrQuarterTonesPitchKind::k_NoQuarterTonesPitch,
      msrOctaveKind::k_NoOctave, // noteOctave,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      msrDurationKind::k_NoDuration, // noteGraphicDuration

      msrQuarterTonesPitchKind::k_NoQuarterTonesPitch, // noteDisplayQuarterTonesPitch
      msrOctaveKind::k_NoOctave, // noteDisplayOctave,

      msrNote::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectYes, // JMI

      kNoteHeadNormal, // JMI
      kNoteHeadFilledYes, // JMI
      kNoteHeadParenthesesNo); // JMI
  assert (o != nullptr);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceSkipNotes () || gGlobalTraceOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Creating skip note '" <<
      o->asString () <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  return o;
}

//________________________________________________________________________
S_msrNote msrNote::createRestNoteWithOctave (
  int           inputLineNumber,
  string        noteMeasureNumber,
  msrOctaveKind noteOctave,
  rational      soundingWholeNotes,
  rational      displayWholeNotes,
  int           dotsNumber)
{
  msrNote * o =
    new msrNote (
      inputLineNumber,
      noteMeasureNumber,

      msrNoteKind::kNoteRestInMeasure, // noteKind

      msrQuarterTonesPitchKind::kQTP_Rest,
      noteOctave,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      msrDurationKind::k_NoDuration, // noteGraphicDuration

      msrQuarterTonesPitchKind::kQTP_Rest,  // noteQuarterTonesDisplayPitchKind
      msrOctaveKind::k_NoOctave,  // noteDisplayOctaveKind

      msrNote::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectYes, // JMI

      kNoteHeadNormal, // JMI
      kNoteHeadFilledYes, // JMI
      kNoteHeadParenthesesNo); // JMI
  assert (o != nullptr);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceRestNotes ()) {
    gLogStream <<
      "Creating rest note '" <<
      o->asShortString () <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  return o;
}

//________________________________________________________________________
S_msrNote msrNote::createSkipNoteWithOctave (
  int           inputLineNumber,
  string        noteMeasureNumber,
  msrOctaveKind noteOctave,
  rational      soundingWholeNotes,
  rational      displayWholeNotes,
  int           dotsNumber)
{
  msrNote * o =
    new msrNote (
      inputLineNumber,
      noteMeasureNumber,

      msrNoteKind::kNoteSkipInMeasure, // noteKind

      msrQuarterTonesPitchKind::kQTP_Skip,
      noteOctave,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      msrDurationKind::k_NoDuration, // noteGraphicDuration JMI ???

      msrQuarterTonesPitchKind::kQTP_Skip,  // noteQuarterTonesDisplayPitchKind
      noteOctave,

      msrNote::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectYes, // JMI

      kNoteHeadNormal, // JMI
      kNoteHeadFilledYes, // JMI
      kNoteHeadParenthesesNo); // JMI
  assert (o != nullptr);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceSkipNotes ()) {
    gLogStream <<
      "Creating skip note '" <<
      o->asShortString () <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  return o;
}

//________________________________________________________________________
S_msrNote msrNote::createRegularNote (
  int                      inputLineNumber,
  string                   noteMeasureNumber,
  msrQuarterTonesPitchKind quarterTonesPitchKind,
  msrOctaveKind            noteOctaveKind,
  rational                 soundingWholeNotes,
  rational                 displayWholeNotes,
  int                      dotsNumber)
{
  msrNote * o =
    new msrNote (
      inputLineNumber,
      noteMeasureNumber,

      msrNoteKind::kNoteRegularInMeasure, // noteKind

      quarterTonesPitchKind,
      noteOctaveKind,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      wholeNotesAsDurationKind (displayWholeNotes), // JMI caution for tuplet members...

      quarterTonesPitchKind,
      noteOctaveKind, // JMI ???

      msrNote::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectYes, // JMI

      kNoteHeadNormal, // JMI
      kNoteHeadFilledYes, // JMI
      kNoteHeadParenthesesNo); // JMI
  assert (o != nullptr);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Creating regular note '" <<
      o->asShortString () <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  return o;
}

//________________________________________________________________________
S_msrNote msrNote::createRestFromString (
  int    inputLineNumber,
  string restString,
  string restMeasureNumber)
{
  // handling restString à la LilyPond, such as "r4.."

  S_msrNote result;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Creating rest from string \"" <<
      restString <<
      "\", restMeasureNumber: '" << restMeasureNumber <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  string regularExpression (
    "[[:space:]]*"
    "r" //
    "[[:space:]]*"
    "([[:digit:]]+)" // restDuration
    "[[:space:]]*"
    "(\\.*)"         // restDots
    "[[:space:]]*"
    );

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "regularExpression = " <<
      regularExpression <<
      endl;
  }
#endif

  regex  e (regularExpression);
  smatch sm;

  regex_match (restString, sm, e);

  unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for rest string \"" << restString <<
      "\" with regex \"" << regularExpression <<
      "\":" <<
      endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    gLogStream << endl;

    --gIndenter;
  }
#endif

  // handling restString à la LilyPond, such as "r4.."

  if (smSize != 3) {
    stringstream s;

    s <<
      "restString \"" << restString <<
      "\" is ill-formed";

    oahError (s.str ());
  }

  string
    restDuration = sm [1],
    restDots     = sm [2];

  unsigned int dotsNumber = restDots.size ();

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "restDuration = \"" <<
      restDuration <<
      "\"" <<
      endl <<

      "restDots = \"" <<
      restDots <<
      "\"" <<
      endl <<
      "dotsNumber = " <<
      dotsNumber <<
      endl;
  }
#endif

  // compute the restDurationKind from restDuration
  msrDurationKind
    restDurationKind =
      msrDurationKindFromString (
        inputLineNumber,
        restDuration);

  // compute the duration whole notes from restDurationKind
  rational
     durationKindFromMslpString =
       msrDurationKindAsWholeNotes (
         restDurationKind);

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "durationKindFromMslpString = " <<
      durationKindFromMslpString <<
      endl;
  }
#endif

  result =
    msrNote::createRestNote (
      inputLineNumber,
      restMeasureNumber,
      durationKindFromMslpString, // soundingWholeNotes
      durationKindFromMslpString, // displayWholeNotes
      dotsNumber);

  return result;
}

//________________________________________________________________________
S_msrNote msrNote::createSkipFromString (
  int    inputLineNumber,
  string skipString,
  string skipMeasureNumber)
{
  // handling skipString à la LilyPond, such as "s4.."

  S_msrNote result;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Creating skip from string \"" <<
      skipString <<
      "\", skipMeasureNumber: '" << skipMeasureNumber <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  string regularExpression (
    "[[:space:]]*"
    "s" //
    "[[:space:]]*"
    "([[:digit:]]+)" // skipDuration
    "[[:space:]]*"
    "(\\.*)"         // skipDots
    "[[:space:]]*"
    );

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "regularExpression = " <<
      regularExpression <<
      endl;
  }
#endif

  regex  e (regularExpression);
  smatch sm;

  regex_match (skipString, sm, e);

  unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for skip string \"" << skipString <<
      "\" with regex \"" << regularExpression <<
      "\":" <<
      endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    gLogStream << endl;

    --gIndenter;
  }
#endif

  // handling skipString à la LilyPond, such as "s4.."

  if (smSize != 3) {
    stringstream s;

    s <<
      "skipString \"" << skipString <<
      "\" is ill-formed";

    oahError (s.str ());
  }

  string
    skipDuration = sm [1],
    skipDots     = sm [2];

  unsigned int dotsNumber = skipDots.size ();

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "skipDuration = \"" <<
      skipDuration <<
      "\"" <<
      endl <<

      "skipDots = \"" <<
      skipDots <<
      "\"" <<
      endl <<
      "dotsNumber = " <<
      dotsNumber <<
      endl;
  }
#endif

  // compute the skipDurationKind from skipDuration
  msrDurationKind
    skipDurationKind =
      msrDurationKindFromString (
        inputLineNumber,
        skipDuration);

  // compute the duration whole notes from skipDurationKind
  rational
     durationKindFromMslpString =
       msrDurationKindAsWholeNotes (
         skipDurationKind);

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "durationKindFromMslpString = " <<
      durationKindFromMslpString <<
      endl;
  }
#endif

  result =
//    msrNote::createSkipNote (
    msrNote::createRestNote (
      inputLineNumber,
      skipMeasureNumber,
      durationKindFromMslpString, // soundingWholeNotes
      durationKindFromMslpString, // displayWholeNotes
      dotsNumber);

  return result;
}

//________________________________________________________________________
S_msrNote msrNote::createNoteFromString (
  int    inputLineNumber,
  string noteString,
  string noteMeasureNumber)
{
  // handling noteString à la LilyPond, such as "bes,4.."

  S_msrNote result;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Creating note from string \"" <<
      noteString <<
      "\", noteMeasureNumber: '" << noteMeasureNumber <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  // noteString contains note specification
  // decipher it to extract its components

  string regularExpression (
    "[[:space:]]*"
    "([[:lower:]]+)" // notePitch
    "[[:space:]]*"
    "([,|']*)"       // noteOctaveIndication
    "[[:space:]]*"
    "([[:digit:]]+)" // noteDuration
    "[[:space:]]*"
    "(\\.*)"         // noteDots
    "[[:space:]]*"
    );

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "regularExpression = " <<
      regularExpression <<
      endl;
  }
#endif

  regex  e (regularExpression);
  smatch sm;

  regex_match (noteString, sm, e);

  unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for note string \"" << noteString <<
      "\" with regex \"" << regularExpression <<
      "\":" <<
      endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    gLogStream << endl;

    --gIndenter;
  }
#endif

  // handling noteString à la LilyPond, such as "bes,4.."

  if (smSize != 5) {
    stringstream s;

    s <<
      "noteString \"" << noteString <<
      "\" is ill-formed";

    oahError (s.str ());
  }

  string
    notePitch            = sm [1],
    noteOctaveIndication = sm [2],
    noteDuration         = sm [3],
    noteDots             = sm [4];

  unsigned int dotsNumber = noteDots.size ();

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "notePitch = \"" <<
      notePitch <<
      "\"" <<
      endl <<

      "noteOctaveIndication = \"" <<
      noteOctaveIndication <<
      "\"" <<
      endl <<

      "noteDuration = \"" <<
      noteDuration <<
      "\"" <<
      endl <<

      "noteDots = \"" <<
      noteDots <<
      "\"" <<
      endl <<
      "dotsNumber = " <<
      dotsNumber <<
      endl;
  }
#endif

  // fetch the quarternotes pitches kind
  msrQuarterTonesPitchKind
    quarterTonesPitchKind =
      quarterTonesPitchKindFromString (
        gGlobalMsrOahGroup->
          getMsrQuarterTonesPitchesLanguageKind (),
        notePitch);

  // compute the octave from noteOctaveIndication
  msrOctaveKind
    octaveKind =
      msrOctaveKindFromCommasOrQuotes (
        inputLineNumber,
        noteOctaveIndication);

  // compute the noteDurationKind from noteDuration
  msrDurationKind
    noteDurationKind =
      msrDurationKindFromString (
        inputLineNumber,
        noteDuration);

  // compute the duration whole notes from noteDurationKind
  rational
     durationKindFromMslpString =
       msrDurationKindAsWholeNotes (
         noteDurationKind);

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "quarterTonesPitchKind = " <<
      msrQuarterTonesPitchKindAsString (quarterTonesPitchKind) <<
      endl <<
      "octaveKind = " <<
      msrOctaveKindAsString (octaveKind) <<
      endl <<
      "durationKindFromMslpString = " <<
      durationKindFromMslpString <<
      endl;
  }
#endif

  result =
    msrNote::createRegularNote (
      inputLineNumber,
      noteMeasureNumber,
      quarterTonesPitchKind, // msrQuarterTonesPitchKind::kQTP_D_Natural
      octaveKind, // msrOctaveKind::kOctave3
      durationKindFromMslpString, // soundingWholeNotes
      durationKindFromMslpString, // displayWholeNotes
      dotsNumber);

  return result;
}

//________________________________________________________________________
S_msrNote msrNote::createNoteFromSemiTonesPitchAndOctave (
  int                          inputLineNumber,
  S_msrSemiTonesPitchAndOctave semiTonesPitchAndOctave)
{
  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    semiTonesPitchAndOctave != nullptr,
    "semiTonesPitchAndOctave is null");

  msrNote * o =
    new msrNote (
      inputLineNumber,
      K_NO_MEASURE_NUMBER, // JMI ???

      msrNoteKind::kNoteRegularInMeasure, // noteKind

      quarterTonesPitchKindFromSemiTonesPitchKind (
        semiTonesPitchAndOctave->
          getSemiTonesPitchKind ()),
      semiTonesPitchAndOctave->
        getOctaveKind (),

      rational (0, 1), // soundingWholeNotes,
      rational (0, 1), // displayWholeNotes,

      0, // dotsNumber,

      msrDurationKind::k_NoDuration, // noteGraphicDuration

      msrQuarterTonesPitchKind::k_NoQuarterTonesPitch, // noteDisplayQuarterTonesPitch
      msrOctaveKind::k_NoOctave, // noteDisplayOctave,

      msrNote::kNoteIsACueNoteNo,

      msrPrintObjectKind::kPrintObjectNo, // JMI

      kNoteHeadNormal, // JMI
      kNoteHeadFilledYes, // JMI
      kNoteHeadParenthesesNo); // JMI
  assert (o != nullptr);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceNotesOctaveEntry ()) {
    gLogStream <<
      "Creating note '" <<
      o->asShortString () <<
      "' from semitones pitch and octave '" <<
      semiTonesPitchAndOctave->asString () <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  return o;
}

//________________________________________________________________________
void msrNote::setNotePositionInMeasure (
  rational positionInMeasure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Setting note position in measure of " << asString () <<
      " to '" <<
      positionInMeasure <<
      "'" <<
      endl;
  }
#endif

  positionInMeasure.rationalise (); // TEMP ? JMI

  string context =
    "setNotePositionInMeasure()";

  setMeasureElementPositionInMeasure (
    positionInMeasure,
    context);

  if (false) { // JMI
  // compute note's position in voice
  rational
     positionInVoice =
      fNoteDirectMeasureUpLink->getMeasurePositionInVoice ()
        +
      positionInMeasure;
  positionInVoice.rationalise ();

  // set note's position in voice
  setMeasureElementPositionInVoice (
    positionInVoice,
    context);

  // update current position in voice
  S_msrVoice
    voice =
      fNoteDirectMeasureUpLink->
        fetchMeasureVoiceUpLink ();

  voice->
    incrementCurrentPositionInVoice (
      fMeasureElementSoundingWholeNotes);
}

  // are there harmonies attached to this note?
  if (fNoteHarmoniesList.size ()) {
    list<S_msrHarmony>::const_iterator i;
    for (
      i=fNoteHarmoniesList.begin (); i!=fNoteHarmoniesList.end (); ++i
    ) {
      // set the harmony position in measure, taking it's offset into account
      (*i)->
        setHarmonyPositionInMeasure (
          positionInMeasure);
    } // for
  }

  // are there figured basses attached to this note?
  if (fNoteFiguredBassesList.size ()) {
    list<S_msrFiguredBass>::const_iterator i;
    for (
      i=fNoteFiguredBassesList.begin (); i!=fNoteFiguredBassesList.end (); ++i
    ) {
      // set the figured bass position in measure
      (*i)->
        setFiguredBassPositionInMeasure (
          positionInMeasure);
    } // for
  }

  // are there dal segnos attached to this note?
  if (fNoteDalSegnos.size ()) {
    list<S_msrDalSegno>::const_iterator i;
    for (
      i=fNoteDalSegnos.begin (); i!=fNoteDalSegnos.end (); ++i
    ) {
      // set the dal segno position in measure
      (*i)->
        setDalSegnoPositionInMeasure (
          positionInMeasure);
    } // for
  }
}

void msrNote::setNoteOccupiesAFullMeasure ()
{
  fNoteOccupiesAFullMeasure = true;

  if (getNoteIsARest ()) {
    // the measure it belongs to is a single-measure rest
    fNoteDirectMeasureUpLink->
      setMeasureIsAFullMeasureRest ();
  }
}

string msrNote::noteSoundingWholeNotesAsMsrString () const
{
  string result;

  if (fMeasureElementSoundingWholeNotes.getNumerator () == 0)
    result = " (no sounding whole notes)";
  else
    result =
      wholeNotesAsMsrString (
        fInputLineNumber,
        fMeasureElementSoundingWholeNotes);

  return result;
  }

string msrNote::noteDisplayWholeNotesAsMsrString () const
{
  string result;

  if (fNoteDisplayWholeNotes.getNumerator () == 0) {
    result = " (no display whole notes)";
  }
  else {
    result =
      wholeNotesAsMsrString (
        fInputLineNumber,
        fNoteDisplayWholeNotes);
  }

  return result;
}

string msrNote::noteHeadKindAsString (
  msrNoteHeadKind noteHeadKind)
{
  string result;

  switch (noteHeadKind) {
    case msrNote::kNoteHeadSlash:
      result = "noteHeadSlash";
      break;
    case msrNote::kNoteHeadTriangle:
      result = "noteHeadTriangle";
      break;
    case msrNote::kNoteHeadDiamond:
      result = "noteHeadDiamond";
      break;
    case msrNote::kNoteHeadSquare:
      result = "noteHeadSquare";
      break;
    case msrNote::kNoteHeadCross:
      result = "noteHeadCross";
      break;
    case msrNote::kNoteHeadX:
      result = "noteHeadX";
      break;
    case msrNote::kNoteHeadCircleX:
      result = "noteHeadCircleX";
      break;
    case msrNote::kNoteHeadInvertedTriangle:
      result = "noteHeadInvertedTriangle";
      break;
    case msrNote::kNoteHeadArrowDown:
      result = "noteHeadArrowDown";
      break;
    case msrNote::kNoteHeadArrowUp:
      result = "noteHeadArrowUp";
      break;
    case msrNote::kNoteHeadSlashed:
      result = "noteHeadSlashed";
      break;
    case msrNote::kNoteHeadBackSlashed:
      result = "noteHeadBackSlashed";
      break;
    case msrNote::kNoteHeadNormal:
      result = "noteHeadNormal";
      break;
    case msrNote::kNoteHeadCluster:
      result = "noteHeadCluster";
      break;
    case msrNote::kNoteHeadCircleDot:
      result = "noteHeadCircleDot";
      break;
    case msrNote::kNoteHeadLeftTriangle:
      result = "noteHeadLeftTriangle";
      break;
    case msrNote::kNoteHeadRectangle:
      result = "noteHeadRectangle";
      break;
    case msrNote::kNoteHeadNone:
      result = "noteHeadNone";
      break;
    case msrNote::kNoteHeadDo:
      result = "noteHeadDo";
      break;
    case msrNote::kNoteHeadRe:
      result = "noteHeadRe";
      break;
    case msrNote::kNoteHeadMi:
      result = "noteHeadMi";
      break;
    case msrNote::kNoteHeadFa:
      result = "noteHeadFa";
      break;
    case msrNote::kNoteHeadFaUp:
      result = "noteHeadFaUp";
      break;
    case msrNote::kNoteHeadSo:
      result = "noteHeadSo";
      break;
    case msrNote::kNoteHeadLa:
      result = "noteHeadLa";
      break;
    case msrNote::kNoteHeadTi:
      result = "noteHeadTi";
      break;
  } // switch

  return result;
}

string msrNote::noteHeadFilledKindAsString (
  msrNoteHeadFilledKind noteHeadFilledKind)
{
  string result;

  switch (noteHeadFilledKind) {
    case msrNote::kNoteHeadFilledYes:
      result = "noteHeadFilledYes";
      break;
    case msrNote::kNoteHeadFilledNo:
      result = "noteHeadFilledNo";
      break;
  } // switch

  return result;
}

string msrNote::noteHeadParenthesesKindAsString (
  msrNoteHeadParenthesesKind noteHeadParenthesesKind)
{
  string result;

  switch (noteHeadParenthesesKind) {
    case msrNote::kNoteHeadParenthesesYes:
      result = "noteHeadParenthesesYes";
      break;
    case msrNote::kNoteHeadParenthesesNo:
      result = "noteHeadParenthesesNo";
      break;
  } // switch

  return result;
}

string msrNote::noteIsACueNoteKindAsString (
  msrNoteIsACueNoteKind noteIsACueNoteKind)
{
  string result;

  switch (noteIsACueNoteKind) {
    case msrNote::kNoteIsACueNoteYes:
      result = "noteIsACueNoteYes";
      break;
    case msrNote::kNoteIsACueNoteNo:
      result = "noteIsACueNoteNo";
      break;
  } // switch

  return result;
}

string msrNote::notePrintObjectKindAsString () const
{
  return
    msrPrintObjectKindAsString (
      fNotePrintObjectKind);
}

string msrNote::noteHeadKindAsString () const
{
  return
    noteHeadKindAsString (
      fNoteHeadKind);
}

string msrNote::noteHeadFilledKindAsString () const
{
  return
    noteHeadFilledKindAsString (
      fNoteHeadFilledKind);
}

string msrNote::noteHeadParenthesesKindAsString () const
{
  return
    noteHeadParenthesesKindAsString (
      fNoteHeadParenthesesKind);
}

msrDiatonicPitchKind msrNote::noteDiatonicPitchKind (
  int inputLineNumber) const
{
  return
    diatonicPitchKindFromQuarterTonesPitchKind (
      inputLineNumber,
      fNoteQuarterTonesPitchKind);
}

bool msrNote::noteIsAPitchedRest () const
{
  return
    getNoteIsARest () && fNoteDisplayOctaveKind != msrOctaveKind::k_NoOctave;
}

void msrNote::setNoteStem (S_msrStem stem)
{
  // register note stem
  fNoteStem = stem;

  // mark note as stemless if relevant
  if (stem->getStemKind () == msrStem::kStemNeutral)
    fNoteIsStemless = true;
}

void msrNote::setNoteBelongsToAChord ()
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceChords ()) {
    gLogStream <<
      "Setting note '" <<
      asShortString () <<
      "' to belong to a chord"
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  fNoteBelongsToAChord = true;
}

void msrNote::determineTupletMemberSoundingFromDisplayWholeNotes (
  int actualNotes, int normalNotes)
{
  /*
  Type indicates the graphic note type, Valid values (from shortest to longest) are 1024th, 512th, 256th, 128th, 64th, 32nd, 16th, eighth, quarter, half, whole, breve, long, and maxima. The size attribute indicates full, cue, or large size, with full the default for regular notes and cue the default for cue and grace notes.

  Quarter note in a triplet, sounds 2/3 of a quarter:
      <note>
        <pitch>
          <step>B</step>
          <octave>4</octave>
        </pitch>
        <duration>20</duration>
        <voice>1</voice>
        <type>quarter</type>
        <time-modification>
          <actual-notes>3</actual-notes>
          <normal-notes>2</normal-notes>
        </time-modification>
        <notations>
          <tuplet number="1" type="start" />
        </notations>
      </note>
  */

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "Determining tuplet sounding from display whole notes" <<
      " for note '" <<
      asShortString () <<
      ", tuplet factor is '" <<
      actualNotes << "/" << normalNotes <<
      "', line " << fInputLineNumber <<
      endl;
  }
#endif

  // the display whole notes are known, compute the sounding ones
  fMeasureElementSoundingWholeNotes =
    fNoteDisplayWholeNotes
      *
    normalNotes
      /
    actualNotes;
  fMeasureElementSoundingWholeNotes.rationalise ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTuplets ()) {
    gLogStream <<
      "The result is: '" <<
      asShortString () <<
      endl;
  }
#endif
}

void msrNote::appendBeamToNote (S_msrBeam beam)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceBeams ()) {
    gLogStream <<
      "Adding beam '" <<
      beam->asShortString () <<
      "' to note '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // check the order of the beams begins, continues and ends
if (false) { // JMI, note not yet append to anything....
  S_msrVoice
    voice = fetchNoteVoiceUpLink ();

  voice->checkBeamNumber (beam, this);
}

  // now append the beam
  fNoteBeams.push_back (beam);
}

void msrNote::appendArticulationToNote (S_msrArticulation art)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceArticulations ()) {
    gLogStream <<
      "Adding articulation '" <<
      art->asShortString () <<
      "' to note '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  fNoteArticulations.push_back (art);
}

void msrNote::appendSpannerToNote (S_msrSpanner spanner)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceSpanners ()) {
    gLogStream <<
      "Appending spanner '" <<
      spanner->spannerKindAsString () <<
      "' to note '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // register note has having a wavy line start
  switch (spanner->getSpannerKind ()) {
    case msrSpanner::kSpannerDashes:
      break;

    case msrSpanner::kSpannerWavyLine:
      switch (spanner->getSpannerTypeKind ()) {
        case msrSpannerTypeKind::kSpannerTypeStart:
          fNoteWavyLineSpannerStart = spanner;
          break;
        case msrSpannerTypeKind::kSpannerTypeStop:
          fNoteWavyLineSpannerStop = spanner;
          break;
        case msrSpannerTypeKind::kSpannerTypeContinue:
          break;
        case msrSpannerTypeKind::k_NoSpannerType:
          // JMI ???
          break;
      } // switch
      break;
  } // switch

  // append spanner to note spanners
  fNoteSpanners.push_back (spanner);
}

void msrNote::appendTechnicalToNote (S_msrTechnical technical)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTechnicals ()) {
    gLogStream <<
      "Adding technical '" <<
      technical->asString () <<
      "' to note '" << asString () <<
      "', line " << fInputLineNumber <<
      endl;
  }
#endif

  // append the technical to the note technicals list
  fNoteTechnicals.push_back (technical);
}

void msrNote::appendTechnicalWithIntegerToNote (
  S_msrTechnicalWithInteger technicalWithInteger)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTechnicals ()) {
    gLogStream <<
      "Adding technical with integer '" <<
      technicalWithInteger->asString () <<
      "' to note '" << asString () <<
      "', line " << fInputLineNumber <<
      endl;
  }
#endif

  // append the technical with integer to the note technicals with integers list
  fNoteTechnicalWithIntegers.push_back (
    technicalWithInteger);
}

void msrNote::appendTechnicalWithFloatToNote (
  S_msrTechnicalWithFloat technicalWithFloat)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTechnicals ()) {
    gLogStream <<
      "Adding technical with float '" <<
      technicalWithFloat->asString () <<
      "' to note '" << asString () <<
      "', line " << fInputLineNumber <<
      endl;
  }
#endif

  // append the technical with float to the note technicals with floats list
  fNoteTechnicalWithFloats.push_back (
    technicalWithFloat);
}

void msrNote::appendTechnicalWithStringToNote (
  S_msrTechnicalWithString technicalWithString)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTechnicals ()) {
    gLogStream <<
      "Adding technical with string'" <<
      technicalWithString->asString () <<
      "' to note '" << asString () <<
      "', line " << fInputLineNumber <<
      endl;
  }
#endif

  // append the technical with string to the note technicals with strings list
  fNoteTechnicalWithStrings.push_back (
    technicalWithString);
}

void msrNote::appendOrnamentToNote (S_msrOrnament ornament)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOrnaments ()) {
    gLogStream <<
      "Adding ornament '" <<
      ornament->asString () <<
      "' to note '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // append the ornament to the note ornaments list
  fNoteOrnaments.push_back (ornament);

  switch (ornament->getOrnamentKind ()) {
    case msrOrnament::kOrnamentTrill:
      fNoteTrillOrnament = ornament;
      break;

/* JMI
    case msrOrnament::kOrnamentDashes:
      fNoteDashesOrnament = ornament;
      break;
*/

    case msrOrnament::kOrnamentDelayedTurn:
      fNoteDelayedTurnOrnament = ornament;
      break;

    case msrOrnament::kOrnamentDelayedInvertedTurn:
      fNoteDelayedInvertedTurnOrnament = ornament;
      break;

    default:
      {}
  } // switch

  // set ornament's note upLink
  ornament->
    setOrnamentNoteUpLink (this);
}

void msrNote::appendGlissandoToNote (S_msrGlissando glissando)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceGlissandos ()) {
    gLogStream <<
      "Adding glissando '" <<
      glissando->asShortString () <<
      "' to note '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // append the glissando to the note glissandos list
  fNoteGlissandos.push_back (glissando);
}

void msrNote::appendSlideToNote (S_msrSlide slide)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceSlides ()) {
    gLogStream <<
      "Adding slide '" <<
      slide->asShortString () <<
      "' to note '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // append the slide to the note glissandos list
  fNoteSlides.push_back (slide);
}

void msrNote::setNoteGraceNotesGroupBefore (
  S_msrGraceNotesGroup graceNotesGroupBefore)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Attaching grace notes group '" <<
      graceNotesGroupBefore->asString () <<
      "' before note '" <<
      asString () <<
      "', line " << graceNotesGroupBefore->getInputLineNumber () <<
      endl;
  }
#endif

  // register the before grace notes group in the note
  fNoteGraceNotesGroupBefore = graceNotesGroupBefore;

  // setup the grace notes group's note upLink
  graceNotesGroupBefore->
    setGraceNotesGroupNoteUpLink (
      this);
}

void msrNote::setNoteGraceNotesGroupAfter (
  S_msrGraceNotesGroup graceNotesGroupAfter)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Attaching grace notes group '" <<
      graceNotesGroupAfter->asString () <<
      "' after note '" <<
      asString () <<
      "', line " << graceNotesGroupAfter->getInputLineNumber () <<
      endl;
  }
#endif

  // register the after grace notes group in the note
  fNoteGraceNotesGroupAfter = graceNotesGroupAfter;

  // setup the grace notes group's note upLink
  graceNotesGroupAfter->
    setGraceNotesGroupNoteUpLink (
      this);
}

/* JMI
void msrNote::setNoteAfterGraceNotesGroup (S_msrGraceNotesGroup afterGraceNotesGroup)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Attaching afterGraceNotesGroup '" << afterGraceNotesGroup->asString () <<
      "' to note '" << asShortString () <<
      "', line " << afterGraceNotesGroup->getInputLineNumber () <<
      endl;
  }
#endif

  // register the grace notes in the note
  fNoteAfterGraceNotesGroup = afterGraceNotesGroup;
}
*/

void msrNote::setNoteSingleTremolo (S_msrSingleTremolo trem)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceTremolos ()) {
    gLogStream <<
      "Adding singleTremolo '" << trem->asString () <<
      "' to note '" << asString () <<
      "', line " << trem->getInputLineNumber () <<
      endl;
  }
#endif

  // register the singleTremolo in the note
  fNoteSingleTremolo = trem;
}

void msrNote::appendDynamicsToNote (S_msrDynamics dynamics)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceDynamics ()) {
    gLogStream <<
      "Attaching dynamics '" <<
      dynamics->asString () <<
      "' to note '" <<
      asString () <<
      "', line " << dynamics->getInputLineNumber () <<
      endl;
  }
#endif

  fNoteDynamics.push_back (dynamics);
}
void msrNote::appendOtherDynamicsToNote (S_msrOtherDynamics otherDynamics)
{
  fNoteOtherDynamics.push_back (otherDynamics);
}

void msrNote::appendWordsToNote (S_msrWords words)
{
  fNoteWords.push_back (words);
}

void msrNote::appendSlurToNote (S_msrSlur slur)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceSlurs ()) {
    gLogStream <<
      "Adding slur '" << slur <<
      "' to note '" << asString () << "'" <<
      endl;
  }
#endif

  fNoteSlurs.push_back (slur);
}

void msrNote::appendLigatureToNote (S_msrLigature ligature)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceLigatures ()) {
    gLogStream <<
      "Appending ligature " << ligature << " to note " << asString () <<
       endl;
  }
#endif

  if (fNoteLigatures.size ()) {
    if (
      fNoteLigatures.back ()->getLigatureKind () == msrLigature::kLigatureStart
        &&
      ligature->getLigatureKind () == msrLigature::kLigatureStop
        &&
      fNoteLigatures.back ()->getLigatureNumber () == ligature->getLigatureNumber ()
      ) {
      // it may happen that a given note has a 'ligature start'
      // and a 'ligature stop' in sequence, ignore both

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceLigatures ()) {
        stringstream s;

        s <<
          "a 'ligature start' is immediately followed by a 'ligature stop'" <<
          endl <<
          "with the same number, ignoring both of them at line " <<
          ligature->getInputLineNumber ();

        msrMusicXMLWarning (
          gGlobalOahOahGroup->getInputSourceName (),
          ligature->getInputLineNumber (),
          s.str ());
      }
#endif

      // remove 'ligature start'
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceLigatures ()) {
        gLogStream <<
          "Removing last ligature (start) for note '" <<
          asShortString () <<
          "'" <<
          endl;
      }
#endif

      fNoteLigatures.pop_back ();

      // don't register 'ligature stop'
    }

    else
      fNoteLigatures.push_back (ligature);
  }

  else
    fNoteLigatures.push_back (ligature);
}

void msrNote::appendPedalToNote (S_msrPedal pedal)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePedals ()) {
    gLogStream <<
      "Appending pedal " << pedal << " to note " << asString () <<
       endl;
  }
#endif

  if (fNotePedals.size ()) {
    if (
      fNotePedals.back ()->getPedalTypeKind () == msrPedal::kPedalStart
        &&
      pedal->getPedalTypeKind () == msrPedal::kPedalStop
    ) {
      // it may happen that a given note has a 'pedal start'
      // and a 'pedal stop' in sequence, ignore both            // JMI ???

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTracePedals ()) {
        stringstream s;

        s <<
          "a 'pedal start' is immediately followed by a 'pedal stop'" <<
          endl <<
          "with the same number, ignoring both of them at line " <<
          pedal->getInputLineNumber ();

        msrMusicXMLWarning (
          gGlobalOahOahGroup->getInputSourceName (),
          pedal->getInputLineNumber (),
          s.str ());
      }
#endif

      // rmeove 'pedal start'
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTracePedals ()) {
        gLogStream <<
          "Removing last pedal (start) for note '" <<
          asShortString () <<
          "'" <<
          endl;
      }
#endif
      fNotePedals.pop_back ();

      // don't register 'pedal stop'
    }

    else
      fNotePedals.push_back (pedal);
  }

  else
    fNotePedals.push_back (pedal);
}

void msrNote::appendSlashToNote (S_msrSlash slash)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceSlashes ()) {
    gLogStream <<
      "Appending slash '" <<
      slash <<
      "' to note '" <<
      asShortString () <<
      "'" <<
      endl;
  }
#endif

  fNoteSlashes.push_back (slash);
}

void msrNote::appendWedgeToNote (S_msrWedge wedge)
{
  fNoteWedges.push_back (wedge);
}

void msrNote::appendSegnoToNote (S_msrSegno segno)
{
  fNoteSegnos.push_back (segno);
}

void msrNote::appendDalSegnoToNote (S_msrDalSegno dalSegno)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceDalSegnos ()) {
    gLogStream <<
      "Appending dal segno " <<
      dalSegno->asShortString () <<
      " to note " <<
      asShortString () <<
      endl;
  }
#endif

  fNoteDalSegnos.push_back (dalSegno);
}

void msrNote::appendCodaToNote (S_msrCoda coda)
{
  fNoteCodas.push_back (coda);
}

void msrNote::appendEyeGlassesToNote (S_msrEyeGlasses eyeGlasses)
{
  fNoteEyeGlasses.push_back (eyeGlasses);
}

void msrNote::appendDampToNote (S_msrDamp damp)
{
  fNoteDamps.push_back (damp);
}

void msrNote::appendDampAllToNote (S_msrDampAll dampAll)
{
  fNoteDampAlls.push_back (dampAll);
}

void msrNote::appendScordaturaToNote (S_msrScordatura scordatura)
{
  fNoteScordaturas.push_back (scordatura);
}

void msrNote::assignMeasureElementPositionInVoice (
  rational& positionInVoice,
  string    context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Assigning note position in voice of " <<
      asString () <<
      " to '" << positionInVoice <<
      "' in measure '" <<
      fMeasureElementMeasureNumber <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    positionInVoice != K_NO_POSITION,
    "positionInVoice == K_NO_POSITION");

  // set measure element position in voice
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Setting note position in voice of " <<
      asString () <<
      " to '" << positionInVoice <<
      "' in measure '" <<
      fMeasureElementMeasureNumber <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

  fMeasureElementPositionInVoice = positionInVoice;

  // account for it in positionInVoice
  positionInVoice +=
    fMeasureElementSoundingWholeNotes;
  positionInVoice.rationalise ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Position in voice becomes " <<
      positionInVoice <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif
}

S_msrDynamics msrNote::removeFirstDynamics () // JMI
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceDynamics ()) {
    gLogStream <<
      "Removing first dynamics from note '" <<
      asShortString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    fNoteDynamics.size () > 0,
    "fNoteDynamics is empty");

  S_msrDynamics dyn = fNoteDynamics.front ();
  fNoteDynamics.pop_front ();

  return dyn;
}

S_msrWedge msrNote::removeFirstWedge () // JMI
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceWedges ()) {
    gLogStream <<
      "Removing first wedge from note '" <<
      asShortString () <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
    __FILE__, __LINE__,
    fNoteDynamics.size () > 0,
    "fNoteDynamics is empty");

  S_msrWedge wedge = fNoteWedges.front ();
  fNoteWedges.pop_front ();
  return wedge;
}

void msrNote::appendSyllableToNote (S_msrSyllable syllable)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceLyrics ()) {
    gLogStream <<
      "Appending syllable '" <<
      syllable->asString () <<
      "' to note '" << asString () <<
      "'" <<
      endl;
  }
#endif

  fNoteSyllables.push_back (syllable);
}

void msrNote::appendHarmonyToNoteHarmoniesList (S_msrHarmony harmony)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceHarmonies ()) {
    gLogStream <<
      "Appending harmony " <<
      harmony->asString () <<
      " to the harmonies list of " <<
      asString () <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  // update the harmony whole notes if it belongs to a tuplet ??? utf8.xml

  fNoteHarmoniesList.push_back (harmony);

  // register this note as the harmony upLink
  harmony->
    setHarmonyNoteUpLink (this);
}

void msrNote::appendFiguredBassToNoteFiguredBassesList (
  S_msrFiguredBass figuredBass)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceFiguredBasses ()) {
    gLogStream <<
      "Append figured bass " <<
      figuredBass->asString () <<
      " to the figured basses list of " <<
      asString () <<
      ", line " << fInputLineNumber <<
      endl;
  }
#endif

  // update the figured bass whole notes if it belongs to a tuplet ??? utf8.xml

  fNoteFiguredBassesList.push_back (figuredBass);

  // register this note as the figuredBass upLink
  figuredBass->
    setFiguredBassNoteUpLink (this);
}

void msrNote::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrNote::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrNote>*
    p =
      dynamic_cast<visitor<S_msrNote>*> (v)) {
        S_msrNote elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrNote::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrNote::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrNote::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrNote>*
    p =
      dynamic_cast<visitor<S_msrNote>*> (v)) {
        S_msrNote elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrNote::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrNote::browseData (basevisitor* v)
{
  // browse the grace notes group before if any
  if (fNoteGraceNotesGroupBefore) {
    // fetch the score
    S_msrScore
      score =
        fetchNoteScoreUpLink ();

    if (score) {
      bool
        inhibitGraceNotesGroupsBeforeBrowsing =
          score->getInhibitGraceNotesGroupsBeforeBrowsing ();

      if (inhibitGraceNotesGroupsBeforeBrowsing) {
#ifdef TRACING_IS_ENABLED
        if (
          gGlobalMsrOahGroup->getTraceMsrVisitors ()
            ||
          gGlobalTraceOahGroup->getTraceNotes ()
            ||
          gGlobalTraceOahGroup->getTraceGraceNotes ()
        ) {
          gLogStream <<
            "% ==> visiting grace notes groups before is inhibited" <<
            endl;
        }
#endif
      }
      else {
        // browse the grace notes group before
        msrBrowser<msrGraceNotesGroup> browser (v);
        browser.browse (*fNoteGraceNotesGroupBefore);
      }
    }
  }

  if (fNoteOctaveShift) {
    // browse the octave shift
    msrBrowser<msrOctaveShift> browser (v);
    browser.browse (*fNoteOctaveShift);
  }

  if (fNoteStem) {
    // browse the stem
    msrBrowser<msrStem> browser (v);
    browser.browse (*fNoteStem);
  }

  // browse the beams if any
  if (fNoteBeams.size ()) {
    ++gIndenter;
    list<S_msrBeam>::const_iterator i;
    for (i=fNoteBeams.begin (); i!=fNoteBeams.end (); ++i) {
      // browse the beam
      msrBrowser<msrBeam> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the articulations if any
  if (fNoteArticulations.size ()) {
    ++gIndenter;
    list<S_msrArticulation>::const_iterator i;
    for (i=fNoteArticulations.begin (); i!=fNoteArticulations.end (); ++i) {
      // browse the articulation
      msrBrowser<msrArticulation> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the spanners if any
  if (fNoteSpanners.size ()) {
    ++gIndenter;
    list<S_msrSpanner>::const_iterator i;
    for (i=fNoteSpanners.begin (); i!=fNoteSpanners.end (); ++i) {
      // browse the spanner
      msrBrowser<msrSpanner> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the technicals if any
  if (fNoteTechnicals.size ()) {
    ++gIndenter;
    list<S_msrTechnical>::const_iterator i;
    for (
      i=fNoteTechnicals.begin ();
      i!=fNoteTechnicals.end ();
      ++i
  ) {
      // browse the technical
      msrBrowser<msrTechnical> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the technicals with integer if any
  if (fNoteTechnicalWithIntegers.size ()) {
    ++gIndenter;
    list<S_msrTechnicalWithInteger>::const_iterator i;
    for (
      i=fNoteTechnicalWithIntegers.begin ();
      i!=fNoteTechnicalWithIntegers.end ();
      ++i
  ) {
      // browse the technical
      msrBrowser<msrTechnicalWithInteger> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the technicals with float if any
  if (fNoteTechnicalWithFloats.size ()) {
    ++gIndenter;
    list<S_msrTechnicalWithFloat>::const_iterator i;
    for (
      i=fNoteTechnicalWithFloats.begin ();
      i!=fNoteTechnicalWithFloats.end ();
      ++i
  ) {
      // browse the technical
      msrBrowser<msrTechnicalWithFloat> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the technicals with string if any
  if (fNoteTechnicalWithStrings.size ()) {
    ++gIndenter;
    list<S_msrTechnicalWithString>::const_iterator i;
    for (
      i=fNoteTechnicalWithStrings.begin ();
      i!=fNoteTechnicalWithStrings.end ();
      ++i
  ) {
      // browse the technical
      msrBrowser<msrTechnicalWithString> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the ornaments if any
  if (fNoteOrnaments.size ()) {
    ++gIndenter;
    list<S_msrOrnament>::const_iterator i;
    for (i=fNoteOrnaments.begin (); i!=fNoteOrnaments.end (); ++i) {
      // browse the ornament
      msrBrowser<msrOrnament> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the glissandos if any
  if (fNoteGlissandos.size ()) {
    ++gIndenter;
    list<S_msrGlissando>::const_iterator i;
    for (i=fNoteGlissandos.begin (); i!=fNoteGlissandos.end (); ++i) {
      // browse the glissando
      msrBrowser<msrGlissando> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the slides if any
  if (fNoteSlides.size ()) {
    ++gIndenter;
    list<S_msrSlide>::const_iterator i;
    for (i=fNoteSlides.begin (); i!=fNoteSlides.end (); ++i) {
      // browse the glissando
      msrBrowser<msrSlide> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the single tremolo if any
  if (fNoteSingleTremolo) {
    // browse the singleTremolo
    msrBrowser<msrSingleTremolo> browser (v);
    browser.browse (*fNoteSingleTremolo);
  }

  if (fNoteTie) {
    // browse the tie
    msrBrowser<msrTie> browser (v);
    browser.browse (*fNoteTie);
  }

  // browse the dynamics if any
  if (fNoteDynamics.size ()) {
    ++gIndenter;
    list<S_msrDynamics>::const_iterator i;
    for (i=fNoteDynamics.begin (); i!=fNoteDynamics.end (); ++i) {
      // browse the dynamics
      msrBrowser<msrDynamics> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the other dynamics if any
  if (fNoteOtherDynamics.size ()) {
    ++gIndenter;
    list<S_msrOtherDynamics>::const_iterator i;
    for (i=fNoteOtherDynamics.begin (); i!=fNoteOtherDynamics.end (); ++i) {
      // browse the other dynamics
      msrBrowser<msrOtherDynamics> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the words if any
  if (fNoteWords.size ()) {
    ++gIndenter;
    list<S_msrWords>::const_iterator i;
    for (i=fNoteWords.begin (); i!=fNoteWords.end (); ++i) {
      // browse the words
      msrBrowser<msrWords> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the slurs if any
  if (fNoteSlurs.size ()) {
    ++gIndenter;
    list<S_msrSlur>::const_iterator i;
    for (i=fNoteSlurs.begin (); i!=fNoteSlurs.end (); ++i) {
      // browse the slur
      msrBrowser<msrSlur> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the ligatures if any
  if (fNoteLigatures.size ()) {
    ++gIndenter;
    list<S_msrLigature>::const_iterator i;
    for (i=fNoteLigatures.begin (); i!=fNoteLigatures.end (); ++i) {
      // browse the ligature
      msrBrowser<msrLigature> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the pedals if any
  if (fNotePedals.size ()) {
    ++gIndenter;
    list<S_msrPedal>::const_iterator i;
    for (i=fNotePedals.begin (); i!=fNotePedals.end (); ++i) {
      // browse the pedal
      msrBrowser<msrPedal> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the wedges if any
  if (fNoteSlashes.size ()) {
    ++gIndenter;
    list<S_msrSlash>::const_iterator i;
    for (i=fNoteSlashes.begin (); i!=fNoteSlashes.end (); ++i) {
      // browse the wedge
      msrBrowser<msrSlash> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the wedges if any
  if (fNoteWedges.size ()) {
    ++gIndenter;
    list<S_msrWedge>::const_iterator i;
    for (i=fNoteWedges.begin (); i!=fNoteWedges.end (); ++i) {
      // browse the wedge
      msrBrowser<msrWedge> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the segnos if any
  if (fNoteSegnos.size ()) {
    ++gIndenter;
    list<S_msrSegno>::const_iterator i;
    for (i=fNoteSegnos.begin (); i!=fNoteSegnos.end (); ++i) {
      // browse the segno
      msrBrowser<msrSegno> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the dal segnos if any
  if (fNoteDalSegnos.size ()) {
    ++gIndenter;
    list<S_msrDalSegno>::const_iterator i;
    for (i=fNoteDalSegnos.begin (); i!=fNoteDalSegnos.end (); ++i) {
      // browse the dal segno
      msrBrowser<msrDalSegno> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the codas if any
  if (fNoteCodas.size ()) {
    ++gIndenter;
    list<S_msrCoda>::const_iterator i;
    for (i=fNoteCodas.begin (); i!=fNoteCodas.end (); ++i) {
      // browse the coda
      msrBrowser<msrCoda> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the eyeglasses if any
  if (fNoteEyeGlasses.size ()) {
    ++gIndenter;
    list<S_msrEyeGlasses>::const_iterator i;
    for (i=fNoteEyeGlasses.begin (); i!=fNoteEyeGlasses.end (); ++i) {
      // browse the eyeglasses
      msrBrowser<msrEyeGlasses> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the damps if any
  if (fNoteDamps.size ()) {
    ++gIndenter;
    list<S_msrDamp>::const_iterator i;
    for (i=fNoteDamps.begin (); i!=fNoteDamps.end (); ++i) {
      // browse the damp
      msrBrowser<msrDamp> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the damp alls if any
  if (fNoteDampAlls.size ()) {
    ++gIndenter;
    list<S_msrDampAll>::const_iterator i;
    for (i=fNoteDampAlls.begin (); i!=fNoteDampAlls.end (); ++i) {
      // browse the damp all
      msrBrowser<msrDampAll> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the scordaturas if any
  if (fNoteScordaturas.size ()) {
    ++gIndenter;
    list<S_msrScordatura>::const_iterator i;
    for (i=fNoteScordaturas.begin (); i!=fNoteScordaturas.end (); ++i) {
      // browse the scordatura
      msrBrowser<msrScordatura> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the harmonies if any
  if (fNoteHarmoniesList.size ()) {
    list<S_msrHarmony>::const_iterator i;
    for (
      i=fNoteHarmoniesList.begin (); i!=fNoteHarmoniesList.end (); ++i
    ) {
      // browse the harmony
      msrBrowser<msrHarmony> browser (v);
      browser.browse (*(*i));
    } // for
  }

  // browse the figured basses if any
  if (fNoteFiguredBassesList.size ()) {
    list<S_msrFiguredBass>::const_iterator i;
    for (
      i=fNoteFiguredBassesList.begin (); i!=fNoteFiguredBassesList.end (); ++i
    ) {
      // browse the figured bass
      msrBrowser<msrFiguredBass> browser (v);
      browser.browse (*(*i));
    } // for
  }

  // browse the syllables if any
  if (fNoteSyllables.size ()) {
    ++gIndenter;
    list<S_msrSyllable>::const_iterator i;
    for (i=fNoteSyllables.begin (); i!=fNoteSyllables.end (); ++i) {
      // browse the syllable
      msrBrowser<msrSyllable> browser (v);
      browser.browse (*(*i));
    } // for
    --gIndenter;
  }

  // browse the grace notes group after if any
  if (fNoteGraceNotesGroupAfter) {
    // fetch the score
    S_msrScore
      score =
        fNoteDirectMeasureUpLink->
          fetchMeasureScoreUpLink ();

    if (score) {
      bool
        inhibitGraceNotesGroupsAfterBrowsing =
          score->getInhibitGraceNotesGroupsAfterBrowsing ();

      if (inhibitGraceNotesGroupsAfterBrowsing) {
#ifdef TRACING_IS_ENABLED
        if (
          gGlobalMsrOahGroup->getTraceMsrVisitors ()
            ||
          gGlobalTraceOahGroup->getTraceNotes ()
            ||
          gGlobalTraceOahGroup->getTraceGraceNotes ()
        ) {
          gLogStream <<
            "% ==> visiting grace notes groups after is inhibited" <<
            endl;
        }
#endif
      }
      else {
        // browse the grace notes group after
        msrBrowser<msrGraceNotesGroup> browser (v);
        browser.browse (*fNoteGraceNotesGroupAfter);
      }
    }
  }
}

string msrNote::notePitchAsString () const
{
  stringstream s;

  switch (fNoteKind) {
    case msrNoteKind::k_NoNote:
    case msrNoteKind::kNoteRestInMeasure:
    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteInDoubleTremolo:
    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
    case msrNoteKind::kNoteRegularInChord:
    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      s <<
        msrQuarterTonesPitchKindAsStringInLanguage (
          fNoteQuarterTonesPitchKind,
          gGlobalMsrOahGroup->
            getMsrQuarterTonesPitchesLanguageKind ());
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
    case msrNoteKind::kNoteUnpitchedInTuplet:
      s << "unpitched ";
      break;
  } // switch

  return s.str ();
}

string msrNote::noteDisplayPitchKindAsString () const
{
  stringstream s;

  s <<
    msrQuarterTonesPitchKindAsStringInLanguage (
      fNoteQuarterTonesDisplayPitchKind,
      gGlobalMsrOahGroup->
        getMsrQuarterTonesPitchesLanguageKind ());

  return s.str ();
}

string msrNote::noteGraphicDurationAsMsrString () const
{
  string
    result =
      msrDurationKindAsString (
        fNoteGraphicDurationKind);

  return result;
}

string msrNote::noteGraphicDurationAsMusicXMLString () const
{
  string
    result =
      msrDurationKindAsMusicXMLType (
        fNoteGraphicDurationKind);

  return result;
}

string msrNote::tupletNoteGraphicDurationAsMsrString ( // JMI
  int actualNotes, int normalNotes) const
{
  return
    wholeNotesAsMsrString (
      fInputLineNumber,
      fMeasureElementSoundingWholeNotes
        *
      rational (actualNotes, normalNotes));
}

string msrNote::noteDiatonicPitchKindAsString (
  int inputLineNumber) const
{
  return
    msrDiatonicPitchKindAsString (
      gGlobalMsrOahGroup->getMsrQuarterTonesPitchesLanguageKind (),
      noteDiatonicPitchKind (
        fInputLineNumber));
}

string msrNote::asShortStringWithRawWholeNotes () const
{
  stringstream s;

  s << "[";

  switch (fNoteKind) {
    case msrNoteKind::k_NoNote:
      s <<
        "***noNote***";
      break;

    case msrNoteKind::kNoteRestInMeasure:
      s <<
        "noteRestInMeasure" <<
        " [" <<
        msrOctaveKindAsString (fNoteOctaveKind) <<
        ":" <<
        ", whole notes: " <<
        fMeasureElementSoundingWholeNotes <<
        " sounding, " <<
        fNoteDisplayWholeNotes <<
        " displayed";

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      s <<
        "noteSkipInMeasure" <<
        ":" <<
        ", whole notes: " <<
        fMeasureElementSoundingWholeNotes <<
        " sounding, " <<
        fNoteDisplayWholeNotes <<
        " displayed";

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      s <<
        notePitchAsString () <<
        "noteUnpitchedInMeasure" <<
        ":" <<
        ", whole notes: " <<
        fMeasureElementSoundingWholeNotes <<
        " sounding, " <<
        fNoteDisplayWholeNotes <<
        " displayed";

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteRegularInMeasure:
      s <<
        "noteRegularInMeasure" <<
        notePitchAsString () <<
        " " <<
        msrOctaveKindAsString (fNoteOctaveKind) <<
        ":" <<
        ", whole notes: " <<
        fMeasureElementSoundingWholeNotes <<
        " sounding, " <<
        fNoteDisplayWholeNotes <<
        " displayed";

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      s <<
        "noteInDoubleTremolo" <<
        " " <<
        notePitchAsString () <<
        " " <<
        msrOctaveKindAsString (fNoteOctaveKind) <<
        ":" <<
        " whole notes: " <<
        fMeasureElementSoundingWholeNotes <<
        " sounding, " <<
        fNoteDisplayWholeNotes <<
        " displayed";
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
      s <<
        "noteRegularInGraceNotesGroup" <<
        " " <<
        notePitchAsString () <<
        " " <<
        noteGraphicDurationAsMsrString () <<
        " " <<
        msrOctaveKindAsString (fNoteOctaveKind);

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      s <<
        "noteSkipInGraceNotesGroup" <<
        ":" <<
        ", whole notes: " <<
        fMeasureElementSoundingWholeNotes <<
        " sounding, " <<
        fNoteDisplayWholeNotes <<
        " displayed";

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      s <<
        "noteInChordInGraceNotesGroup" <<
        " " <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString () <<
        " " <<
        msrOctaveKindAsString (fNoteOctaveKind);

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << "."; // JMI
      } // for
      break;

    case msrNoteKind::kNoteRegularInChord:
      s <<
        "noteRegularInChord" <<
        " " <<
        notePitchAsString () <<
        msrOctaveKindAsString (fNoteOctaveKind) <<
        ", whole notes: " <<
        fMeasureElementSoundingWholeNotes <<
        " sounding, " <<
        fNoteDisplayWholeNotes <<
        " displayed";

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteRegularInTuplet:
      s <<
        "noteRegularInTuplet" <<
        " " <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString () <<
        msrOctaveKindAsString (fNoteOctaveKind) <<
        ", whole notes: " <<
        fMeasureElementSoundingWholeNotes <<
        " sounding, " <<
        fNoteDisplayWholeNotes <<
        " displayed";

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for

      s <<
        ", noteTupletFactor " << fNoteTupletFactor;
      break;

    case msrNoteKind::kNoteRestInTuplet:
      s <<
        "noteRestInTuplet" <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString () <<
        ", whole notes: " <<
        fMeasureElementSoundingWholeNotes <<
        " sounding, " <<
        fNoteDisplayWholeNotes <<
        " displayed";

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for

      s <<
        ", noteTupletFactor " << fNoteTupletFactor;
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      s <<
        "noteInTupletInGraceNotesGroup" <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString ();

      if (! getNoteIsARest ()) {
        s <<
          msrOctaveKindAsString (fNoteOctaveKind);
      }

      s <<
        ", whole notes: " <<
        fMeasureElementSoundingWholeNotes <<
        " sounding, " <<
        fNoteDisplayWholeNotes <<
        " displayed";

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for

      s <<
        ", noteTupletFactor " << fNoteTupletFactor;
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      s <<
        "noteUnpitchedInTuplet " <<
        noteGraphicDurationAsMsrString () <<
        ", whole notes: " <<
        fMeasureElementSoundingWholeNotes <<
        " sounding, " <<
        fNoteDisplayWholeNotes <<
        " displayed";

      s <<
        ", noteTupletFactor " << fNoteTupletFactor;
      break;
  } // switch

  s <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

string msrNote::asShortString () const
{
  stringstream s;

  s << "[";

  switch (fNoteKind) {
    case msrNoteKind::k_NoNote:
      s <<
        "noNote" <<
        ":" <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteRestInMeasure:
      s <<
        "noteRestInMeasure" <<
        ":" <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      s <<
        "noteSkipInMeasure" <<
        ":" <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      s <<
        "noteUnpitchedInMeasure" <<
        ":" <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteRegularInMeasure:
      {
        s <<
          "noteRegularInMeasure " <<
          notePitchAsString () <<
          noteSoundingWholeNotesAsMsrString ();

        for (int i = 0; i < fNoteDotsNumber; ++i) {
          s << ".";
        } // for

        s <<
          ", " <<
          msrOctaveKindAsString (fNoteOctaveKind);

        S_msrVoice
          voice =
            fetchNoteVoiceUpLink ();

        s <<
          ", voice: ";
        if (voice) {
          s <<
            voice->getVoiceNumber ();
        }
        else {
          s << "*none*";
        }

        S_msrStaff
          staff;

        if (voice) {
          staff =
            voice->
              getVoiceStaffUpLink ();
        }

        s <<
          ", staff: ";
        if (staff) {
          s <<
            staff->getStaffNumber ();
        }
        else {
          s << "*none*";
        }
      }
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      s <<
        "noteInDoubleTremolo " <<
        notePitchAsString () <<
        noteSoundingWholeNotesAsMsrString () <<
        msrOctaveKindAsString (fNoteOctaveKind);

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
      s <<
        "noteRegularInGraceNotesGroup " <<
        " " <<
        notePitchAsString () <<
        " " <<
        noteGraphicDurationAsMsrString () <<
        " " <<
        msrOctaveKindAsString (fNoteOctaveKind);

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      s <<
        "noteSkipInGraceNotesGroup" <<
        " " <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

   case msrNoteKind::kNoteInChordInGraceNotesGroup:
      s <<
        "noteInChordInGraceNotesGroup " <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString () <<
        msrOctaveKindAsString (fNoteOctaveKind);

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << "."; // JMI
      } // for
      break;

    case msrNoteKind::kNoteRegularInChord:
      s <<
        "noteRegularInChord " <<
        notePitchAsString () <<
        " " <<
        noteSoundingWholeNotesAsMsrString () <<
        msrOctaveKindAsString (fNoteOctaveKind);

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for
      break;

    case msrNoteKind::kNoteRegularInTuplet:
      s <<
        "noteRegularInTuplet" <<
        ":" <<
        notePitchAsString () <<
        ", whole notes: " <<
        fMeasureElementSoundingWholeNotes <<
        " sounding, " <<
        fNoteDisplayWholeNotes <<
        " display" <<
        /* JMI
        notePartUpLink ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectTupletUpLink->getTupletActualNotes (),
            fNoteDirectTupletUpLink->getTupletNormalNotes ());
            */
        msrOctaveKindAsString (fNoteOctaveKind) <<
        msrOctaveKindAsString (fNoteOctaveKind);

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for

      s <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteRestInTuplet:
      s <<
        "noteRestInTuplet" <<
        ":" <<
        notePitchAsString () <<
        ", whole notes: " <<
        fMeasureElementSoundingWholeNotes <<
        " sounding, " <<
        fNoteDisplayWholeNotes <<
        " display";
        /* JMI
        notePartUpLink ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectTupletUpLink->getTupletActualNotes (),
            fNoteDirectTupletUpLink->getTupletNormalNotes ());
            */

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for

      s <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      s <<
        "noteInTupletInGraceNotesGroup" <<
        ":" <<
        notePitchAsString () <<
        ", whole notes: " <<
        fMeasureElementSoundingWholeNotes <<
        " sounding, " <<
        fNoteDisplayWholeNotes <<
        " display";
        /* JMI
        notePartUpLink ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectTupletUpLink->getTupletActualNotes (),
            fNoteDirectTupletUpLink->getTupletNormalNotes ());
            */

      if (! getNoteIsARest ()) {
        s <<
        msrOctaveKindAsString (fNoteOctaveKind) <<
        msrOctaveKindAsString (fNoteOctaveKind);
      }

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for

      s <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      s <<
        "noteUnpitchedInTuplet" <<
        ", whole notes: " <<
        fMeasureElementSoundingWholeNotes <<
        " sounding, " <<
        fNoteDisplayWholeNotes <<
        " displayed";
        /* JMI
        notePartUpLink ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectTupletUpLink->getTupletActualNotes (),
            fNoteDirectTupletUpLink->getTupletNormalNotes ());
            */

      for (int i = 0; i < fNoteDotsNumber; ++i) {
        s << ".";
      } // for

      s <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;
  } // switch

  s <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

string msrNote::noteComplementsAsString () const
{
  stringstream s;

  S_msrVoice
    voice =
      fetchNoteVoiceUpLink ();

  s <<
    ", voice: ";
  if (voice) {
    s <<
      voice->getVoiceNumber ();
  }
  else {
    s << "*none*";
  }

  S_msrStaff
    staff;

  if (voice) {
    staff =
      voice->
        getVoiceStaffUpLink ();
  }

  s <<
    ", staff: ";
  if (staff) {
    s <<
      staff->getStaffNumber ();
  }
  else {
    s << "*none*";
  }

  s <<
    ", measureNumber: ";
  if (fMeasureElementMeasureNumber == K_NO_MEASURE_NUMBER) {
    s << "*unknown*";
  }
  else {
    s << fMeasureElementMeasureNumber;
  }

  s <<
    ", measureElementPositionInMeasure " <<
    fMeasureElementPositionInMeasure;
    /* JMI
  if (fMeasureElementPositionInMeasure == K_NO_POSITION_MEASURE_NUMBER) {
    s << "unknown (" << fMeasureElementPositionInMeasure << ")";
  }
  else {
    s << fMeasureElementPositionInMeasure;
  }
*/

  return s.str ();
}

string msrNote::soundingNoteEssentialsAsString () const
{
  stringstream s;

  s <<
    notePitchAsString () <<
    " " <<
    noteSoundingWholeNotesAsMsrString ();

  for (int i = 0; i < fNoteDotsNumber; ++i) {
    s << ".";
  } // for

  s <<
    " (" <<
    msrOctaveKindAsString (fNoteOctaveKind) <<
    ")";

  s <<
    noteComplementsAsString ();

  return s.str ();
}

string msrNote::nonSoundingNoteEssentialsAsString () const
{
  stringstream s;

  s <<
    notePitchAsString () <<
    " " <<
    noteDisplayWholeNotesAsMsrString ();

  for (int i = 0; i < fNoteDotsNumber; ++i) {
    s << ".";
  } // for

  s <<
    " (" <<
    msrOctaveKindAsString (fNoteOctaveKind) <<
    ")";

  s <<
    noteComplementsAsString ();

  return s.str ();
}

string msrNote::asString () const
{
  stringstream s;

  s <<
    "[=== ";

  switch (fNoteKind) {
    case msrNoteKind::k_NoNote:
      s <<
        "*noNote*";
      break;

    case msrNoteKind::kNoteRestInMeasure:
      s <<
        "noteRestInMeasure, ";

      if (fNoteOccupiesAFullMeasure) {
        s <<
          "R" <<
          /* JMI
          multipleRestMeasuresWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes);
            */
          nonSoundingNoteEssentialsAsString ();
      }
      else {
        s <<
          "r" <<
          nonSoundingNoteEssentialsAsString ();
      }

      s <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      s <<
        "noteSkipInMeasure:" <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      s <<
        "noteUnpitchedInMeasure " <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteRegularInMeasure:
      s <<
        "noteRegularInMeasure " <<
        soundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      s <<
        "noteInDoubleTremolo " <<
        soundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
      s <<
        "noteRegularInGraceNotesGroup " <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      s <<
        "noteSkipInGraceNotesGroup " <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      s <<
        "noteInChordInGraceNotesGroup " <<
        nonSoundingNoteEssentialsAsString () <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteRegularInChord:
      s <<
        "noteRegularInChord " <<
        soundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteRegularInTuplet:
      s <<
        "noteRegularInTuplet " <<
        soundingNoteEssentialsAsString ();
/* JMI
        notePartUpLink ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectTupletUpLink->getTupletActualNotes (),
            fNoteDirectTupletUpLink->getTupletNormalNotes ())
            */
      break;

    case msrNoteKind::kNoteRestInTuplet:
      s <<
        "noteRestInTuplet " <<
        nonSoundingNoteEssentialsAsString ();
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      s <<
        "noteInTupletInGraceNotesGroup " <<
        nonSoundingNoteEssentialsAsString ();
/* JMI
        notePartUpLink ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectTupletUpLink->getTupletActualNotes (),
            fNoteDirectTupletUpLink->getTupletNormalNotes ())
            */

      s <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      s <<
        "noteUnpitchedInTuplet " <<
        noteSoundingWholeNotesAsMsrString ();
/* JMI
        notePartUpLink ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteDirectTupletUpLink->getTupletActualNotes (),
            fNoteDirectTupletUpLink->getTupletNormalNotes ())
            */
      s <<
        ", noteTupletFactor " << fNoteTupletFactor.asString ();
      break;
  } // switch
  s << " ";


/* JMI
  s << left <<
    ", positionInMeasure: ";
    / * JMI
  if (fMeasureElementPositionInMeasure == K_NO_POSITION_MEASURE_NUMBER) {
    s << "unknown (" << fMeasureElementPositionInMeasure << ")";
  }
  else {
    s << fMeasureElementPositionInMeasure;
  }
  * /
  s << fMeasureElementPositionInMeasure;

  s <<
    ", positionInVoice: " <<
    fMeasureElementPositionInVoice;
*/

  if (fNoteOccupiesAFullMeasure) {
    s <<
      ", full measure";
  }

  if (fNoteTrillOrnament) {
    s <<
      ", has a trill ornament";
  }

  if (fNoteDashesOrnament) {
    s <<
      ", has a dashes ornament";
  }

  if (fNoteWavyLineSpannerStart) {
    s <<
      ", has a wavy line spanner start";
  }

  if (fNoteWavyLineSpannerStop) {
    s <<
      ", has a wavy line spanner stop";
  }

  if (fNoteIsFollowedByGraceNotesGroup) {
    s <<
      ", followed by grace notes group";
  }

  if (fNoteTie) {
    s <<
      ", " << msrTieKindAsString (fNoteTie->getTieKind ());
  }

  s <<
    ", " << noteIsACueNoteKindAsString (fNoteIsACueNoteKind);

  s <<
    ", line " << fInputLineNumber <<
    " ===]";

  return s.str ();
}

void msrNote::printNoteEssentials (ostream& os) const //JMI superflous
{}

void msrNote::print (ostream& os) const
{
  // print the note itself
  os <<
    asShortString () <<
    endl;

  ++gIndenter;

  const unsigned int fieldWidth = 34;

  // print measure number
  os << left <<
    setw (fieldWidth) <<
    "noteMeasureNumber" << " : ";
  if (fMeasureElementMeasureNumber == K_NO_MEASURE_NUMBER) {
    os <<
      "unknown";
  }
  else if (fMeasureElementMeasureNumber.size ()) {
    os <<
      fMeasureElementMeasureNumber;
  }
  else {
    stringstream s;

    s <<
      "noteMeasureNumber is empty in note " <<
      this->asString () <<
      "'";

// JMI     msrInternalError (
    msrInternalWarning (
      gGlobalOahOahGroup->getInputSourceName (),
      fInputLineNumber,
      s.str ());
  }
  os << endl;

  // print position in measure
  os << left <<
    setw (fieldWidth) <<
    "measureElementPositionInMeasure" << " : ";
    /* JMI
  if (fMeasureElementPositionInMeasure == K_NO_POSITION_MEASURE_NUMBER) {
    os << "unknown (" << fMeasureElementPositionInMeasure << ")";
  }
  else {
    os << fMeasureElementPositionInMeasure;
  }
  */
  os <<
    fMeasureElementPositionInMeasure <<
    endl;

  // print position in voice
  os << left <<
    setw (fieldWidth) << "measureElementPositionInVoice" << " : " <<
    fMeasureElementPositionInVoice <<
    endl <<
    setw (fieldWidth) << "measureElementMomentInVoice" << " : " <<
    endl;
  ++gIndenter;
  os <<
    fMeasureElementMomentInVoice;
  --gIndenter;

  // print note measure uplink
  os <<
    setw (fieldWidth) <<
    "noteDirectMeasureUpLink" << " : ";

  if (fNoteDirectMeasureUpLink) {
    os << endl;

    ++gIndenter;

    os <<
      fNoteDirectMeasureUpLink->asShortString () <<
      endl;

    --gIndenter;
  }
  else {
    os <<
      "none";
  }
  os << endl;

  // print note chord uplink
  os <<
    setw (fieldWidth) <<
    "noteDirectChordUpLink" << " : ";

  if (fNoteDirectChordUpLink) {
    os <<
      fNoteDirectChordUpLink->asShortString ();
  }
  else {
    os <<
      " : " << "none";
  }
  os << endl;

  // print note grace notes group uplink
  os <<
    setw (fieldWidth) <<
    "noteGraceNoteGroupUpLink" << " : ";

  if (fNoteDirectGraceNotesGroupUpLink) {
    os <<
      fNoteDirectGraceNotesGroupUpLink->asShortString ();
  }
  else {
    os <<
      " : " << "none";
  }
  os << endl;

  // print note tuplet uplink
  os <<
    setw (fieldWidth) <<
    "noteDirectTupletUpLink" << " : ";

  if (fNoteDirectTupletUpLink) {
    os <<
      fNoteDirectTupletUpLink->asShortString ();
  }
  else {
    os <<
      " : " << "none";
  }
  os << endl;

  // print sounding and displayed whole notes
  switch (fNoteKind) {
    case msrNoteKind::k_NoNote:
    case msrNoteKind::kNoteRestInMeasure:
    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInMeasure:
    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteInDoubleTremolo:
    case msrNoteKind::kNoteRegularInChord:
      os << left <<
        setw (fieldWidth) <<
        "noteSoundingWholeNotes" << " : " <<
        fMeasureElementSoundingWholeNotes <<
        endl <<
        setw (fieldWidth) <<
        "noteDisplayWholeNotes" << " : " <<
        fNoteDisplayWholeNotes <<
        endl;
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      os <<
        setw (fieldWidth) <<
        "noteDisplayWholeNotes" << " : " <<
        fNoteDisplayWholeNotes <<
        endl;
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
      os <<
        setw (fieldWidth) <<
        "noteSoundingWholeNotes" << " : " <<
        fMeasureElementSoundingWholeNotes <<
        endl <<
        setw (fieldWidth) <<
        "noteDisplayWholeNotes" << " : " <<
        fNoteDisplayWholeNotes <<
        endl <<
        setw (fieldWidth) <<
        "tupletSoundingWholeNotes" << " : ";

        if (fNoteDirectTupletUpLink) {
          os <<
            wholeNotesAsMsrString (
              fInputLineNumber,
              getNoteDirectTupletUpLink ()->
                getTupletSoundingWholeNotes ());
        }
        else {
          os <<
            "*** unknown yet ***";
        }
        os << endl;

      os <<
        setw (fieldWidth) <<
        "noteTupletFactor" << " : " <<
        endl;

      ++gIndenter;
        os <<
          fNoteTupletFactor;
      --gIndenter;
      break;
    } // switch

  // print note full measure length
  // may be unknown if there is no time signature
  rational
    measureFullLength =
      fNoteDirectMeasureUpLink
        ?
          fNoteDirectMeasureUpLink->
            getFullMeasureWholeNotesDuration ()
        : rational (0, 1); // JMI

  os << left <<
    setw (fieldWidth) <<
    "measureFullLength" << " : ";
  if (measureFullLength.getNumerator () == 0) {
    os <<
      "unknown, there's no time signature";
  }
  else {
    os <<
      measureFullLength;
  }
  os << endl;

  // chord member?
  os << left <<
    setw (fieldWidth) <<
    "noteBelongsToAChord" << " : " <<
    booleanAsString (fNoteBelongsToAChord) <<
    endl;

  // tuplet member?
  os << left <<
    setw (fieldWidth) <<
    "noteBelongsToATuplet" << " : " <<
    booleanAsString (fNoteBelongsToATuplet) <<
    endl;

  // note occupied a full measure?
  os << left <<
    setw (fieldWidth) <<
    "noteOccupiesAFullMeasure" << " : " <<
    booleanAsString (fNoteOccupiesAFullMeasure) <<
    endl;

  // multiple rest member?
  os << left <<
    setw (fieldWidth) <<
    "noteBelongsToARestMeasures" << " : " <<
    booleanAsString (fNoteBelongsToARestMeasures) <<
    endl <<
    setw (fieldWidth) <<
    "noteRestMeasuresSequenceNumber" << " : " <<
    fNoteRestMeasuresSequenceNumber <<
    endl;

  // note print kind
  os << left <<
    setw (fieldWidth) <<
   "notePrintObjectKind" << " : " <<
    notePrintObjectKindAsString () <<
    endl;

  // print the grace notes group before if any
  if (fNoteGraceNotesGroupBefore || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteGraceNotesGroupBefore";
    if (fNoteGraceNotesGroupBefore) {
      os << endl;

      ++gIndenter;

      os << fNoteGraceNotesGroupBefore;

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // note head
  os << left <<
    setw (fieldWidth) <<
    "noteHeadKind" << " : " <<
    noteHeadKindAsString () <<
    endl <<
    setw (fieldWidth) <<
    "noteHeadFilledKind" << " : " <<
    noteHeadFilledKindAsString () <<
    endl <<
    setw (fieldWidth) <<
    "noteHeadParentheses" << " : " <<
    noteHeadParenthesesKindAsString () <<
    endl;

  // accidentals
  os << left <<
    setw (fieldWidth) <<
    "noteAccidentalKind" << " : " <<
    msrAccidentalKindAsString (
      fNoteAccidentalKind) <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "noteEditorialAccidentalKind" << " : " <<
    msrEditorialAccidentalKindAsString (
      fNoteEditorialAccidentalKind) <<
    endl <<
    setw (fieldWidth) <<
    "noteCautionaryAccidentalKind" << " : " <<
    msrCautionaryAccidentalKindAsString (
      fNoteCautionaryAccidentalKind) <<
    endl;

  // cue note???
  os << left <<
    setw (fieldWidth) <<
    "noteIsACueNoteKind" << " : " <<
    noteIsACueNoteKindAsString (fNoteIsACueNoteKind) <<
    endl;

  // short cuts for efficiency
  os << left <<
    setw (fieldWidth) <<
    "noteIsAGraceNote" << " : " <<
    booleanAsString (getNoteIsAGraceNote ()) <<
    endl;

  // note redundant information (for speed)

  stringstream s;

// JMI  if (fNoteIsStemless || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteIsStemless" <<
      " : " <<
      booleanAsString (
        fNoteIsStemless) <<
      endl;
// JMI  }

 // JMI if (fNoteIsAChordsFirstMemberNote || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteIsAChordsFirstMemberNote" <<
      " : " <<
      booleanAsString (
        fNoteIsAChordsFirstMemberNote) <<
      endl;
 // JMI }

 // JMI if (fNoteIsFirstNoteInADoubleTremolo || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteIsFirstNoteInADoubleTremolo" <<
      " : " <<
      booleanAsString (
        fNoteIsFirstNoteInADoubleTremolo) <<
      endl;
// JMI  }
 // JMI if (fNoteIsSecondNoteInADoubleTremolo || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteIsSecondNoteInADoubleTremolo" <<
      " : " <<
      booleanAsString (
        fNoteIsSecondNoteInADoubleTremolo) <<
      endl;
// JMI  }

  if (fNoteTrillOrnament || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteTrillOrnament" <<
      " : " <<
      fNoteTrillOrnament-> asString () <<
      endl;
  }

  if (fNoteDashesOrnament || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteDashesOrnament" <<
      " : " <<
      fNoteDashesOrnament-> asString () <<
      endl;
  }

  if (fNoteDelayedTurnOrnament || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteDelayedTurnOrnament" <<
      " : " <<
      fNoteDelayedTurnOrnament-> asString () <<
      endl;
  }
  if (fNoteDelayedInvertedTurnOrnament || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteDelayedInvertedTurnOrnament" <<
      " : " <<
      fNoteDelayedInvertedTurnOrnament-> asString () <<
      endl;
  }

  if (fNoteWavyLineSpannerStart || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteWavyLineSpannerStart" <<
      " : " <<
      fNoteWavyLineSpannerStart->asShortString () <<
      endl;
  }
  if (fNoteWavyLineSpannerStop || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteWavyLineSpannerStop" <<
      " : " <<
      fNoteWavyLineSpannerStop->asShortString () <<
      endl;
  }

  if (fNoteIsFollowedByGraceNotesGroup || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteIsFollowedByGraceNotesGroup" <<
      " : " <<
      booleanAsString (
        fNoteIsFollowedByGraceNotesGroup) <<
      endl;
  }

  // print whole notes durations as MSR strings
  switch (fNoteKind) {
    case msrNoteKind::k_NoNote:
      break;

    case msrNoteKind::kNoteRestInMeasure:
      os << left <<
        setw (fieldWidth) <<
        "noteSoundingWholeNotesAsMsrString" << " : \"" <<
        noteSoundingWholeNotesAsMsrString () <<
        "\"" <<
        endl;
      break;

    case msrNoteKind::kNoteSkipInMeasure:
      os << left <<
        setw (fieldWidth) <<
        "noteSoundingWholeNotesAsMsrString" << " : \"" <<
        noteSoundingWholeNotesAsMsrString () <<
        "\"" <<
        endl;
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      os << left <<
        setw (fieldWidth) <<
        "noteSoundingWholeNotesAsMsrString" << " : \"" <<
        noteSoundingWholeNotesAsMsrString () <<
        "\"" <<
        endl <<
        setw (fieldWidth) <<
        "noteDisplayWholeNotesAsMsrString" << " : \"" <<
        noteDisplayWholeNotesAsMsrString () <<
        "\"" <<
        endl <<
        setw (fieldWidth) <<
        "noteGraphicDurationAsMsrString" << " : \"" <<
        noteGraphicDurationAsMsrString () <<
        "\"" <<
        setw (fieldWidth) <<
        "noteGraphicDurationAsMusicXMLString" << " : \"" <<
        noteGraphicDurationAsMsrString () <<
        "\"" <<
        endl;
      break;

    case msrNoteKind::kNoteRegularInMeasure:
      os << left <<
        setw (fieldWidth) <<
        "noteSoundingWholeNotesAsMsrString" << " : \"" <<
        noteSoundingWholeNotesAsMsrString () <<
        "\"" <<
        endl <<
        setw (fieldWidth) <<
        "noteDisplayWholeNotesAsMsrString" << " : \"" <<
        noteDisplayWholeNotesAsMsrString () <<
        "\"" <<
        endl <<
        setw (fieldWidth) <<
        "noteGraphicDurationAsMsrString" << " : \"" <<
        noteGraphicDurationAsMsrString () <<
        "\"" <<
        endl;
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      // JMI
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      os << left <<
        setw (fieldWidth) <<
        "noteGraphicDurationAsMsrString" << " : \"" <<
        noteGraphicDurationAsMsrString () <<
        "\"" <<
        endl;
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      os << left <<
        setw (fieldWidth) <<
        "noteSoundingWholeNotes" << " : " <<
        fMeasureElementSoundingWholeNotes <<
        endl <<
        setw (fieldWidth) <<
        "noteDisplayWholeNotes" << " : " <<
        fNoteDisplayWholeNotes <<
        endl <<
        setw (fieldWidth) <<
        "noteGraphicDurationAsMsrString" << " : \"" <<
        noteGraphicDurationAsMsrString () <<
        "\"" <<
        endl;
      break;

    case msrNoteKind::kNoteRegularInChord:
      // JMI
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
      /* JMI
      os << left <<
        setw (fieldWidth) <<
        "noteTupletNoteGraphicDurationAsMsrString" << " : \"" <<
        fNoteTupletNoteGraphicDurationAsMsrString <<
        "\"" <<
        endl <<
        setw (fieldWidth) <<
        "noteTupletNoteSoundingWholeNotesAsMsrString" << " : ";
          */

      if (fNoteDirectTupletUpLink) {
        os <<
          "\"" <<
          wholeNotesAsMsrString (
            fInputLineNumber,
            getNoteDirectTupletUpLink ()->
              getTupletSoundingWholeNotes ()) <<
          "\"";
      }
      else {
        os <<
          "*** unknown yet ***";
      }
      os << endl;

      os <<
        setw (fieldWidth) <<
        "positionInTuplet" << " : " <<
        fPositionInTuplet <<
        endl;

      os <<
        setw (fieldWidth) <<
        "noteGraphicDurationAsMsrString" << " : \"" <<
        noteGraphicDurationAsMsrString () <<
        "\"" <<
        endl;
      break;
  } // switch

  // print the octave shift if any
  if (fNoteOctaveShift || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteOctaveShift";
    if (fNoteOctaveShift) {
      os << endl;

      ++gIndenter;

      os <<
        fNoteOctaveShift;

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the stem if any
  if (fNoteStem || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteStem";
    if (fNoteStem) {
      os << endl;

      ++gIndenter;

      os <<
        fNoteStem;

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the note color
  os <<
    setw (fieldWidth) <<
    "noteAlphaRGBColor" << " : " <<
    fNoteAlphaRGBColor.asString () <<
    endl <<
    setw (fieldWidth) <<
    "noteAlphaRGBColorHasBenSet" << " : " <<
    booleanAsString (fNoteAlphaRGBColorHasBenSet) <<
    endl;

  // print the tie if any
  if (fNoteTie || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteTie";
    if (fNoteTie) {
      os << endl;

      ++gIndenter;

      os <<
        fNoteTie;

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the beams if any
  unsigned int noteBeamsSize = fNoteBeams.size ();

  if (noteBeamsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteBeams";
    if (fNoteBeams.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrBeam>::const_iterator
        iBegin = fNoteBeams.begin (),
        iEnd   = fNoteBeams.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the articulations if any
  unsigned int noteArticulationsSize = fNoteArticulations.size ();

  if (noteArticulationsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteArticulations";
    if (fNoteArticulations.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrArticulation>::const_iterator
        iBegin = fNoteArticulations.begin (),
        iEnd   = fNoteArticulations.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the spanners if any
  unsigned int noteSpannersSize = fNoteSpanners.size ();

  if (noteSpannersSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteSpanners";
    if (fNoteSpanners.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrSpanner>::const_iterator
        iBegin = fNoteSpanners.begin (),
        iEnd   = fNoteSpanners.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the technicals if any
  unsigned int noteTechnicalsSize = fNoteTechnicals.size ();

  if (noteTechnicalsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteTechnicals";
    if (fNoteTechnicals.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrTechnical>::const_iterator
        iBegin = fNoteTechnicals.begin (),
        iEnd   = fNoteTechnicals.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the technicals with integer if any
  unsigned int noteTechnicalWithIntegersSize =
    fNoteTechnicalWithIntegers.size ();

  if (noteTechnicalWithIntegersSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteTechnicalWithIntegers";
    if (fNoteTechnicalWithIntegers.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrTechnicalWithInteger>::const_iterator
        iBegin = fNoteTechnicalWithIntegers.begin (),
        iEnd   = fNoteTechnicalWithIntegers.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the technicals with float if any
  unsigned int noteTechnicalWithFloatsSize = fNoteTechnicalWithFloats.size ();

  if (noteTechnicalWithFloatsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteTechnicalWithFloats";
    if (fNoteTechnicalWithFloats.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrTechnicalWithFloat>::const_iterator
        iBegin = fNoteTechnicalWithFloats.begin (),
        iEnd   = fNoteTechnicalWithFloats.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the technicals with string if any
  unsigned int noteTechnicalWithStringsSize = fNoteTechnicalWithStrings.size ();

  if (noteTechnicalWithStringsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteTechnicalWithStrings";
    if (fNoteTechnicalWithStrings.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrTechnicalWithString>::const_iterator
        iBegin = fNoteTechnicalWithStrings.begin (),
        iEnd   = fNoteTechnicalWithStrings.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the ornaments if any
  unsigned int noteOrnamentsSize = fNoteOrnaments.size ();

  if (noteOrnamentsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteOrnaments";
    if (fNoteOrnaments.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrOrnament>::const_iterator
        iBegin = fNoteOrnaments.begin (),
        iEnd   = fNoteOrnaments.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the glissandos if any
  unsigned int noteGlissandosSize = fNoteGlissandos.size ();

  if (noteGlissandosSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteGlissandos";
    if (fNoteGlissandos.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrGlissando>::const_iterator
        iBegin = fNoteGlissandos.begin (),
        iEnd   = fNoteGlissandos.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the slides if any
  unsigned int noteSlidesSize = fNoteSlides.size ();

  if (noteSlidesSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteSlides";
    if (fNoteSlides.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrSlide>::const_iterator
        iBegin = fNoteSlides.begin (),
        iEnd   = fNoteSlides.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the singleTremolo if any
  if (fNoteSingleTremolo || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteSingleTremolo";
    if (fNoteSingleTremolo) {
      os << endl;

      ++gIndenter;

      os << fNoteSingleTremolo;

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the dynamics if any
  unsigned int noteDynamicsSize = fNoteDynamics.size ();

  if (noteDynamicsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteDynamics";
    if (fNoteDynamics.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrDynamics>::const_iterator
        iBegin = fNoteDynamics.begin (),
        iEnd   = fNoteDynamics.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the other dynamics if any
  unsigned int noteOtherDynamicsSize = fNoteOtherDynamics.size ();

  if (noteOtherDynamicsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteOtherDynamics";
    if (fNoteOtherDynamics.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrOtherDynamics>::const_iterator
        iBegin = fNoteOtherDynamics.begin (),
        iEnd   = fNoteOtherDynamics.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the words if any
  unsigned int noteWordsSize = fNoteWords.size ();

  if (noteWordsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteWords";
    if (fNoteWords.size ()) {
      os << endl;
      ++gIndenter;

      list<S_msrWords>::const_iterator
        iBegin = fNoteWords.begin (),
        iEnd   = fNoteWords.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the slurs if any
  unsigned int noteSlursSize = fNoteSlurs.size ();

  if (noteSlursSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteSlurs";
    if (fNoteSlurs.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrSlur>::const_iterator
        iBegin = fNoteSlurs.begin (),
        iEnd   = fNoteSlurs.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the ligatures if any
  unsigned int noteLigaturesSize = fNoteLigatures.size ();

  if (noteLigaturesSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteLigatures";
    if (fNoteLigatures.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrLigature>::const_iterator
        iBegin = fNoteLigatures.begin (),
        iEnd   = fNoteLigatures.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the pedals if any
  unsigned int notePedalsSize = fNotePedals.size ();

  if (notePedalsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "notePedals";
    if (fNotePedals.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrPedal>::const_iterator
        iBegin = fNotePedals.begin (),
        iEnd   = fNotePedals.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the slashes if any
  unsigned int noteSlashesSize = fNoteSlashes.size ();

  if (noteSlashesSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteSlashes";
    if (fNoteSlashes.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrSlash>::const_iterator
        iBegin = fNoteSlashes.begin (),
        iEnd   = fNoteSlashes.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
      }

  // print the wedges if any
  unsigned int noteWedgesSize = fNoteWedges.size ();

  if (noteWedgesSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteWedges";
    if (fNoteWedges.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrWedge>::const_iterator
        iBegin = fNoteWedges.begin (),
        iEnd   = fNoteWedges.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the segnos if any
  unsigned int noteSegnosSize = fNoteSegnos.size ();

  if (noteSegnosSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteSegnos";
    if (fNoteSegnos.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrSegno>::const_iterator
        iBegin = fNoteSegnos.begin (),
        iEnd   = fNoteSegnos.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the dal segnos if any
  unsigned int noteDalSegnosSize = fNoteDalSegnos.size ();

  if (noteDalSegnosSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteDalSegnos";
    if (fNoteDalSegnos.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrDalSegno>::const_iterator
        iBegin = fNoteDalSegnos.begin (),
        iEnd   = fNoteDalSegnos.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the codas if any
  unsigned int noteCodasSize = fNoteCodas.size ();

  if (noteCodasSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteCodas";
    if (fNoteCodas.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrCoda>::const_iterator
        iBegin = fNoteCodas.begin (),
        iEnd   = fNoteCodas.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the eyeglasses if any
  unsigned int noteEyeGlassesSize = fNoteEyeGlasses.size ();

  if (noteEyeGlassesSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteEyeGlasses";
    if (fNoteEyeGlasses.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrEyeGlasses>::const_iterator
        iBegin = fNoteEyeGlasses.begin (),
        iEnd   = fNoteEyeGlasses.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the damps if any
  unsigned int noteDampsSize = fNoteDamps.size ();

  if (noteDampsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteDamps";
    if (fNoteDamps.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrDamp>::const_iterator
        iBegin = fNoteDamps.begin (),
        iEnd   = fNoteDamps.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the dampAlls if any
  unsigned int noteDampAllsSize = fNoteDampAlls.size ();

  if (noteDampAllsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteDampAlls";
    if (fNoteDampAlls.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrDampAll>::const_iterator
        iBegin = fNoteDampAlls.begin (),
        iEnd   = fNoteDampAlls.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the scordaturas if any
  unsigned int noteScordaturasSize = fNoteScordaturas.size ();

  if (noteScordaturasSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteScordaturas";
    if (fNoteScordaturas.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrScordatura>::const_iterator
        iBegin = fNoteScordaturas.begin (),
        iEnd   = fNoteScordaturas.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the harmonies associated to this note if any
  unsigned int noteHarmoniesListSize = fNoteHarmoniesList.size ();

  if (noteHarmoniesListSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteHarmonies";
    if (noteHarmoniesListSize) {
      os << endl;

      ++gIndenter;

      list<S_msrHarmony>::const_iterator
        iBegin = fNoteHarmoniesList.begin (),
        iEnd   = fNoteHarmoniesList.end (),
        i      = iBegin;
      for ( ; ; ) {
        S_msrHarmony
          harmony = (*i);

        os << harmony;

        if (++i == iEnd) break;
        // no endl here
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the figured basses associated to this note if any
  unsigned int noteFiguredBassesListSize = fNoteFiguredBassesList.size ();

  if (noteFiguredBassesListSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteFiguredBasses";
    if (noteFiguredBassesListSize) {
      os << endl;

      ++gIndenter;

      list<S_msrFiguredBass>::const_iterator
        iBegin = fNoteFiguredBassesList.begin (),
        iEnd   = fNoteFiguredBassesList.end (),
        i      = iBegin;
      for ( ; ; ) {
        S_msrFiguredBass
          figuredBass = (*i);

        os << figuredBass;

        if (++i == iEnd) break;
        // no endl here
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the syllables associated to this note if any
  unsigned int noteSyllablesSize = fNoteSyllables.size ();

  if (noteSyllablesSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteSyllables";
    if (noteSyllablesSize) {
      os << endl;

      ++gIndenter;

      list<S_msrSyllable>::const_iterator
        iBegin = fNoteSyllables.begin (),
        iEnd   = fNoteSyllables.end (),
        i      = iBegin;
      for ( ; ; ) {
        S_msrSyllable
          syllable = (*i);

        os << syllable;

  /* JMI
        os <<
          syllable->syllableKindAsString () <<
            syllable->syllableExtendKindAsString () <<
          " : ";

        msrSyllable::writeTextsList (
          syllable->getSyllableTextsList (),
          os);

        os <<
          ", stanza " <<
          syllable->
            getSyllableStanzaUpLink ()->
              getStanzaNumber () <<
          ", line " << syllable->getInputLineNumber () <<
          ", noteUpLink: " <<
          syllable->
            getSyllableNoteUpLink ()->
              asShortString ();
  */

        if (++i == iEnd) break;
        // no endl here
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the after grace group notes after if any
  if (fNoteGraceNotesGroupAfter || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteGraceNotesGroupAfter";
    if (fNoteGraceNotesGroupAfter) {
      os << endl;

      ++gIndenter;

      os << fNoteGraceNotesGroupAfter;

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  --gIndenter;
}

void msrNote::printShort (ostream& os) const
{
  // print the note itself
  os <<
    asString () <<
    endl;

  ++gIndenter;

  const unsigned int fieldWidth = 34;

/*
  // print measure number
  os << left <<
    setw (fieldWidth) <<
    "noteMeasureNumber" << " : ";
  if (fMeasureElementMeasureNumber == K_NO_MEASURE_NUMBER) {
    os <<
      "unknown";
  }
  else if (fMeasureElementMeasureNumber.size ()) {
    os <<
      fMeasureElementMeasureNumber;
  }
  else {
    stringstream s;

    s <<
      "noteMeasureNumber is empty in note " <<
      this->asString () <<
      "'";

// JMI     msrInternalError (
    msrInternalWarning (
      gGlobalOahOahGroup->getInputSourceName (),
      fInputLineNumber,
      s.str ());
  }
  os << endl;
*/

  // print position in measure
  os << left <<
    setw (fieldWidth) <<
    "measureElementPositionInMeasure" << " : ";
    /* JMI
  if (fMeasureElementPositionInMeasure == K_NO_POSITION_MEASURE_NUMBER) {
    os << "unknown (" << fMeasureElementPositionInMeasure << ")";
  }
  else {
    os << fMeasureElementPositionInMeasure;
  }
  */
  os <<
    fMeasureElementPositionInMeasure <<
    endl;

  // print position in voice
  os << left <<
    setw (fieldWidth) << "measureElementPositionInVoice" << " : " <<
    fMeasureElementPositionInVoice <<
    endl;

/*
  // print note measure uplink
  os <<
    setw (fieldWidth) <<
    "noteDirectMeasureUpLink" << " : ";

  if (fNoteDirectMeasureUpLink) {
    os << endl;

    ++gIndenter;

    os <<
      fNoteDirectMeasureUpLink->asShortString () <<
      endl;

    --gIndenter;
  }
  else {
    os <<
      "none";
  }
  os << endl;

  // print note chord uplink
  os <<
    setw (fieldWidth) <<
    "noteDirectChordUpLink" << " : ";

  if (fNoteDirectChordUpLink) {
    os <<
      fNoteDirectChordUpLink->asShortString ();
  }
  else {
    os <<
      " : " << "none";
  }
  os << endl;

  // print note grace notes group uplink
  os <<
    setw (fieldWidth) <<
    "noteGraceNoteGroupUpLink" << " : ";

  if (fNoteDirectGraceNotesGroupUpLink) {
    os <<
      fNoteDirectGraceNotesGroupUpLink->asShortString ();
  }
  else {
    os <<
      " : " << "none";
  }
  os << endl;

  // print note tuplet uplink
  os <<
    setw (fieldWidth) <<
    "noteDirectTupletUpLink" << " : ";

  if (fNoteDirectTupletUpLink) {
    os <<
      fNoteDirectTupletUpLink->asShortString ();
  }
  else {
    os <<
      " : " << "none";
  }
  os << endl;
*/

  // print sounding and displayed whole notes
  switch (fNoteKind) {
    case msrNoteKind::k_NoNote:
    case msrNoteKind::kNoteRestInMeasure:
    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteUnpitchedInMeasure:
    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteInDoubleTremolo:
    case msrNoteKind::kNoteRegularInChord:
      os << left <<
        setw (fieldWidth) <<
        "noteSoundingWholeNotes" << " : " <<
        fMeasureElementSoundingWholeNotes <<
        endl <<
        setw (fieldWidth) <<
        "noteDisplayWholeNotes" << " : " <<
        fNoteDisplayWholeNotes <<
        endl;
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      os <<
        setw (fieldWidth) <<
        "noteDisplayWholeNotes" << " : " <<
        fNoteDisplayWholeNotes <<
        endl;
      break;

    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      os << left <<
        setw (fieldWidth) <<
        "noteSoundingWholeNotes" << " : " <<
        fMeasureElementSoundingWholeNotes <<
        endl <<
        setw (fieldWidth) <<
        "noteDisplayWholeNotes" << " : " <<
        fNoteDisplayWholeNotes <<
        endl <<
        setw (fieldWidth) <<
        "noteGraphicDurationAsMsrString" << " : \"" <<
        noteGraphicDurationAsMsrString () <<
        "\"" <<
        endl;
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
      os <<
        setw (fieldWidth) <<
        "noteSoundingWholeNotes" << " : " <<
        fMeasureElementSoundingWholeNotes <<
        endl <<
        setw (fieldWidth) <<
        "noteDisplayWholeNotes" << " : " <<
        fNoteDisplayWholeNotes <<
        endl <<
        setw (fieldWidth) <<
        "tupletSoundingWholeNotes" << " : ";

        if (fNoteDirectTupletUpLink) {
          os <<
            wholeNotesAsMsrString (
              fInputLineNumber,
              getNoteDirectTupletUpLink ()->
                getTupletSoundingWholeNotes ());
        }
        else {
          os <<
            "*** unknown yet ***";
        }
        os << endl;

      os <<
        setw (fieldWidth) <<
        "noteTupletFactor" << " : " <<
        endl;

      ++gIndenter;
        os <<
          fNoteTupletFactor;
      --gIndenter;
      break;
    } // switch

/*
  // print note full measure length
  // may be unknown if there is no time signature
  rational
    measureFullLength =
      fNoteDirectMeasureUpLink
        ?
          fNoteDirectMeasureUpLink->
            getFullMeasureWholeNotesDuration ()
        : rational (0, 1); // JMI

  os << left <<
    setw (fieldWidth) <<
    "measureFullLength" << " : ";
  if (measureFullLength.getNumerator () == 0) {
    os <<
      "unknown, there's no time signature";
  }
  else {
    os <<
      measureFullLength;
  }
  os << endl;

  // chord member?
  os << left <<
    setw (fieldWidth) <<
    "noteBelongsToAChord" << " : " <<
    booleanAsString (fNoteBelongsToAChord) <<
    endl;

  // tuplet member?
  os << left <<
    setw (fieldWidth) <<
    "noteBelongsToATuplet" << " : " <<
    booleanAsString (fNoteBelongsToATuplet) <<
    endl;

  // note occupied a full measure?
  os << left <<
    setw (fieldWidth) <<
    "noteOccupiesAFullMeasure" << " : " <<
    booleanAsString (fNoteOccupiesAFullMeasure) <<
    endl;

  // multiple rest member?
  os << left <<
    setw (fieldWidth) <<
    "noteBelongsToARestMeasures" << " : " <<
    booleanAsString (fNoteBelongsToARestMeasures) <<
    endl <<
    setw (fieldWidth) <<
    "noteRestMeasuresSequenceNumber" << " : " <<
    fNoteRestMeasuresSequenceNumber <<
    endl;

  // note print kind
  os << left <<
    setw (fieldWidth) <<
   "notePrintObjectKind" << " : " <<
    notePrintObjectKindAsString () <<
    endl;
*/

  // print the grace notes group before if any
  if (fNoteGraceNotesGroupBefore || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteGraceNotesGroupBefore";
    if (fNoteGraceNotesGroupBefore) {
      os << endl;

      ++gIndenter;

      fNoteGraceNotesGroupBefore->printShort (os);

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

/*
  // note head
  os << left <<
    setw (fieldWidth) <<
    "noteHeadKind" << " : " <<
    noteHeadKindAsString () <<
    endl <<
    setw (fieldWidth) <<
    "noteHeadFilledKind" << " : " <<
    noteHeadFilledKindAsString () <<
    endl <<
    setw (fieldWidth) <<
    "noteHeadParentheses" << " : " <<
    noteHeadParenthesesKindAsString () <<
    endl;

  // accidentals
  os << left <<
    setw (fieldWidth) <<
    "noteAccidentalKind" << " : " <<
    msrAccidentalKindAsString (
      fNoteAccidentalKind) <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "noteEditorialAccidentalKind" << " : " <<
    msrEditorialAccidentalKindAsString (
      fNoteEditorialAccidentalKind) <<
    endl <<
    setw (fieldWidth) <<
    "noteCautionaryAccidentalKind" << " : " <<
    msrCautionaryAccidentalKindAsString (
      fNoteCautionaryAccidentalKind) <<
    endl;

  // cue note???
  os << left <<
    setw (fieldWidth) <<
    "noteIsACueNoteKind" << " : " <<
    noteIsACueNoteKindAsString (fNoteIsACueNoteKind) <<
    endl;

  // short cuts for efficiency
  os << left <<
    setw (fieldWidth) <<
    "noteIsAGraceNote" << " : " <<
    booleanAsString (getNoteIsAGraceNote ()) <<
    endl;

  // note redundant information (for speed)

  stringstream s;

// JMI  if (fNoteIsStemless || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteIsStemless" <<
      " : " <<
      booleanAsString (
        fNoteIsStemless) <<
      endl;
// JMI  }

 // JMI if (fNoteIsAChordsFirstMemberNote || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteIsAChordsFirstMemberNote" <<
      " : " <<
      booleanAsString (
        fNoteIsAChordsFirstMemberNote) <<
      endl;
 // JMI }

 // JMI if (fNoteIsFirstNoteInADoubleTremolo || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteIsFirstNoteInADoubleTremolo" <<
      " : " <<
      booleanAsString (
        fNoteIsFirstNoteInADoubleTremolo) <<
      endl;
// JMI  }
 // JMI if (fNoteIsSecondNoteInADoubleTremolo || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteIsSecondNoteInADoubleTremolo" <<
      " : " <<
      booleanAsString (
        fNoteIsSecondNoteInADoubleTremolo) <<
      endl;
// JMI  }

  if (fNoteTrillOrnament || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteTrillOrnament" <<
      " : " <<
      fNoteTrillOrnament-> asString () <<
      endl;
  }

  if (fNoteDashesOrnament || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteDashesOrnament" <<
      " : " <<
      fNoteDashesOrnament-> asString () <<
      endl;
  }

  if (fNoteDelayedTurnOrnament || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteDelayedTurnOrnament" <<
      " : " <<
      fNoteDelayedTurnOrnament-> asString () <<
      endl;
  }
  if (fNoteDelayedInvertedTurnOrnament || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteDelayedInvertedTurnOrnament" <<
      " : " <<
      fNoteDelayedInvertedTurnOrnament-> asString () <<
      endl;
  }

  if (fNoteWavyLineSpannerStart || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteWavyLineSpannerStart" <<
      " : " <<
      fNoteWavyLineSpannerStart->asShortString () <<
      endl;
  }
  if (fNoteWavyLineSpannerStop || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteWavyLineSpannerStop" <<
      " : " <<
      fNoteWavyLineSpannerStop->asShortString () <<
      endl;
  }

  if (fNoteIsFollowedByGraceNotesGroup || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteIsFollowedByGraceNotesGroup" <<
      " : " <<
      booleanAsString (
        fNoteIsFollowedByGraceNotesGroup) <<
      endl;
  }

  // print whole notes durations as MSR strings
  switch (fNoteKind) {
    case msrNoteKind::k_NoNote:
      break;

    case msrNoteKind::kNoteRestInMeasure:
      {
        os << left <<
          setw (fieldWidth) <<
          "noteSoundingWholeNotesAsMsrString" << " : \"" <<
          noteSoundingWholeNotesAsMsrString () <<
          "\"" <<
          endl;
      }
      break;

    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
      {
        os << left <<
          setw (fieldWidth) <<
          "noteSoundingWholeNotesAsMsrString" << " : \"" <<
          noteSoundingWholeNotesAsMsrString () <<
          "\"" <<
          endl;
      }
      break;

    case msrNoteKind::kNoteUnpitchedInMeasure:
      {
        os << left <<
          setw (fieldWidth) <<
          "noteSoundingWholeNotesAsMsrString" << " : \"" <<
          noteSoundingWholeNotesAsMsrString () <<
          "\"" <<
          endl <<
          setw (fieldWidth) <<
          "noteDisplayWholeNotesAsMsrString" << " : \"" <<
          noteDisplayWholeNotesAsMsrString () <<
          "\"" <<
          endl <<
          setw (fieldWidth) <<
          "noteGraphicDurationAsMsrString" << " : \"" <<
          noteGraphicDurationAsMsrString () <<
          "\"" <<
          endl;
      }
      break;

    case msrNoteKind::kNoteRegularInMeasure:
      {
        os << left <<
          setw (fieldWidth) <<
          "noteSoundingWholeNotesAsMsrString" << " : \"" <<
          noteSoundingWholeNotesAsMsrString () <<
          "\"" <<
          endl <<
          setw (fieldWidth) <<
          "noteDisplayWholeNotesAsMsrString" << " : \"" <<
          noteDisplayWholeNotesAsMsrString () <<
          "\"" <<
          endl <<
          setw (fieldWidth) <<
          "noteGraphicDurationAsMsrString" << " : \"" <<
          noteGraphicDurationAsMsrString () <<
          "\"" <<
          endl;
      }
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      {
        // JMI
      }
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      {
        os << left <<
          setw (fieldWidth) <<
          "noteGraphicDurationAsMsrString" << " : \"" <<
          noteGraphicDurationAsMsrString () <<
          "\"" <<
          endl;
      }
      break;

    case msrNoteKind::kNoteRegularInChord:
      {
        // JMI
      }
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
    case msrNoteKind::kNoteUnpitchedInTuplet:
      / * JMI
      os << left <<
        setw (fieldWidth) <<
        "noteTupletNoteGraphicDurationAsMsrString" << " : \"" <<
        fNoteTupletNoteGraphicDurationAsMsrString <<
        "\"" <<
        endl <<
        setw (fieldWidth) <<
        "noteTupletNoteSoundingWholeNotesAsMsrString" << " : ";
          * /

      if (fNoteDirectTupletUpLink) {
        os <<
          "\"" <<
          wholeNotesAsMsrString (
            fInputLineNumber,
            getNoteDirectTupletUpLink ()->
              getTupletSoundingWholeNotes ()) <<
          "\"";
      }
      else {
        os <<
          "*** unknown yet ***";
      }
      os << endl;

      os <<
        setw (fieldWidth) <<
        "positionInTuplet" << " : " <<
        fPositionInTuplet <<
        endl;

      os <<
        setw (fieldWidth) <<
        "noteGraphicDurationAsMsrString" << " : \"" <<
        noteGraphicDurationAsMsrString () <<
        "\"" <<
        endl;
      break;
  } // switch

  // print the octave shift if any
  if (fNoteOctaveShift || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteOctaveShift";
    if (fNoteOctaveShift) {
      os << endl;

      ++gIndenter;

      os <<
        fNoteOctaveShift;

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the stem if any
  if (fNoteStem || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteStem";
    if (fNoteStem) {
      os << endl;

      ++gIndenter;

      os <<
        fNoteStem;

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the note color
  os <<
    setw (fieldWidth) <<
    "noteAlphaRGBColor" << " : " <<
    fNoteAlphaRGBColor.asString () <<
    endl <<
    setw (fieldWidth) <<
    "noteAlphaRGBColorHasBenSet" << " : " <<
    booleanAsString (fNoteAlphaRGBColorHasBenSet) <<
    endl;

  // print the tie if any
  if (fNoteTie || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteTie";
    if (fNoteTie) {
      os << endl;

      ++gIndenter;

      os <<
        fNoteTie;

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }
*/

  // print the beams if any
  unsigned int noteBeamsSize = fNoteBeams.size ();

  if (noteBeamsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteBeams";
    if (fNoteBeams.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrBeam>::const_iterator
        iBegin = fNoteBeams.begin (),
        iEnd   = fNoteBeams.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

/*
  // print the articulations if any
  unsigned int noteArticulationsSize = fNoteArticulations.size ();

  if (noteArticulationsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteArticulations";
    if (fNoteArticulations.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrArticulation>::const_iterator
        iBegin = fNoteArticulations.begin (),
        iEnd   = fNoteArticulations.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the spanners if any
  unsigned int noteSpannersSize = fNoteSpanners.size ();

  if (noteSpannersSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteSpanners";
    if (fNoteSpanners.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrSpanner>::const_iterator
        iBegin = fNoteSpanners.begin (),
        iEnd   = fNoteSpanners.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the technicals if any
  unsigned int noteTechnicalsSize = fNoteTechnicals.size ();

  if (noteTechnicalsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteTechnicals";
    if (fNoteTechnicals.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrTechnical>::const_iterator
        iBegin = fNoteTechnicals.begin (),
        iEnd   = fNoteTechnicals.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the technicals with integer if any
  unsigned int noteTechnicalWithIntegersSize = fNoteTechnicalWithIntegers.size ();

  if (noteTechnicalWithIntegersSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteTechnicalWithIntegers";
    if (fNoteTechnicalWithIntegers.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrTechnicalWithInteger>::const_iterator
        iBegin = fNoteTechnicalWithIntegers.begin (),
        iEnd   = fNoteTechnicalWithIntegers.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the technicals with float if any
  unsigned int noteTechnicalWithFloatsSize = fNoteTechnicalWithFloats.size ();

  if (noteTechnicalWithFloatsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteTechnicalWithFloats";
    if (fNoteTechnicalWithFloats.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrTechnicalWithFloat>::const_iterator
        iBegin = fNoteTechnicalWithFloats.begin (),
        iEnd   = fNoteTechnicalWithFloats.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the technicals with string if any
  unsigned int noteTechnicalWithStringsSize = fNoteTechnicalWithStrings.size ();

  if (noteTechnicalWithStringsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteTechnicalWithStrings";
    if (fNoteTechnicalWithStrings.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrTechnicalWithString>::const_iterator
        iBegin = fNoteTechnicalWithStrings.begin (),
        iEnd   = fNoteTechnicalWithStrings.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }
*/

  // print the ornaments if any
  unsigned int noteOrnamentsSize = fNoteOrnaments.size ();

  if (noteOrnamentsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteOrnaments";
    if (fNoteOrnaments.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrOrnament>::const_iterator
        iBegin = fNoteOrnaments.begin (),
        iEnd   = fNoteOrnaments.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

/*
  // print the glissandos if any
  unsigned int noteGlissandosSize = fNoteGlissandos.size ();

  if (noteGlissandosSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteGlissandos";
    if (fNoteGlissandos.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrGlissando>::const_iterator
        iBegin = fNoteGlissandos.begin (),
        iEnd   = fNoteGlissandos.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the slides if any
  unsigned int noteSlidesSize = fNoteSlides.size ();

  if (noteSlidesSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteSlides";
    if (fNoteSlides.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrSlide>::const_iterator
        iBegin = fNoteSlides.begin (),
        iEnd   = fNoteSlides.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }
*/

/*
  // print the singleTremolo if any
  if (fNoteSingleTremolo || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteSingleTremolo";
    if (fNoteSingleTremolo) {
      os << endl;

      ++gIndenter;

      os << fNoteSingleTremolo;

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the dynamics if any
  unsigned int noteDynamicsSize = fNoteDynamics.size ();

  if (noteDynamicsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteDynamics";
    if (fNoteDynamics.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrDynamics>::const_iterator
        iBegin = fNoteDynamics.begin (),
        iEnd   = fNoteDynamics.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the other dynamics if any
  unsigned int noteOtherDynamicsSize = fNoteOtherDynamics.size ();

  if (noteOtherDynamicsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteOtherDynamics";
    if (fNoteOtherDynamics.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrOtherDynamics>::const_iterator
        iBegin = fNoteOtherDynamics.begin (),
        iEnd   = fNoteOtherDynamics.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }
*/

  // print the words if any
  unsigned int noteWordsSize = fNoteWords.size ();

  if (noteWordsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteWords";
    if (fNoteWords.size ()) {
      os << endl;
      ++gIndenter;

      list<S_msrWords>::const_iterator
        iBegin = fNoteWords.begin (),
        iEnd   = fNoteWords.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the slurs if any
  unsigned int noteSlursSize = fNoteSlurs.size ();

  if (noteSlursSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteSlurs";
    if (fNoteSlurs.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrSlur>::const_iterator
        iBegin = fNoteSlurs.begin (),
        iEnd   = fNoteSlurs.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the ligatures if any
  unsigned int noteLigaturesSize = fNoteLigatures.size ();

  if (noteLigaturesSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteLigatures";
    if (fNoteLigatures.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrLigature>::const_iterator
        iBegin = fNoteLigatures.begin (),
        iEnd   = fNoteLigatures.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the pedals if any
  unsigned int notePedalsSize = fNotePedals.size ();

  if (notePedalsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "notePedals";
    if (fNotePedals.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrPedal>::const_iterator
        iBegin = fNotePedals.begin (),
        iEnd   = fNotePedals.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the slashes if any
  unsigned int noteSlashesSize = fNoteSlashes.size ();

  if (noteSlashesSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteSlashes";
    if (fNoteSlashes.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrSlash>::const_iterator
        iBegin = fNoteSlashes.begin (),
        iEnd   = fNoteSlashes.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

/*
  // print the wedges if any
  unsigned int noteWedgesSize = fNoteWedges.size ();

  if (noteWedgesSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteWedges";
    if (fNoteWedges.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrWedge>::const_iterator
        iBegin = fNoteWedges.begin (),
        iEnd   = fNoteWedges.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the segnos if any
  unsigned int noteSegnosSize = fNoteSegnos.size ();

  if (noteSegnosSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteSegnos";
    if (fNoteSegnos.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrSegno>::const_iterator
        iBegin = fNoteSegnos.begin (),
        iEnd   = fNoteSegnos.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the dal segnos if any
  unsigned int noteDalSegnosSize = fNoteDalSegnos.size ();

  if (noteDalSegnosSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteDalSegnos";
    if (fNoteDalSegnos.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrDalSegno>::const_iterator
        iBegin = fNoteDalSegnos.begin (),
        iEnd   = fNoteDalSegnos.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the codas if any
  unsigned int noteCodasSize = fNoteCodas.size ();

  if (noteCodasSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteCodas";
    if (fNoteCodas.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrCoda>::const_iterator
        iBegin = fNoteCodas.begin (),
        iEnd   = fNoteCodas.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the eyeglasses if any
  unsigned int noteEyeGlassesSize = fNoteEyeGlasses.size ();

  if (noteEyeGlassesSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteEyeGlasses";
    if (fNoteEyeGlasses.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrEyeGlasses>::const_iterator
        iBegin = fNoteEyeGlasses.begin (),
        iEnd   = fNoteEyeGlasses.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the damps if any
  unsigned int noteDampsSize = fNoteDamps.size ();

  if (noteDampsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteDamps";
    if (fNoteDamps.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrDamp>::const_iterator
        iBegin = fNoteDamps.begin (),
        iEnd   = fNoteDamps.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the dampAlls if any
  unsigned int noteDampAllsSize = fNoteDampAlls.size ();

  if (noteDampAllsSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteDampAlls";
    if (fNoteDampAlls.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrDampAll>::const_iterator
        iBegin = fNoteDampAlls.begin (),
        iEnd   = fNoteDampAlls.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the scordaturas if any
  unsigned int noteScordaturasSize = fNoteScordaturas.size ();

  if (noteScordaturasSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteScordaturas";
    if (fNoteScordaturas.size ()) {
      os << endl;

      ++gIndenter;

      list<S_msrScordatura>::const_iterator
        iBegin = fNoteScordaturas.begin (),
        iEnd   = fNoteScordaturas.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }
*/

  // print the harmonies associated to this note if any
  unsigned int noteHarmoniesListSize = fNoteHarmoniesList.size ();

  if (noteHarmoniesListSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteHarmonies";
    if (noteHarmoniesListSize) {
      os << endl;

      ++gIndenter;

      list<S_msrHarmony>::const_iterator
        iBegin = fNoteHarmoniesList.begin (),
        iEnd   = fNoteHarmoniesList.end (),
        i      = iBegin;
      for ( ; ; ) {
        S_msrHarmony
          harmony = (*i);

        os << harmony;

        if (++i == iEnd) break;
        // no endl here
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the figured basses associated to this note if any
  unsigned int noteFiguredBassesListSize = fNoteFiguredBassesList.size ();

  if (noteFiguredBassesListSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteFiguredBasses";
    if (noteFiguredBassesListSize) {
      os << endl;

      ++gIndenter;

      list<S_msrFiguredBass>::const_iterator
        iBegin = fNoteFiguredBassesList.begin (),
        iEnd   = fNoteFiguredBassesList.end (),
        i      = iBegin;
      for ( ; ; ) {
        S_msrFiguredBass
          figuredBass = (*i);

        os << figuredBass;

        if (++i == iEnd) break;
        // no endl here
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the syllables associated to this note if any
  unsigned int noteSyllablesSize = fNoteSyllables.size ();

  if (noteSyllablesSize || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteSyllables";
    if (noteSyllablesSize) {
      os << endl;

      ++gIndenter;

      list<S_msrSyllable>::const_iterator
        iBegin = fNoteSyllables.begin (),
        iEnd   = fNoteSyllables.end (),
        i      = iBegin;
      for ( ; ; ) {
        S_msrSyllable
          syllable = (*i);

        os << syllable;

  /* JMI
        os <<
          syllable->syllableKindAsString () <<
            syllable->syllableExtendKindAsString () <<
          " : ";

        msrSyllable::writeTextsList (
          syllable->getSyllableTextsList (),
          os);

        os <<
          ", stanza " <<
          syllable->
            getSyllableStanzaUpLink ()->
              getStanzaNumber () <<
          ", line " << syllable->getInputLineNumber () <<
          ", noteUpLink: " <<
          syllable->
            getSyllableNoteUpLink ()->
              asShortString ();
  */

        if (++i == iEnd) break;
        // no endl here
      } // for

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the after grace group notes after if any
  if (fNoteGraceNotesGroupAfter || gGlobalMsrOahGroup->getDisplayMsrDetails ()) {
    os <<
      setw (fieldWidth) <<
      "noteGraceNotesGroupAfter";
    if (fNoteGraceNotesGroupAfter) {
      os << endl;

      ++gIndenter;

      fNoteGraceNotesGroupAfter->printShort (os);

      --gIndenter;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_msrNote& elt)
{
  elt->print (os);
  return os;
}


}

/* JMI
// tuplet upLink
S_msrTuplet msrNote::fetchNoteTupletUpLink () const
{
  S_msrTuplet result;

  switch (fNoteKind) {
    case msrNoteKind::k_NoNote:
      break;

    case msrNoteKind::kNoteRegularInMeasure:
    case msrNoteKind::kNoteRestInMeasure:
    case msrNoteKind::kNoteSkipInMeasure:
    case msrNoteKind::kNoteUnpitchedInMeasure:
      result = fNoteDirectTupletUpLink;
      break;

    case msrNoteKind::kNoteRegularInChord:
      if (fNoteDirectChordUpLink) {
        result =
          fNoteDirectChordUpLink->
            fetchChordTupletUpLink ();
      }
      break;

    case msrNoteKind::kNoteRegularInTuplet:
    case msrNoteKind::kNoteRestInTuplet:
      if (fNoteDirectTupletUpLink) {
        result =
          fNoteDirectTupletUpLink->
            fetchTupletTupletUpLink ();
      }
      break;

    case msrNoteKind::kNoteInDoubleTremolo:
      break;

    case msrNoteKind::kNoteRegularInGraceNotesGroup:
    case msrNoteKind::kNoteSkipInGraceNotesGroup:
    / * JMi
      if (fNoteDirectGraceNotesGroupUpLink) {
        result =
          fNoteDirectGraceNotesGroupUpLink->
            getGraceNotesGroupVoiceUpLink ();
              / * JMI
              getGraceNotesGroupNoteUpLink ()->
              fetchNoteVoiceUpLink ();
              * /
      }
    * /
      break;

    case msrNoteKind::kNoteInChordInGraceNotesGroup:
      if (fNoteDirectChordUpLink) {
        result =
          fNoteDirectChordUpLink->
            getChordDirectGraceNotesGroupUpLink ()->
              fetchGraceNoteGroupTupletUpLink ();
      }
      break;

    case msrNoteKind::kNoteInTupletInGraceNotesGroup:
      / * cannot happen ??? JMI
      if (fNoteDirectTupletUpLink) {
        result =
          fNoteDirectTupletUpLink->
            getTupletDirectTupletUpLink ()->
              fetchTupletVoiceUpLink ();
      }
      * /
      break;

    case msrNoteKind::kNoteUnpitchedInTuplet:
      break;
  } // switch

  return result;
}
*/

