/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "msrGraceNotes.h"

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
S_msrGraceNotesGroup msrGraceNotesGroup::create (
  int                    inputLineNumber,
  msrGraceNotesGroupKind graceNotesGroupKind,
  bool                   graceNotesGroupIsSlashed,
  bool                   graceNotesGroupIsBeamed,
  string                 graceNotesGroupMeasureNumber,
  S_msrVoice             graceNotesGroupVoiceUpLink)
{
  msrGraceNotesGroup* o =
    new msrGraceNotesGroup (
      inputLineNumber,
      graceNotesGroupKind,
      graceNotesGroupIsSlashed,
      graceNotesGroupIsBeamed,
      graceNotesGroupMeasureNumber,
      graceNotesGroupVoiceUpLink);
  assert (o!=0);

  return o;
}

msrGraceNotesGroup::msrGraceNotesGroup (
  int                    inputLineNumber,
  msrGraceNotesGroupKind graceNotesGroupKind,
  bool                   graceNotesGroupIsSlashed,
  bool                   graceNotesGroupIsBeamed,
  string                 graceNotesGroupMeasureNumber,
  S_msrVoice             graceNotesGroupVoiceUpLink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msgAssert(
    graceNotesGroupVoiceUpLink != nullptr,
    "graceNotesGroupVoiceUpLink is null");

  fGraceNotesGroupVoiceUpLink =
    graceNotesGroupVoiceUpLink;

  fGraceNotesGroupKind = graceNotesGroupKind;

  fGraceNotesGroupIsTied = false;

  fGraceNotesGroupIsSlashed = graceNotesGroupIsSlashed;
  fGraceNotesGroupIsBeamed = graceNotesGroupIsBeamed;

  fGraceNotesGroupMeasureNumber = graceNotesGroupMeasureNumber;

  // grace notes are followed by notes
  // unless they are last in a measure
  fGraceNotesGroupIsFollowedByNotes = true;
}

msrGraceNotesGroup::~msrGraceNotesGroup ()
{}

S_msrGraceNotesGroup msrGraceNotesGroup::createGraceNotesGroupNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Creating a newborn clone of grace notes group '" <<
      asShortString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  msgAssert(
    containingVoice != nullptr,
    "containingVoice is null");

  S_msrGraceNotesGroup
    newbornClone =
      msrGraceNotesGroup::create (
        fInputLineNumber,
        fGraceNotesGroupKind,
        fGraceNotesGroupIsSlashed,
        fGraceNotesGroupIsBeamed,
        fGraceNotesGroupMeasureNumber,
        containingVoice);

  newbornClone->fGraceNotesGroupIsTied =
    fGraceNotesGroupIsTied;

  newbornClone->fGraceNotesGroupIsFollowedByNotes =
    fGraceNotesGroupIsFollowedByNotes;

  return newbornClone;
}

S_msrPart msrGraceNotesGroup::fetchGraceNotesGroupPartUpLink () const
{
  return
    fGraceNotesGroupVoiceUpLink->
      fetchVoicePartUpLink ();
}

string msrGraceNotesGroup::graceNotesGroupKindAsString (
  msrGraceNotesGroupKind graceNotesGroupKind)
{
  string result;

  switch (graceNotesGroupKind) {
    case msrGraceNotesGroup::kGraceNotesGroupBefore:
      result = "graceNotesGroupBefore";
      break;
    case msrGraceNotesGroup::kGraceNotesGroupAfter:
      result = "graceNotesGroupAfter";
      break;
  } // switch

  return result;
}

S_msrGraceNotesGroup msrGraceNotesGroup::createSkipGraceNotesGroupClone (
  S_msrVoice containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Creating a skip clone of grace notes group '" <<
      asShortString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
    containingVoice != nullptr,
    "containingVoice is null");

  // create the grace notes group
  S_msrGraceNotesGroup
    clone =
      msrGraceNotesGroup::create (
        fInputLineNumber,
        fGraceNotesGroupKind,
        fGraceNotesGroupIsSlashed,
        fGraceNotesGroupIsBeamed,
        fGraceNotesGroupMeasureNumber,
        containingVoice);

  clone->fGraceNotesGroupIsTied =
    fGraceNotesGroupIsTied;

  clone->fGraceNotesGroupIsFollowedByNotes =
    fGraceNotesGroupIsFollowedByNotes;

  // populating the clone with skips
  for (
    list<S_msrMeasureElement>::const_iterator i=fGraceNotesGroupElementsList.begin ();
    i!=fGraceNotesGroupElementsList.end ();
    i++
  ) {
    if (
      S_msrNote note = dynamic_cast<msrNote*>(&(*(*i)))
    ) {
      // create grace skip note with same duration as note
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
        gLogStream <<
          "Creating a skip grace note" <<
          ", soundingWholeNotes: " << note->getNoteSoundingWholeNotes () <<
          ", displayWholeNotes: " << note->getNoteDisplayWholeNotes () <<
          endl;
      }
#endif

      S_msrNote
        skip =
          msrNote::createGraceSkipNote (
            note->            getInputLineNumber (),
            note->            getMeasureElementMeasureNumber (),
            note->            getNoteSoundingWholeNotes (), // 0/1 JMI
            note->            getNoteDisplayWholeNotes (),
            note->            getNoteDotsNumber ());
            /* JMI
            containingVoice-> getRegularVoiceStaffSequentialNumber (), // JMI
            containingVoice-> getVoiceNumber ());
            */

      // append it to the grace notes
      clone->
        appendNoteToGraceNotesGroup (skip);
    }

    else if (
      S_msrChord chord = dynamic_cast<msrChord*>(&(*(*i)))
    ) {
      // fetch the chord's first note
      S_msrNote
        chordFirstNote =
          chord->getChordNotesVector () [0];

      // create grace skip note with same duration as chord
      S_msrNote
        skip =
          msrNote::createGraceSkipNote (
            chordFirstNote->  getInputLineNumber (),
            chordFirstNote->  getMeasureElementMeasureNumber (),
            chordFirstNote->  getNoteSoundingWholeNotes (), // 0/1 JMI
            chordFirstNote->  getNoteDisplayWholeNotes (),
            chordFirstNote->  getNoteDotsNumber ());
            /*
            containingVoice-> getRegularVoiceStaffSequentialNumber (), // JMI
            containingVoice-> getVoiceNumber ());
            */

      // append it to the grace notes
      clone->
        appendNoteToGraceNotesGroup (skip);
    }

    else {
      msrInternalError (
        gGlobalOahOahGroup->getInputSourceName (),
        fInputLineNumber,
        __FILE__, __LINE__,
        "grace notes element should be a note or a chord");
    }
  } // for

  return clone;
}

void msrGraceNotesGroup::appendNoteToGraceNotesGroup (S_msrNote note)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Appending note '" <<
      note->asShortString () <<
      "' to grace notes group" <<
      asShortString () <<
      " in voice \"" <<
      fGraceNotesGroupVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fGraceNotesGroupElementsList.push_back (note);

  // register note's grace notes groups upLink
  note->
    setNoteGraceNotesGroupUpLink (this);

  // is this grace note tied?
  if (note->getNoteTie ()) {
    fGraceNotesGroupIsTied = true;
  }
}

void msrGraceNotesGroup::appendChordToGraceNotesGroup (S_msrChord chord)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Appending chord '" <<
      chord->asShortString () <<
      "' to grace notes group '" <<
      asShortString () <<
      "' in voice \"" <<
      fGraceNotesGroupVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  fGraceNotesGroupElementsList.push_back (chord);
}

S_msrNote msrGraceNotesGroup::removeLastNoteFromGraceNotesGroup (
  int inputLineNumber)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Removing last note from grace notes group '" <<
      asShortString () <<
      "' in voice \"" <<
      fGraceNotesGroupVoiceUpLink->getVoiceName () <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
    fGraceNotesGroupElementsList.size () != 0,
    "fGraceNotesGroupElementsList.size () == 0");

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "Removing last note from grace notes '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  S_msrNote result;

  if (
    S_msrNote note = dynamic_cast<msrNote*>(&(*fGraceNotesGroupElementsList.back ()))
    ) {
    result = note;
  }

  else {
    msrInternalError (
      gGlobalOahOahGroup->getInputSourceName (),
      fInputLineNumber,
      __FILE__, __LINE__,
      "removeLastNoteFromGraceNotesGroup (): grace notes group element should be a note");
  }

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceGraceNotes ()) {
    gLogStream <<
      "This last note from grace notes '" <<
      asString () <<
      "' turns out to be '" <<
      result->asShortString () <<
      "'" <<
      endl;
  }
#endif

  fGraceNotesGroupElementsList.pop_back ();

  return result;
}

void msrGraceNotesGroup::setGraceNotesGroupElementsPositionInMeasure (
  S_msrMeasure measure,
  rational     positionInMeasure)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Setting grace notes group elements position in measure of " << asString () <<
      " to '" <<
      positionInMeasure <<
      "'" <<
      endl;
  }
#endif

/* JMI
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
*/

  // set the grace notes group's elements' position in measure
  if (fGraceNotesGroupElementsList.size ()) {
  /* JMI
  if (false) { // JMI
    // compute chord's position in voice
    rational
       positionInVoice =
        fGraceNotesGroupNoteUpLink->
          getNoteMeasureUpLink ()->
            getMeasurePositionInVoice ()
          +
        positionInMeasure;
}
*/

    string context =
      "setGraceNotesGroupElementsPositionInMeasure()";

    for (
      list<S_msrMeasureElement>::const_iterator i =
        fGraceNotesGroupElementsList.begin ();
      i!=fGraceNotesGroupElementsList.end ();
      i++
    ) {
      S_msrMeasureElement
        measureElement = (*i);

      // set measure element's position in measure
      measureElement->
        setMeasureElementPositionInMeasure (
          positionInMeasure,
          context);

/* JMI
      // set measure element's position in measure
  if (false) { // JMI
      measureElement->
        setMeasureElementPositionInVoice (
          positionInVoice,
          context);
}
*/
    } // for
  }
}

void msrGraceNotesGroup::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrGraceNotesGroup::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrGraceNotesGroup>*
    p =
      dynamic_cast<visitor<S_msrGraceNotesGroup>*> (v)) {
        S_msrGraceNotesGroup elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrGraceNotesGroup::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrGraceNotesGroup::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrGraceNotesGroup::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrGraceNotesGroup>*
    p =
      dynamic_cast<visitor<S_msrGraceNotesGroup>*> (v)) {
        S_msrGraceNotesGroup elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrGraceNotesGroup::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrGraceNotesGroup::browseData (basevisitor* v)
{
  list<S_msrMeasureElement>::const_iterator i;

  for (
    i=fGraceNotesGroupElementsList.begin ();
    i!=fGraceNotesGroupElementsList.end ();
    i++
  ) {
    // browse the element (note or chord)
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for
}

string msrGraceNotesGroup::asShortString () const
{
  stringstream s;

  s <<
    "[GraceNotesGroup" <<
    ", graceNotesGroupKind: " <<
    graceNotesGroupKindAsString (
      fGraceNotesGroupKind) <<
    ", graceNotesGroupMeasureNumber: \"" << fGraceNotesGroupMeasureNumber <<
    "\", ";

  if (fGraceNotesGroupElementsList.size ()) {
    list<S_msrMeasureElement>::const_iterator
      iBegin = fGraceNotesGroupElementsList.begin (),
      iEnd   = fGraceNotesGroupElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i)->asShortString ();
      if (++i == iEnd) break;
      s << ", ";
    } // for
  }

  s <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

string msrGraceNotesGroup::asString () const
{
  stringstream s;

  s <<
    "[GraceNotesGroup" <<
    ", graceNotesGroupKind: " <<
    graceNotesGroupKindAsString (
      fGraceNotesGroupKind) <<
    ", graceNotesGroupMeasureNumber: \"" << fGraceNotesGroupMeasureNumber <<
    "\", line " << fInputLineNumber <<
    ", ";

  if (fGraceNotesGroupElementsList.size ()) {
    list<S_msrMeasureElement>::const_iterator
      iBegin = fGraceNotesGroupElementsList.begin (),
      iEnd   = fGraceNotesGroupElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i)->asString ();
      if (++i == iEnd) break;
      s << ", ";
    } // for
  }

  s << "]";

  return s.str ();
}

void msrGraceNotesGroup::print (ostream& os) const
{
  os <<
    "GraceNotesGroup" <<
    ", line " << fInputLineNumber <<
    ", " <<
    singularOrPlural (
      fGraceNotesGroupElementsList.size (), "element", "elements") <<
    endl;

  gIndenter++;

  const int fieldWidth = 33;

  os <<
    setw (fieldWidth) <<
    "graceNotesGroupVoiceUpLink" << " : ";
    if (fGraceNotesGroupVoiceUpLink) {
      os <<
        fGraceNotesGroupVoiceUpLink->asShortString ();
    }
    else {
      os <<
        "none";
    }
  os << endl;

  os <<
    setw (fieldWidth) <<
    "graceNotesGroupNoteUpLink" << " : ";
    if (fGraceNotesGroupNoteUpLink) {
      os <<
        fGraceNotesGroupNoteUpLink->asShortString ();
    }
    else {
      os <<
        "none";
    }
  os << endl;

  os <<
    setw (fieldWidth) <<
    "graceNotesGroupIsSlashed" << " : " <<
    booleanAsString (fGraceNotesGroupIsSlashed) <<
    endl <<

    setw (fieldWidth) <<
    "graceNotesGroupIsTied" << " : " <<
    booleanAsString (fGraceNotesGroupIsTied) <<
    endl <<

    setw (fieldWidth) <<
    "graceNotesGroupIsBeamed" << " : " <<
    booleanAsString (fGraceNotesGroupIsBeamed) <<
    endl <<

    setw (fieldWidth) <<
    "graceNotesGroupIsFollowedByNotes" << " : " <<
    booleanAsString (fGraceNotesGroupIsFollowedByNotes) <<
    endl <<

    setw (fieldWidth) <<
    "graceNotesGroupMeasureNumber" << " : " <<
    fGraceNotesGroupMeasureNumber <<
    endl;

  os <<
    setw (fieldWidth) <<
    "graceNotesGroupElementsList";
  if (fGraceNotesGroupElementsList.size ()) {
    os << endl;

    gIndenter++;

    list<S_msrMeasureElement>::const_iterator
      iBegin = fGraceNotesGroupElementsList.begin (),
      iEnd   = fGraceNotesGroupElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }
  else {
    os <<
       " : " <<
       "none" <<
      endl;
  }

  gIndenter--;
}

void msrGraceNotesGroup::printShort (ostream& os) const
{
  os <<
    "GraceNotesGroup" <<
    ", line " << fInputLineNumber <<
    ", " <<
    singularOrPlural (
      fGraceNotesGroupElementsList.size (), "element", "elements") <<
    endl;

  gIndenter++;

  const int fieldWidth = 33;

/*
  os <<
    setw (fieldWidth) <<
    "graceNotesGroupVoiceUpLink" << " : ";
    if (fGraceNotesGroupVoiceUpLink) {
      os <<
        fGraceNotesGroupVoiceUpLink->asShortString ();
    }
    else {
      os <<
        "none";
    }
  os << endl;
*/

  os <<
    setw (fieldWidth) <<
    "graceNotesGroupNoteUpLink" << " : ";
    if (fGraceNotesGroupNoteUpLink) {
      os <<
        fGraceNotesGroupNoteUpLink->asShortString ();
    }
    else {
      os <<
        "none";
    }
  os << endl;

  os <<
    setw (fieldWidth) <<
    "graceNotesGroupIsSlashed" << " : " <<
    booleanAsString (fGraceNotesGroupIsSlashed) <<
    endl <<

    setw (fieldWidth) <<
    "graceNotesGroupIsTied" << " : " <<
    booleanAsString (fGraceNotesGroupIsTied) <<
    endl <<

    setw (fieldWidth) <<
    "graceNotesGroupIsBeamed" << " : " <<
    booleanAsString (fGraceNotesGroupIsBeamed) <<
    endl;

/*
  os <<
    setw (fieldWidth) <<
    "graceNotesGroupIsFollowedByNotes" << " : " <<
    booleanAsString (fGraceNotesGroupIsFollowedByNotes) <<
    endl <<

    setw (fieldWidth) <<
    "graceNotesGroupMeasureNumber" << " : " <<
    fGraceNotesGroupMeasureNumber <<
    endl;
*/

  os <<
    setw (fieldWidth) <<
    "graceNotesGroupElementsList";
  if (fGraceNotesGroupElementsList.size ()) {
    os << endl;

    gIndenter++;

    list<S_msrMeasureElement>::const_iterator
      iBegin = fGraceNotesGroupElementsList.begin (),
      iEnd   = fGraceNotesGroupElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      (*i)->printShort (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }
  else {
    os <<
       " : " <<
       "none" <<
      endl;
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrGraceNotesGroup& elt)
{
  elt->print (os);
  return os;
}


}
