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
S_msrTuplet msrTuplet::create (
  int                     inputLineNumber,
  string                  tupletMeasureNumber,
  int                     tupletNumber,
  msrTupletBracketKind    tupletBracketKind,
  msrTupletLineShapeKind  tupletLineShapeKind,
  msrTupletShowNumberKind tupletShowNumberKind,
  msrTupletShowTypeKind   tupletShowTypeKind,
  msrTupletFactor         tupletFactor,
  rational                memberNotesSoundingWholeNotes,
  rational                memberNotesDisplayWholeNotes)
{
  msrTuplet* o =
    new msrTuplet (
      inputLineNumber,
      tupletMeasureNumber,
      tupletNumber,
      tupletBracketKind,
      tupletLineShapeKind,
      tupletShowNumberKind,
      tupletShowTypeKind,
      tupletFactor,
      memberNotesSoundingWholeNotes,
      memberNotesDisplayWholeNotes);
  assert(o!=0);
  return o;
}

msrTuplet::msrTuplet (
  int                     inputLineNumber,
  string                  tupletMeasureNumber,
  int                     tupletNumber,
  msrTupletBracketKind    tupletBracketKind,
  msrTupletLineShapeKind  tupletLineShapeKind,
  msrTupletShowNumberKind tupletShowNumberKind,
  msrTupletShowTypeKind   tupletShowTypeKind,
  msrTupletFactor         tupletFactor,
  rational                memberNotesSoundingWholeNotes,
  rational                memberNotesDisplayWholeNotes)
    : msrTupletElement (inputLineNumber)
{
  fTupletNumber = tupletNumber;

  fTupletBracketKind    = tupletBracketKind;
  fTupletLineShapeKind  = tupletLineShapeKind;
  fTupletShowNumberKind = tupletShowNumberKind;
  fTupletShowTypeKind   = tupletShowTypeKind;

  fTupletFactor = tupletFactor;

  fMemberNotesSoundingWholeNotes = memberNotesSoundingWholeNotes;
  fMemberNotesDisplayWholeNotes  = memberNotesDisplayWholeNotes;

  fMeasureElementSoundingWholeNotes       = rational (0, 1);
  fTupletDisplayWholeNotes  = rational (0, 1);

#ifdef TRACE_OAH
  if (globalTraceOah->fTraceTuplets) {
    gLogOstream <<
      "Creating tuplet:" <<
      endl;

    gIndenter++;

    this->print (gLogOstream);

    gIndenter--;
  }
#endif
}

msrTuplet::~msrTuplet ()
{}

S_msrTuplet msrTuplet::createTupletNewbornClone ()
{
#ifdef TRACE_OAH
  if (globalTraceOah->fTraceTuplets) {
    gLogOstream <<
      "Creating a newborn clone of tuplet " <<
      asString () <<
      endl;
  }
#endif

  S_msrTuplet
    newbornClone =
      msrTuplet::create (
        fInputLineNumber,
        fMeasureElementMeasureNumber,
        fTupletNumber,
        fTupletBracketKind,
        fTupletLineShapeKind,
        fTupletShowNumberKind,
        fTupletShowTypeKind,
        fTupletFactor,
        fMemberNotesSoundingWholeNotes,
        fMemberNotesDisplayWholeNotes);

/* JMI ???
  newbornClone->fMeasureElementSoundingWholeNotes =
    fMeasureElementSoundingWholeNotes;

  newbornClone->fTupletDisplayWholeNotes =
    fTupletDisplayWholeNotes;

  newbornClone->fMeasureElementMeasureNumber =
    fMeasureElementMeasureNumber;

  newbornClone->fMeasureElementPositionInMeasure =
    fMeasureElementPositionInMeasure;
*/

  return newbornClone;
}

string msrTuplet::tupletTypeKindAsString (
  msrTupletTypeKind tupletTypeKind)
{
  string result;

  switch (tupletTypeKind) {
    case msrTuplet::kTupletTypeNone:
      result = "tupletTypeNone";
      break;
    case msrTuplet::kTupletTypeStart:
      result = "tupletTypeStart";
      break;
    case msrTuplet::kTupletTypeContinue:
      result = "tupletTypeContinue";
      break;
    case msrTuplet::kTupletTypeStop:
      result = "tupletTypeStop";
      break;
    case msrTuplet::kTupletTypeStartAndStopInARow:
      result = "tupletTypeStartAndStopInARow";
      break;
  } // switch

  return result;
}

string msrTuplet::tupletBracketKindAsString (
  msrTupletBracketKind tupletBracketKind)
{
  string result;

  switch (tupletBracketKind) {
    case msrTuplet::kTupletBracketYes:
      result = "tupletBracketYes";
      break;
    case msrTuplet::kTupletBracketNo:
      result = "tupletBracketNo";
      break;
  } // switch

  return result;
}

string msrTuplet::tupletLineShapeKindAsString (
  msrTupletLineShapeKind tupletLineShapeKind)
{
  string result;

  switch (tupletLineShapeKind) {
    case msrTuplet::kTupletLineShapeStraight:
      result = "tupletLineShapeStraight";
      break;
    case msrTuplet::kTupletLineShapeCurved:
      result = "tupletLineShapeCurved";
      break;
  } // switch

  return result;
}

string msrTuplet::tupletShowNumberKindAsString (
  msrTupletShowNumberKind tupletShowNumberKind)
{
  string result;

  switch (tupletShowNumberKind) {
    case msrTuplet::kTupletShowNumberActual:
      result = "tupletShowNumberActual";
      break;
    case msrTuplet::kTupletShowNumberBoth:
      result = "tupletShowNumberBoth";
      break;
    case msrTuplet::kTupletShowNumberNone:
      result = "tupletShowNumberNone";
      break;
  } // switch

  return result;
}

string msrTuplet::tupletShowTypeKindAsString (
  msrTupletShowTypeKind tupletShowTypeKind)
{
  string result;

  switch (tupletShowTypeKind) {
    case msrTuplet::kTupletShowTypeActual:
      result = "tupletShowTypeActual";
      break;
    case msrTuplet::kTupletShowTypeBoth:
      result = "tupletShowTypeBoth";
      break;
    case msrTuplet::kTupletShowTypeNone:
      result = "tupletShowTypeNone";
      break;
  } // switch

  return result;
}

void msrTuplet::appendNoteToTuplet (
  S_msrNote  note,
  S_msrVoice voice)
{
  int inputLineNumber =
    note->getInputLineNumber ();

#ifdef TRACE_OAH
  if (globalTraceOah->fTraceTuplets) {
    gLogOstream <<
      "Adding note " <<
      note->asShortString () <<
      // the information is missing to display it the normal way
      " to tuplet " <<
      asString () <<
      endl;
  }
#endif

  // append note to elements list
  fTupletElementsList.push_back (note);

  // set note's position in tuplet
  note->setPositionInTuplet (
    fTupletElementsList.size ());

  // register note's tuplet upLink
  note->
    setNoteTupletUpLink (this);

  // register note's measure upLink // JMI ???
  note->
    setNoteMeasureUpLink (fTupletMeasureUpLink);

  // account for note duration in tuplet duration
  fMeasureElementSoundingWholeNotes +=
    note->getNoteSoundingWholeNotes ();
  fMeasureElementSoundingWholeNotes.rationalise ();

  fTupletDisplayWholeNotes += // JMI
    note->getNoteDisplayWholeNotes ();
  fTupletDisplayWholeNotes.rationalise ();

  // register note's tuplet factor
  note->
    setNoteTupletFactor (fTupletFactor);

  // is this note the shortest one in this voice?
  voice->
    registerShortestNoteInVoiceIfRelevant (note);

  // fetch voice last measure
  S_msrMeasure
    voiceLastMeasure =
      voice->fetchVoiceLastMeasure (
        inputLineNumber);

  // account for the duration of note in voice last measure
  voiceLastMeasure->
    accountForTupletMemberNoteDurationInMeasure (
      note);
}

void msrTuplet::appendChordToTuplet (S_msrChord chord)
{
#ifdef TRACE_OAH
  if (globalTraceOah->fTraceTuplets) {
    gLogOstream <<
      "Adding chord " <<
      chord->asString () <<
      " to tuplet " <<
      asString () <<
      endl;
  }
#endif

  // append chord to elements list
  fTupletElementsList.push_back (chord);

  // set chord's position in tuplet
  chord->setPositionInTuplet (
    fTupletElementsList.size ());

  // set chord's position in tuplet
  chord->setPositionInTuplet (
    fTupletElementsList.size ());

  // DO NOT account for the chord duration,
  // since its first note has been accounted for already
  /* JMI
  fMeasureElementSoundingWholeNotes +=
    chord->getChordSoundingWholeNotes ();
  fMeasureElementSoundingWholeNotes.rationalise ();
*/

  fTupletDisplayWholeNotes += // JMI
    chord->getChordDisplayWholeNotes ();
  fTupletDisplayWholeNotes.rationalise ();

/* too early JMI
  // populate chord's measure number
  chord->setChordMeasureNumber (
    fMeasureElementMeasureNumber);
*/
}

void msrTuplet::addTupletToTuplet (S_msrTuplet tuplet)
{
#ifdef TRACE_OAH
  if (globalTraceOah->fTraceTuplets) {
    gLogOstream <<
      "Adding tuplet " <<
      tuplet->asString () <<
      " to tuplet " <<
      asString () <<
      endl;
  }
#endif

/* JMI
  // unapply containing tuplet factor,
  // i.e 3/2 inside 5/4 becomes 15/8 in MusicXML...
  tuplet->
    unapplySoundingFactorToTupletMembers (
      this->getTupletNormalNotes (),
      this->getTupletNormalNotes ();
  */

  // append tuplet to elements list
  fTupletElementsList.push_back (tuplet);

  // set tuplet's position in tuplet
  tuplet->setPositionInTuplet (
    fTupletElementsList.size ());

  // account for tuplet duration
  fMeasureElementSoundingWholeNotes +=
    tuplet->getTupletSoundingWholeNotes ();
  fMeasureElementSoundingWholeNotes.rationalise ();

  fTupletDisplayWholeNotes += // JMI
    tuplet->getTupletDisplayWholeNotes ();
  fTupletDisplayWholeNotes.rationalise ();
}

void msrTuplet::addTupletToTupletClone (S_msrTuplet tuplet)
{
#ifdef TRACE_OAH
  if (globalTraceOah->fTraceTuplets) {
    gLogOstream <<
      "Adding tuplet " <<
      tuplet->asString () <<
      " to tuplet " <<
      asString () <<
      endl;
  }
#endif

  // dont' unapply containing tuplet factor,
  // this has been done when building the MSR from MusicXML

  // append tuplet to elements list
  fTupletElementsList.push_back (tuplet);

  // set tuplet's position in tuplet
  tuplet->setPositionInTuplet (
    fTupletElementsList.size ());

  // account for tuplet duration
  fMeasureElementSoundingWholeNotes +=
    tuplet->getTupletSoundingWholeNotes ();
  fMeasureElementSoundingWholeNotes.rationalise ();

  fTupletDisplayWholeNotes +=
    tuplet->getTupletDisplayWholeNotes ();
  fTupletDisplayWholeNotes.rationalise ();
}

S_msrNote msrTuplet::fetchTupletFirstNonGraceNote () const
{
  S_msrNote result;

  if (fTupletElementsList.size ()) {
    S_msrElement
      firstTupletElement =
        fTupletElementsList.front ();

    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(*firstTupletElement))
      ) {
      // first element is a note, we have it
      result = note;
    }

    else if (
      S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(*firstTupletElement))
      ) {
      // first element is another tuplet, recurse
      result = tuplet->fetchTupletFirstNonGraceNote ();
    }
  }

  else {
    msrInternalError (
      globalOahOah->fInputSourceName,
      fInputLineNumber,
      __FILE__, __LINE__,
      "cannot access the first note of an empty tuplet");
  }

  return result;
}

S_msrNote msrTuplet::removeFirstNoteFromTuplet (
  int inputLineNumber)
{
  S_msrNote result;

#ifdef TRACE_OAH
  if (globalTraceOah->fTraceTuplets) {
    gLogOstream <<
      "Removing first note from tuplet " <<
      asString () <<
      endl;
  }
#endif

  if (fTupletElementsList.size ()) {
    S_msrElement
      firstTupletElement =
        fTupletElementsList.front ();

    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(*firstTupletElement))
      ) {
      fTupletElementsList.pop_front ();
      result = note;
    }

    else {
      if (true) {
        this->print (gLogOstream);
      }

      msrInternalError (
        globalOahOah->fInputSourceName,
        fInputLineNumber,
        __FILE__, __LINE__,
        "removeFirstNoteFromTuplet () expects a note as the first tuplet element");
    }

/* JMI
    for (
      list<S_msrElement>::iterator i=fTupletElementsList.begin ();
      i!=fTupletElementsList.end ();
      ++i) {
      if ((*i) == note) {
        // found note, erase it
        i = fTupletElementsList.erase (i);

        // account for note duration
        fMeasureElementSoundingWholeNotes -=
          note->getNoteSoundingWholeNotes ();
        fMeasureElementSoundingWholeNotes.rationalise ();

        fTupletDisplayWholeNotes -= // JMI
          note->getNoteDisplayWholeNotes ();
        fTupletDisplayWholeNotes.rationalise ();

        // don't update measure number nor position in measure: // JMI
        // they have not been set yet

        // return from function
        return;
      }
    } // for

    stringstream s;

    s <<
      "cannot remove note " <<
      note <<
      " from tuplet " << asString () <<
      " in voice \"" <<
      fTupletMeasureUpLink->
        getMeasureSegmentUpLink ()->
          getSegmentVoiceUpLink ()->
            getVoiceName () <<
      "\"," <<
      " since this note has not been found in fTupletElementsList";

    msrInternalError (
      globalOahOah->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  */
  }

  else {
    stringstream s;

    s <<
      "cannot remove the first note of an empty tuplet " <<
      " in voice \"" <<
      fTupletMeasureUpLink->
        getMeasureSegmentUpLink ()->
          getSegmentVoiceUpLink ()->
            getVoiceName () <<
      "\"";

    msrInternalError (
      globalOahOah->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  return result;
}

S_msrNote msrTuplet::removeLastNoteFromTuplet (
  int inputLineNumber)
{
  S_msrNote result;

#ifdef TRACE_OAH
  if (globalTraceOah->fTraceTuplets) {
    gLogOstream <<
      "Removing last note from tuplet " <<
      asString () <<
      endl;
  }
#endif

  if (fTupletElementsList.size ()) {
    S_msrElement
      lastTupletElement =
        fTupletElementsList.back ();

    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(*lastTupletElement))
    ) {
      // remove note from tuplet elements list
      fTupletElementsList.pop_back ();

/*
      // decrement the tuplet sounding whole notes accordingly ??? JMI BAD???
      fMeasureElementSoundingWholeNotes +=
        note->getNoteSoundingWholeNotes ();
      fMeasureElementSoundingWholeNotes.rationalise ();
*/

      result = note;
    }

    else {
      if (true) { // JMI
        this->print (gLogOstream);
      }

      msrInternalError (
        globalOahOah->fInputSourceName,
        fInputLineNumber,
        __FILE__, __LINE__,
        "removeLastNoteFromTuplet () expects a note as the last tuplet element");
    }
  }

  else {
    stringstream s;

    s <<
      "cannot remove the last note of an empty tuplet " <<
      " in voice \"" <<
      fTupletMeasureUpLink->
        getMeasureSegmentUpLink ()->
          getSegmentVoiceUpLink ()->
            getVoiceName () <<
      "\"";

    msrInternalError (
      globalOahOah->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

#ifdef TRACE_OAH
  if (globalTraceOah->fTraceTuplets) {
    gLogOstream <<
      "This last note from tuplet " <<
      asString () <<
      " turns out to be " <<
      result->asShortString () <<
      endl;
  }
#endif

  return result;
}

rational msrTuplet::setTupletMembersPositionInMeasure (
  S_msrMeasure measure,
  rational     positionInMeasure)
  // returns the position in measure after the tuplet
{
#ifdef TRACE_OAH
  if (globalTraceOah->fTracePositionsInMeasures) {
    gLogOstream <<
      "Setting tuplet position in measure of " << asString () <<
      " to '" <<
      positionInMeasure <<
      "'" <<
      endl;
  }
#endif

  string context =
    "setTupletMembersPositionInMeasure()";

  setMeasureElementPositionInMeasure (
    positionInMeasure,
    context);

  if (false) { // JMI
  // compute tuplet's position in voice
  rational
     positionInVoice =
      fTupletMeasureUpLink->getMeasurePositionInVoice ()
        +
      positionInMeasure;
  positionInVoice.rationalise ();

  // set tuplet's position in voice
  setMeasureElementPositionInVoice (
    positionInVoice,
    context);

  // update current position in voice
  S_msrVoice
    voice =
      fTupletMeasureUpLink->
        fetchMeasureVoiceUpLink ();

  voice->
    incrementCurrentPositionInVoice (
      fMeasureElementSoundingWholeNotes);
}

  // current position
  rational currentPosition = positionInMeasure;

  // compute position in measure for the tuplets elements
  for (
    list<S_msrTupletElement>::const_iterator i = fTupletElementsList.begin ();
    i != fTupletElementsList.end ();
    i++
  ) {
    // set tuplet element position in measure

    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(*(*i)))
    ) {
      // note
      note->
        setNoteMeasureUpLink (
          measure);

      note->
        setNotePositionInMeasure (
          currentPosition);

      currentPosition +=
        note->
          getNoteSoundingWholeNotes ();
      currentPosition.rationalise ();
    }

    else if (
      S_msrChord chord = dynamic_cast<msrChord*>(&(*(*i)))
    ) {
      // chord
      chord->
        setChordMembersPositionInMeasure (
          measure,
          currentPosition);
      currentPosition.rationalise ();

      currentPosition +=
        chord->
          getChordSoundingWholeNotes ();
      currentPosition.rationalise ();
    }

    else if (
      S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(*(*i)))
    ) {
      // nested tuplet
      currentPosition =
        tuplet->
          setTupletMembersPositionInMeasure ( // a function JMI ???
            measure,
            currentPosition);

      currentPosition +=
        tuplet->
          getTupletSoundingWholeNotes ();
      currentPosition.rationalise ();
    }

    else {
      msrInternalError (
        globalOahOah->fInputSourceName,
        fInputLineNumber,
        __FILE__, __LINE__,
        "tuplet member should be a note, a chord or another tuplet");
    }
  } // for

  return currentPosition;
}

void msrTuplet::unapplySoundingFactorToTupletMembers (
  const msrTupletFactor& containingTupletFactor)
  /*
  int containingTupletActualNotes,
  int containingTupletNormalNotes)
  */
{
#ifdef TRACE_OAH
  if (globalTraceOah->fTraceTuplets) {
    gLogOstream <<
      "unapplySoundingFactorToTupletMembers ()" <<
      endl;

    gIndenter++;

    gLogOstream <<
      "% fTupletFactor = " << fTupletFactor.asString () <<
      endl <<
      "% containingTupletFactor = " << containingTupletFactor.asString () <<
      endl;

    gIndenter--;
  }
#endif

  fTupletFactor.setTupletActualNotes (
    fTupletFactor.getTupletActualNotes ()
      /
    containingTupletFactor.getTupletActualNotes ());
  fTupletFactor.setTupletNormalNotes (
    fTupletFactor.getTupletNormalNotes ()
      /
    containingTupletFactor.getTupletNormalNotes ());

/* JMI
  fTupletFactor.getTupletNormalNotes () /=
    containingTupletFactor.getTupletNormalNotes ();
  fTupletFactor.fTupletNormalNotes /=
    containingTupletFactor.fTupletNormalNotes;
  */
}

/* JMI ???
void msrTuplet::finalizeTuplet (
  int inputLineNumber)
{
#ifdef TRACE_OAH
  if (globalTraceOah->fTraceTuplets) {
    gLogOstream <<
      "Finalizing tuplet " <<
      asString () <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

/ * JMI
  // we can now set the position in measure for all the tuplet members
  setTupletMembersPositionInMeasure (
    fMeasureElementPositionInMeasure);
  * /
}
*/

void msrTuplet::acceptIn (basevisitor* v)
{
  if (globalMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrTuplet::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrTuplet>*
    p =
      dynamic_cast<visitor<S_msrTuplet>*> (v)) {
        S_msrTuplet elem = this;

        if (globalMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrTuplet::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrTuplet::acceptOut (basevisitor* v)
{
  if (globalMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrTuplet::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrTuplet>*
    p =
      dynamic_cast<visitor<S_msrTuplet>*> (v)) {
        S_msrTuplet elem = this;

        if (globalMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrTuplet::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrTuplet::browseData (basevisitor* v)
{
  for (
    list<S_msrTupletElement>::const_iterator i = fTupletElementsList.begin ();
    i != fTupletElementsList.end ();
    i++
  ) {
    // browse tuplet element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for
}

string msrTuplet::asString () const
{
  stringstream s;

  s <<
    "[" <<
    "Tuplet " <<
    fTupletFactor.asString () <<
    " " << fMeasureElementSoundingWholeNotes << " tupletSoundingWholeNotes" <<
    ", measure ' "<<
    fMeasureElementMeasureNumber <<
    "':";

  if (fMeasureElementPositionInMeasure.getNumerator () < 0) {
    s << "?";
  }
  else {
    s << fMeasureElementPositionInMeasure;
  }

  s << "[[";

  if (fTupletElementsList.size ()) {
    list<S_msrTupletElement>::const_iterator
      iBegin = fTupletElementsList.begin (),
      iEnd   = fTupletElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {

      if (
        S_msrNote note = dynamic_cast<msrNote*>(&(*(*i)))
        ) {
        s <<
          note->asShortString ();
      }

      else if (
        S_msrChord chord = dynamic_cast<msrChord*>(&(*(*i)))
        ) {
        s <<
          chord->asString ();
      }

      else if (
        S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(*(*i)))
        ) {
        s <<
          tuplet->asString ();
      }

      else {
        msrInternalError (
          globalOahOah->fInputSourceName,
          fInputLineNumber,
          __FILE__, __LINE__,
          "tuplet member should be a note, a chord or another tuplet");
      }

      if (++i == iEnd) break;
      s << " ";

    } // for
  }

  s << "]]" << "]";

  return s.str ();
}

void msrTuplet::print (ostream& os) const
{
  os <<
    "Tuplet " <<
    fTupletNumber <<
    ", " <<
    fTupletFactor.asString () <<
    ", " <<
    singularOrPlural (
      fTupletElementsList.size (), "element", "elements") <<
    ", whole notes: " <<
    fMeasureElementSoundingWholeNotes << " sounding, " <<
    fTupletDisplayWholeNotes << " displayed" <<
    ", meas "<<
    fMeasureElementMeasureNumber <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 30;

  os << left <<
    setw (fieldWidth) <<
    "tupletBracketKind" << " : " <<
    tupletBracketKindAsString (
      fTupletBracketKind) <<
    endl <<
    setw (fieldWidth) <<
    "tupletLineShapeKind" << " : " <<
    tupletLineShapeKindAsString (
      fTupletLineShapeKind) <<
    endl <<
    setw (fieldWidth) <<
    "tupletShowNumberKind" << " : " <<
    tupletShowNumberKindAsString (
      fTupletShowNumberKind) <<
    endl <<
    setw (fieldWidth) <<
    "tupletShowTypeKind" << " : " <<
    tupletShowTypeKindAsString (
      fTupletShowTypeKind) <<
    endl <<

    setw (fieldWidth) <<
    "memberNotesSoundingWholeNotes" << " : " <<
    fMemberNotesSoundingWholeNotes <<
    endl <<
    setw (fieldWidth) <<
    "memberNotesDisplayWholeNotes" << " : " <<
    fMemberNotesDisplayWholeNotes <<
    endl <<

    setw (fieldWidth) <<
    "tupletSoundingWholeNotes" << " : " <<
    fMeasureElementSoundingWholeNotes <<
    endl <<
    setw (fieldWidth) <<
    "tupletDisplayWholeNotes" << " : " <<
    fTupletDisplayWholeNotes <<
    endl <<

    setw (fieldWidth) <<
    "tupletMeasureNumber" << " : " <<
    fMeasureElementMeasureNumber <<
    endl <<
    setw (fieldWidth) <<
    "positionInMeasure" << " : " <<
    fMeasureElementPositionInMeasure <<
    endl <<
    setw (fieldWidth) <<
    "positionInVoice" << " : " <<
    fMeasureElementPositionInVoice <<
    endl <<
    endl;

/* JMI ???
  os << left <<
    setw (fieldWidth) <<
    "(position in measure" << " : ";
  if (fMeasureElementPositionInMeasure.getNumerator () < 0) {
    os << "???)";
  }
  else {
    os << fMeasureElementPositionInMeasure << ")";
  }
  os << endl;
    */

  gIndenter--;

  if (fTupletElementsList.size ()) {
    gIndenter++;

    list<S_msrTupletElement>::const_iterator
      iBegin = fTupletElementsList.begin (),
      iEnd   = fTupletElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;

  // JMI  os << endl;
  }

  os << left <<
    setw (fieldWidth) <<
    "tupletMeasureUpLink" << " : ";
  if (fTupletMeasureUpLink) {
    os <<
      fTupletMeasureUpLink->asShortString ();
  }
  else {
    os << "none";
  }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "tupletTupletUpLink" << " : ";
  if (fTupletTupletUpLink) {
    os <<
      fTupletTupletUpLink->asShortString ();
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

void msrTuplet::printShort (ostream& os)
{
  os <<
    "Tuplet " <<
    fTupletNumber <<
    ", " <<
    fTupletFactor.asString () <<
    ", " <<
    singularOrPlural (
      fTupletElementsList.size (), "element", "elements") <<
    ", whole notes: " <<
    fMeasureElementSoundingWholeNotes << " sounding, " <<
    fTupletDisplayWholeNotes << " displayed" <<
    ", meas "<<
    fMeasureElementMeasureNumber <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 30;

/*
  os << left <<
    setw (fieldWidth) <<
    "tupletBracketKind" << " : " <<
    tupletBracketKindAsString (
      fTupletBracketKind) <<
    endl <<
    setw (fieldWidth) <<
    "tupletLineShapeKind" << " : " <<
    tupletLineShapeKindAsString (
      fTupletLineShapeKind) <<
    endl <<
    setw (fieldWidth) <<
    "tupletShowNumberKind" << " : " <<
    tupletShowNumberKindAsString (
      fTupletShowNumberKind) <<
    endl <<
    setw (fieldWidth) <<
    "tupletShowTypeKind" << " : " <<
    tupletShowTypeKindAsString (
      fTupletShowTypeKind) <<
    endl <<
*/

  os << left <<
    setw (fieldWidth) <<
    "memberNotesSoundingWholeNotes" << " : " <<
    fMemberNotesSoundingWholeNotes <<
    endl <<
    setw (fieldWidth) <<
    "memberNotesDisplayWholeNotes" << " : " <<
    fMemberNotesDisplayWholeNotes <<
    endl <<

    setw (fieldWidth) <<
    "tupletSoundingWholeNotes" << " : " <<
    fMeasureElementSoundingWholeNotes <<
    endl <<
    setw (fieldWidth) <<
    "tupletDisplayWholeNotes" << " : " <<
    fTupletDisplayWholeNotes <<
    endl <<

    setw (fieldWidth) <<
    "tupletMeasureNumber" << " : " <<
    fMeasureElementMeasureNumber <<
    endl <<
    setw (fieldWidth) <<
    "positionInMeasure" << " : " <<
    fMeasureElementPositionInMeasure <<
    endl <<
    setw (fieldWidth) <<
    "positionInVoice" << " : " << fMeasureElementPositionInVoice <<
    endl <<
    endl;

/* JMI ???
  os << left <<
    setw (fieldWidth) <<
    "(position in measure" << " : ";
  if (fMeasureElementPositionInMeasure.getNumerator () < 0) {
    os << "???)";
  }
  else {
    os << fMeasureElementPositionInMeasure << ")";
  }
  os << endl;
    */

  gIndenter--;

  if (fTupletElementsList.size ()) {
  os << left <<
    setw (fieldWidth) <<
      "tupletElements" << " :" <<
      endl;

    gIndenter++;

    list<S_msrTupletElement>::const_iterator
      iBegin = fTupletElementsList.begin (),
      iEnd   = fTupletElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        (*i)->asShortString () <<
        endl;
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;

  // JMI  os << endl;
  }

  os << left <<
    setw (fieldWidth) <<
    "tupletMeasureUpLink" << " : ";
  if (fTupletMeasureUpLink) {
    os <<
      fTupletMeasureUpLink->asShortString ();
  }
  else {
    os << "none";
  }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "tupletTupletUpLink" << " : ";
  if (fTupletTupletUpLink) {
    os <<
      fTupletTupletUpLink->asShortString ();
  }
  else {
    os << "none";
  }
  os << endl;
}

ostream& operator<< (ostream& os, const S_msrTuplet& elt)
{
  elt->print (os);
  return os;
}


}
