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
S_msrAfterGraceNotesGroupContents msrAfterGraceNotesGroupContents::create (
  int        inputLineNumber,
  S_msrVoice afterGraceNotesGroupContentsVoiceUpLink)
{
  msrAfterGraceNotesGroupContents* o =
    new msrAfterGraceNotesGroupContents (
      inputLineNumber,
      afterGraceNotesGroupContentsVoiceUpLink);
  assert(o!=0);

  return o;
}

msrAfterGraceNotesGroupContents::msrAfterGraceNotesGroupContents (
  int        inputLineNumber,
  S_msrVoice afterGraceNotesGroupContentsVoiceUpLink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msgAssert(
    afterGraceNotesGroupContentsVoiceUpLink != nullptr,
    "afterGraceNotesGroupContentsVoiceUpLink is null");

  // set after notes contents's voice upLink
  fAfterGraceNotesGroupContentsVoiceUpLink =
    afterGraceNotesGroupContentsVoiceUpLink;
}

msrAfterGraceNotesGroupContents::~msrAfterGraceNotesGroupContents ()
{}

S_msrPart msrAfterGraceNotesGroupContents::fetchAfterGraceNotesGroupContentsPartUpLink () const
{
  return
    fAfterGraceNotesGroupContentsVoiceUpLink->
      fetchVoicePartUpLink ();
}

S_msrAfterGraceNotesGroupContents msrAfterGraceNotesGroupContents::createAfterGraceNotesGroupContentsNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->fTraceGraceNotes) {
    gLogOstream <<
      "Creating a newborn clone of after grace notes group" <<
      endl;
  }
#endif

  // sanity check
  msgAssert(
    containingVoice != nullptr,
    "containingVoice is null");

  S_msrAfterGraceNotesGroupContents
    newbornClone =
      msrAfterGraceNotesGroupContents::create (
        fInputLineNumber,
        containingVoice);

  return newbornClone;
}

void msrAfterGraceNotesGroupContents::appendNoteToAfterGraceNotesGroupContents (
  S_msrNote note)
{
  fAfterGraceNotesGroupContentsNotesList.push_back (note);
}

void msrAfterGraceNotesGroupContents::appendNoteToAfterGraceNotesGroupContents (
  S_msrChord chord)
{
  fAfterGraceNotesGroupContentsNotesList.push_back (chord);
}

void msrAfterGraceNotesGroupContents::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrAfterGraceNotesGroupContents::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrAfterGraceNotesGroupContents>*
    p =
      dynamic_cast<visitor<S_msrAfterGraceNotesGroupContents>*> (v)) {
        S_msrAfterGraceNotesGroupContents elem = this;

        if (gGlobalMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrAfterGraceNotesGroupContents::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrAfterGraceNotesGroupContents::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrAfterGraceNotesGroupContents::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrAfterGraceNotesGroupContents>*
    p =
      dynamic_cast<visitor<S_msrAfterGraceNotesGroupContents>*> (v)) {
        S_msrAfterGraceNotesGroupContents elem = this;

        if (gGlobalMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrAfterGraceNotesGroupContents::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrAfterGraceNotesGroupContents::browseData (basevisitor* v)
{
  list<S_msrMeasureElement>::const_iterator i;

  for (
    i=fAfterGraceNotesGroupContentsNotesList.begin ();
    i!=fAfterGraceNotesGroupContentsNotesList.end ();
    i++
  ) {
    // browse the note
    msrBrowser<msrMeasureElement> browser (v);
    browser.browse (*(*i));
  } // for
}

string msrAfterGraceNotesGroupContents::asShortString () const
{
  stringstream s;

  s <<
    "AfterGraceNotesGroupContents" <<
    ", " <<
    singularOrPlural (
      fAfterGraceNotesGroupContentsNotesList.size (), "note", "notes");


  if (fAfterGraceNotesGroupContentsNotesList.size ()) {
    list<S_msrMeasureElement>::const_iterator
      iBegin = fAfterGraceNotesGroupContentsNotesList.begin (),
      iEnd   = fAfterGraceNotesGroupContentsNotesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      s << (*i)->asShortString ();
      if (++i == iEnd) break;
      s << " ";
    } // for
  }

  return s.str ();
}

void msrAfterGraceNotesGroupContents::print (ostream& os) const
{
  os <<
    "AfterGraceNotesGroupContents" <<
    ", " <<
    singularOrPlural (
      fAfterGraceNotesGroupContentsNotesList.size (), "note", "notes") <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  if (fAfterGraceNotesGroupContentsNotesList.size ()) {
    list<S_msrMeasureElement>::const_iterator
      iBegin = fAfterGraceNotesGroupContentsNotesList.begin (),
      iEnd   = fAfterGraceNotesGroupContentsNotesList.end (),
      i      = iBegin;

    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
   // JMI   os << endl;
    } // for
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrAfterGraceNotesGroupContents& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrAfterGraceNotesGroup msrAfterGraceNotesGroup::create (
  int          inputLineNumber,
  S_msrElement afterGraceNotesGroupElement,
  bool         afterGraceNotesGroupIsSlashed,
  S_msrVoice   afterGraceNotesGroupVoiceUpLink)
{
  msrAfterGraceNotesGroup* o =
    new msrAfterGraceNotesGroup (
      inputLineNumber,
      afterGraceNotesGroupElement,
      afterGraceNotesGroupIsSlashed,
      afterGraceNotesGroupVoiceUpLink);
  assert(o!=0);

  return o;
}

msrAfterGraceNotesGroup::msrAfterGraceNotesGroup (
  int          inputLineNumber,
  S_msrElement afterGraceNotesGroupElement,
  bool         afterGraceNotesGroupIsSlashed,
  S_msrVoice   afterGraceNotesGroupVoiceUpLink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msgAssert(
    afterGraceNotesGroupVoiceUpLink != nullptr,
    "afterGraceNotesGroupVoiceUpLink is null");

  // set after gracenotes group voice upLink
  fAfterGraceNotesGroupVoiceUpLink =
    afterGraceNotesGroupVoiceUpLink;

  // pupulate this after grace notes group
  fAfterGraceNotesGroupElement =
    afterGraceNotesGroupElement;

  fAfterGraceNotesGroupIsSlashed =
    afterGraceNotesGroupIsSlashed;

  // create the after grace notes contents
  fAfterGraceNotesGroupContents =
    msrAfterGraceNotesGroupContents::create (
      inputLineNumber,
      afterGraceNotesGroupVoiceUpLink);
}

msrAfterGraceNotesGroup::~msrAfterGraceNotesGroup ()
{}

S_msrPart msrAfterGraceNotesGroup::fetchAfterGraceNotesGroupPartUpLink () const
{
  return
    fAfterGraceNotesGroupVoiceUpLink->
      fetchVoicePartUpLink ();
}

S_msrAfterGraceNotesGroup msrAfterGraceNotesGroup::createAfterGraceNotesGroupNewbornClone (
  S_msrNote  noteClone,
  S_msrVoice containingVoice)
{
#ifdef TRACE_OAH
  if (gGlobalTraceOahGroup->fTraceGraceNotes) {
    gLogOstream <<
      "Creating a newborn clone of after grace notes group '" <<
      asShortString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  msgAssert(
    noteClone != nullptr,
    "noteClone is null");

  // sanity check
  msgAssert(
    containingVoice != nullptr,
    "containingVoice is null");

  S_msrAfterGraceNotesGroup
    newbornClone =
      msrAfterGraceNotesGroup::create (
        fInputLineNumber,
        noteClone,
        fAfterGraceNotesGroupIsSlashed,
        containingVoice);

  return newbornClone;
}

void msrAfterGraceNotesGroup::appendNoteToAfterGraceNotesGroupContents (
  S_msrNote note)
{
  fAfterGraceNotesGroupContents->
    appendNoteToAfterGraceNotesGroupContents (
      note);
}

void msrAfterGraceNotesGroup::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrAfterGraceNotesGroup::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrAfterGraceNotesGroup>*
    p =
      dynamic_cast<visitor<S_msrAfterGraceNotesGroup>*> (v)) {
        S_msrAfterGraceNotesGroup elem = this;

        if (gGlobalMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrAfterGraceNotesGroup::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrAfterGraceNotesGroup::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrAfterGraceNotesGroup::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrAfterGraceNotesGroup>*
    p =
      dynamic_cast<visitor<S_msrAfterGraceNotesGroup>*> (v)) {
        S_msrAfterGraceNotesGroup elem = this;

        if (gGlobalMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrAfterGraceNotesGroup::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrAfterGraceNotesGroup::browseData (basevisitor* v)
{
  {
    // browse the afterGraceNotesGroup note
    msrBrowser<msrElement> browser (v);
    browser.browse (*fAfterGraceNotesGroupElement);
  }

  {
    // browse the afterGraceNotesGroupContents
    msrBrowser<msrAfterGraceNotesGroupContents> browser (v);
    browser.browse (*fAfterGraceNotesGroupContents);
  }
}

string msrAfterGraceNotesGroup::asShortString () const
{
  stringstream s;

  s <<
    "AfterGraceNotesGroup " <<
    ", AfterGraceNotesGroupElement: " <<
    "JMI ???fAfterGraceNotesGroupElement->asShortString ()" <<
    ", fAfterGraceNotesGroupContents: " <<
    fAfterGraceNotesGroupContents->asShortString ();

  return s.str ();
}

void msrAfterGraceNotesGroup::print (ostream& os) const
{
  os <<
    "AfterGraceNotesGroup" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  // print the afterGraceNotesGroup element
  os <<
    "Element:" <<
    endl;
  gIndenter++;
  os <<
    fAfterGraceNotesGroupElement;
  gIndenter--;

  os <<
    "afterGraceNotesGroupIsSlashed: " <<
    booleanAsString (fAfterGraceNotesGroupIsSlashed) <<
    endl;

  // print the afterGraceNotesGroup contents
  os <<
    fAfterGraceNotesGroupContents;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrAfterGraceNotesGroup& elt)
{
  elt->print (os);
  return os;
}


}
