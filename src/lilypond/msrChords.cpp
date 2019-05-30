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

#include "generalOptions.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "msrOptions.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrChord msrChord::create (
  int             inputLineNumber,
  rational        chordSoundingWholeNotes,
  rational        chordDisplayWholeNotes,
  msrDurationKind chordGraphicDurationKind)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Creating a chord" <<
      ", chordSoundingWholeNotes = " << chordSoundingWholeNotes <<
      ", chordDisplayWholeNotes = " << chordDisplayWholeNotes <<
      ", chordGraphicDuration = " <<
      msrDurationKindAsString (chordGraphicDurationKind) <<
      endl;
 }
#endif

  msrChord* o =
    new msrChord (
      inputLineNumber,
      chordSoundingWholeNotes, chordDisplayWholeNotes,
      chordGraphicDurationKind);
  assert(o!=0);

  return o;
}

msrChord::msrChord (
  int             inputLineNumber,
  rational        chordSoundingWholeNotes,
  rational        chordDisplayWholeNotes,
  msrDurationKind chordGraphicDurationKind)
    : msrTupletElement (inputLineNumber)
{
  fChordSoundingWholeNotes = chordSoundingWholeNotes;
  fChordDisplayWholeNotes  = chordDisplayWholeNotes;

  fChordGraphicDurationKind = chordGraphicDurationKind;

  fChordIsFirstChordInADoubleTremolo  = false;
  fChordIsSecondChordInADoubleTremolo = false;
}

msrChord::~msrChord ()
{}

S_msrChord msrChord::createChordNewbornClone (
  S_msrPart containingPart)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Creating a newborn clone of chord '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingPart != nullptr,
    "containingPart is null");

  S_msrChord
    newbornClone =
      msrChord::create (
        fInputLineNumber,
        fChordSoundingWholeNotes,
        fChordDisplayWholeNotes,
        fChordGraphicDurationKind);

  newbornClone->
    fChordPositionInMeasure =
      fChordPositionInMeasure;

  newbornClone->
    fChordIsFirstChordInADoubleTremolo =
      fChordIsFirstChordInADoubleTremolo;

  newbornClone->
    fChordIsSecondChordInADoubleTremolo =
      fChordIsSecondChordInADoubleTremolo;

  return newbornClone;
}

void msrChord::setChordSoundingWholeNotes (
  rational wholeNotes)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Setting chord sounding whole notes to '" <<
      wholeNotes <<
      "' for chord '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  fChordSoundingWholeNotes = wholeNotes;
}

void msrChord::setChordDisplayWholeNotes (
  rational wholeNotes)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Setting chord displayed whole notes to '" <<
      wholeNotes <<
      "' for chord '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  fChordDisplayWholeNotes = wholeNotes;
}

void msrChord::addFirstNoteToChord (
  S_msrNote  note,
  S_msrVoice voice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Adding first note '" <<
      note->asString () <<
      "' to chord '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // append note to chord notes
  fChordNotesVector.push_back (note);

  // register note's chord uplink
  note->
    setNoteChordUplink (this);

  // mark note as belonging to a chord
  note->setNoteBelongsToAChord ();

  // mark note as being the first one in the chord
  note->setNoteIsAChordsFirstMemberNote ();

  // is this note the shortest one in this voice?
  voice->
    registerShortestNoteIfRelevant (note);

  // register note as the last appended one into this voice
  /* JMI
  voice->
    registerNoteAsVoiceLastAppendedNote (note);
    */

  // populate note's position in measure // JMI
  note->
    setNotePositionInMeasure (
      fChordPositionInMeasure);
}

void msrChord::addAnotherNoteToChord (
  S_msrNote  note,
  S_msrVoice voice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Adding another note '" <<
      note->asShortString () <<
      "' to chord '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  fChordNotesVector.push_back (note);

  // register note's chord uplink
  note->
    setNoteChordUplink (this);

  // mark note as belonging to a chord
  note->setNoteBelongsToAChord ();

/* JMI
  // register note as the last appended one into this voice
  voice->
    registerNoteAsVoiceLastAppendedNote (note);
    */

  // populate note's position in measure
  note->setNotePositionInMeasure (
    fChordPositionInMeasure);
}

void msrChord::setChordFirstNotePositionInMeasure (
  rational positionInMeasure)
{
  if (fChordNotesVector.size ()) { // JMI
    fChordNotesVector.front ()->
      setNotePositionInMeasure (positionInMeasure);
  }
  else {
    // exit (44); JMI
 }
}

S_msrNote msrChord::fetchChordFirstNonGraceNote () const
{
  S_msrNote result;

  if (fChordNotesVector.size ()) {
    result = fChordNotesVector.front ();
  }

  else {
    msrInternalError (
      gGeneralOptions->fInputSourceName,
      fInputLineNumber,
      __FILE__, __LINE__,
      "cannot access the first note of an empty chord");
  }

  return result;
}

void msrChord::appendArticulationToChord (S_msrArticulation art)
{
  msrArticulation::msrArticulationKind
    articulationKind =
      art->
        getArticulationKind ();

  // don't append the same articulation several times
  for (
    list<S_msrArticulation>::const_iterator i = fChordArticulations.begin ();
    i!=fChordArticulations.end ();
    i++
  ) {
      if ((*i)->getArticulationKind () == articulationKind)
        return;
  } // for

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceArticulations || gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Appending articulation '" <<
      art->articulationKindAsString () <<
      "' to chord" <<
      endl;
  }
#endif

  fChordArticulations.push_back (art);
}

void msrChord::appendSpannerToChord (S_msrSpanner span)
{
  msrSpanner::msrSpannerKind
    spannerKind =
      span->
        getSpannerKind ();

  // don't append the same spanner several times
  for (
    list<S_msrSpanner>::const_iterator i = fChordSpanners.begin ();
    i!=fChordSpanners.end ();
    i++
  ) {
      if ((*i)->getSpannerKind () == spannerKind)
        return;
  } // for

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceSpanners || gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Appending spanner '" <<
      span->spannerKindAsString () <<
      "' to chord '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  fChordSpanners.push_back (span);
}

void msrChord::setChordSingleTremolo (S_msrSingleTremolo trem)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTremolos || gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Adding singleTremolo '" <<
      trem->asString () <<
      "' to chord" <<
      endl;
  }
#endif

  fChordSingleTremolo = trem;
}

void msrChord::appendTechnicalToChord (S_msrTechnical tech)
{
  msrTechnical::msrTechnicalKind
    technicalKind =
      tech->
        getTechnicalKind ();

  // don't append the same technical several times
  for (
    list<S_msrTechnical>::const_iterator i =
      fChordTechnicals.begin ();
    i!=fChordTechnicals.end ();
    i++
  ) {
      if ((*i)->getTechnicalKind () == technicalKind)
        return;
  } // for

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Appending technical '" <<
      tech->asString () <<
      "' to chord" <<
      endl;
  }
#endif

  fChordTechnicals.push_back (tech);
}

void msrChord::appendTechnicalWithIntegerToChord (
  S_msrTechnicalWithInteger tech)
{
  msrTechnicalWithInteger::msrTechnicalWithIntegerKind
    technicalWithIntegerKind =
      tech->
        getTechnicalWithIntegerKind ();

  // don't append the same technical several times
  for (
    list<S_msrTechnicalWithInteger>::const_iterator i =
      fChordTechnicalWithIntegers.begin ();
    i!=fChordTechnicalWithIntegers.end ();
    i++
  ) {
      if ((*i)->getTechnicalWithIntegerKind () == technicalWithIntegerKind)
        return;
  } // for

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Appending technical with integer '" <<
      tech->asString () <<
      "' to chord" <<
      endl;
  }
#endif

  fChordTechnicalWithIntegers.push_back (tech);
}

void msrChord::appendTechnicalWithFloatToChord (
  S_msrTechnicalWithFloat tech)
{
  msrTechnicalWithFloat::msrTechnicalWithFloatKind
    technicalWithFloatKind =
      tech->
        getTechnicalWithFloatKind ();

  // don't append the same technical several times
  for (
    list<S_msrTechnicalWithFloat>::const_iterator i =
      fChordTechnicalWithFloats.begin ();
    i!=fChordTechnicalWithFloats.end ();
    i++
  ) {
      if ((*i)->getTechnicalWithFloatKind () == technicalWithFloatKind)
        return;
  } // for

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Appending technical with integer '" <<
      tech->asString () <<
      "' to chord" <<
      endl;
  }
#endif

  fChordTechnicalWithFloats.push_back (tech);
}

void msrChord::appendTechnicalWithStringToChord (
  S_msrTechnicalWithString tech)
{
  msrTechnicalWithString::msrTechnicalWithStringKind
    technicalWithStringKind =
      tech->
        getTechnicalWithStringKind ();

  // don't append the same technical several times
  for (
    list<S_msrTechnicalWithString>::const_iterator i =
      fChordTechnicalWithStrings.begin ();
    i!=fChordTechnicalWithStrings.end ();
    i++
  ) {
      if ((*i)->getTechnicalWithStringKind () == technicalWithStringKind)
        return;
  } // for

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Appending technical with string '" <<
      tech->asString () <<
      "' to chord" <<
      endl;
  }
#endif

  fChordTechnicalWithStrings.push_back (tech);
}

void msrChord::appendOrnamentToChord (S_msrOrnament orn)
{
  msrOrnament::msrOrnamentKind
    ornamentKind =
      orn->
        getOrnamentKind ();

  // don't append the same ornament several times
  for (
    list<S_msrOrnament>::const_iterator i = fChordOrnaments.begin ();
    i!=fChordOrnaments.end ();
    i++
  ) {
      if ((*i)->getOrnamentKind () == ornamentKind)
        return;
  } // for

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Appending ornament '" <<
      orn->ornamentKindAsString () <<
      "' to chord" <<
      endl;
  }
#endif

  fChordOrnaments.push_back (orn);
}

void msrChord::appendGlissandoToChord (S_msrGlissando gliss)
{
  msrGlissando::msrGlissandoTypeKind
    glissandoTypeKind =
      gliss->
        getGlissandoTypeKind ();

  // don't append the same slissando several times
  for (
    list<S_msrGlissando>::const_iterator i = fChordGlissandos.begin ();
    i!=fChordGlissandos.end ();
    i++
  ) {
      if ((*i)->getGlissandoTypeKind () == glissandoTypeKind)
        return;
  } // for

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Appending glissando '" <<
      gliss->asString () <<
      "' to chord" <<
      endl;
  }
#endif

  fChordGlissandos.push_back (gliss);
}

void msrChord::appendSlideToChord (S_msrSlide slide)
{
  msrSlide::msrSlideTypeKind
    slideTypeKind =
      slide->
        getSlideTypeKind ();

  // don't append the same slide several times
  for (
    list<S_msrSlide>::const_iterator i = fChordSlides.begin ();
    i!=fChordSlides.end ();
    i++
  ) {
      if ((*i)->getSlideTypeKind () == slideTypeKind)
        return;
  } // for

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Appending slide '" <<
      slide->asString () <<
      "' to chord" <<
      endl;
  }
#endif

  fChordSlides.push_back (slide);
}

void msrChord::appendBeamToChord (S_msrBeam beam)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceBeams || gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Appending beam '" <<
      beam->asString () <<
      "' to chord" <<
      endl;
  }
#endif

  fChordBeams.push_back (beam);
}

void msrChord::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrChord::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrChord>*
    p =
      dynamic_cast<visitor<S_msrChord>*> (v)) {
        S_msrChord elem = this;

        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrChord::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrChord::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrChord::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrChord>*
    p =
      dynamic_cast<visitor<S_msrChord>*> (v)) {
        S_msrChord elem = this;

        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrChord::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrChord::browseData (basevisitor* v)
{
  for (
    vector<S_msrNote>::const_iterator i = fChordNotesVector.begin ();
    i != fChordNotesVector.end ();
    i++ ) {
    // browse chord note
    msrBrowser<msrNote> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrArticulation>::const_iterator i = fChordArticulations.begin ();
    i != fChordArticulations.end ();
    i++ ) {
    // browse the articulation
    msrBrowser<msrArticulation> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrSpanner>::const_iterator i = fChordSpanners.begin ();
    i != fChordSpanners.end ();
    i++ ) {
    // browse the spanner
    msrBrowser<msrSpanner> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrTechnical>::const_iterator i = fChordTechnicals.begin ();
    i != fChordTechnicals.end ();
    i++ ) {
    // browse the technical
    msrBrowser<msrTechnical> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrOrnament>::const_iterator i = fChordOrnaments.begin ();
    i != fChordOrnaments.end ();
    i++ ) {
    // browse the ornament
    msrBrowser<msrOrnament> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrGlissando>::const_iterator i = fChordGlissandos.begin ();
    i != fChordGlissandos.end ();
    i++ ) {
    // browse the glissando
    msrBrowser<msrGlissando> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrSlide>::const_iterator i = fChordSlides.begin ();
    i != fChordSlides.end ();
    i++ ) {
    // browse the slide
    msrBrowser<msrSlide> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrDynamics>::const_iterator i = fChordDynamics.begin ();
    i != fChordDynamics.end ();
    i++ ) {
    // browse the dynamics
    msrBrowser<msrDynamics> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrOtherDynamics>::const_iterator i = fChordOtherDynamics.begin ();
    i != fChordOtherDynamics.end ();
    i++ ) {
    // browse the other dynamics
    msrBrowser<msrOtherDynamics> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrWords>::const_iterator i = fChordWords.begin ();
    i != fChordWords.end ();
    i++ ) {
    // browse the words
    msrBrowser<msrWords> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrTie>::const_iterator i = fChordTies.begin ();
    i != fChordTies.end ();
    i++ ) {
    // browse the tie
    msrBrowser<msrTie> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrSlur>::const_iterator i = fChordSlurs.begin ();
    i != fChordSlurs.end ();
    i++ ) {
    // browse the slur
    msrBrowser<msrSlur> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrLigature>::const_iterator i = fChordLigatures.begin ();
    i != fChordLigatures.end ();
    i++ ) {
    // browse the ligature
    msrBrowser<msrLigature> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrSlash>::const_iterator i = fChordSlashes.begin ();
    i != fChordSlashes.end ();
    i++ ) {
    // browse the slash
    msrBrowser<msrSlash> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrWedge>::const_iterator i = fChordWedges.begin ();
    i != fChordWedges.end ();
    i++ ) {
    // browse the wedge
    msrBrowser<msrWedge> browser (v);
    browser.browse (*(*i));
  } // for

  if (fChordGraceNotesGroupBefore) {
    // browse the grace notes group before
    msrBrowser<msrGraceNotesGroup> browser (v);
    browser.browse (*fChordGraceNotesGroupBefore);
  }
  if (fChordGraceNotesGroupAfter) {
    // browse the grace notes group after
    msrBrowser<msrGraceNotesGroup> browser (v);
    browser.browse (*fChordGraceNotesGroupAfter);
  }

  if (fChordHarmony) {
    // browse the harmony
    msrBrowser<msrHarmony> browser (v);
    browser.browse (*fChordHarmony);
  }

  if (fChordFiguredBass) {
    // browse the figured bass
    msrBrowser<msrFiguredBass> browser (v);
    browser.browse (*fChordFiguredBass);
  }
}

/* JMI
void msrChord::applyTupletMemberDisplayFactorToChordMembers (
  int actualNotes, int normalNotes)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceTuplets || gTraceOptions->fTraceChords) {
    gLogIOstream <<
      "Applying tuplet member sounding factor '" <<
      actualNotes << "/" << normalNotes <<
      "' to the members of chord '" << asStringwithRawDivisions () <<
      "', line " << fInputLineNumber <<
      endl;

  for (
    vector<S_msrNote>::const_iterator i = fChordNotesVector.begin ();
    i != fChordNotesVector.end ();
    ++i) {
    (*i)->
      applyTupletMemberSoundingFactorToNote (
        actualNotes, normalNotes);
  } // for
}
#endif
*/

string msrChord::chordSoundingWholeNotesAsMsrString ()
{
  return
    wholeNotesAsMsrString (
      fInputLineNumber,
      fChordSoundingWholeNotes);
}

string msrChord::chordDisplayWholeNotesAsMsrString ()
{
  return
    wholeNotesAsMsrString (
      fInputLineNumber,
      fChordDisplayWholeNotes);
}

/* JMI
string msrChord::chordGraphicDurationAsMsrString () const
{
  string result;

  result =
    msrDurationAsString (
      fChordGraphicDuration);

  return result;
}
*/

string msrChord::asStringwithRawDivisions () const
{
  stringstream s;

  s << "<";

  if (fChordNotesVector.size ()) {
    vector<S_msrNote>::const_iterator
      iBegin = fChordNotesVector.begin (),
      iEnd   = fChordNotesVector.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrNote
        note = (*i);

      s <<
      /* JMI
        note->notePitchAsString () <<
        note->noteSoundingWholeNotesAsMsrString () <<
        "[" << note->getNoteOctave () << "]"
        */

        note->asShortStringWithRawWholeNotes ();

      if (++i == iEnd) break;
      s << " ";
    } // for
  }

  s << ">";

  return s.str ();
}

string msrChord::asString () const
{
  stringstream s;

  s << "<";

  if (fChordNotesVector.size ()) {
    vector<S_msrNote>::const_iterator
      iBegin = fChordNotesVector.begin (),
      iEnd   = fChordNotesVector.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrNote
        note = (*i);

      s <<
        note->notePitchAsString () <<
        ", whole notes: " <<
        note->getNoteSoundingWholeNotes () <<
        " sounding, " <<
        note->getNoteDisplayWholeNotes () <<
        " displayed," <<
        "[" << note->getNoteOctave () << "]";

      if (++i == iEnd) break;
      s << " ";
    } // for
  }

  s << ">";

  return s.str ();
}

string msrChord::asShortString () const
{
  stringstream s;

  s << "<";

  if (fChordNotesVector.size ()) {
    vector<S_msrNote>::const_iterator
      iBegin = fChordNotesVector.begin (),
      iEnd   = fChordNotesVector.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrNote
        note = (*i);

      s <<
        "'" <<
        note->notePitchAsString () <<
        "', whole notes: " <<
        note->getNoteSoundingWholeNotes () <<
        " sounding, " <<
        note->getNoteDisplayWholeNotes () <<
        " displayed," <<
        "[" << note->getNoteOctave () << "]";

      if (++i == iEnd) break;
      s << " ";
    } // for
  }

  s << ">";

  return s.str ();
}

void msrChord::print (ostream& os)
{
  rational
    chordMeasureFullLength =
      fChordMeasureUplink
        ?
          fChordMeasureUplink->
            getFullMeasureWholeNotes ()
        : rational (0, 1); // JMI

  os <<
    "Chord, " <<
    singularOrPlural (
      fChordNotesVector.size (), "note", "notes") <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 35;

  os << left <<
    setw (fieldWidth) <<
    "chordSoundingWholeNotes" << " : " << fChordSoundingWholeNotes <<
    endl <<
    setw (fieldWidth) <<
    "chordDisplayWholeNotes" << " : " << fChordDisplayWholeNotes <<
    endl <<
    setw (fieldWidth) <<
    "chordMeasureNumber" << " : " << fChordMeasureNumber <<
    endl <<
    setw (fieldWidth) <<
    "chordPositionInMeasure" << " : " << fChordPositionInMeasure <<
    endl <<
    setw (fieldWidth) <<
    "chordMeasureFullLength" << " : " << chordMeasureFullLength <<
    endl;

  // print simplified position in measure if relevant
// JMI  if (fChordMeasureUplink) {
    // the chord measure uplink may not have been set yet
    rational
      chordPositionBis =
        fChordPositionInMeasure;
    fChordPositionInMeasure.rationalise ();

    if (
      chordPositionBis.getNumerator ()
        !=
      fChordPositionInMeasure.getNumerator ()) {
      // print rationalized rational view
      os << left <<
        setw (fieldWidth) <<
        "fChordPositionInMeasure" << " : " << chordPositionBis <<
        endl;
    }

  os << left <<
    setw (fieldWidth) <<
    "chordIsFirstChordInADoubleTremolo" << " : " <<
    booleanAsString (fChordIsFirstChordInADoubleTremolo) <<
    endl <<
    setw (fieldWidth) <<
    "chordIsSecondChordInADoubleTremolo" << " : " <<
    booleanAsString (fChordIsSecondChordInADoubleTremolo) <<
    endl;

  // print the articulations if any
  int chordArticulationsSize = fChordArticulations.size ();

  if (chordArticulationsSize || gMsrOptions->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordArticulations";
    if (chordArticulationsSize) {
      os <<
        endl;
      gIndenter++;

      list<S_msrArticulation>::const_iterator i;
      for (i=fChordArticulations.begin (); i!=fChordArticulations.end (); i++) {
        os << (*i);
      } // for

    gIndenter--;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the spanners if any
  int chordSpannersSize = fChordSpanners.size ();

  if (chordSpannersSize || gMsrOptions->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordSpanners";
    if (chordSpannersSize) {
      os <<
        endl;
      gIndenter++;

      list<S_msrSpanner>::const_iterator i;
      for (i=fChordSpanners.begin (); i!=fChordSpanners.end (); i++) {
        os << (*i);
      } // for

    gIndenter--;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the technicals if any
  int chordTechnicalsSize = fChordTechnicals.size ();

  if (chordTechnicalsSize || gMsrOptions->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordTechnicals";
    if (chordTechnicalsSize) {
      os <<
        endl;
      gIndenter++;

      list<S_msrTechnical>::const_iterator i;
      for (i=fChordTechnicals.begin (); i!=fChordTechnicals.end (); i++) {
        os << (*i);
      } // for

    gIndenter--;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the ornaments if any
  int chordOrnamentsSize = fChordOrnaments.size ();

  if (chordOrnamentsSize || gMsrOptions->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordOrnaments";
    if (chordOrnamentsSize) {
      os <<
        endl;
      gIndenter++;

      list<S_msrOrnament>::const_iterator i;
      for (i=fChordOrnaments.begin (); i!=fChordOrnaments.end (); i++) {
        os << (*i);
      } // for

    gIndenter--;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the glissandos if any
  int chordGlissandosSize = fChordGlissandos.size ();

  if (chordGlissandosSize || gMsrOptions->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordGlissandos";
    if (chordGlissandosSize) {
      os <<
        endl;
      gIndenter++;

      list<S_msrGlissando>::const_iterator i;
      for (i=fChordGlissandos.begin (); i!=fChordGlissandos.end (); i++) {
        os << (*i);
      } // for

    gIndenter--;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the slides if any
  int chordSlidesSize = fChordSlides.size ();

  if (chordSlidesSize || gMsrOptions->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordSlides";
    if (chordSlidesSize) {
      os <<
        endl;
      gIndenter++;

      list<S_msrSlide>::const_iterator i;
      for (i=fChordSlides.begin (); i!=fChordSlides.end (); i++) {
        os << (*i);
      } // for

    gIndenter--;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the dynamics if any
  int chordDynamicsSize = fChordDynamics.size ();

  if (chordDynamicsSize || gMsrOptions->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordDynamics";
    if (chordDynamicsSize) {
      os <<
        endl;
      gIndenter++;

      list<S_msrDynamics>::const_iterator i;
      for (i=fChordDynamics.begin (); i!=fChordDynamics.end (); i++) {
        os << (*i);
      } // for

    gIndenter--;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the other dynamics if any
  int chordOtherDynamicsSize = fChordOtherDynamics.size ();

  if (chordOtherDynamicsSize || gMsrOptions->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordOtherDynamics";
    if (chordOtherDynamicsSize) {
      os <<
        endl;
      gIndenter++;

      list<S_msrOtherDynamics>::const_iterator i;
      for (i=fChordOtherDynamics.begin (); i!=fChordOtherDynamics.end (); i++) {
        os << (*i);
      } // for

    gIndenter--;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the stems if any
  int chordStemsSize = fChordStems.size ();

  if (chordStemsSize || gMsrOptions->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordStems";
    if (chordStemsSize) {
      os <<
        endl;
      gIndenter++;

      list<S_msrStem>::const_iterator i;
      for (i=fChordStems.begin (); i!=fChordStems.end (); i++) {
        os << (*i);
      } // for

    gIndenter--;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the beams if any
  int chordBeamsSize = fChordBeams.size ();

  if (chordBeamsSize || gMsrOptions->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordBeams";
    if (chordBeamsSize) {
      os <<
        endl;
      gIndenter++;

      list<S_msrBeam>::const_iterator i;
      for (i=fChordBeams.begin (); i!=fChordBeams.end (); i++) {
        os << (*i);
      } // for

    gIndenter--;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the words if any
  int chordWordsSize = fChordWords.size ();

  if (chordWordsSize || gMsrOptions->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordWords";
    if (chordWordsSize) {
      os <<
        endl;
      gIndenter++;

      list<S_msrWords>::const_iterator i;
      for (i=fChordWords.begin (); i!=fChordWords.end (); i++) {
        os << (*i);
      } // for

    gIndenter--;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the ties if any
  int chordTiesSize = fChordTies.size ();

  if (chordTiesSize || gMsrOptions->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordTies";
    if (chordTiesSize) {
      os <<
        endl;
      gIndenter++;

      list<S_msrTie>::const_iterator i;
      for (i=fChordTies.begin (); i!=fChordTies.end (); i++) {
        os << (*i);
      } // for

      gIndenter--;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the slurs if any
  int chordSlursSize = fChordSlurs.size ();

  if (chordSlursSize || gMsrOptions->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordSlurs";
    if (chordSlursSize) {
      os <<
        endl;
      gIndenter++;

      list<S_msrSlur>::const_iterator i;
      for (i=fChordSlurs.begin (); i!=fChordSlurs.end (); i++) {
        os << (*i);
      } // for

    gIndenter--;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the ligatures if any
  int chordLigaturesSize = fChordLigatures.size ();

  if (chordLigaturesSize || gMsrOptions->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordLigatures";
    if (chordLigaturesSize) {
      os <<
        endl;
      gIndenter++;

      list<S_msrLigature>::const_iterator i;
      for (i=fChordLigatures.begin (); i!=fChordLigatures.end (); i++) {
        os << (*i);
      } // for

    gIndenter--;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the slashes if any
  int chordSlashesSize = fChordSlashes.size ();

  if (chordSlashesSize || gMsrOptions->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordSlashes";
    if (chordSlashesSize) {
      os <<
        endl;
      gIndenter++;

      list<S_msrSlash>::const_iterator i;
      for (i=fChordSlashes.begin (); i!=fChordSlashes.end (); i++) {
        os << (*i);
      } // for

    gIndenter--;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the wedges if any
  int chordWedgesSize = fChordWedges.size ();

  if (chordWedgesSize || gMsrOptions->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordWedges";
    if (chordWedgesSize) {
      os <<
        endl;
      gIndenter++;

      list<S_msrWedge>::const_iterator i;
      for (i=fChordWedges.begin (); i!=fChordWedges.end (); i++) {
        os << (*i);
      } // for

    gIndenter--;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the grace notes groups if any
  if (fChordGraceNotesGroupBefore || gMsrOptions->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordGraceNotesGroupBefore" << " : " <<
      endl;

    gIndenter++;

    if (fChordGraceNotesGroupBefore) {
      os <<
        fChordGraceNotesGroupBefore->asString () <<
        endl;
    }
    else {
      os <<
        "none";
    }

    gIndenter--;
  }
  if (fChordGraceNotesGroupAfter || gMsrOptions->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordGraceNotesGroupAfter" << " : " <<
      endl;

    gIndenter++;

    if (fChordGraceNotesGroupAfter) {
      os <<
        fChordGraceNotesGroupAfter->asString () <<
        endl;
    }
    else {
      os <<
        "none";
    }

    gIndenter--;
  }

  // print the harmony if any
  if (fChordHarmony || gMsrOptions->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordHarmony" << " : " <<
      endl;

    gIndenter++;

    if (fChordHarmony) {
      os <<
        fChordHarmony->asString () <<
        endl;
    }
    else {
      os <<
        "none";
    }

    gIndenter--;
  }

  // print the figured bass if any
  if (fChordFiguredBass || gMsrOptions->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordFiguredBass" << " : " <<
      endl;

    gIndenter++;

   if (fChordFiguredBass) {
      os <<
        fChordFiguredBass->asString () <<
        endl;
    }
    else {
      os <<
        "none";
    }

    gIndenter--;
  }

  os <<
    endl;

  // print the chord notes if any
  int chordNotesVectorSize = fChordNotesVector.size ();

  if (chordNotesVectorSize || gMsrOptions->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordNotes";
    if (chordNotesVectorSize) {
      os <<
        endl;
      gIndenter++;

      vector<S_msrNote>::const_iterator
        iBegin = fChordNotesVector.begin (),
        iEnd   = fChordNotesVector.end (),
        i      = iBegin;

      for ( ; ; ) {
        os << (*i);
        if (++i == iEnd) break;
        os << endl;
      } // for

      gIndenter--;
    }
    else {
      os << ":" << "none" <<
      endl;
    }
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrChord& elt)
{
  elt->print (os);
  return os;
}


}
