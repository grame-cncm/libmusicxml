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

  if (false) { // JMI
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
}

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
      /* JMI
  if (false) { // JMI
      note->
        setMeasureElementPositionInVoice (
          positionInVoice,
          context); // they all share the same one
}
          */

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

/*
void msrChord::appendSlurToChord (S_msrSlur slur)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceSlurs) {
    gLogOstream <<
      "Adding slur '" << slur <<
      "' to chord '" << asString () << "'" <<
      endl;
  }
#endif

  fChordSlurs.push_back (slur);
}
*/

void msrChord::appendChordSlurLinkToChord (S_msrChordSlurLink chordSlurLink)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceSlurs) {
    gLogOstream <<
      "Adding slur link '" << chordSlurLink->asString() <<
      "' to chord '" << asString () << "'" <<
      endl;
  }
#endif

  fChordSlurLinks.push_back (chordSlurLink);
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

/* JMI
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
*/

void msrChord::appendChordBeamLinkToChord (S_msrChordBeamLink chordBeamLink)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceBeams) {
    gLogOstream <<
      "Adding beam link '" << chordBeamLink->asString() <<
      "' to chord '" << asString () << "'" <<
      endl;
  }
#endif

  fChordBeamLinks.push_back (chordBeamLink);
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
  if (fChordGraceNotesGroupLinkBefore) {
    // browse the grace notes group before
    msrBrowser<msrChordGraceNotesGroupLink> browser (v);
    browser.browse (*fChordGraceNotesGroupLinkBefore);
  }

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

/* JMI
  for (
    list<S_msrBeam>::const_iterator i = fChordBeams.begin ();
    i != fChordBeams.end ();
    i++
  ) {
    // browse chord beams
    msrBrowser<msrBeam> browser (v);
    browser.browse (*(*i));
  } // for
  */
  for (
    list<S_msrChordBeamLink>::const_iterator i = fChordBeamLinks.begin ();
    i != fChordBeamLinks.end ();
    i++
  ) {
    // browse the beam link
    msrBrowser<msrChordBeamLink> browser (v);
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

/* JMI
  for (
    list<S_msrSlur>::const_iterator i = fChordSlurs.begin ();
    i != fChordSlurs.end ();
    i++
  ) {
    // browse the slur
    msrBrowser<msrSlur> browser (v);
    browser.browse (*(*i));
  } // for
*/

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

  for (
    list<S_msrChordSlurLink>::const_iterator i = fChordSlurLinks.begin ();
    i != fChordSlurLinks.end ();
    i++
  ) {
    // browse the slur link
    msrBrowser<msrChordSlurLink> browser (v);
    browser.browse (*(*i));
  } // for

/*
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
*/

  if (fChordGraceNotesGroupLinkAfter) {
    // browse the grace notes group after
    msrBrowser<msrChordGraceNotesGroupLink> browser (v);
    browser.browse (*fChordGraceNotesGroupLinkAfter);
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
        "[" <<
        note->notePitchAsString () <<
        ", whole notes: " <<
        " sounding " <<
        note->getNoteSoundingWholeNotes () <<
        ", displayed " <<
        note->getNoteDisplayWholeNotes () <<
        ", octave: " << note->getNoteOctave () <<
        "]";

      if (++i == iEnd) break;
      s << " ";
    } // for
  }

  s << ">[";

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
        "[" <<
        note->notePitchAsString () <<
        "', whole notes: " <<
        " sounding " <<
        note->getNoteSoundingWholeNotes () <<
        ", displayed " <<
        note->getNoteDisplayWholeNotes () <<
        ", octave: " << note->getNoteOctave () <<
        "]";

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
    endl <<
    endl;

  // print the chord grace notes groups links if any
  if (fChordGraceNotesGroupLinkBefore || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordGraceNotesGroupLinkBefore";

    if (fChordGraceNotesGroupLinkBefore) {
      os << endl;
      os << gTab << fChordGraceNotesGroupLinkBefore;
    }
    else {
      os << " : " << "none" << endl; // JMI TEST
    }
  }

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

/*
  // print the beams if any
  int chordBeamsSize = fChordBeams.size ();

  if (chordBeamsSize || gMsrOah->fDisplayMsrDetails) {
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
*/
  // print the beam links if any
  int chordBeamLinksSize = fChordBeamLinks.size ();

  if (gTraceOah->fTraceBeams || chordBeamLinksSize || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "===> chordBeamLinks ===>";
    if (chordBeamLinksSize) {
      os << endl;
      gIndenter++;

      list<S_msrChordBeamLink>::const_iterator i;
      for (i=fChordBeamLinks.begin (); i!=fChordBeamLinks.end (); i++) {
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

  if (chordWordsSize || gMsrOah->fDisplayMsrDetails) {
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

  // print the ligatures if any
  int chordLigaturesSize = fChordLigatures.size ();

  if (chordLigaturesSize || gMsrOah->fDisplayMsrDetails) {
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

  // print the harmonies associated to this chord if any
  int chordHarmoniesListSize = fChordHarmoniesList.size ();

  if (chordHarmoniesListSize > 0 || gMsrOah->fDisplayMsrDetails) {
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
  if (fChordFiguredBass || gMsrOah->fDisplayMsrDetails) {
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
      os << "none";
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

    os << endl;
  }

/* JMI
  // print the slurs if any
  int chordSlursSize = fChordSlurs.size ();

  if (chordSlursSize || gMsrOah->fDisplayMsrDetails) {
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
*/

  // print the slur links if any
  int chordSlurLinksSize = fChordSlurLinks.size ();

  if (gTraceOah->fTraceSlurs || chordSlurLinksSize || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "===> chordSlurLinks ===>";
    if (chordSlurLinksSize) {
      os << endl;
      gIndenter++;

      list<S_msrChordSlurLink>::const_iterator i;
      for (i=fChordSlurLinks.begin (); i!=fChordSlurLinks.end (); i++) {
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

/*
  // print the chord grace notes groups if any
  if (fChordGraceNotesGroupBefore || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordGraceNotesGroupBefore";

    if (fChordGraceNotesGroupBefore) {
      os << endl;
      gIndenter++;
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

  if (fChordGraceNotesGroupAfter || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordGraceNotesGroupAfter";

    if (fChordGraceNotesGroupAfter) {
      os << endl;
      gIndenter++;
      os <<
        fChordGraceNotesGroupAfter->asString ();
      gIndenter--;
    }
    else {
      os << " : " << "none";
    }
    os << endl;
  }
*/

  if (fChordGraceNotesGroupLinkAfter || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordGraceNotesGroupLinkAfter";

    if (fChordGraceNotesGroupLinkAfter) {
      os << endl;
      os << gTab << fChordGraceNotesGroupLinkAfter;
    }
    else {
      os << " : " << "none";
    }
    os << endl;
  }

  gIndenter--;
}

void msrChord::printShort (ostream& os) const
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

/*
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
*/

  os <<
    setw (fieldWidth) <<
    "positionInTuplet" << " : " <<
    fPositionInTuplet <<
    endl;

/*
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
*/

  // print the chord grace notes group link before if any
  if (fChordGraceNotesGroupLinkBefore || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "===> chordGraceNotesGroupLinkBefore ===>";

    if (fChordGraceNotesGroupLinkBefore) {
      os << endl;
      os << gTab << fChordGraceNotesGroupLinkBefore->asShortString ();
    }
    else {
      os << " : " << "none";
    }
    os << endl;
  }

/*
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
*/

/*
  // print the beams if any
  int chordBeamsSize = fChordBeams.size ();

  if (true || chordBeamsSize || gMsrOah->fDisplayMsrDetails) {
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
*/

  // print the beam links if any
  int chordBeamLinksSize = fChordBeamLinks.size ();

  if (gTraceOah->fTraceBeams || chordBeamLinksSize || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordBeamLinks";
    if (chordBeamLinksSize) {
      os << endl;
      gIndenter++;

      list<S_msrChordBeamLink>::const_iterator i;
      for (i=fChordBeamLinks.begin (); i!=fChordBeamLinks.end (); i++) {
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

  if (chordWordsSize || gMsrOah->fDisplayMsrDetails) {
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

/* JMI
  // print the slurs if any
  int chordSlursSize = fChordSlurs.size ();

  if (chordSlursSize || gMsrOah->fDisplayMsrDetails) {
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
*/

  // print the slur links if any
  int chordSlurLinksSize = fChordSlurLinks.size ();

  if (gTraceOah->fTraceSlurs || chordSlurLinksSize || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordSlurLinks";
    if (chordSlurLinksSize) {
      os << endl;
      gIndenter++;

      list<S_msrChordSlurLink>::const_iterator i;
      for (i=fChordSlurLinks.begin (); i!=fChordSlurLinks.end (); i++) {
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

  if (chordLigaturesSize || gMsrOah->fDisplayMsrDetails) {
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

/*
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
*/

  // print the harmonies associated to this chord if any
  int chordHarmoniesListSize = fChordHarmoniesList.size ();

  if (chordHarmoniesListSize > 0 || gMsrOah->fDisplayMsrDetails) {
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
  if (fChordFiguredBass || gMsrOah->fDisplayMsrDetails) {
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
        (*i)->printShort (os);
        if (++i == iEnd) break;
        os << endl;
      } // for

      gIndenter--;
    }
    else {
      os << ":" << "none" <<
      endl;
    }

    os << endl;
  }

/*
  // print the chord grace notes groups if any
  if (fChordGraceNotesGroupBefore || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordGraceNotesGroupBefore";

    if (fChordGraceNotesGroupBefore) {
      os << endl;
      gIndenter++;
      fChordGraceNotesGroupBefore->printShort (os);
      gIndenter--;
    }
    else {
      os << " : " << "none" << endl; // JMI TEST
    }
//    os << endl;
  }

  if (fChordGraceNotesGroupAfter || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "chordGraceNotesGroupAfter";

    if (fChordGraceNotesGroupAfter) {
      os << endl;
      gIndenter++;
      fChordGraceNotesGroupAfter->printShort (os);
      gIndenter--;
    }
    else {
      os << " : " << "none";
    }
    os << endl;
  }
*/

  // print the chord grace notes group link after if any
  if (fChordGraceNotesGroupLinkAfter || gMsrOah->fDisplayMsrDetails) {
    os <<
      setw (fieldWidth) <<
      "===> chordGraceNotesGroupLinkAfter ===>";

    if (fChordGraceNotesGroupLinkAfter) {
      os << endl;
      os << gTab << fChordGraceNotesGroupLinkAfter->asShortString ();
    }
    else {
      os << " : " << "none";
    }
    os << endl;
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrChord& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrChordBeamLink msrChordBeamLink::create (
  int        inputLineNumber,
  S_msrBeam  originalBeam,
  S_msrChord chordUpLink)
{
  msrChordBeamLink* o =
    new msrChordBeamLink (
      inputLineNumber,
      originalBeam,
      chordUpLink);
  assert(o!=0);

  return o;
}

msrChordBeamLink::msrChordBeamLink (
  int        inputLineNumber,
  S_msrBeam  originalBeam,
  S_msrChord chordUpLink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert(
    originalBeam != nullptr,
    "originalBeam is null");

  // sanity check
  msrAssert(
    chordUpLink != nullptr,
    "chordUpLink is null");

  fChordUpLink = chordUpLink;

  fOriginalBeam = originalBeam;
}

msrChordBeamLink::~msrChordBeamLink ()
{}

S_msrChordBeamLink msrChordBeamLink::createBeamNewbornClone ()
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceGraceNotes) {
    gLogOstream <<
      "Creating a newborn clone of grace notes group link '" <<
      asShortString () <<
      "'" <<
      endl;
  }
#endif

/* JMI
  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
*/

  S_msrChordBeamLink
    newbornClone =
      msrChordBeamLink::create (
        fInputLineNumber,
        fOriginalBeam,
        fChordUpLink);

  return newbornClone;
}

void msrChordBeamLink::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrChordBeamLink::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrChordBeamLink>*
    p =
      dynamic_cast<visitor<S_msrChordBeamLink>*> (v)) {
        S_msrChordBeamLink elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrChordBeamLink::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrChordBeamLink::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrChordBeamLink::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrChordBeamLink>*
    p =
      dynamic_cast<visitor<S_msrChordBeamLink>*> (v)) {
        S_msrChordBeamLink elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrChordBeamLink::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrChordBeamLink::browseData (basevisitor* v)
{
  // browse the original grace notes group
  msrBrowser<msrBeam> browser (v);
  browser.browse (*fOriginalBeam);
}

string msrChordBeamLink::asShortString () const
{
  stringstream s;

  s <<
    "[ChordBeamLink" <<
    ", originalBeam \"" <<
    fOriginalBeam->asShortString () <<
    ", chordUpLink \"" <<
    fChordUpLink->asShortString () <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

string msrChordBeamLink::asString () const
{
  stringstream s;

  s <<
    "[ChordBeamLink" <<
    ", originalBeam \"" <<
    fOriginalBeam->asString () <<
    ", chordUpLink \"" <<
    fChordUpLink->asString () <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrChordBeamLink::print (ostream& os) const
{
  os <<
    "ChordBeamLink" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 33;

  os <<
    setw (fieldWidth) <<
    "originalBeam:" <<
    endl;

  gIndenter++;
  os <<
    fOriginalBeam <<
    fChordUpLink;
  gIndenter--;

  gIndenter--;
}

void msrChordBeamLink::printShort (ostream& os) const
{
  os <<
    "ChordBeamLink" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 33;

  os <<
    setw (fieldWidth) <<
    "originalBeam:" <<
    endl;

  gIndenter++;
  fOriginalBeam->printShort (os);
  fChordUpLink->printShort (os);
  gIndenter--;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrChordBeamLink& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrChordSlurLink msrChordSlurLink::create (
  int        inputLineNumber,
  S_msrSlur  originalSlur,
  S_msrChord chordUpLink)
{
  msrChordSlurLink* o =
    new msrChordSlurLink (
      inputLineNumber,
      originalSlur,
      chordUpLink);
  assert(o!=0);

  return o;
}

msrChordSlurLink::msrChordSlurLink (
  int        inputLineNumber,
  S_msrSlur  originalSlur,
  S_msrChord chordUpLink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert(
    originalSlur != nullptr,
    "originalSlur is null");

  // sanity check
  msrAssert(
    chordUpLink != nullptr,
    "chordUpLink is null");

  fChordUpLink = chordUpLink;

  fOriginalSlur = originalSlur;
}

msrChordSlurLink::~msrChordSlurLink ()
{}

S_msrChordSlurLink msrChordSlurLink::createSlurNewbornClone ()
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceGraceNotes) {
    gLogOstream <<
      "Creating a newborn clone of grace notes group link '" <<
      asShortString () <<
      "'" <<
      endl;
  }
#endif

/* JMI
  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
*/

  S_msrChordSlurLink
    newbornClone =
      msrChordSlurLink::create (
        fInputLineNumber,
        fOriginalSlur,
        fChordUpLink);

  return newbornClone;
}

void msrChordSlurLink::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrChordSlurLink::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrChordSlurLink>*
    p =
      dynamic_cast<visitor<S_msrChordSlurLink>*> (v)) {
        S_msrChordSlurLink elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrChordSlurLink::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrChordSlurLink::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrChordSlurLink::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrChordSlurLink>*
    p =
      dynamic_cast<visitor<S_msrChordSlurLink>*> (v)) {
        S_msrChordSlurLink elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrChordSlurLink::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrChordSlurLink::browseData (basevisitor* v)
{
  // browse the original grace notes group
  msrBrowser<msrSlur> browser (v);
  browser.browse (*fOriginalSlur);
}

string msrChordSlurLink::asShortString () const
{
  stringstream s;

  s <<
    "[ChordSlurLink" <<
    ", originalSlur \"" <<
    fOriginalSlur->asShortString () <<
    ", chordUpLink \"" <<
    fChordUpLink->asShortString () <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

string msrChordSlurLink::asString () const
{
  stringstream s;

  s <<
    "[ChordSlurLink" <<
    ", originalSlur \"" <<
    fOriginalSlur->asString () <<
    ", chordUpLink \"" <<
    fChordUpLink->asString () <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrChordSlurLink::print (ostream& os) const
{
  os <<
    "ChordSlurLink" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 33;

  os <<
    setw (fieldWidth) <<
    "originalSlur:" <<
    endl;

  gIndenter++;
  os <<
    fOriginalSlur <<
    fChordUpLink;
  gIndenter--;

  gIndenter--;
}

void msrChordSlurLink::printShort (ostream& os) const
{
  os <<
    "ChordSlurLink" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 33;

  os <<
    setw (fieldWidth) <<
    "originalSlur:" <<
    endl;

  gIndenter++;
  fOriginalSlur->printShort (os);
  fChordUpLink->printShort (os);
  gIndenter--;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrChordSlurLink& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrChordGraceNotesGroupLink msrChordGraceNotesGroupLink::create (
  int                  inputLineNumber,
  S_msrGraceNotesGroup originalGraceNotesGroup,
  S_msrChord           chordUpLink)
{
  msrChordGraceNotesGroupLink* o =
    new msrChordGraceNotesGroupLink (
      inputLineNumber,
      originalGraceNotesGroup,
      chordUpLink);
  assert(o!=0);

  return o;
}

msrChordGraceNotesGroupLink::msrChordGraceNotesGroupLink (
  int                  inputLineNumber,
  S_msrGraceNotesGroup originalGraceNotesGroup,
  S_msrChord           chordUpLink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert(
    originalGraceNotesGroup != nullptr,
    "originalGraceNotesGroup is null");

  // sanity check
  msrAssert(
    chordUpLink != nullptr,
    "chordUpLink is null");

  fChordUpLink = chordUpLink;

  fOriginalGraceNotesGroup = originalGraceNotesGroup;
}

msrChordGraceNotesGroupLink::~msrChordGraceNotesGroupLink ()
{}

S_msrChordGraceNotesGroupLink msrChordGraceNotesGroupLink::createChordGraceNotesGroupLinkNewbornClone ()
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceGraceNotes) {
    gLogOstream <<
      "Creating a newborn clone of grace notes group link '" <<
      asShortString () <<
      "'" <<
      endl;
  }
#endif

/* JMI
  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");
*/

  S_msrChordGraceNotesGroupLink
    newbornClone =
      msrChordGraceNotesGroupLink::create (
        fInputLineNumber,
        fOriginalGraceNotesGroup,
        fChordUpLink);

  return newbornClone;
}

void msrChordGraceNotesGroupLink::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrChordGraceNotesGroupLink::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrChordGraceNotesGroupLink>*
    p =
      dynamic_cast<visitor<S_msrChordGraceNotesGroupLink>*> (v)) {
        S_msrChordGraceNotesGroupLink elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrChordGraceNotesGroupLink::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrChordGraceNotesGroupLink::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrChordGraceNotesGroupLink::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrChordGraceNotesGroupLink>*
    p =
      dynamic_cast<visitor<S_msrChordGraceNotesGroupLink>*> (v)) {
        S_msrChordGraceNotesGroupLink elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrChordGraceNotesGroupLink::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrChordGraceNotesGroupLink::browseData (basevisitor* v)
{
  list<S_msrMeasureElement>::const_iterator i;

  // browse the original grace notes group
  msrBrowser<msrGraceNotesGroup> browser (v);
  browser.browse (*fOriginalGraceNotesGroup);
}

string msrChordGraceNotesGroupLink::asShortString () const
{
  stringstream s;

  s <<
    "[chordGraceNotesGroupLink" <<
    ", fOriginalGraceNotesGroup: " <<
    fOriginalGraceNotesGroup->asShortString () <<
    ", chordUpLink \"" <<
    fChordUpLink->asShortString () <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

string msrChordGraceNotesGroupLink::asString () const
{
  stringstream s;

  s <<
    "[chordGraceNotesGroupLink" <<
    ", originalGraceNotesGroup \"" <<
    fOriginalGraceNotesGroup->asString () <<
    ", chordUpLink \"" <<
    fChordUpLink->asString () <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrChordGraceNotesGroupLink::print (ostream& os) const
{
  os <<
    "chordGraceNotesGroupLink" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 33;

  os <<
    setw (fieldWidth) <<
    "originalGraceNotesGroup" <<
    endl;

  gIndenter++;
  os <<
    fOriginalGraceNotesGroup <<
    fChordUpLink;
  gIndenter--;

  gIndenter--;
}

void msrChordGraceNotesGroupLink::printShort (ostream& os) const
{
  os <<
    "chordGraceNotesGroupLink" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 33;

  os <<
    setw (fieldWidth) <<
    "originalGraceNotesGroup" <<
    endl;

  gIndenter++;
  fOriginalGraceNotesGroup->printShort (os);
  fChordUpLink->printShort (os);
  gIndenter--;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrChordGraceNotesGroupLink& elt)
{
  elt->print (os);
  return os;
}


}
