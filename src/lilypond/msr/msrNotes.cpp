/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "msrMutualDependencies.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

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

  rational                   noteSoundingWholeNotes,
  rational                   noteDisplayWholeNotes,

  int                        noteDotsNumber,

  msrDurationKind            noteGraphicDurationKind,

  int                        noteOctave,

  msrQuarterTonesPitchKind   noteQuarterTonesDisplayPitchKind,
  int                        noteDisplayOctave,

  bool                       noteIsARest,
  bool                       noteIsUnpitched,

  bool                       noteIsACueNote,
  bool                       noteIsAGraceNote,

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

      noteSoundingWholeNotes,
      noteDisplayWholeNotes,

      noteDotsNumber,

      noteGraphicDurationKind,

      noteOctave,

      noteQuarterTonesDisplayPitchKind,
      noteDisplayOctave,

      noteIsARest,
      noteIsUnpitched,

      noteIsACueNote,
      noteIsAGraceNote,

      notePrintObjectKind,

      noteHeadKind,
      noteHeadFilledKind,
      noteHeadParenthesesKind);
  assert(o!=0);

  return o;
}

msrNote::msrNote (
  int                        inputLineNumber,
  string                     noteMeasureNumber,

  msrNoteKind                noteKind,

  msrQuarterTonesPitchKind   noteQuarterTonesPitchKind,

  rational                   noteSoundingWholeNotes,
  rational                   noteDisplayWholeNotes,

  int                        noteDotsNumber,

  msrDurationKind            noteGraphicDurationKind,

  int                        noteOctave,

  msrQuarterTonesPitchKind   noteQuarterTonesDisplayPitchKind,
  int                        noteDisplayOctave,

  bool                       noteIsARest,
  bool                       noteIsUnpitched,

  bool                       noteIsACueNote,
  bool                       noteIsAGraceNote,

  msrPrintObjectKind         notePrintObjectKind,

  msrNoteHeadKind            noteHeadKind,
  msrNoteHeadFilledKind      noteHeadFilledKind,
  msrNoteHeadParenthesesKind noteHeadParenthesesKind)
  : msrTupletElement (inputLineNumber),
    fNoteColor ("", "")
{
  fMeasureElementMeasureNumber = noteMeasureNumber;

  // basic note description
  fNoteKind = noteKind;

  fNoteQuarterTonesPitchKind  = noteQuarterTonesPitchKind;

  fMeasureElementSoundingWholeNotes = noteSoundingWholeNotes;
  fNoteDisplayWholeNotes  = noteDisplayWholeNotes;

  fNoteDotsNumber = noteDotsNumber;

  fNoteGraphicDurationKind = noteGraphicDurationKind;

  fNoteTupletFactor = rational (1, 1);

  fNoteOctave = noteOctave;

  fNoteQuarterTonesDisplayPitchKind = noteQuarterTonesDisplayPitchKind;
  fNoteDisplayOctave                = noteDisplayOctave;

  fNoteIsARest     = noteIsARest;
  fNoteIsUnpitched = noteIsUnpitched;

  fNoteIsACueNote   = noteIsACueNote;
  fNoteIsAGraceNote = noteIsAGraceNote;

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

  if (fNoteIsARest && fNoteDisplayOctave != K_NO_OCTAVE) {
    // this note is a pitched rest:
    // copy the display octave to the the note octave, // JMI
    // to be used in octave relative code generation
    fNoteQuarterTonesPitchKind = fNoteQuarterTonesDisplayPitchKind;
    fNoteOctave = fNoteDisplayOctave;
  }

  // note accidentals
  // ------------------------------------------------------

  fNoteAccidentalKind =
    msrNote::kNoteAccidentalNone; // default value

  fNoteEditorialAccidentalKind =
    msrNote::kNoteEditorialAccidentalNo; // default value

  fNoteCautionaryAccidentalKind =
    msrNote::kNoteCautionaryAccidentalNo; // default value

  // note context
  // ------------------------------------------------------

  fNoteStaffNumber = 0;
  fNoteVoiceNumber = 0;

  fNoteBelongsToAChord = false;
  fNoteBelongsToATuplet = false;
  fNoteOccupiesAFullMeasure = false;

  // note lyrics
  // ------------------------------------------------------

#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotesDetails) {
    gLogOstream <<
      endl <<
      "Initializing a note" <<
      ", kind: ";
    if (fNoteKind == k_NoNoteKind)
      gLogOstream <<
        "not yet known";
    else
      gLogOstream <<
        noteKindAsString (fNoteKind);
    gLogOstream <<
      ", line " << fInputLineNumber << ":" <<
      endl;

    gIndenter++;

    const int fieldWidth = 30;

    gLogOstream <<
      left <<
        setw (fieldWidth) <<
        "fNoteQuarterTonesPitch" << " = " <<
        msrQuarterTonesPitchKindAsString (
          gMsrOah->fMsrQuarterTonesPitchesLanguageKind,
          fNoteQuarterTonesPitchKind) <<
        endl;

    gLogOstream <<
      left <<
        setw (fieldWidth) <<
        "fMeasureElementSoundingWholeNotes" << " = " <<
        fMeasureElementSoundingWholeNotes <<
        endl;

    gLogOstream <<
      left <<
        setw (fieldWidth) <<
        "fNoteDisplayWholeNotes" << " = " <<
        fNoteDisplayWholeNotes <<
        endl;

    gLogOstream <<
      left <<
        setw (fieldWidth) <<
        "fNoteDotsNumber" << " = " <<
        fNoteDotsNumber <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteGraphicDuration" << " = ";
      if (fNoteGraphicDurationKind != k_NoDuration) {
        gLogOstream <<
          msrDurationKindAsString (
            fNoteGraphicDurationKind);
      }
      else {
        gLogOstream << "none";
      }

    gLogOstream <<
      left <<
        setw (fieldWidth) <<
        "fNoteTupletFactor" << " = " <<
        fNoteTupletFactor <<
        endl;

    gLogOstream <<
      endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteOctave" << " = " <<
        fNoteOctave <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteDisplayOctave" << " = " <<
        fNoteDisplayOctave <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteIsARest" << " = " <<
        booleanAsString (fNoteIsARest) <<
        endl <<
      left <<
        setw (fieldWidth) <<
        "fNoteIsUnpitched" << " = " <<
         booleanAsString (fNoteIsUnpitched) <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteIsACueNote" << " = " <<
         booleanAsString (fNoteIsACueNote) <<
        endl <<
      left <<
        setw (fieldWidth) <<
        "fNoteIsAGraceNote" << " = " <<
         booleanAsString (fNoteIsAGraceNote) <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNotePrintObjectKind" << " = " <<
         msrPrintObjectKindAsString (fNotePrintObjectKind) <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteHeadKind" << " = " <<
         noteHeadKindAsString (fNoteHeadKind) <<
        endl <<
      left <<
        setw (fieldWidth) <<
        "fNoteHeadFilledKind" << " = " <<
         noteHeadFilledKindAsString (fNoteHeadFilledKind) <<
        endl <<
      left <<
        setw (fieldWidth) <<
        "fNoteHeadParenthesesKind" << " = " <<
         noteHeadParenthesesKindAsString (fNoteHeadParenthesesKind) <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteAccidentalKind" << " = " <<
        noteAccidentalKindAsString (
          fNoteAccidentalKind) <<
        endl <<
      left <<
        setw (fieldWidth) <<
        "fNoteEditorialAccidentalKind" << " = " <<
        noteEditorialAccidentalKindAsString (
          fNoteEditorialAccidentalKind) <<
        endl <<
      left <<
        setw (fieldWidth) <<
        "fNoteCautionaryAccidentalKind" << " = " <<
        noteCautionaryAccidentalKindAsString (
          fNoteCautionaryAccidentalKind) <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fStaffNumber" << " = " <<
        fNoteStaffNumber <<
        endl <<
      left <<
        setw (fieldWidth) <<
        "fNoteVoiceNumber" << " = " <<
        fNoteVoiceNumber <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteBelongsToAChord" << " = " <<
         booleanAsString (fNoteBelongsToAChord) <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteBelongsToATuplet" << " = " <<
         booleanAsString (fNoteBelongsToATuplet) <<
        endl <<
        endl <<

      left <<
        setw (fieldWidth) <<
        "fNoteOccupiesAFullMeasure" << " = " <<
         booleanAsString (fNoteOccupiesAFullMeasure) <<
        endl <<
      endl;

    gIndenter--;
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

void msrNote::setNoteKind (msrNoteKind noteKind)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotes) {
    gLogOstream <<
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

S_msrNote msrNote::createNoteNewbornClone (
  S_msrPart containingPart)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotes) {
    gLogOstream <<
      "Creating a newborn clone of note '" <<
      asString () <<
      "' in part " <<
      containingPart->
        getPartCombinedName () <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingPart != nullptr,
    "containingPart is null");

  S_msrNote
    newbornClone =
      msrNote::create (
        fInputLineNumber,
        fMeasureElementMeasureNumber,

        fNoteKind,

        fNoteQuarterTonesPitchKind,

        fMeasureElementSoundingWholeNotes,
        fNoteDisplayWholeNotes,

        fNoteDotsNumber,

        fNoteGraphicDurationKind,

        fNoteOctave,

        fNoteQuarterTonesDisplayPitchKind,
        fNoteDisplayOctave,

        fNoteIsARest,
        fNoteIsUnpitched,

        fNoteIsACueNote,
        fNoteIsAGraceNote,

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

  // staff and voice context
  // ------------------------------------------------------

  newbornClone->
    fNoteStaffNumber =
      fNoteStaffNumber;
  newbornClone->
    fNoteVoiceNumber =
      fNoteVoiceNumber;

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

  newbornClone->fNoteColor =
    fNoteColor;

  // upLinks
  // ------------------------------------------------------

  /* JMI
    S_msrTuplet           fNoteTupletUpLink;

    S_msrMeasure          fNoteMeasureUpLink;
*/

  return newbornClone;
}

S_msrNote msrNote::createNoteDeepCopy (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotes) {
    gLogOstream <<
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
  msrAssert(
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

        fMeasureElementSoundingWholeNotes,
        fNoteDisplayWholeNotes,

        fNoteDotsNumber,

        fNoteGraphicDurationKind,

        fNoteOctave,

        fNoteQuarterTonesDisplayPitchKind,
        fNoteDisplayOctave,

        fNoteIsARest,
        fNoteIsUnpitched,

        fNoteIsACueNote,
        fNoteIsAGraceNote,

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

  // staff and voice context
  // ------------------------------------------------------

  noteDeepCopy->
    fNoteStaffNumber =
      fNoteStaffNumber;
  noteDeepCopy->
    fNoteVoiceNumber =
      fNoteVoiceNumber;

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
    for (i=fNoteSyllables.begin (); i!=fNoteSyllables.end (); i++) {
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
    for (i=fNoteBeams.begin (); i!=fNoteBeams.end (); i++) {
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
      i++
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
      i++
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
      i++
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
      i++
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
      i++
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
    for (i=fNoteOrnaments.begin (); i!=fNoteOrnaments.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteOrnaments.push_back ((*i));
    } // for
  }

  // glissandos
  // ------------------------------------------------------

  {
    list<S_msrGlissando>::const_iterator i;
    for (i=fNoteGlissandos.begin (); i!=fNoteGlissandos.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteGlissandos.push_back ((*i));
    } // for
  }

  // slides
  // ------------------------------------------------------

  {
    list<S_msrSlide>::const_iterator i;
    for (i=fNoteSlides.begin (); i!=fNoteSlides.end (); i++) {
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
    for (i=fNoteDynamics.begin (); i!=fNoteDynamics.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteDynamics.push_back ((*i));
    } // for
  }

  {
    list<S_msrOtherDynamics>::const_iterator i;
    for (i=fNoteOtherDynamics.begin (); i!=fNoteOtherDynamics.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteOtherDynamics.push_back ((*i));
    } // for
  }

  // slashes
  // ------------------------------------------------------

  {
    list<S_msrSlash>::const_iterator i;
    for (i=fNoteSlashes.begin (); i!=fNoteSlashes.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteSlashes.push_back ((*i));
    } // for
  }

  // wedges
  // ------------------------------------------------------

  {
    list<S_msrWedge>::const_iterator i;
    for (i=fNoteWedges.begin (); i!=fNoteWedges.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteWedges.push_back ((*i));
    } // for
  }

  // eyeglasses
  // ------------------------------------------------------

  {
    list<S_msrEyeGlasses>::const_iterator i;
    for (i=fNoteEyeGlasses.begin (); i!=fNoteEyeGlasses.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteEyeGlasses.push_back ((*i));
    } // for
  }

  // damps
  // ------------------------------------------------------

  {
    list<S_msrDamp>::const_iterator i;
    for (i=fNoteDamps.begin (); i!=fNoteDamps.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteDamps.push_back ((*i));
    } // for
  }

  // damp alls
  // ------------------------------------------------------

  {
    list<S_msrDampAll>::const_iterator i;
    for (i=fNoteDampAlls.begin (); i!=fNoteDampAlls.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteDampAlls.push_back ((*i));
    } // for
  }

  // scordaturas
  // ------------------------------------------------------

  {
    list<S_msrScordatura>::const_iterator i;
    for (i=fNoteScordaturas.begin (); i!=fNoteScordaturas.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteScordaturas.push_back ((*i));
    } // for
  }

  // words
  // ------------------------------------------------------

  {
    list<S_msrWords>::const_iterator i;
    for (i=fNoteWords.begin (); i!=fNoteWords.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteWords.push_back ((*i));
    } // for
  }

  // slurs
  // ------------------------------------------------------

  {
    list<S_msrSlur>::const_iterator i;
    for (i=fNoteSlurs.begin (); i!=fNoteSlurs.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteSlurs.push_back ((*i));
    } // for
  }

  // ligatures
  // ------------------------------------------------------

  {
    list<S_msrLigature>::const_iterator i;
    for (i=fNoteLigatures.begin (); i!=fNoteLigatures.end (); i++) {
      // share this data
      noteDeepCopy->
        fNoteLigatures.push_back ((*i));
    } // for
  }

  // pedals
  // ------------------------------------------------------

  {
    list<S_msrPedal>::const_iterator i;
    for (i=fNotePedals.begin (); i!=fNotePedals.end (); i++) {
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

  noteDeepCopy->fNoteColor =
    fNoteColor;

  // upLinks
  // ------------------------------------------------------

  /* JMI

    S_msrTuplet           fNoteChordUpLink;

    S_msrGraceNotesGroup  fNoteGraceNoteGroupUpLink;

    S_msrTuplet           fNoteTupletUpLink;

    S_msrMeasure          fNoteMeasureUpLink;
*/

  return noteDeepCopy;
}

S_msrNote msrNote::createRestNote (
  int       inputLineNumber,
  string    noteMeasureNumber,
  rational  soundingWholeNotes,
  rational  displayWholeNotes,
  int       dotsNumber,
  int       staffNumber,
  int       voiceNumber)
{
  msrNote * o =
    new msrNote (
      inputLineNumber,
      noteMeasureNumber,

      kRestNote, // noteKind

      k_NoQuarterTonesPitch_QTP,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      k_NoDuration, // noteGraphicDuration

      K_NO_OCTAVE, // noteOctave,

      k_NoQuarterTonesPitch_QTP, // noteDisplayQuarterTonesPitch
      K_NO_OCTAVE, // noteDisplayOctave,

      false, // noteIsARest
      false, // noteIsUnpitched

      false, // noteIsACueNote
      false, // noteIsAGraceNote

      kPrintObjectYes, // JMI

      kNoteHeadNormal, // JMI
      kNoteHeadFilledYes, // JMI
      kNoteHeadParenthesesNo); // JMI
  assert(o!=0);

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestNotes) {
    gLogOstream <<
      "Creating rest note '" <<
      o->asShortString () <<
      ", line " << inputLineNumber <<
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
  int       dotsNumber,
  int       staffNumber,
  int       voiceNumber)
{
  msrNote * o =
    new msrNote (
      inputLineNumber,
      noteMeasureNumber,

      kSkipNote, // noteKind

      k_NoQuarterTonesPitch_QTP,

      soundingWholeNotes,
      displayWholeNotes,

      dotsNumber,

      k_NoDuration, // noteGraphicDuration

      K_NO_OCTAVE, // noteOctave,

      k_NoQuarterTonesPitch_QTP, // noteDisplayQuarterTonesPitch
      K_NO_OCTAVE, // noteDisplayOctave,

      false, // noteIsARest
      false, // noteIsUnpitched

      false, // noteIsACueNote
      false, // noteIsAGraceNote

      kPrintObjectYes, // JMI

      kNoteHeadNormal, // JMI
      kNoteHeadFilledYes, // JMI
      kNoteHeadParenthesesNo); // JMI
  assert(o!=0);

#ifdef TRACE_OAH
  if (gTraceOah->fTraceSkipNotes) {
    gLogOstream <<
      "Creating skip note '" <<
      o->asShortString () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  return o;
}

//________________________________________________________________________
S_msrNote msrNote::createNoteFromSemiTonesPitchAndOctave (
  int                          inputLineNumber,
  S_msrSemiTonesPitchAndOctave semiTonesPitchAndOctave)
{
  // sanity check
  msrAssert (
    semiTonesPitchAndOctave != nullptr,
    "semiTonesPitchAndOctave is null");

  msrNote * o =
    new msrNote (
      inputLineNumber,
      K_NO_MEASURE_NUMBER,

      kRegularNote, // noteKind

      quarterTonesPitchKindFromSemiTonesPitchKind (
        semiTonesPitchAndOctave->getSemiTonesPitchKind ()),

      rational (0, 1), // soundingWholeNotes,
      rational (0, 1), // displayWholeNotes,

      0, // dotsNumber,

      k_NoDuration, // noteGraphicDuration

      semiTonesPitchAndOctave->getOctave (), // noteOctave,

      k_NoQuarterTonesPitch_QTP, // noteDisplayQuarterTonesPitch
      K_NO_OCTAVE, // noteDisplayOctave,

      false, // noteIsARest
      false, // noteIsUnpitched

      false, // noteIsACueNote
      false, // noteIsAGraceNote

      kPrintObjectNo, // JMI

      kNoteHeadNormal, // JMI
      kNoteHeadFilledYes, // JMI
      kNoteHeadParenthesesNo); // JMI
  assert(o!=0);

#ifdef TRACE_OAH
  if (gTraceOah->fTraceNotesOctaveEntry) {
    gLogOstream <<
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

void msrNote::setNotePositionInMeasure (
  rational positionInMeasure)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTracePositionsInMeasures) {
    gLogOstream <<
      "Setting note position in measure of " << asString () <<
      " to '" <<
      positionInMeasure <<
      "'" <<
      endl;
  }
#endif

 // JMI if (positionInMeasure.getNumerator () == 60) abort ();

  positionInMeasure.rationalise (); // TEMP ? JMI

  msrMeasureElement::setMeasureElementPositionInMeasure (
    positionInMeasure,
    "setNotePositionInMeasure()");

  // are there harmonies attached to this note?
  if (fNoteHarmoniesList.size ()) {
    list<S_msrHarmony>::const_iterator i;
    for (
      i=fNoteHarmoniesList.begin (); i!=fNoteHarmoniesList.end (); i++
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
      i=fNoteFiguredBassesList.begin (); i!=fNoteFiguredBassesList.end (); i++
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
      i=fNoteDalSegnos.begin (); i!=fNoteDalSegnos.end (); i++
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

  if (fNoteIsARest) {
    // the measure it belongs to is a single-measure rest
    fNoteMeasureUpLink->
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
    result = "(no display whole notes)";
  }
  else {
    result =
      wholeNotesAsMsrString (
        fInputLineNumber,
        fNoteDisplayWholeNotes);
  }

  return result;
}

string msrNote::noteKindAsString (
  msrNoteKind noteKind)
{
  string result;

  switch (noteKind) {
    case msrNote::k_NoNoteKind:
      result = "noNoteKind???";
      break;

    case msrNote::kRestNote:
      result = "restNote";
      break;

    case msrNote::kSkipNote:
      result = "skipNote";
      break;

    case msrNote::kUnpitchedNote:
      result = "unpitchedNote";
      break;

    case msrNote::kRegularNote:
      result = "regularNote";
      break;

    case msrNote::kDoubleTremoloMemberNote:
      result = "doubleTremoloMemberNote";
      break;

    case msrNote::kGraceNote:
      result = "graceNote";
      break;

    case msrNote::kGraceChordMemberNote:
      result = "graceChordMemberNote";
      break;

    case msrNote::kChordMemberNote:
      result = "chordMemberNote";
      break;

    case msrNote::kTupletMemberNote:
      result = "tupletMemberNote";
      break;

    case msrNote::kGraceTupletMemberNote:
      result = "graceTupletMemberNote";
      break;

    case msrNote::kTupletMemberUnpitchedNote:
      result = "tupletMemberUnpitchedNote";
      break;
  } // switch

  return result;
}

// accidentals
//______________________________________________________________________________
string msrNote::noteAccidentalKindAsString (
  msrNoteAccidentalKind noteAccidentalKind)
{
  string result;

  switch (noteAccidentalKind) {
    case msrNote::kNoteAccidentalNone:
      result = "noteAccidentalNone";
      break;

    case msrNote::kNoteAccidentalSharp:
      result = "noteAccidentalSharp";
      break;
    case msrNote::kNoteAccidentalNatural:
      result = "noteAccidentalNatural";
      break;
    case msrNote::kNoteAccidentalFlat:
      result = "noteAccidentalFlat";
      break;
    case msrNote::kNoteAccidentalDoubleSharp:
      result = "noteAccidentalDoubleSharp";
      break;
    case msrNote::kNoteAccidentalSharpSharp:
      result = "noteAccidentalSharpSharp";
      break;
    case msrNote::kNoteAccidentalFlatFlat:
      result = "noteAccidentalFlatFlat";
      break;
    case msrNote::kNoteAccidentalNaturalSharp:
      result = "noteAccidentalNaturalSharp";
      break;
    case msrNote::kNoteAccidentalNaturalFlat:
      result = "noteAccidentalNaturalFlat";
      break;
    case msrNote::kNoteAccidentalQuarterFlat:
      result = "noteAccidentalQuarterFlat";
      break;
    case msrNote::kNoteAccidentalQuarterSharp:
      result = "noteAccidentalQuarterSharp";
      break;
    case msrNote::kNoteAccidentalThreeQuartersFlat:
      result = "noteAccidentalThreeQuartersFlat";
      break;
    case msrNote::kNoteAccidentalThreeQuartersSharp:
      result = "noteAccidentalThreeQuartersSharp";
      break;

    case msrNote::kNoteAccidentalSharpDown:
      result = "noteAccidentalSharpDown";
      break;
    case msrNote::kNoteAccidentalSharpUp:
      result = "noteAccidentalSharpUp";
      break;
    case msrNote::kNoteAccidentalNaturalDown:
      result = "noteAccidentalNaturalDown";
      break;
    case msrNote::kNoteAccidentalNaturalUp:
      result = "noteAccidentalNaturalUp";
      break;
    case msrNote::kNoteAccidentalFlatDown:
      result = "noteAccidentalFlatDown";
      break;
    case msrNote::kNoteAccidentalFlatUp:
      result = "noteAccidentalFlatUp";
      break;
    case msrNote::kNoteAccidentalTripleSharp:
      result = "noteAccidentalTripleSharp";
      break;
    case msrNote::kNoteAccidentalTripleFlat:
      result = "noteAccidentalTripleFlat";
      break;
    case msrNote::kNoteAccidentalSlashQuarterSharp:
      result = "noteAccidentalSlashQuarterSharp";
      break;
    case msrNote::kNoteAccidentalSlashSharp:
      result = "noteAccidentalSlashSharp";
      break;
    case msrNote::kNoteAccidentalSlashFlat:
      result = "noteAccidentalSlashFlat";
      break;
    case msrNote::kNoteAccidentaldoubleSlashFlat:
      result = "noteAccidentaldoubleSlashFlat";
      break;
    case msrNote::kNoteAccidentalSharp_1:
      result = "noteAccidentalSharp_1";
      break;
    case msrNote::kNoteAccidentalSharp_2:
      result = "noteAccidentalSharp_2";
      break;
    case msrNote::kNoteAccidentalSharp_3:
      result = "noteAccidentalSharp_3";
      break;
    case msrNote::kNoteAccidentalSharp_5:
      result = "noteAccidentalSharp_5";
      break;
    case msrNote::kNoteAccidentalFlat_1:
      result = "noteAccidentalFlat_1";
      break;
    case msrNote::kNoteAccidentalFlat_2:
      result = "noteAccidentalFlat_2";
      break;
    case msrNote::kNoteAccidentalFlat_3:
      result = "noteAccidentalFlat_3";
      break;
    case msrNote::kNoteAccidentalFlat_4:
      result = "noteAccidentalFlat_4";
      break;
    case msrNote::kNoteAccidentalSori:
      result = "noteAccidentalSori";
      break;
    case msrNote::kNoteAccidentalKoron:
      result = "noteAccidentalKoron";
      break;
  } // switch

  return result;
}

string msrNote::noteAccidentalKindAsMusicXMLString (
  msrNoteAccidentalKind noteAccidentalKind)
{
  string result;

  switch (noteAccidentalKind) {
    case msrNote::kNoteAccidentalNone:
      break;

    case msrNote::kNoteAccidentalSharp:
      result = "sharp";
      break;
    case msrNote::kNoteAccidentalNatural:
      result = "natural";
      break;
    case msrNote::kNoteAccidentalFlat:
      result = "flat";
      break;
    case msrNote::kNoteAccidentalDoubleSharp:
      result = "double-sharp";
      break;
    case msrNote::kNoteAccidentalSharpSharp:
      result = "sharp-sharp";
      break;
    case msrNote::kNoteAccidentalFlatFlat:
      result = "flat-flat";
      break;
    case msrNote::kNoteAccidentalNaturalSharp:
      result = "natural-sharp";
      break;
    case msrNote::kNoteAccidentalNaturalFlat:
      result = "natural-flat";
      break;
    case msrNote::kNoteAccidentalQuarterFlat:
      result = "quarter-flat";
      break;
    case msrNote::kNoteAccidentalQuarterSharp:
      result = "quarter-sharp";
      break;
    case msrNote::kNoteAccidentalThreeQuartersFlat:
      result = "three-quarters-flat";
      break;
    case msrNote::kNoteAccidentalThreeQuartersSharp:
      result = "three-quarters-sharp";
      break;

    case msrNote::kNoteAccidentalSharpDown:
      result = "sharp-down";
      break;
    case msrNote::kNoteAccidentalSharpUp:
      result = "sharp-up";
      break;
    case msrNote::kNoteAccidentalNaturalDown:
      result = "natural-down";
      break;
    case msrNote::kNoteAccidentalNaturalUp:
      result = "natural-up";
      break;
    case msrNote::kNoteAccidentalFlatDown:
      result = "flat-down";
      break;
    case msrNote::kNoteAccidentalFlatUp:
      result = "flat-up";
      break;
    case msrNote::kNoteAccidentalTripleSharp:
      result = "triple-sharp";
      break;
    case msrNote::kNoteAccidentalTripleFlat:
      result = "triple-flat";
      break;
    case msrNote::kNoteAccidentalSlashQuarterSharp:
      result = "slash-quarter-sharp";
      break;
    case msrNote::kNoteAccidentalSlashSharp:
      result = "slash-sharp";
      break;
    case msrNote::kNoteAccidentalSlashFlat:
      result = "slash-flat";
      break;
    case msrNote::kNoteAccidentaldoubleSlashFlat:
      result = "double-slash-flat";
      break;
    case msrNote::kNoteAccidentalSharp_1:
      result = "sharp-1";
      break;
    case msrNote::kNoteAccidentalSharp_2:
      result = "sharp-2";
      break;
    case msrNote::kNoteAccidentalSharp_3:
      result = "sharp-3";
      break;
    case msrNote::kNoteAccidentalSharp_5:
      result = "sharp-5";
      break;
    case msrNote::kNoteAccidentalFlat_1:
      result = "flat-1";
      break;
    case msrNote::kNoteAccidentalFlat_2:
      result = "flat-2";
      break;
    case msrNote::kNoteAccidentalFlat_3:
      result = "flat-3";
      break;
    case msrNote::kNoteAccidentalFlat_4:
      result = "flat-4";
      break;
    case msrNote::kNoteAccidentalSori:
      result = "sori";
      break;
    case msrNote::kNoteAccidentalKoron:
      result = "koron";
      break;
  } // switch

  return result;
}

string msrNote::noteEditorialAccidentalKindAsString (
  msrNoteEditorialAccidentalKind noteEditorialAccidentalKind)
{
  string result;

  switch (noteEditorialAccidentalKind) {
    case msrNote::kNoteEditorialAccidentalYes:
      result = "noteEditorialAccidentalYes";
      break;
    case msrNote::kNoteEditorialAccidentalNo:
      result = "noteEditorialAccidentalNo";
      break;
  } // switch

  return result;
}

string msrNote::noteCautionaryAccidentalKindAsString (
  msrNoteCautionaryAccidentalKind noteCautionaryAccidentalKind)
{
  string result;

  switch (noteCautionaryAccidentalKind) {
    case msrNote::kNoteCautionaryAccidentalYes:
      result = "noteCautionaryAccidentalYes";
      break;
    case msrNote::kNoteCautionaryAccidentalNo:
      result = "noteCautionaryAccidentalNo";
      break;
  } // switch

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

string msrNote::noteDisplayOctaveAsString () const
{
  return
    fNoteDisplayOctave == K_NO_OCTAVE
      ? "no display octave"
      : to_string (fNoteDisplayOctave);
}

bool msrNote::noteIsAPitchedRest () const
{
  return
    fNoteIsARest && fNoteDisplayOctave != K_NO_OCTAVE;
}

void msrNote::setNoteStem (S_msrStem stem)
{
  // register note stem
  fNoteStem = stem;

  // mark note as stemless if relevant
  if (stem->getStemKind () == msrStem::kStemNone)
    fNoteIsStemless = true;
}

void msrNote::setNoteBelongsToAChord ()
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceChords) {
    gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceTuplets) {
    gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceTuplets) {
    gLogOstream <<
      "The result is: '" <<
      asShortString () <<
      endl;
  }
#endif
}

void msrNote::appendBeamToNote (S_msrBeam beam)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceBeams) {
    gLogOstream <<
      "Adding beam '" <<
      beam->asShortString () <<
      "' to note '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  fNoteBeams.push_back (beam);
}

void msrNote::appendArticulationToNote (S_msrArticulation art)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceArticulations) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceSpanners) {
    gLogOstream <<
      "Adding spanner '" <<
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
        case kSpannerTypeStart:
          fNoteWavyLineSpannerStart = spanner;
          break;
        case kSpannerTypeStop:
          fNoteWavyLineSpannerStop = spanner;
          break;
        case kSpannerTypeContinue:
          break;
        case k_NoSpannerType:
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTechnicals) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTechnicals) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTechnicals) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTechnicals) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceOrnaments) {
    gLogOstream <<
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

    case msrOrnament::kOrnamentDashes:
      fNoteDashesOrnament = ornament;
      break;

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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceGlissandos) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceSlides) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceGraceNotes) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceGraceNotes) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceGraceNotes) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTremolos) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceDynamics) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceSlurs) {
    gLogOstream <<
      "Adding slur '" << slur <<
      "' to note '" << asString () << "'" <<
      endl;
  }
#endif

  fNoteSlurs.push_back (slur);
}

void msrNote::appendLigatureToNote (S_msrLigature ligature)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceLigatures) {
    gLogOstream <<
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

#ifdef TRACE_OAH
      if (gTraceOah->fTraceLigatures) {
        stringstream s;

        s <<
          "a 'ligature start' is immediately followed by a 'ligature stop'" <<
          endl <<
          "with the same number, ignoring both of them at line " <<
          ligature->getInputLineNumber ();

        msrMusicXMLWarning (
          gOahOah->fInputSourceName,
          ligature->getInputLineNumber (),
          s.str ());
      }
#endif

      // remove 'ligature start'
#ifdef TRACE_OAH
      if (gTraceOah->fTraceLigatures) {
        gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTracePedals) {
    gLogOstream <<
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

#ifdef TRACE_OAH
      if (gTraceOah->fTracePedals) {
        stringstream s;

        s <<
          "a 'pedal start' is immediately followed by a 'pedal stop'" <<
          endl <<
          "with the same number, ignoring both of them at line " <<
          pedal->getInputLineNumber ();

        msrMusicXMLWarning (
          gOahOah->fInputSourceName,
          pedal->getInputLineNumber (),
          s.str ());
      }
#endif

      // rmeove 'pedal start'
#ifdef TRACE_OAH
      if (gTraceOah->fTracePedals) {
        gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceSlashes) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceDalSegnos) {
    gLogOstream <<
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

S_msrDynamics msrNote::removeFirstDynamics () // JMI
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceDynamics) {
    gLogOstream <<
      "Removing first dynamics from note '" <<
      asShortString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    fNoteDynamics.size () > 0,
    "fNoteDynamics is empty");

  S_msrDynamics dyn = fNoteDynamics.front ();
  fNoteDynamics.pop_front ();

  return dyn;
}

S_msrWedge msrNote::removeFirstWedge () // JMI
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceWedges) {
    gLogOstream <<
      "Removing first wedge from note '" <<
      asShortString () <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    fNoteDynamics.size () > 0,
    "fNoteDynamics is empty");

  S_msrWedge wedge = fNoteWedges.front ();
  fNoteWedges.pop_front ();
  return wedge;
}

void msrNote::appendSyllableToNote (S_msrSyllable syllable)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceLyrics) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceHarmonies) {
    gLogOstream <<
      "Append harmony " <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceFiguredBasses) {
    gLogOstream <<
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
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrNote::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrNote>*
    p =
      dynamic_cast<visitor<S_msrNote>*> (v)) {
        S_msrNote elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrNote::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrNote::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrNote::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrNote>*
    p =
      dynamic_cast<visitor<S_msrNote>*> (v)) {
        S_msrNote elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrNote::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrNote::browseData (basevisitor* v)
{
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
    gIndenter++;
    list<S_msrBeam>::const_iterator i;
    for (i=fNoteBeams.begin (); i!=fNoteBeams.end (); i++) {
      // browse the beam
      msrBrowser<msrBeam> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the articulations if any
  if (fNoteArticulations.size ()) {
    gIndenter++;
    list<S_msrArticulation>::const_iterator i;
    for (i=fNoteArticulations.begin (); i!=fNoteArticulations.end (); i++) {
      // browse the articulation
      msrBrowser<msrArticulation> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the spanners if any
  if (fNoteSpanners.size ()) {
    gIndenter++;
    list<S_msrSpanner>::const_iterator i;
    for (i=fNoteSpanners.begin (); i!=fNoteSpanners.end (); i++) {
      // browse the spanner
      msrBrowser<msrSpanner> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the technicals if any
  if (fNoteTechnicals.size ()) {
    gIndenter++;
    list<S_msrTechnical>::const_iterator i;
    for (
      i=fNoteTechnicals.begin ();
      i!=fNoteTechnicals.end ();
      i++
  ) {
      // browse the technical
      msrBrowser<msrTechnical> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the technicals with integer if any
  if (fNoteTechnicalWithIntegers.size ()) {
    gIndenter++;
    list<S_msrTechnicalWithInteger>::const_iterator i;
    for (
      i=fNoteTechnicalWithIntegers.begin ();
      i!=fNoteTechnicalWithIntegers.end ();
      i++
  ) {
      // browse the technical
      msrBrowser<msrTechnicalWithInteger> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the technicals with float if any
  if (fNoteTechnicalWithFloats.size ()) {
    gIndenter++;
    list<S_msrTechnicalWithFloat>::const_iterator i;
    for (
      i=fNoteTechnicalWithFloats.begin ();
      i!=fNoteTechnicalWithFloats.end ();
      i++
  ) {
      // browse the technical
      msrBrowser<msrTechnicalWithFloat> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the technicals with string if any
  if (fNoteTechnicalWithStrings.size ()) {
    gIndenter++;
    list<S_msrTechnicalWithString>::const_iterator i;
    for (
      i=fNoteTechnicalWithStrings.begin ();
      i!=fNoteTechnicalWithStrings.end ();
      i++
  ) {
      // browse the technical
      msrBrowser<msrTechnicalWithString> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the ornaments if any
  if (fNoteOrnaments.size ()) {
    gIndenter++;
    list<S_msrOrnament>::const_iterator i;
    for (i=fNoteOrnaments.begin (); i!=fNoteOrnaments.end (); i++) {
      // browse the ornament
      msrBrowser<msrOrnament> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the glissandos if any
  if (fNoteGlissandos.size ()) {
    gIndenter++;
    list<S_msrGlissando>::const_iterator i;
    for (i=fNoteGlissandos.begin (); i!=fNoteGlissandos.end (); i++) {
      // browse the glissando
      msrBrowser<msrGlissando> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the slides if any
  if (fNoteSlides.size ()) {
    gIndenter++;
    list<S_msrSlide>::const_iterator i;
    for (i=fNoteSlides.begin (); i!=fNoteSlides.end (); i++) {
      // browse the glissando
      msrBrowser<msrSlide> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the grace notes group before if any
  if (fNoteGraceNotesGroupBefore) {
    // browse the grace notes group
    msrBrowser<msrGraceNotesGroup> browser (v);
    browser.browse (*fNoteGraceNotesGroupBefore);
  }

  // browse the after grace notes group after if any
  if (fNoteGraceNotesGroupAfter) {
    // browse the after grace notes grup
    msrBrowser<msrGraceNotesGroup> browser (v);
    browser.browse (*fNoteGraceNotesGroupAfter);
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
    gIndenter++;
    list<S_msrDynamics>::const_iterator i;
    for (i=fNoteDynamics.begin (); i!=fNoteDynamics.end (); i++) {
      // browse the dynamics
      msrBrowser<msrDynamics> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the other dynamics if any
  if (fNoteOtherDynamics.size ()) {
    gIndenter++;
    list<S_msrOtherDynamics>::const_iterator i;
    for (i=fNoteOtherDynamics.begin (); i!=fNoteOtherDynamics.end (); i++) {
      // browse the other dynamics
      msrBrowser<msrOtherDynamics> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the words if any
  if (fNoteWords.size ()) {
    gIndenter++;
    list<S_msrWords>::const_iterator i;
    for (i=fNoteWords.begin (); i!=fNoteWords.end (); i++) {
      // browse the words
      msrBrowser<msrWords> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the slurs if any
  if (fNoteSlurs.size ()) {
    gIndenter++;
    list<S_msrSlur>::const_iterator i;
    for (i=fNoteSlurs.begin (); i!=fNoteSlurs.end (); i++) {
      // browse the slur
      msrBrowser<msrSlur> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the ligatures if any
  if (fNoteLigatures.size ()) {
    gIndenter++;
    list<S_msrLigature>::const_iterator i;
    for (i=fNoteLigatures.begin (); i!=fNoteLigatures.end (); i++) {
      // browse the ligature
      msrBrowser<msrLigature> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the pedals if any
  if (fNotePedals.size ()) {
    gIndenter++;
    list<S_msrPedal>::const_iterator i;
    for (i=fNotePedals.begin (); i!=fNotePedals.end (); i++) {
      // browse the pedal
      msrBrowser<msrPedal> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the wedges if any
  if (fNoteSlashes.size ()) {
    gIndenter++;
    list<S_msrSlash>::const_iterator i;
    for (i=fNoteSlashes.begin (); i!=fNoteSlashes.end (); i++) {
      // browse the wedge
      msrBrowser<msrSlash> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the wedges if any
  if (fNoteWedges.size ()) {
    gIndenter++;
    list<S_msrWedge>::const_iterator i;
    for (i=fNoteWedges.begin (); i!=fNoteWedges.end (); i++) {
      // browse the wedge
      msrBrowser<msrWedge> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the segnos if any
  if (fNoteSegnos.size ()) {
    gIndenter++;
    list<S_msrSegno>::const_iterator i;
    for (i=fNoteSegnos.begin (); i!=fNoteSegnos.end (); i++) {
      // browse the segno
      msrBrowser<msrSegno> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the dal segnos if any
  if (fNoteDalSegnos.size ()) {
    gIndenter++;
    list<S_msrDalSegno>::const_iterator i;
    for (i=fNoteDalSegnos.begin (); i!=fNoteDalSegnos.end (); i++) {
      // browse the dal segno
      msrBrowser<msrDalSegno> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the codas if any
  if (fNoteCodas.size ()) {
    gIndenter++;
    list<S_msrCoda>::const_iterator i;
    for (i=fNoteCodas.begin (); i!=fNoteCodas.end (); i++) {
      // browse the coda
      msrBrowser<msrCoda> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the eyeglasses if any
  if (fNoteEyeGlasses.size ()) {
    gIndenter++;
    list<S_msrEyeGlasses>::const_iterator i;
    for (i=fNoteEyeGlasses.begin (); i!=fNoteEyeGlasses.end (); i++) {
      // browse the eyeglasses
      msrBrowser<msrEyeGlasses> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the damps if any
  if (fNoteDamps.size ()) {
    gIndenter++;
    list<S_msrDamp>::const_iterator i;
    for (i=fNoteDamps.begin (); i!=fNoteDamps.end (); i++) {
      // browse the damp
      msrBrowser<msrDamp> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the damp alls if any
  if (fNoteDampAlls.size ()) {
    gIndenter++;
    list<S_msrDampAll>::const_iterator i;
    for (i=fNoteDampAlls.begin (); i!=fNoteDampAlls.end (); i++) {
      // browse the damp all
      msrBrowser<msrDampAll> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the scordaturas if any
  if (fNoteScordaturas.size ()) {
    gIndenter++;
    list<S_msrScordatura>::const_iterator i;
    for (i=fNoteScordaturas.begin (); i!=fNoteScordaturas.end (); i++) {
      // browse the scordatura
      msrBrowser<msrScordatura> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }

  // browse the harmonies if any
  if (fNoteHarmoniesList.size ()) {
    list<S_msrHarmony>::const_iterator i;
    for (
      i=fNoteHarmoniesList.begin (); i!=fNoteHarmoniesList.end (); i++
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
      i=fNoteFiguredBassesList.begin (); i!=fNoteFiguredBassesList.end (); i++
    ) {
      // browse the figured bass
      msrBrowser<msrFiguredBass> browser (v);
      browser.browse (*(*i));
    } // for
  }

  // browse the syllables if any
  if (fNoteSyllables.size ()) {
    gIndenter++;
    list<S_msrSyllable>::const_iterator i;
    for (i=fNoteSyllables.begin (); i!=fNoteSyllables.end (); i++) {
      // browse the syllable
      msrBrowser<msrSyllable> browser (v);
      browser.browse (*(*i));
    } // for
    gIndenter--;
  }
}

string msrNote::notePitchAsString () const
{
  stringstream s;

  if (fNoteIsUnpitched) {
    s << "unpitched ";
  }
  else {
    s <<
      msrQuarterTonesPitchKindAsString (
        gMsrOah->fMsrQuarterTonesPitchesLanguageKind,
        fNoteQuarterTonesPitchKind);
  }

  return s.str ();
}

string msrNote::noteDisplayPitchKindAsString () const
{
  stringstream s;

  s <<
    msrQuarterTonesPitchKindAsString (
      gMsrOah->fMsrQuarterTonesPitchesLanguageKind,
      fNoteQuarterTonesDisplayPitchKind);

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
      gMsrOah->fMsrQuarterTonesPitchesLanguageKind,
      noteDiatonicPitchKind (
        fInputLineNumber));
}

string msrNote::asShortStringWithRawWholeNotes () const
{
  stringstream s;

  s << "[";

  switch (fNoteKind) {
    case msrNote::k_NoNoteKind:
      s <<
        "noNoteKind";
      break;

    case msrNote::kRestNote:
      s <<
        "Rest note" <<
        " [octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]" <<
        ":" <<
        ", whole notes: " <<
        fMeasureElementSoundingWholeNotes <<
        " sounding, " <<
        fNoteDisplayWholeNotes <<
        " displayed";

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << ".";
      } // for
      break;

    case msrNote::kSkipNote:
      s <<
        "Skip note" <<
        ":" <<
        ", whole notes: " <<
        fMeasureElementSoundingWholeNotes <<
        " sounding, " <<
        fNoteDisplayWholeNotes <<
        " displayed";

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << ".";
      } // for
      break;

    case msrNote::kUnpitchedNote:
      s <<
        notePitchAsString () <<
        "Unpitched note" <<
        ":" <<
        ", whole notes: " <<
        fMeasureElementSoundingWholeNotes <<
        " sounding, " <<
        fNoteDisplayWholeNotes <<
        " displayed";

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << ".";
      } // for
      break;

    case msrNote::kRegularNote:
      s <<
        "Standalone note '" <<
        notePitchAsString () <<
        "' " <<
        "[octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]" <<
        ":" <<
        ", whole notes: " <<
        fMeasureElementSoundingWholeNotes <<
        " sounding, " <<
        fNoteDisplayWholeNotes <<
        " displayed";

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << ".";
      } // for
      break;

    case msrNote::kDoubleTremoloMemberNote:
      s <<
        "DoubleTremoloMember note '" <<
        notePitchAsString () <<
        "' " <<
        "[octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]" <<
        ":" <<
        " whole notes: " <<
        fMeasureElementSoundingWholeNotes <<
        " sounding, " <<
        fNoteDisplayWholeNotes <<
        " displayed";
      break;

    case msrNote::kGraceNote:
      s <<
        "Grace note '" <<
        notePitchAsString () <<
        "' " <<
        noteGraphicDurationAsMsrString () <<
        "[octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << ".";
      } // for
      break;

    case msrNote::kGraceChordMemberNote:
      s <<
        "GraceChordMember note '" <<
        notePitchAsString () <<
        "' " <<
        noteGraphicDurationAsMsrString () <<
        "[octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << "."; // JMI
      } // for
      break;

    case msrNote::kChordMemberNote:
      s <<
        "ChordMember note '" <<
        notePitchAsString () <<
        "' " <<
        "[octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]" <<
        ", whole notes: " <<
        fMeasureElementSoundingWholeNotes <<
        " sounding, " <<
        fNoteDisplayWholeNotes <<
        " displayed";

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << ".";
      } // for
      break;

    case msrNote::kTupletMemberNote:
      s <<
        "TupletMember note '" <<
        notePitchAsString () <<
        "' " <<
        noteGraphicDurationAsMsrString ();

      if (! fNoteIsARest) {
        s <<
        "[octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
      }

      s <<
        ", whole notes: " <<
        fMeasureElementSoundingWholeNotes <<
        " sounding, " <<
        fNoteDisplayWholeNotes <<
        " displayed";

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << ".";
      } // for

      s <<
        ", noteTupletFactor " << fNoteTupletFactor;
      break;

    case msrNote::kGraceTupletMemberNote:
      s <<
        "GraceTupletMember note '" <<
        notePitchAsString () <<
        "' " <<
        noteGraphicDurationAsMsrString ();

      if (! fNoteIsARest) {
        s <<
        "[octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
      }

      s <<
        ", whole notes: " <<
        fMeasureElementSoundingWholeNotes <<
        " sounding, " <<
        fNoteDisplayWholeNotes <<
        " displayed";

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << ".";
      } // for

      s <<
        ", noteTupletFactor " << fNoteTupletFactor;
      break;

    case msrNote::kTupletMemberUnpitchedNote:
      s <<
        "TupletMemberUnpitched note " <<
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
    case msrNote::k_NoNoteKind:
      s <<
        "noNoteKind" <<
        ":" <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << ".";
      } // for
      break;

    case msrNote::kRestNote:
      s <<
        "RestNote" <<
        ":" <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << ".";
      } // for
      break;

    case msrNote::kSkipNote:
      s <<
        "SkipNote" <<
        ":" <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << ".";
      } // for
      break;

    case msrNote::kUnpitchedNote:
      s <<
        "UnpitchedNote" <<
        ":" <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << ".";
      } // for
      break;

    case msrNote::kRegularNote:
      s <<
        "RegularNote '" <<
        notePitchAsString () <<
        noteSoundingWholeNotesAsMsrString () <<
        "' [octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << ".";
      } // for
      break;

    case msrNote::kDoubleTremoloMemberNote:
      s <<
        "DoubleTremoloMemberNote '" <<
        notePitchAsString () <<
        noteSoundingWholeNotesAsMsrString () <<
        "' [octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << ".";
      } // for
      break;

    case msrNote::kGraceNote:
      s <<
        "GraceNote '" <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString () <<
        "' [octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << ".";
      } // for
      break;

   case msrNote::kGraceChordMemberNote:
      s <<
        "TraceChordMemberNote '" <<
        notePitchAsString () <<
        noteGraphicDurationAsMsrString () <<
        "' [octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << "."; // JMI
      } // for
      break;

    case msrNote::kChordMemberNote:
      s <<
        "ChordMemberNote '" <<
        notePitchAsString () <<
        noteSoundingWholeNotesAsMsrString () <<
        "' [octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << ".";
      } // for
      break;

    case msrNote::kTupletMemberNote:
      s <<
        "TupletMemberNote" <<
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
            fNoteTupletUpLink->getTupletActualNotes (),
            fNoteTupletUpLink->getTupletNormalNotes ());
            */

      if (! fNoteIsARest) {
        s <<
        "[octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
      }

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << ".";
      } // for

      s <<
        ", noteTupletFactor " << fNoteTupletFactor;
      break;

    case msrNote::kGraceTupletMemberNote:
      s <<
        "GraceTupletMemberNote" <<
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
            fNoteTupletUpLink->getTupletActualNotes (),
            fNoteTupletUpLink->getTupletNormalNotes ());
            */

      if (! fNoteIsARest) {
        s <<
        "[octave: " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
      }

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << ".";
      } // for

      s <<
        ", noteTupletFactor " << fNoteTupletFactor;
      break;

    case msrNote::kTupletMemberUnpitchedNote:
      s <<
        "TupletMemberUnpitchedNote" <<
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
            fNoteTupletUpLink->getTupletActualNotes (),
            fNoteTupletUpLink->getTupletNormalNotes ());
            */

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << ".";
      } // for

      s <<
        ", noteTupletFactor " << fNoteTupletFactor;
      break;
  } // switch

  s <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

string msrNote::asString () const
{
  stringstream s;

  s <<
    "[=== ";

  switch (fNoteKind) {
    case msrNote::k_NoNoteKind:
      s <<
        "noNoteKind???";
      break;

    case msrNote::kRestNote:
      s <<
        "restNote, ";
      if (fNoteOccupiesAFullMeasure) {
        s <<
          "R" <<
          /* JMI
          multipleRestMeasuresWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes);
            */
          noteSoundingWholeNotesAsMsrString ();
      }
      else {
        s <<
          "r" <<
          noteSoundingWholeNotesAsMsrString ();
      }

      s <<
        " (" <<
        noteDisplayPitchKindAsString () <<
        ", " <<
        noteSoundingWholeNotesAsMsrString () <<
        ", octave" " "<< noteDisplayOctaveAsString () <<
        ")" <<
        ", whole notes: " <<
        fMeasureElementSoundingWholeNotes <<
        " sounding, " <<
        fNoteDisplayWholeNotes <<
        " displayed" <<
        ":" <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << ".";
      } // for
      break;

    case msrNote::kSkipNote:
      s <<
        "skipNote:" <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << ".";
      } // for
      break;

    case msrNote::kUnpitchedNote:
      s <<
        "unpitchedNote"<<
        " " <<
        noteSoundingWholeNotesAsMsrString ();

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << ".";
      } // for
      break;

    case msrNote::kRegularNote:
      s <<
        "regularNote '"<<
        notePitchAsString () <<
        /* JMI
        "', " <<
        noteDiatonicPitchKindAsString (
          noteDiatonicPitchKind (fInputLineNumber)) <<
        "', " <<
        */
        noteSoundingWholeNotesAsMsrString () <<
        "', [octave " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << ".";
      } // for
      break;

    case msrNote::kDoubleTremoloMemberNote:
      s <<
        "doubleTremoloMemberNote '"<<
        notePitchAsString () <<
        noteSoundingWholeNotesAsMsrString () <<
        "' [octave" " " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
      break;

    case msrNote::kGraceNote:
      s <<
        "graceNote '"<<
        notePitchAsString () <<
 // JMI       noteGraphicDurationAsMsrString () <<
        noteDisplayWholeNotesAsMsrString () <<
        "' [octave" " " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << "."; // JMI
      } // for
      break;

    case msrNote::kGraceChordMemberNote:
      s <<
        "graceChordMemberNote '"<<
        notePitchAsString () <<
//        " " <<
 // JMI       noteGraphicDurationAsMsrString () <<
        noteDisplayWholeNotesAsMsrString () <<
        "' [octave" " " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << "."; // JMI
      } // for

      s <<
        ", noteTupletFactor " << fNoteTupletFactor;
      break;

    case msrNote::kChordMemberNote:
      s <<
        "chordMemberNote '"<<
        notePitchAsString () <<
        noteSoundingWholeNotesAsMsrString () <<
 // JMI       ", " << fMeasureElementSoundingWholeNotes << " sounding whole notes, " <<
        "' [octave" " " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
      break;

    case msrNote::kTupletMemberNote:
      s <<
        "tupletMemberNote '"<<
        notePitchAsString () <<
        "' " <<
        fMeasureElementSoundingWholeNotes <<
        " sounding, " <<
        fNoteDisplayWholeNotes <<
        " displayed";
/* JMI
        notePartUpLink ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteTupletUpLink->getTupletActualNotes (),
            fNoteTupletUpLink->getTupletNormalNotes ())
            */

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << ".";
      } // for

      if (! fNoteIsARest) {
        s <<
        " [octave" " " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
      }
      break;

    case msrNote::kGraceTupletMemberNote:
      s <<
        "graceTupletMemberNote '"<<
        notePitchAsString () <<
        fMeasureElementSoundingWholeNotes <<
        " sounding, " <<
        fNoteDisplayWholeNotes <<
        " displayed";
/* JMI
        notePartUpLink ()->
          tupletSoundingWholeNotesAsMsrString (
            fInputLineNumber,
            fMeasureElementSoundingWholeNotes,
            fNoteTupletUpLink->getTupletActualNotes (),
            fNoteTupletUpLink->getTupletNormalNotes ())
            */

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << ".";
      } // for

      if (! fNoteIsARest) {
        s <<
        " [octave" " " << fNoteOctave << ", " << noteDisplayOctaveAsString () << "]";
      }

      s <<
        ", noteTupletFactor " << fNoteTupletFactor;
      break;

    case msrNote::kTupletMemberUnpitchedNote:
      s <<
        "tupletMemberUnpitchedNote "<<
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
            fNoteTupletUpLink->getTupletActualNotes (),
            fNoteTupletUpLink->getTupletNormalNotes ())
            */

      for (int i = 0; i < fNoteDotsNumber; i++) {
        s << ".";
      } // for

      s <<
        ", noteTupletFactor " << fNoteTupletFactor;
      break;
  } // switch

  s << left <<
    ", measureNumber: ";
  if (fMeasureElementMeasureNumber == K_NO_MEASURE_NUMBER) {
    s << "unknown";
  }
  else {
    s << fMeasureElementMeasureNumber;
  }

  s << left <<
    ", positionInMeasure: ";
    /* JMI
  if (fMeasureElementPositionInMeasure == K_NO_POSITION_MEASURE_NUMBER) {
    s << "unknown (" << fMeasureElementPositionInMeasure << ")";
  }
  else {
    s << fMeasureElementPositionInMeasure;
  }
  */
  s << fMeasureElementPositionInMeasure;

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
      ", " << fNoteTie->tieKindAsString ();
  }

  if (fNoteIsACueNote) {
    s <<
      ", " << "cue note";
  }

  s <<
    ", line " << fInputLineNumber <<
    " ===]";

  return s.str ();
}

void msrNote::print (ostream& os) const
{
  rational
    measureFullLength =
      fNoteMeasureUpLink
        ?
          fNoteMeasureUpLink->
            getFullMeasureWholeNotesDuration ()
        : rational (0, 1); // JMI

  // print the note itself
  os <<
    asString () <<
    endl;

  gIndenter++;

  const int fieldWidth = 34;

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
      "noteMeasureNumber is empty in note '" <<
      this->asString () <<
      "'";

// JMI     msrInternalError (
    msrInternalWarning (
      gOahOah->fInputSourceName,
      fInputLineNumber,
      s.str ());
  }
  os << endl;

  // print position in measure
  os << left <<
    setw (fieldWidth) <<
    "positionInMeasure" << " : ";
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

  // print note measure uplink
  os <<
    setw (fieldWidth) <<
    "noteMeasureUpLink" << " : ";

  if (fNoteMeasureUpLink) {
    os << endl;

    gIndenter++;

    os <<
      fNoteMeasureUpLink->asShortString () <<
      endl;

    gIndenter--;
  }
  else {
    os <<
      "none";
  }
  os << endl;

  // print note chord uplink
  os <<
    setw (fieldWidth) <<
    "noteChordUpLink" << " : ";

  if (fNoteChordUpLink) {
    os <<
      fNoteChordUpLink->asShortString ();
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

  if (fNoteGraceNotesGroupUpLink) {
    os <<
      fNoteGraceNotesGroupUpLink->asShortString ();
  }
  else {
    os <<
      " : " << "none";
  }
  os << endl;

  // print note tuplet uplink
  os <<
    setw (fieldWidth) <<
    "noteTupletUpLink" << " : ";

  if (fNoteTupletUpLink) {
    os <<
      fNoteTupletUpLink->asShortString ();
  }
  else {
    os <<
      " : " << "none";
  }
  os << endl;

  // print sounding and displayed whole notes
  switch (fNoteKind) {
    case msrNote::k_NoNoteKind:
    case msrNote::kRestNote:
    case msrNote::kSkipNote:
    case msrNote::kUnpitchedNote:
    case msrNote::kRegularNote:
    case msrNote::kDoubleTremoloMemberNote:
    case msrNote::kChordMemberNote:
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

    case msrNote::kGraceNote:
    case msrNote::kGraceChordMemberNote:
      os <<
        setw (fieldWidth) <<
        "noteDisplayWholeNotes" << " : " <<
        fNoteDisplayWholeNotes <<
        endl;
      break;

    case msrNote::kTupletMemberNote:
    case msrNote::kGraceTupletMemberNote:
    case msrNote::kTupletMemberUnpitchedNote:
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

        if (fNoteTupletUpLink) {
          os <<
            wholeNotesAsMsrString (
              fInputLineNumber,
              getNoteTupletUpLink ()->
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

      gIndenter++;
        os <<
          fNoteTupletFactor;
      gIndenter--;
      break;
    } // switch

  // print note full measure length
  // may be unknown if there is no time signature
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
    noteAccidentalKindAsString (
      fNoteAccidentalKind) <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "noteEditorialAccidentalKind" << " : " <<
    noteEditorialAccidentalKindAsString (
      fNoteEditorialAccidentalKind) <<
    endl <<
    setw (fieldWidth) <<
    "noteCautionaryAccidentalKind" << " : " <<
    noteCautionaryAccidentalKindAsString (
      fNoteCautionaryAccidentalKind) <<
    endl;

  // short cuts for efficiency
  os << left <<
    setw (fieldWidth) <<
    "noteIsARest" << " : " <<
    booleanAsString (fNoteIsARest) <<
    endl <<
    setw (fieldWidth) <<
    "noteIsUnpitched" << " : " <<
    booleanAsString (fNoteIsUnpitched) <<
    endl <<
    setw (fieldWidth) <<
    "noteIsACueNote" << " : " <<
    booleanAsString (fNoteIsACueNote) <<
    endl <<
    setw (fieldWidth) <<
    "noteIsAGraceNote" << " : " <<
    booleanAsString (fNoteIsAGraceNote) <<
    endl;

  // note redundant information (for speed)

  stringstream s;

// JMI  if (fNoteIsStemless || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteIsStemless" <<
      " : " <<
      booleanAsString (
        fNoteIsStemless) <<
      endl;
// JMI  }

 // JMI if (fNoteIsAChordsFirstMemberNote || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteIsAChordsFirstMemberNote" <<
      " : " <<
      booleanAsString (
        fNoteIsAChordsFirstMemberNote) <<
      endl;
 // JMI }

 // JMI if (fNoteIsFirstNoteInADoubleTremolo || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteIsFirstNoteInADoubleTremolo" <<
      " : " <<
      booleanAsString (
        fNoteIsFirstNoteInADoubleTremolo) <<
      endl;
// JMI  }
 // JMI if (fNoteIsSecondNoteInADoubleTremolo || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteIsSecondNoteInADoubleTremolo" <<
      " : " <<
      booleanAsString (
        fNoteIsSecondNoteInADoubleTremolo) <<
      endl;
// JMI  }

  if (fNoteTrillOrnament || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteTrillOrnament" <<
      " : " <<
      booleanAsString (
        fNoteTrillOrnament) <<
      endl;
  }

  if (fNoteDashesOrnament || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteDashesOrnament" <<
      " : " <<
      booleanAsString (
        fNoteDashesOrnament) <<
      endl;
  }

  if (fNoteDelayedTurnOrnament || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteDelayedTurnOrnament" <<
      " : " <<
      booleanAsString (
        fNoteDelayedTurnOrnament) <<
      endl;
  }
  if (fNoteDelayedInvertedTurnOrnament || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteDelayedInvertedTurnOrnament" <<
      " : " <<
      booleanAsString (
        fNoteDelayedInvertedTurnOrnament) <<
      endl;
  }

  if (fNoteWavyLineSpannerStart || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteWavyLineSpannerStart" <<
      " : " <<
      fNoteWavyLineSpannerStart->asShortString () <<
      endl;
  }
  if (fNoteWavyLineSpannerStop || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteWavyLineSpannerStop" <<
      " : " <<
      fNoteWavyLineSpannerStop->asShortString () <<
      endl;
  }

  if (fNoteIsFollowedByGraceNotesGroup || gMsrOah->fDisplayMsrDetails) {
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
    case msrNote::k_NoNoteKind:
      break;

    case msrNote::kRestNote:
      {
        os << left <<
          setw (fieldWidth) <<
          "noteSoundingWholeNotesAsMsrString" << " : \"" <<
          noteSoundingWholeNotesAsMsrString () <<
          "\"" <<
          endl;
      }
      break;

    case msrNote::kSkipNote:
      {
        os << left <<
          setw (fieldWidth) <<
          "noteSoundingWholeNotesAsMsrString" << " : \"" <<
          noteSoundingWholeNotesAsMsrString () <<
          "\"" <<
          endl;
      }
      break;

    case msrNote::kUnpitchedNote:
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

    case msrNote::kRegularNote:
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

    case msrNote::kDoubleTremoloMemberNote:
      {
        // JMI
      }
      break;

    case msrNote::kGraceNote:
    case msrNote::kGraceChordMemberNote:
      {
        os << left <<
          setw (fieldWidth) <<
          "noteGraphicDurationAsMsrString" << " : \"" <<
          noteGraphicDurationAsMsrString () <<
          "\"" <<
          endl;
      }
      break;

    case msrNote::kChordMemberNote:
      {
        // JMI
      }
      break;

    case msrNote::kTupletMemberNote:
    case msrNote::kGraceTupletMemberNote:
    case msrNote::kTupletMemberUnpitchedNote:
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

      if (fNoteTupletUpLink) {
        os <<
          "\"" <<
          wholeNotesAsMsrString (
            fInputLineNumber,
            getNoteTupletUpLink ()->
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
        "noteGraphicDurationAsMsrString" << " : \"" <<
        noteGraphicDurationAsMsrString () <<
        "\"" <<
        endl;
      break;
  } // switch

  // print the octave shift if any
  if (fNoteOctaveShift || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteOctaveShift";
    if (fNoteOctaveShift) {
      os << endl;

      gIndenter++;

      os <<
        fNoteOctaveShift;

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the stem if any
  if (fNoteStem || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteStem";
    if (fNoteStem) {
      os << endl;

      gIndenter++;

      os <<
        fNoteStem;

      gIndenter--;
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
    "noteColor" << " : " <<
    fNoteColor.asString () <<
    endl;

  // print the tie if any
  if (fNoteTie || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteTie";
    if (fNoteTie) {
      os << endl;

      gIndenter++;

      os <<
        fNoteTie;

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the beams if any
  int noteBeamsSize = fNoteBeams.size ();

  if (noteBeamsSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteBeams";
    if (fNoteBeams.size ()) {
      os << endl;

      gIndenter++;

      list<S_msrBeam>::const_iterator
        iBegin = fNoteBeams.begin (),
        iEnd   = fNoteBeams.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the articulations if any
  int noteArticulationsSize = fNoteArticulations.size ();

  if (noteArticulationsSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteArticulations";
    if (fNoteArticulations.size ()) {
      os << endl;

      gIndenter++;

      list<S_msrArticulation>::const_iterator
        iBegin = fNoteArticulations.begin (),
        iEnd   = fNoteArticulations.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the spanners if any
  int noteSpannersSize = fNoteSpanners.size ();

  if (noteSpannersSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteSpanners";
    if (fNoteSpanners.size ()) {
      os << endl;

      gIndenter++;

      list<S_msrSpanner>::const_iterator
        iBegin = fNoteSpanners.begin (),
        iEnd   = fNoteSpanners.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the technicals if any
  int noteTechnicalsSize = fNoteTechnicals.size ();

  if (noteTechnicalsSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteTechnicals";
    if (fNoteTechnicals.size ()) {
      os << endl;

      gIndenter++;

      list<S_msrTechnical>::const_iterator
        iBegin = fNoteTechnicals.begin (),
        iEnd   = fNoteTechnicals.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the technicals with integer if any
  int noteTechnicalWithIntegersSize = fNoteTechnicalWithIntegers.size ();

  if (noteTechnicalWithIntegersSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteTechnicalWithIntegers";
    if (fNoteTechnicalWithIntegers.size ()) {
      os << endl;

      gIndenter++;

      list<S_msrTechnicalWithInteger>::const_iterator
        iBegin = fNoteTechnicalWithIntegers.begin (),
        iEnd   = fNoteTechnicalWithIntegers.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the technicals with float if any
  int noteTechnicalWithFloatsSize = fNoteTechnicalWithFloats.size ();

  if (noteTechnicalWithFloatsSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteTechnicalWithFloats";
    if (fNoteTechnicalWithFloats.size ()) {
      os << endl;

      gIndenter++;

      list<S_msrTechnicalWithFloat>::const_iterator
        iBegin = fNoteTechnicalWithFloats.begin (),
        iEnd   = fNoteTechnicalWithFloats.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the technicals with string if any
  int noteTechnicalWithStringsSize = fNoteTechnicalWithStrings.size ();

  if (noteTechnicalWithStringsSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteTechnicalWithStrings";
    if (fNoteTechnicalWithStrings.size ()) {
      os << endl;

      gIndenter++;

      list<S_msrTechnicalWithString>::const_iterator
        iBegin = fNoteTechnicalWithStrings.begin (),
        iEnd   = fNoteTechnicalWithStrings.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the ornaments if any
  int noteOrnamentsSize = fNoteOrnaments.size ();

  if (noteOrnamentsSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteOrnaments";
    if (fNoteOrnaments.size ()) {
      os << endl;

      gIndenter++;

      list<S_msrOrnament>::const_iterator
        iBegin = fNoteOrnaments.begin (),
        iEnd   = fNoteOrnaments.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the glissandos if any
  int noteGlissandosSize = fNoteGlissandos.size ();

  if (noteGlissandosSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteGlissandos";
    if (fNoteGlissandos.size ()) {
      os << endl;

      gIndenter++;

      list<S_msrGlissando>::const_iterator
        iBegin = fNoteGlissandos.begin (),
        iEnd   = fNoteGlissandos.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the slides if any
  int noteSlidesSize = fNoteSlides.size ();

  if (noteSlidesSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteSlides";
    if (fNoteSlides.size ()) {
      os << endl;

      gIndenter++;

      list<S_msrSlide>::const_iterator
        iBegin = fNoteSlides.begin (),
        iEnd   = fNoteSlides.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the grace notes group before if any
  if (fNoteGraceNotesGroupBefore || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteGraceNotesGroupBefore";
    if (fNoteGraceNotesGroupBefore) {
      os << endl;

      gIndenter++;

      os << fNoteGraceNotesGroupBefore;

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the after grace group notes after if any
  if (fNoteGraceNotesGroupAfter || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteGraceNotesGroupAfter";
    if (fNoteGraceNotesGroupAfter) {
      os << endl;

      gIndenter++;

      os << fNoteGraceNotesGroupAfter;

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the singleTremolo if any
  if (fNoteSingleTremolo || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteSingleTremolo";
    if (fNoteSingleTremolo) {
      os << endl;

      gIndenter++;

      os << fNoteSingleTremolo;

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the dynamics if any
  int noteDynamicsSize = fNoteDynamics.size ();

  if (noteDynamicsSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteDynamics";
    if (fNoteDynamics.size ()) {
      os << endl;

      gIndenter++;

      list<S_msrDynamics>::const_iterator
        iBegin = fNoteDynamics.begin (),
        iEnd   = fNoteDynamics.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the other dynamics if any
  int noteOtherDynamicsSize = fNoteOtherDynamics.size ();

  if (noteOtherDynamicsSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteOtherDynamics";
    if (fNoteOtherDynamics.size ()) {
      os << endl;

      gIndenter++;

      list<S_msrOtherDynamics>::const_iterator
        iBegin = fNoteOtherDynamics.begin (),
        iEnd   = fNoteOtherDynamics.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the words if any
  int noteWordsSize = fNoteWords.size ();

  if (noteWordsSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteWords";
    if (fNoteWords.size ()) {
      os << endl;
      gIndenter++;

      list<S_msrWords>::const_iterator
        iBegin = fNoteWords.begin (),
        iEnd   = fNoteWords.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the slurs if any
  int noteSlursSize = fNoteSlurs.size ();

  if (noteSlursSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteSlurs";
    if (fNoteSlurs.size ()) {
      os << endl;

      gIndenter++;

      list<S_msrSlur>::const_iterator
        iBegin = fNoteSlurs.begin (),
        iEnd   = fNoteSlurs.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the ligatures if any
  int noteLigaturesSize = fNoteLigatures.size ();

  if (noteLigaturesSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteLigatures";
    if (fNoteLigatures.size ()) {
      os << endl;

      gIndenter++;

      list<S_msrLigature>::const_iterator
        iBegin = fNoteLigatures.begin (),
        iEnd   = fNoteLigatures.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the pedals if any
  int notePedalsSize = fNotePedals.size ();

  if (notePedalsSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "notePedals";
    if (fNotePedals.size ()) {
      os << endl;

      gIndenter++;

      list<S_msrPedal>::const_iterator
        iBegin = fNotePedals.begin (),
        iEnd   = fNotePedals.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the slashes if any
  int noteSlashesSize = fNoteSlashes.size ();

  if (noteSlashesSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteSlashes";
    if (fNoteSlashes.size ()) {
      os << endl;

      gIndenter++;

      list<S_msrSlash>::const_iterator
        iBegin = fNoteSlashes.begin (),
        iEnd   = fNoteSlashes.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
      }

  // print the wedges if any
  int noteWedgesSize = fNoteWedges.size ();

  if (noteWedgesSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteWedges";
    if (fNoteWedges.size ()) {
      os << endl;

      gIndenter++;

      list<S_msrWedge>::const_iterator
        iBegin = fNoteWedges.begin (),
        iEnd   = fNoteWedges.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the segnos if any
  int noteSegnosSize = fNoteSegnos.size ();

  if (noteSegnosSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteSegnos";
    if (fNoteSegnos.size ()) {
      os << endl;

      gIndenter++;

      list<S_msrSegno>::const_iterator
        iBegin = fNoteSegnos.begin (),
        iEnd   = fNoteSegnos.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the dal segnos if any
  int noteDalSegnosSize = fNoteDalSegnos.size ();

  if (noteDalSegnosSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteDalSegnos";
    if (fNoteDalSegnos.size ()) {
      os << endl;

      gIndenter++;

      list<S_msrDalSegno>::const_iterator
        iBegin = fNoteDalSegnos.begin (),
        iEnd   = fNoteDalSegnos.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the codas if any
  int noteCodasSize = fNoteCodas.size ();

  if (noteCodasSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteCodas";
    if (fNoteCodas.size ()) {
      os << endl;

      gIndenter++;

      list<S_msrCoda>::const_iterator
        iBegin = fNoteCodas.begin (),
        iEnd   = fNoteCodas.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the eyeglasses if any
  int noteEyeGlassesSize = fNoteEyeGlasses.size ();

  if (noteEyeGlassesSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteEyeGlasses";
    if (fNoteEyeGlasses.size ()) {
      os << endl;

      gIndenter++;

      list<S_msrEyeGlasses>::const_iterator
        iBegin = fNoteEyeGlasses.begin (),
        iEnd   = fNoteEyeGlasses.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the damps if any
  int noteDampsSize = fNoteDamps.size ();

  if (noteDampsSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteDamps";
    if (fNoteDamps.size ()) {
      os << endl;

      gIndenter++;

      list<S_msrDamp>::const_iterator
        iBegin = fNoteDamps.begin (),
        iEnd   = fNoteDamps.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the dampAlls if any
  int noteDampAllsSize = fNoteDampAlls.size ();

  if (noteDampAllsSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteDampAlls";
    if (fNoteDampAlls.size ()) {
      os << endl;

      gIndenter++;

      list<S_msrDampAll>::const_iterator
        iBegin = fNoteDampAlls.begin (),
        iEnd   = fNoteDampAlls.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the scordaturas if any
  int noteScordaturasSize = fNoteScordaturas.size ();

  if (noteScordaturasSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteScordaturas";
    if (fNoteScordaturas.size ()) {
      os << endl;

      gIndenter++;

      list<S_msrScordatura>::const_iterator
        iBegin = fNoteScordaturas.begin (),
        iEnd   = fNoteScordaturas.end (),
        i      = iBegin;
      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        // no endl here;
      } // for

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the harmonies associated to this note if any
  int noteHarmoniesListSize = fNoteHarmoniesList.size ();

  if (noteHarmoniesListSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteHarmonies";
    if (noteHarmoniesListSize) {
      os << endl;

      gIndenter++;

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

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the figured basses associated to this note if any
  int noteFiguredBassesListSize = fNoteFiguredBassesList.size ();

  if (noteFiguredBassesListSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteFiguredBasses";
    if (noteFiguredBassesListSize) {
      os << endl;

      gIndenter++;

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

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  // print the syllables associated to this note if any
  int noteSyllablesSize = fNoteSyllables.size ();

  if (noteSyllablesSize > 0 || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "noteSyllables";
    if (noteSyllablesSize) {
      os << endl;

      gIndenter++;

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
          ", " <<
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
              asShortString () const;
  */

        if (++i == iEnd) break;
        // no endl here
      } // for

      gIndenter--;
    }
    else {
      os << " : " <<
        "none" <<
        endl;
    }
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrNote& elt)
{
  elt->print (os);
  return os;
}


}
