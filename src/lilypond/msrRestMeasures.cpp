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
S_msrRestMeasuresContents msrRestMeasuresContents::create (
  int               inputLineNumber,
  S_msrRestMeasures restMeasures)
{
  msrRestMeasuresContents* o =
    new msrRestMeasuresContents (
      inputLineNumber,
      restMeasures);
  assert(o!=0);
  return o;
}

msrRestMeasuresContents::msrRestMeasuresContents (
  int              inputLineNumber,
  S_msrRestMeasures restMeasures)
    : msrElement (inputLineNumber)
{
  fRestMeasuresContentsRestMeasuresUpLink =
    restMeasures;
}

msrRestMeasuresContents::~msrRestMeasuresContents ()
{}

S_msrRestMeasuresContents msrRestMeasuresContents::createRestMeasuresContentsNewbornClone (
  S_msrRestMeasures restMeasures)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    gLogOstream <<
      "Creating a newborn clone of a rest measures contents" <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    restMeasures != nullptr,
    "restMeasures is null");

  S_msrRestMeasuresContents
    newbornClone =
      msrRestMeasuresContents::create (
        fInputLineNumber,
        restMeasures);

  return newbornClone;
}

void msrRestMeasuresContents::setRestMeasuresContentsSegment (
  int          inputLineNumber,
  S_msrSegment restMeasuresContentsSegment)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures || gTraceOah->fTraceSegments) {
    gLogOstream <<
      "Setting rest measures contents segment containing " <<
      singularOrPlural (
        restMeasuresContentsMeasuresNumber (),
        "measure",
        "measures") <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    restMeasuresContentsSegment != nullptr,
    "restMeasuresContentsSegment is null");

  fRestMeasuresContentsSegment =
    restMeasuresContentsSegment;
}

int msrRestMeasuresContents::restMeasuresContentsMeasuresNumber () const
{
  int result;

  if (fRestMeasuresContentsSegment) {
    result =
      fRestMeasuresContentsSegment->
      getSegmentMeasuresList ().size ();
  }
  else {
    result = 0;
  }

  return result;
}

void msrRestMeasuresContents::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrRestMeasuresContents::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrRestMeasuresContents>*
    p =
      dynamic_cast<visitor<S_msrRestMeasuresContents>*> (v)) {
        S_msrRestMeasuresContents elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrRestMeasuresContents::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrRestMeasuresContents::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrRestMeasuresContents::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrRestMeasuresContents>*
    p =
      dynamic_cast<visitor<S_msrRestMeasuresContents>*> (v)) {
        S_msrRestMeasuresContents elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrRestMeasuresContents::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrRestMeasuresContents::browseData (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrRestMeasuresContents::browseData ()" <<
      endl;
  }

  if (fRestMeasuresContentsSegment) {
    // browse the pattern segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fRestMeasuresContentsSegment);
  }
}

string msrRestMeasuresContents::asString () const
{
  stringstream s;

  s <<

    "RestMeasuresContents" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      restMeasuresContentsMeasuresNumber (),
      "contents measure",
      "contents measures") <<
    ")";

  return s.str ();
}

void msrRestMeasuresContents::print (ostream& os) const
{
  os <<
    endl <<
    asString () <<
    endl;

  gIndenter++;

  // print the pattern segment
  os <<
    "Contents segment:";

  if (! fRestMeasuresContentsSegment) {
    os <<
      " none" <<
      endl;
  }
  else {
    os << endl;

    gIndenter++;

    os <<
      fRestMeasuresContentsSegment;

    gIndenter--;
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrRestMeasuresContents& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrRestMeasures msrRestMeasures::create (
  int        inputLineNumber,
  rational   restMeasuresMeasureSoundingNotes,
  int        restMeasuresNumber,
  S_msrVoice voiceUpLink)
{
  msrRestMeasures* o =
    new msrRestMeasures (
      inputLineNumber,
      restMeasuresMeasureSoundingNotes,
      restMeasuresNumber,
      voiceUpLink);
  assert(o!=0);
  return o;
}

S_msrRestMeasures msrRestMeasures::create (
  int          inputLineNumber,
  S_msrMeasure restMeasureClone,
  S_msrVoice   voiceUpLink)
{
  msrRestMeasures* o =
    new msrRestMeasures (
      inputLineNumber,
      restMeasureClone,
      voiceUpLink);
  assert(o!=0);
  return o;
}

msrRestMeasures::msrRestMeasures (
  int        inputLineNumber,
  rational   restMeasuresMeasureSoundingNotes,
  int        restMeasuresNumber,
  S_msrVoice voiceUpLink)
    : msrVoiceElement (inputLineNumber)
{
  fRestMeasuresVoiceUpLink = voiceUpLink;

  fRestMeasuresMeasureSoundingNotes = restMeasuresMeasureSoundingNotes;

  fRestMeasuresNumber = restMeasuresNumber;

  fRestMeasuresLastMeasurePuristNumber = -1;
}

msrRestMeasures::msrRestMeasures (
  int          inputLineNumber,
  S_msrMeasure restMeasureClone,
  S_msrVoice   voiceUpLink)
    : msrVoiceElement (inputLineNumber)
{
  fRestMeasuresVoiceUpLink = voiceUpLink;

  fRestMeasuresMeasureSoundingNotes =
    restMeasureClone->
      getFullMeasureWholeNotesDuration (); // JMI ???

  fRestMeasuresNumber = 1; // will evolve JMI

  fRestMeasuresLastMeasurePuristNumber = -1;

  // create the rest measures contents
  fRestMeasuresContents =
    msrRestMeasuresContents::create (
      inputLineNumber,
      this);
}

msrRestMeasures::~msrRestMeasures ()
{}

S_msrRestMeasures msrRestMeasures::createRestMeasuresNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    gLogOstream <<
      "Creating a newborn clone of rest measures '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");

  S_msrRestMeasures
    newbornClone =
      msrRestMeasures::create (
        fInputLineNumber,
        fRestMeasuresMeasureSoundingNotes,
        fRestMeasuresNumber,
        containingVoice);

/* JMI
  newbornClone->fRestMeasuresNextMeasureNumber =
    fRestMeasuresNextMeasureNumber;
    */

  return newbornClone;
}

void msrRestMeasures::setRestMeasuresContents (
  S_msrRestMeasuresContents restMeasuresContents)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    gLogOstream <<
      "Setting rest measures contents containing " <<
      singularOrPlural (
        restMeasuresContents->
          restMeasuresContentsMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    restMeasuresContents != nullptr,
    "restMeasuresContents is null");

  fRestMeasuresContents = restMeasuresContents;
}

void msrRestMeasures::setRestMeasuresNextMeasureNumber (
  string nextMeasureNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    gLogOstream <<
      "Setting rest measures next measure number to '" <<
      "' " <<
      nextMeasureNumber <<
      endl;
  }
#endif

  fRestMeasuresNextMeasureNumber =
    nextMeasureNumber;
}

void msrRestMeasures::setRestMeasuresLastMeasurePuristMeasureNumber (
  int inputLineNumber)
{
  // sanity check
  msrAssert (
    fRestMeasuresContents != nullptr,
    "fRestMeasuresContents is null");

  S_msrSegment
    restMeasuresContentsSegment =
      fRestMeasuresContents->
        getRestMeasuresContentsSegment ();

  // sanity check
  msrAssert (
    restMeasuresContentsSegment != nullptr,
    "restMeasuresContentsSegment is null");

  // get rest measures contents segment measures list
  const list<S_msrMeasure>&
    contentsSegmentMeasuresList =
      restMeasuresContentsSegment->
        getSegmentMeasuresList ();

  // get rest measures contents last measure's purist number
  int lastMeasuresPuristNumber = -1;

  if (contentsSegmentMeasuresList.size ()) {
    lastMeasuresPuristNumber =
      contentsSegmentMeasuresList.back ()->
        getMeasurePuristNumber ();
  }
  else {
    stringstream s;

    s <<
      "cannot get rest measures contents last measure purist number" <<
      " because its measures list is empty" <<
      " in voice clone '" <<
      asShortString () <<
      "' ";

    msrInternalError (
      gOahOah->fInputSourceName,
      fInputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    gLogOstream <<
      "Setting rest measures last measure purist number to '" <<
      "' " <<
      lastMeasuresPuristNumber <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  fRestMeasuresLastMeasurePuristNumber =
    lastMeasuresPuristNumber;
}

void msrRestMeasures::appendMeasureCloneToRestMeasures (
  S_msrMeasure measureClone)
{
  fRestMeasuresContents->
    getRestMeasuresContentsSegment ()->
      appendMeasureToSegment (
        measureClone);
}

void msrRestMeasures::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrRestMeasures::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrRestMeasures>*
    p =
      dynamic_cast<visitor<S_msrRestMeasures>*> (v)) {
        S_msrRestMeasures elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrRestMeasures::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrRestMeasures::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrRestMeasures::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrRestMeasures>*
    p =
      dynamic_cast<visitor<S_msrRestMeasures>*> (v)) {
        S_msrRestMeasures elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrRestMeasures::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrRestMeasures::browseData (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrRestMeasures::browseData ()" <<
      endl;
  }

/* JMI
  // get the part upLink
  S_msrPart
    partUpLink =
      fRestMeasuresVoiceUpLink->
        fetchVoicePartUpLink ();

  // get the part group upLink
  S_msrPartGroup
    partGroupUpLink =
      partUpLink->
        getPartPartGroupUpLink ();

  // get the score upLink
  S_msrScore
    scoreUpLink =
      partGroupUpLink->
        getPartGroupScoreUpLink ();

  bool inhibitRestMeasuresBrowsing =
    scoreUpLink->
      getInhibitRestMeasuresBrowsing ();

  if (inhibitRestMeasuresBrowsing) {
    if (gMsrOah->fTraceMsrVisitors || gTraceOah->fTraceRestMeasures) {
      gLogOstream <<
        "% ==> visiting rest measures is inhibited" <<
        endl;
    }
  }
*/

  if (fRestMeasuresContents) {
 // JMI   if (! inhibitRestMeasuresBrowsing) {
      // browse the rest measures contents
      msrBrowser<msrRestMeasuresContents> browser (v);
      browser.browse (*fRestMeasuresContents);
//    }
  }
}

string msrRestMeasures::asString () const
{
  stringstream s;

  s <<
    "RestMeasures" <<
    ", restMeasuresLastMeasurePuristNumber: '" <<
    fRestMeasuresLastMeasurePuristNumber <<
    "'" <<
    ", restMeasuresMeasureSoundingNotes: " <<
    fRestMeasuresMeasureSoundingNotes <<
    ", " <<
    singularOrPlural (
      fRestMeasuresNumber,
        "rest measure",
        "rest measures") <<
    ", restMeasuresNextMeasureNumber: '" <<
    fRestMeasuresNextMeasureNumber <<
    "'" <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrRestMeasures::displayRestMeasures (
  int    inputLineNumber,
  string context)
{
  gLogOstream <<
    endl <<
    "*********>> Rest measures " << context << " \"" <<
 // JMI   getVoiceName () <<
    "\"" <<
    ", line " << inputLineNumber <<
    " contains:" <<
    endl;

  gIndenter++;
  print (gLogOstream);
  gIndenter--;

  gLogOstream <<
    "<<*********" <<
    endl <<
    endl;
}

void msrRestMeasures::print (ostream& os) const
{
  os <<
    "RestMeasures" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 36;

  os << left <<
    setw (fieldWidth) <<
    "restMeasuresLastMeasurePuristNumber" << " : " <<
    fRestMeasuresLastMeasurePuristNumber <<
    endl <<
    setw (fieldWidth) <<
    "restMeasuresMeasureSoundingNotes" << " : " <<
    fRestMeasuresMeasureSoundingNotes <<
    endl <<
    setw (fieldWidth) <<
    "restMeasuresNumber" << " : " <<
    fRestMeasuresNumber <<
    endl <<
    setw (fieldWidth) <<
    "restMeasuresNextMeasureNumber" << " : '" <<
    fRestMeasuresNextMeasureNumber <<
    "'" <<
    endl;

  // print the voice upLink
  os << left <<
    setw (fieldWidth) <<
    "restMeasuresVoiceUpLink" << " : " <<
    "\"" <<
    fRestMeasuresVoiceUpLink->getVoiceName () <<
    "\"" <<
    endl;

  // print the rests contents
  if (! fRestMeasuresContents) {
    os << left <<
      setw (fieldWidth) <<
      "restMeasuresContents" << " : " << "none" <<
      endl;
  }

  else {
    os <<
      fRestMeasuresContents;
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrRestMeasures& elt)
{
  elt->print (os);
  return os;
}


}
