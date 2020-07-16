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

#include "generalOah.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "msrOah.h"


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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceChords) {
    gLogOstream <<
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
// JMI  fChordSoundingWholeNotes = chordSoundingWholeNotes;
  fMeasureElementSoundingWholeNotes = chordSoundingWholeNotes;

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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceChords) {
    gLogOstream <<
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
  // JMI      fChordSoundingWholeNotes,
        fMeasureElementSoundingWholeNotes,
        fChordDisplayWholeNotes,
        fChordGraphicDurationKind);

/*
  newbornClone->
    fChordPositionInMeasure =
      fChordPositionInMeasure;

  newbornClone->
    fChordIsFirstChordInADoubleTremolo =
      fChordIsFirstChordInADoubleTremolo;

  newbornClone->
    fChordIsSecondChordInADoubleTremolo =
      fChordIsSecondChordInADoubleTremolo;
*/

  return newbornClone;
}

void msrChord::setChordSoundingWholeNotes (
  rational wholeNotes)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceChords) {
    gLogOstream <<
      "Setting chord sounding whole notes to '" <<
      wholeNotes <<
      "' for chord '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

// JMI  fChordSoundingWholeNotes = wholeNotes;
  fMeasureElementSoundingWholeNotes = wholeNotes;
}

void msrChord::setChordDisplayWholeNotes (
  rational wholeNotes)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceChords) {
    gLogOstream <<
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

void msrChord::setChordMembersPositionInMeasure (
  S_msrMeasure measure,
  rational     positionInMeasure)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTracePositionsInMeasures) {
    gLogOstream <<
      "Setting chord members position in measure of " << asString () <<
      " to '" <<
      positionInMeasure <<
      "'" <<
      endl;
  }
#endif

  string context =
    "setChordMembersPositionInMeasure()";

  setMeasureElementPositionInMeasure (
    positionInMeasure,
    context);

  // compute chord's position in voice
  rational
     positionInVoice =
      fChordMeasureUpLink->getMeasurePositionInVoice ()
        +
      positionInMeasure;

  // set chord's position in voice
  setMeasureElementPositionInVoice (
    positionInVoice,
    context);

  // update current position in voice
  S_msrVoice
    voice =
      measure->
        fetchMeasureVoiceUpLink ();

  voice->
    incrementCurrentPositionInVoice (
      fChordNotesVector [0]->getNoteSoundingWholeNotes ());

  // set the chord's elements' position in measure
  if (fChordNotesVector.size ()) {
    vector<S_msrNote>::const_iterator
      iBegin = fChordNotesVector.begin (),
      iEnd   = fChordNotesVector.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrNote
        note = (*i);

      // set note's measure uplink
      note->
        setNoteMeasureUpLink (
          measure);

      // set note's position in measure
      note->
        setNotePositionInMeasure (
          positionInMeasure); // they all share the same one

      // set note's position in voice
      note->
        setMeasureElementPositionInVoice (
          positionInVoice,
          context); // they all share the same one

      if (++i == iEnd) break;
    } // for
  }

  // are there dal segnos attached to this chord?
  if (fChordDalSegnos.size ()) {
    list<S_msrDalSegno>::const_iterator i;
    for (
      i=fChordDalSegnos.begin (); i!=fChordDalSegnos.end (); i++
    ) {
      // set the dal segno position in measure
      (*i)->
        setDalSegnoPositionInMeasure (
          positionInMeasure);
    } // for
  }
}

void msrChord::addFirstNoteToChord (
  S_msrNote  note,
  S_msrVoice voice)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceChords) {
    gLogOstream <<
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

  // register note's chord upLink
  note->
    setNoteChordUpLink (this);

/* JMI too early
  // register note's measure upLink
  note->
    setNoteMeasureUpLink (fChordMeasureUpLink);
*/

  // mark note as belonging to a chord
  note->setNoteBelongsToAChord ();

  // mark note as being the first one in the chord
  note->setNoteIsAChordsFirstMemberNote ();

  // is this note the shortest one in this voice?
  voice->
    registerShortestNoteInVoiceIfRelevant (note);

  // register note as the last appended one into this voice
  /* JMI
  voice->
    registerNoteAsVoiceLastAppendedNote (note);
    */
}

void msrChord::addAnotherNoteToChord (
  S_msrNote  note,
  S_msrVoice voice)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceChords) {
    gLogOstream <<
      "Adding another note '" <<
      note->asShortString () <<
      "' to chord '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  fChordNotesVector.push_back (note);

  // register note's chord upLink
  note->
    setNoteChordUpLink (this);

  // mark note as belonging to a chord
  note->setNoteBelongsToAChord ();

/* JMI
  // register note as the last appended one into this voice
  voice->
    registerNoteAsVoiceLastAppendedNote (note);
    */
}

S_msrNote msrChord::fetchChordFirstNonGraceNote () const
{
  S_msrNote result;

  if (fChordNotesVector.size ()) {
    result = fChordNotesVector.front ();
  }

  else {
    msrInternalError (
      gOahOah->fInputSourceName,
      fInputLineNumber,
      __FILE__, __LINE__,
      "cannot access the first note of an empty chord");
  }

  return result;
}

void msrChord::appendDalSegnoToChord (S_msrDalSegno dalSegno)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceDalSegnos) {
    gLogOstream <<
      "Appending dal segno " <<
      dalSegno->asShortString () <<
      " to chord " <<
      asShortString () <<
      endl;
  }
#endif

  fChordDalSegnos.push_back (dalSegno);
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceArticulations) {
    gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceSpanners) {
    gLogOstream <<
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTremolos) {
    gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceChords) {
    gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceChords) {
    gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceChords) {
    gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceChords) {
    gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceChords) {
    gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceChords) {
    gLogOstream <<
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

#ifdef TRACE_OAH
  if (gTraceOah->fTraceChords) {
    gLogOstream <<
      "Appending slide '" <<
      slide->asString () <<
      "' to chord" <<
      endl;
  }
#endif

  fChordSlides.push_back (slide);
}

void msrChord::appendStemToChord (S_msrStem stem)
{
  // sanity check
  for (
    list<S_msrStem>::const_iterator i = fChordStems.begin ();
    i != fChordStems.end ();
    i++
  ) {
    // is stem's stem kind consistent with the other ones' for this chord?
    if (stem->getStemKind () != (*i)->getStemKind ()) {
      stringstream s;

      s <<
        "stem " << stem->asString () <<
        " and " << (*i)->asString () <<
        " don't have the same kind, but are in one and the same chord";

//      msrInternalError ( // not internal actually JMI ???
      msrInternalWarning ( // not internal actually JMI ???
        gOahOah->fInputSourceName,
        fInputLineNumber,
  //      __FILE__, __LINE__,
        s.str ());
    }
  } // for

  fChordStems.push_back (stem);
}

void msrChord::appendBeamToChord (S_msrBeam beam)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceBeams) {
    gLogOstream <<
      "Appending beam '" <<
      beam->asString () <<
      "' to chord" <<
      endl;
  }
#endif

  fChordBeams.push_back (beam);
}

void msrChord::finalizeChord (
  int inputLineNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceChords) {
    gLogOstream <<
      "Finalizing chord '" <<
      asString () <<
      "', line " << inputLineNumber <<
      endl <<
      "fMeasureElementPositionInMeasure = " <<
      endl <<
      fMeasureElementPositionInMeasure <<
      endl;
  }
#endif

  // we can now set the position in measures for all the chord members
  setChordMembersPositionInMeasure (
    fChordMeasureUpLink,
    fMeasureElementPositionInMeasure);
}

void msrChord::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrChord::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrChord>*
    p =
      dynamic_cast<visitor<S_msrChord>*> (v)) {
        S_msrChord elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrChord::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrChord::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrChord::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrChord>*
    p =
      dynamic_cast<visitor<S_msrChord>*> (v)) {
        S_msrChord elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
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
    i++
  ) {
    // browse chord note
    msrBrowser<msrNote> browser (v);
    browser.browse (*(*i));
  } // for


  for (
    list<S_msrStem>::const_iterator i = fChordStems.begin ();
    i != fChordStems.end ();
    i++
  ) {
    // browse chord stems
    msrBrowser<msrStem> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrBeam>::const_iterator i = fChordBeams.begin ();
    i != fChordBeams.end ();
    i++
  ) {
    // browse chord beams
    msrBrowser<msrBeam> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrArticulation>::const_iterator i = fChordArticulations.begin ();
    i != fChordArticulations.end ();
    i++
  ) {
    // browse the articulation
    msrBrowser<msrArticulation> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrSpanner>::const_iterator i = fChordSpanners.begin ();
    i != fChordSpanners.end ();
    i++
  ) {
    // browse the spanner
    msrBrowser<msrSpanner> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrTechnical>::const_iterator i = fChordTechnicals.begin ();
    i != fChordTechnicals.end ();
    i++
  ) {
    // browse the technical
    msrBrowser<msrTechnical> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrOrnament>::const_iterator i = fChordOrnaments.begin ();
    i != fChordOrnaments.end ();
    i++
  ) {
    // browse the ornament
    msrBrowser<msrOrnament> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrGlissando>::const_iterator i = fChordGlissandos.begin ();
    i != fChordGlissandos.end ();
    i++
  ) {
    // browse the glissando
    msrBrowser<msrGlissando> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrSlide>::const_iterator i = fChordSlides.begin ();
    i != fChordSlides.end ();
    i++
  ) {
    // browse the slide
    msrBrowser<msrSlide> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrDynamics>::const_iterator i = fChordDynamics.begin ();
    i != fChordDynamics.end ();
    i++
  ) {
    // browse the dynamics
    msrBrowser<msrDynamics> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrOtherDynamics>::const_iterator i = fChordOtherDynamics.begin ();
    i != fChordOtherDynamics.end ();
    i++
  ) {
    // browse the other dynamics
    msrBrowser<msrOtherDynamics> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrWords>::const_iterator i = fChordWords.begin ();
    i != fChordWords.end ();
    i++
  ) {
    // browse the words
    msrBrowser<msrWords> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrTie>::const_iterator i = fChordTies.begin ();
    i != fChordTies.end ();
    i++
  ) {
    // browse the tie
    msrBrowser<msrTie> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrSlur>::const_iterator i = fChordSlurs.begin ();
    i != fChordSlurs.end ();
    i++
  ) {
    // browse the slur
    msrBrowser<msrSlur> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrLigature>::const_iterator i = fChordLigatures.begin ();
    i != fChordLigatures.end ();
    i++
  ) {
    // browse the ligature
    msrBrowser<msrLigature> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrSlash>::const_iterator i = fChordSlashes.begin ();
    i != fChordSlashes.end ();
    i++
  ) {
    // browse the slash
    msrBrowser<msrSlash> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrWedge>::const_iterator i = fChordWedges.begin ();
    i != fChordWedges.end ();
    i++
  ) {
    // browse the wedge
    msrBrowser<msrWedge> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrSegno>::const_iterator i = fChordSegnos.begin ();
    i != fChordSegnos.end ();
    i++
  ) {
    // browse the segno
    msrBrowser<msrSegno> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrDalSegno>::const_iterator i = fChordDalSegnos.begin ();
    i != fChordDalSegnos.end ();
    i++
  ) {
    // browse the dal segno
    msrBrowser<msrDalSegno> browser (v);
    browser.browse (*(*i));
  } // for

  for (
    list<S_msrCoda>::const_iterator i = fChordCodas.begin ();
    i != fChordCodas.end ();
    i++
  ) {
    // browse the coda
    msrBrowser<msrCoda> browser (v);
    browser.browse (*(*i));
  } // for

  if (fChordOctaveShift) {
    // browse the octave shift
    msrBrowser<msrOctaveShift> browser (v);
    browser.browse (*fChordOctaveShift);
  }

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

  // browse the harmonies if any
  if (fChordHarmoniesList.size ()) {
    list<S_msrHarmony>::const_iterator i;
    for (i=fChordHarmoniesList.begin (); i!=fChordHarmoniesList.end (); i++) {
      // browse the harmony
      msrBrowser<msrHarmony> browser (v);
      browser.browse (*(*i));
    } // for
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
#ifdef TRACE_OAH
  if (gTraceOah->fTraceTuplets) {
    gLogOstream <<
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

string msrChord::asStringwithRawDivisions () const
{
  stringstream s;

  s << "[<";

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

  s << ">]";

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

void msrChord::print (ostream& os) const
{
  rational
    chordMeasureFullLength =
      fChordMeasureUpLink
        ?
          fChordMeasureUpLink->
            getFullMeasureWholeNotesDuration ()
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
 // JMI   "chordSoundingWholeNotes" << " : " << fChordSoundingWholeNotes <<
    "chordSoundingWholeNotes" << " : " << fMeasureElementSoundingWholeNotes <<
    endl <<
    setw (fieldWidth) <<
    "chordDisplayWholeNotes" << " : " << fChordDisplayWholeNotes <<
    endl <<
    setw (fieldWidth) <<
    "measureNumber" << " : " << fMeasureElementMeasureNumber <<
    endl <<
    setw (fieldWidth) <<
    "positionInMeasure" << " : " << fMeasureElementPositionInMeasure <<
    endl <<
    setw (fieldWidth) <<
    "positionInVoice" << " : " << fMeasureElementPositionInVoice <<
    endl <<
    setw (fieldWidth) <<
    "chordMeasureFullLength" << " : " << chordMeasureFullLength <<
    endl;

  // print simplified position in measure if relevant
// JMI  if (fChordMeasureUpLink) {
    // the chord measure upLink may not have been set yet
    rational
      chordPositionBis =
        fMeasureElementPositionInMeasure;
    chordPositionBis.rationalise ();

    if (
      chordPositionBis.getNumerator ()
        !=
      fMeasureElementPositionInMeasure.getNumerator ()
    ) {
      // print rationalized rational view
      os << left <<
        setw (fieldWidth) <<
        "chordPositionBis" << " : " << chordPositionBis <<
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

  if (chordArticulationsSize || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordArticulations";
    if (chordArticulationsSize) {
      os << endl;
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

  if (chordSpannersSize || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordSpanners";
    if (chordSpannersSize) {
      os << endl;
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

  if (chordTechnicalsSize || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordTechnicals";
    if (chordTechnicalsSize) {
      os << endl;
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

  if (chordOrnamentsSize || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordOrnaments";
    if (chordOrnamentsSize) {
      os << endl;
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

  if (chordGlissandosSize || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordGlissandos";
    if (chordGlissandosSize) {
      os << endl;
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

  if (chordSlidesSize || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordSlides";
    if (chordSlidesSize) {
      os << endl;
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

  if (chordDynamicsSize || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordDynamics";
    if (chordDynamicsSize) {
      os << endl;
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

  if (chordOtherDynamicsSize || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordOtherDynamics";
    if (chordOtherDynamicsSize) {
      os << endl;
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

  if (chordStemsSize || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordStems";
    if (chordStemsSize) {
      os << endl;
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

  if (
    gTraceOah->fTraceBeams
      ||
    chordBeamsSize || gMsrOah->fDisplayMsrDetails
  ) {
    os <<
      setw (fieldWidth) <<
      "chordBeams";
    if (chordBeamsSize) {
      os << endl;
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

  if (
    gTraceOah->fTraceWords
      ||
    chordWordsSize || gMsrOah->fDisplayMsrDetails
  ) {
    os <<
      setw (fieldWidth) <<
      "chordWords";
    if (chordWordsSize) {
      os << endl;
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

  if (chordTiesSize || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordTies";
    if (chordTiesSize) {
      os << endl;
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

  if (
    gTraceOah->fTraceSlurs
      ||
    chordSlursSize || gMsrOah->fDisplayMsrDetails
  ) {
    os <<
      setw (fieldWidth) <<
      "chordSlurs";
    if (chordSlursSize) {
      os << endl;
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

  if (
    gTraceOah->fTraceLigatures
      ||
    chordLigaturesSize || gMsrOah->fDisplayMsrDetails
  ) {
    os <<
      setw (fieldWidth) <<
      "chordLigatures";
    if (chordLigaturesSize) {
      os << endl;
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

  if (chordSlashesSize || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordSlashes";
    if (chordSlashesSize) {
      os << endl;
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

  if (chordWedgesSize || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordWedges";
    if (chordWedgesSize) {
      os << endl;
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

  // print the segnos if any
  int chordSegnosSize = fChordSegnos.size ();

  if (chordSegnosSize || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordSegnos";
    if (chordSegnosSize) {
      os << endl;
      gIndenter++;

      list<S_msrSegno>::const_iterator i;
      for (i=fChordSegnos.begin (); i!=fChordSegnos.end (); i++) {
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

  // print the dal segnos if any
  int chordDalSegnosSize = fChordDalSegnos.size ();

  if (chordDalSegnosSize || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordDalSegnos";
    if (chordDalSegnosSize) {
      os << endl;
      gIndenter++;

      list<S_msrDalSegno>::const_iterator i;
      for (i=fChordDalSegnos.begin (); i!=fChordDalSegnos.end (); i++) {
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

  // print the codas if any
  int chordCodasSize = fChordCodas.size ();

  if (chordCodasSize || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordCodas";
    if (chordCodasSize) {
      os << endl;
      gIndenter++;

      list<S_msrCoda>::const_iterator i;
      for (i=fChordCodas.begin (); i!=fChordCodas.end (); i++) {
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

  // print the octave shift if any
  if (fChordOctaveShift || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordOctaveShift";
    if (fChordOctaveShift) {
      os << endl;
      gIndenter++;
        os << fChordOctaveShift;
      gIndenter--;
    }
    else {
      os <<
        " : " << "none" <<
      endl;
    }
  }

  // print the chord grace notes groups if any
  if (
    gTraceOah->fTraceGraceNotes
      ||
    fChordGraceNotesGroupBefore || gMsrOah->fDisplayMsrDetails
  ) {
    os <<
      setw (fieldWidth) <<
      "chordGraceNotesGroupBefore";

    if (fChordGraceNotesGroupBefore) {
      gIndenter++;
      os << endl;
      os <<
        fChordGraceNotesGroupBefore->asString () <<
        endl;
      gIndenter--;
    }
    else {
      os << " : " << "none" << endl; // JMI TEST
    }
//    os << endl;
  }

  if (
    gTraceOah->fTraceGraceNotes
      ||
    fChordGraceNotesGroupAfter || gMsrOah->fDisplayMsrDetails
  ) {
    os <<
      setw (fieldWidth) <<
      "chordGraceNotesGroupAfter";

    if (fChordGraceNotesGroupAfter) {
      gIndenter++;
      os << endl;
      os <<
        fChordGraceNotesGroupAfter->asString ();
      gIndenter--;
    }
    else {
      os << " : " << "none";
    }
    os << endl;
  }

  // print the harmonies associated to this chord if any
  int chordHarmoniesListSize = fChordHarmoniesList.size ();

  if (
    gTraceOah->fTraceHarmonies
      ||
    chordHarmoniesListSize > 0 || gMsrOah->fDisplayMsrDetails
  ) {
    os <<
      setw (fieldWidth) <<
      "chordHarmonies";
    if (chordHarmoniesListSize) {
      os << endl;
      gIndenter++;

      list<S_msrHarmony>::const_iterator
        iBegin = fChordHarmoniesList.begin (),
        iEnd   = fChordHarmoniesList.end (),
        i      = iBegin;
      for ( ; ; ) {
        S_msrHarmony
          harmony = (*i);

        os << harmony->asString ();

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

  // print the figured bass if any
  if (
    gTraceOah->fTraceFiguredBasses
      ||
    fChordFiguredBass || gMsrOah->fDisplayMsrDetails
  ) {
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

  os << endl;

  // print the chord notes if any
  int chordNotesVectorSize = fChordNotesVector.size ();

  if (chordNotesVectorSize || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordNotes";
    if (chordNotesVectorSize) {
      os << endl;
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

  // print the chord position in measure
  os <<
    setw (fieldWidth) <<
    "positionInMeasure" << " : " << fMeasureElementPositionInMeasure <<
    endl;

  gIndenter--;

  os <<
    "chordTupletUpLink" << " : ";
  if (fChordTupletUpLink) {
    os <<
      fChordTupletUpLink->asShortString ();
  }
  else {
    os << "none";
  }
  os << endl;

  os <<
    setw (fieldWidth) <<
    "positionInTuplet" << " : " <<
    fPositionInTuplet <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrChord& elt)
{
  elt->print (os);
  return os;
}


}
