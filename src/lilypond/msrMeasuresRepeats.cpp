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

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "msrOptions.h"


using namespace std;

namespace MusicXML2
{

/* JMI
//______________________________________________________________________________
S_msrMeasuresRepeatElement msrMeasuresRepeatElement::create (
  int                 inputLineNumber,
  S_msrMeasuresRepeat measuresRepeatUplink)
{
  msrMeasuresRepeatElement* o =
    new msrMeasuresRepeatElement (
      inputLineNumber,
      measuresRepeatUplink);
  assert(o!=0);
  return o;
}

msrMeasuresRepeatElement::msrMeasuresRepeatElement (
  int                 inputLineNumber,
  S_msrMeasuresRepeat measuresRepeatUplink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert (
    measuresRepeatUplink != nullptr,
    "measuresRepeatUplink is null");

  fMeasuresRepeatElementMeasuresRepeatUplink = measuresRepeatUplink;
}

msrMeasuresRepeatElement::~msrMeasuresRepeatElement ()
{}

void msrMeasuresRepeatElement::appendSegmentToMeasuresRepeatElementsList ( // JMI ???
  int          inputLineNumber,
  S_msrSegment segment,
  string       context)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending segment '" << segment <<
      "' to measuresRepeat element elements list '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    segment != nullptr,
    "segment is null");

  fMeasuresRepeatElementElementsList.push_back (segment);
}

void msrMeasuresRepeatElement::appendMeasuresRepeatToMeasuresRepeatElementsList (
  int                  inputLineNumber,
  S_msrMeasuresRepeat  measuresRepeat,
  string               context)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending measuresRepeat '" << measuresRepeat <<
      "' to measuresRepeat element elements list '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    measuresRepeat != nullptr,
    "measuresRepeat is null");

  fMeasuresRepeatElementElementsList.push_back (measuresRepeat);
}

void msrMeasuresRepeatElement::appendVoiceElementToMeasuresRepeatElementsList (
  int               inputLineNumber,
  S_msrVoiceElement voiceElement,
  string            context)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceVoices) {
    gLogIOstream <<
      "Appending voice element '" << voiceElement <<
      "' to measuresRepeat element elements list '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    voiceElement != nullptr,
    "voiceElement is null");

  fMeasuresRepeatElementElementsList.push_back (voiceElement);
}

S_msrNote msrMeasuresRepeatElement::fetchMeasuresRepeatElementFirstNonGraceNote () const
{

  S_msrNote result;

  // fetch the first note in the first measure to which
  // a grace notes group can be attached
  // i.e. one not in a grace notes group itself,
  // possibly inside a chord or tuplet

  if (fMeasuresRepeatElementElementsList.size ()) {
    list<S_msrVoiceElement>::const_iterator
      iBegin = fMeasuresRepeatElementElementsList.begin (),
      iEnd   = fMeasuresRepeatElementElementsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      S_msrVoiceElement element = (*i);

      if (
        S_msrNote note = dynamic_cast<msrNote*>(&(*element))
        ) {
        result = note;
        break;
      }

      else if (
        S_msrChord chord = dynamic_cast<msrChord*>(&(*element))
        ) {
        // get the chord's first note
        result = chord->fetchChordFirstNonGraceNote ();
        break;
      }

      else if (
        S_msrTuplet tuplet = dynamic_cast<msrTuplet*>(&(*element))
        ) {
        // get the tuplet's first note
        result = tuplet->fetchTupletFirstNonGraceNote ();
        break;
      }

      else if (
        S_msrClef clef = dynamic_cast<msrClef*>(&(*element))
        ) {
        // ignore this clef
      }

      else if (
        S_msrKey key = dynamic_cast<msrKey*>(&(*element))
        ) {
        // ignore this key
      }

      else if (
        S_msrTime time = dynamic_cast<msrTime*>(&(*element))
        ) {
        // ignore this time
      }

      else {
        stringstream s;

        s <<
          "tuplet first element should be a note, a chord or another tuplet, found instead '" <<
          element->asShortString () <<
          "'";

        msrInternalError (
          gGeneralOptions->fInputSourceName,
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

      if (++i == iEnd) break;
    } // for
  }

  return result;
}

void msrMeasuresRepeatElement::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeatElement::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrMeasuresRepeatElement>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeatElement>*> (v)) {
        S_msrMeasuresRepeatElement elem = this;

        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasuresRepeatElement::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasuresRepeatElement::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeatElement::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMeasuresRepeatElement>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeatElement>*> (v)) {
        S_msrMeasuresRepeatElement elem = this;

        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasuresRepeatElement::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasuresRepeatElement::browseData (basevisitor* v)
{
  // browse the elements
  if (fMeasuresRepeatElementElementsList.size ()) {
    for (
      list<S_msrVoiceElement>::const_iterator i = fMeasuresRepeatElementElementsList.begin ();
      i != fMeasuresRepeatElementElementsList.end ();
      i++
  ) {
      // browse the element
      msrBrowser<msrVoiceElement> browser (v);
      browser.browse (*(*i));
    } // for
  }
}

string msrMeasuresRepeatElement::asString () const
{
  stringstream s;

  s <<
    "MeasuresRepeatElement" <<
    ", measuresRepeat uplink: '" <<
    fMeasuresRepeatElementMeasuresRepeatUplink->
      asShortString () <<
    "', line " << fInputLineNumber <<
    endl;

  return s.str ();
}

void msrMeasuresRepeatElement::print (ostream& os)
{
  os <<
    "MeasuresRepeatElement" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  os <<
    "measuresRepeat uplink: '" <<
    fMeasuresRepeatElementMeasuresRepeatUplink->
      asShortString () <<
      "'" <<
    endl <<
    endl;

  // print the elements
  int elementsNumber =
    fMeasuresRepeatElementElementsList.size ();

  os <<
    "measuresRepeatElementElementsList: ";
  if (elementsNumber) {
    os <<
      "(" <<
      singularOrPlural (
        elementsNumber, "element", "elements") <<
      ")";
  }
  else {
    os << "none";
  }
  os <<
    endl;

  if (elementsNumber) {
    os <<
      endl;

    gIndenter++;

    list<S_msrVoiceElement>::const_iterator
      iBegin = fMeasuresRepeatElementElementsList.begin (),
      iEnd   = fMeasuresRepeatElementElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the element
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }

  gIndenter--;
}

void msrMeasuresRepeatElement::shortPrint (ostream& os)
{
  os <<
    "MeasuresRepeatElement" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

/ / * JMI
  os <<
    "measuresRepeat uplink: '" <<
    fMeasuresRepeatElementMeasuresRepeatUplink->
      asShortString () <<
      "'" <<
    endl;
* /

  // print the elements
  int elementsNumber =
    fMeasuresRepeatElementElementsList.size ();

  os <<
    "repeatElementElementsList: ";
  if (elementsNumber) {
    os <<
      "(" <<
      singularOrPlural (
        elementsNumber, "element", "elements") <<
      ")";
  }
  else {
    os << "none";
  }
  os <<
    endl;

  if (elementsNumber) {
    os <<
      endl;

    gIndenter++;

    list<S_msrVoiceElement>::const_iterator
      iBegin = fMeasuresRepeatElementElementsList.begin (),
      iEnd   = fMeasuresRepeatElementElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the element
      (*i)->shortPrint (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrMeasuresRepeatElement& elt)
{
  elt->print (os);
  return os;
}
*/

//______________________________________________________________________________
S_msrMeasuresRepeatPattern msrMeasuresRepeatPattern::create (
  int                 inputLineNumber,
  S_msrMeasuresRepeat measuresRepeatUplink)
{
  msrMeasuresRepeatPattern* o =
    new msrMeasuresRepeatPattern (
      inputLineNumber,
      measuresRepeatUplink);
  assert(o!=0);
  return o;
}

msrMeasuresRepeatPattern::msrMeasuresRepeatPattern (
  int                 inputLineNumber,
  S_msrMeasuresRepeat measuresRepeatUplink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert (
    measuresRepeatUplink != nullptr,
    "measuresRepeatUplink is null");

  fMeasuresRepeatUplink = measuresRepeatUplink;
}

msrMeasuresRepeatPattern::~msrMeasuresRepeatPattern ()
{}

void msrMeasuresRepeatPattern::setMeasuresRepeatPatternSegment (
    S_msrSegment measuresRepeatPatternSegment)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Setting measures repeat pattern segment containing " <<
      singularOrPlural (
          measuresRepeatPatternMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    measuresRepeatPatternSegment != nullptr,
    "measuresRepeatPatternSegment is null");

  fMeasuresRepeatPatternSegment =
    measuresRepeatPatternSegment;
}

int msrMeasuresRepeatPattern::measuresRepeatPatternMeasuresNumber () const
{
  int result;

  if (fMeasuresRepeatPatternSegment) {
    result =
      fMeasuresRepeatPatternSegment->
      getSegmentMeasuresList ().size ();
  }
  else {
    result = 0;
  }

  return result;
}

void msrMeasuresRepeatPattern::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeatPattern::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrMeasuresRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeatPattern>*> (v)) {
        S_msrMeasuresRepeatPattern elem = this;

        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasuresRepeatPattern::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasuresRepeatPattern::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeatPattern::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMeasuresRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeatPattern>*> (v)) {
        S_msrMeasuresRepeatPattern elem = this;

        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasuresRepeatPattern::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasuresRepeatPattern::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeatPattern::browseData ()" <<
      endl;
  }

  if (fMeasuresRepeatPatternSegment) {
    // browse the pattern segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fMeasuresRepeatPatternSegment);
  }
}

string msrMeasuresRepeatPattern::asString () const
{
  stringstream s;

  s <<

    "MeasuresRepeatPattern" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      measuresRepeatPatternMeasuresNumber (),
      "repeated measure",
      "repeated measures") <<
    ")";

  return s.str ();
}

void msrMeasuresRepeatPattern::print (ostream& os)
{
  os <<
    endl <<
    asString () <<
    endl <<
    endl;

  gIndenter++;

  // print the pattern segment
  os <<
    "Pattern segment:";

  if (! fMeasuresRepeatPatternSegment) {
    os <<
      " none" <<
      endl;
  }
  else {
    os <<
      endl;

    gIndenter++;

    os <<
      fMeasuresRepeatPatternSegment;

    gIndenter--;
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrMeasuresRepeatPattern& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrMeasuresRepeatReplicas msrMeasuresRepeatReplicas::create (
  int                 inputLineNumber,
  S_msrMeasuresRepeat measuresRepeatUplink)
{
  msrMeasuresRepeatReplicas* o =
    new msrMeasuresRepeatReplicas (
      inputLineNumber,
      measuresRepeatUplink);
  assert(o!=0);
  return o;
}

msrMeasuresRepeatReplicas::msrMeasuresRepeatReplicas (
  int                 inputLineNumber,
  S_msrMeasuresRepeat measuresRepeatUplink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert (
    measuresRepeatUplink != nullptr,
    "measuresRepeatUplink is null");

  fMeasuresRepeatUplink = measuresRepeatUplink;
}

msrMeasuresRepeatReplicas::~msrMeasuresRepeatReplicas ()
{}

void msrMeasuresRepeatReplicas::setMeasuresRepeatReplicasSegment (
  S_msrSegment measuresRepeatReplicasSegment)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Setting measures repeat replicas segment containing " <<
      singularOrPlural (
          measuresRepeatReplicasMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    measuresRepeatReplicasSegment != nullptr,
    "measuresRepeatReplicasSegment is null");

  fMeasuresRepeatReplicasSegment =
    measuresRepeatReplicasSegment;
}

int msrMeasuresRepeatReplicas::measuresRepeatReplicasMeasuresNumber () const
{
  int result;

  if (fMeasuresRepeatReplicasSegment) {
    result =
      fMeasuresRepeatReplicasSegment->
      getSegmentMeasuresList ().size ();
  }
  else {
    result = 0;
  }

  return result;
}

void msrMeasuresRepeatReplicas::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeatReplicas::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrMeasuresRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeatReplicas>*> (v)) {
        S_msrMeasuresRepeatReplicas elem = this;

        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasuresRepeatReplicas::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasuresRepeatReplicas::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeatReplicas::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMeasuresRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeatReplicas>*> (v)) {
        S_msrMeasuresRepeatReplicas elem = this;

        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasuresRepeatReplicas::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasuresRepeatReplicas::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeatReplicas::browseData ()" <<
      endl;
  }

  if (fMeasuresRepeatReplicasSegment) {
  // browse the replicas segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fMeasuresRepeatReplicasSegment);
  }
}

string msrMeasuresRepeatReplicas::asString () const
{
  stringstream s;

  s <<

    "MeasuresRepeatReplicas" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      measuresRepeatReplicasMeasuresNumber (),
      "replicas measure",
      "replicas measures") <<
    ")";

  return s.str ();
}

void msrMeasuresRepeatReplicas::print (ostream& os)
{
  os <<
    endl <<
    asString () <<
    endl <<
    endl;

  gIndenter++;

  // print the replicas segment
  os <<
    "Replicas segment:";

  if (! fMeasuresRepeatReplicasSegment) {
    os <<
      " none" <<
      endl;
  }
  else {
    os <<
      endl;

    gIndenter++;

    os <<
      fMeasuresRepeatReplicasSegment;

    gIndenter--;
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrMeasuresRepeatReplicas& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrMeasuresRepeat msrMeasuresRepeat::create (
  int        inputLineNumber,
  int        measuresRepeatMeasuresNumber,
  int        measuresRepeatSlashesNumber,
  S_msrVoice voiceUplink)
{
  msrMeasuresRepeat* o =
    new msrMeasuresRepeat (
      inputLineNumber,
      measuresRepeatMeasuresNumber,
      measuresRepeatSlashesNumber,
      voiceUplink);
  assert(o!=0);
  return o;
}

msrMeasuresRepeat::msrMeasuresRepeat (
  int        inputLineNumber,
  int        measuresRepeatMeasuresNumber,
  int        measuresRepeatSlashesNumber,
  S_msrVoice voiceUplink)
    : msrVoiceElement (inputLineNumber)
{
  // sanity check
  msrAssert (
    measuresRepeatMeasuresNumber > 0,
    "measuresRepeatMeasuresNumber is not positive");

  fMeasuresRepeatMeasuresNumber = measuresRepeatMeasuresNumber;

  // sanity check
  msrAssert (
    measuresRepeatSlashesNumber > 0,
    "measuresRepeatSlashesNumber is not positive");

  fMeasuresRepeatSlashesNumber  = measuresRepeatSlashesNumber;

  fMeasuresRepeatVoiceUplink = voiceUplink;

  // measures repeat build phase
  fCurrentMeasuresRepeatBuildPhaseKind =
    msrMeasuresRepeat::kMeasuresRepeatBuildPhaseJustCreated;
}

msrMeasuresRepeat::~msrMeasuresRepeat ()
{}

S_msrMeasuresRepeat msrMeasuresRepeat::createMeasuresRepeatNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasuresRepeats) {
    gLogIOstream <<
      "Creating a newborn clone of measures repeat '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");

  S_msrMeasuresRepeat
    newbornClone =
      msrMeasuresRepeat::create (
        fInputLineNumber,
        fMeasuresRepeatMeasuresNumber,
        fMeasuresRepeatSlashesNumber,
        containingVoice);

  return newbornClone;
}

void msrMeasuresRepeat::setMeasuresRepeatPattern (
  S_msrMeasuresRepeatPattern measuresRepeatPattern)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Setting measures repeat pattern containing " <<
      singularOrPlural (
        measuresRepeatPattern->
          measuresRepeatPatternMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    measuresRepeatPattern != nullptr,
    "measuresRepeatPattern is null");

  fMeasuresRepeatPattern = measuresRepeatPattern;

  // set currentRepeat's build phase
  fCurrentMeasuresRepeatBuildPhaseKind =
    msrMeasuresRepeat::kMeasuresRepeatBuildPhaseInPattern;
}

void msrMeasuresRepeat::setMeasuresRepeatReplicas (
  S_msrMeasuresRepeatReplicas measuresRepeatReplicas)
{
#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceRepeats) {
    gLogIOstream <<
      "Setting measures repeat replicas containing " <<
      singularOrPlural (
        measuresRepeatReplicas->
          measuresRepeatReplicasMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    measuresRepeatReplicas != nullptr,
    "measuresRepeatReplicas is null");

  fMeasuresRepeatReplicas = measuresRepeatReplicas;

  // set currentRepeat's build phase
  fCurrentMeasuresRepeatBuildPhaseKind =
    msrMeasuresRepeat::kMeasuresRepeatBuildPhaseInReplicas;
}

int msrMeasuresRepeat::measuresRepeatReplicasNumber () const
{
  int patternMeasuresNumber =
    measuresRepeatPatternMeasuresNumber ();

  // sanity check
  msrAssert (
    patternMeasuresNumber > 0,
    "patternMeasuresNumber is not positive");

  return
    measuresRepeatReplicasMeasuresNumber ()
      /
    patternMeasuresNumber;
}

void msrMeasuresRepeat::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeat::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrMeasuresRepeat>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeat>*> (v)) {
        S_msrMeasuresRepeat elem = this;

        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasuresRepeat::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasuresRepeat::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeat::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMeasuresRepeat>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeat>*> (v)) {
        S_msrMeasuresRepeat elem = this;

        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrMeasuresRepeat::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasuresRepeat::browseData (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrMeasuresRepeat::browseData ()" <<
      endl;
  }

  if (fMeasuresRepeatPattern) {
  // browse the measdure repeat pattern
    msrBrowser<msrMeasuresRepeatPattern> browser (v);
    browser.browse (*fMeasuresRepeatPattern);
  }

  // fetch the score
  S_msrScore
    score =
      fMeasuresRepeatVoiceUplink->
        fetchVoicePartUplink ()->
          getPartPartGroupUplink ()->
            getPartGroupScoreUplink ();

  bool inhibitMeasuresRepeatReplicasBrowsing =
    score->getInhibitMeasuresRepeatReplicasBrowsing ();

  if (inhibitMeasuresRepeatReplicasBrowsing) {
    if (
      gMsrOptions->fTraceMsrVisitors
#ifdef TRACE_OPTIONS
        ||
      gTraceOptions->fTraceRepeats
#endif
    ) {
      gLogIOstream <<
        "% ==> visiting measures repeat replicas is inhibited" <<
        endl;
    }
  }

  if (fMeasuresRepeatReplicas) {
    if (! inhibitMeasuresRepeatReplicasBrowsing) {
      // browse the measures repeat replicas
      msrBrowser<msrMeasuresRepeatReplicas> browser (v);
      browser.browse (*fMeasuresRepeatReplicas);
    }
  }
}

string msrMeasuresRepeat::measuresRepeatBuildPhaseKindAsString (
  msrMeasuresRepeatBuildPhaseKind measuresRepeatBuildPhaseKind)
{
  string result;

  switch (measuresRepeatBuildPhaseKind) {
    case msrMeasuresRepeat::kMeasuresRepeatBuildPhaseJustCreated:
      result = "measuresRepeatBuildPhaseJustCreated";
      break;
    case msrMeasuresRepeat::kMeasuresRepeatBuildPhaseInPattern:
      result = "measuresRepeatBuildPhaseInPattern";
      break;
    case msrMeasuresRepeat::kMeasuresRepeatBuildPhaseInReplicas:
      result = "measuresRepeatBuildPhaseInReplicas";
      break;
    case msrMeasuresRepeat::kMeasuresRepeatBuildPhaseCompleted:
      result = "measuresRepeatBuildPhaseCompleted";
      break;
  } // switch

  return result;
}

string msrMeasuresRepeat::asString () const
{
  stringstream s;

  s <<

    "MeasuresRepeat" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      measuresRepeatPatternMeasuresNumber (),
      "repeated measure",
      "repeated measures") <<
    ", " <<
    singularOrPlural (
      measuresRepeatReplicasMeasuresNumber (),
      "replicas measure",
      "replicas measures") <<
    ", " <<
    measuresRepeatReplicasNumber () << " replicas" <<
    ")";

  return s.str ();
}

void msrMeasuresRepeat::displayMeasuresRepeat (
  int    inputLineNumber,
  string context)
{
  gLogIOstream <<
    endl <<
    "*********>> MeasuresRepeat " <<
    ", measuresRepeatMeasuresNumber: '" <<
    fMeasuresRepeatMeasuresNumber <<
    ", measuresRepeatSlashesNumber: '" <<
    fMeasuresRepeatSlashesNumber <<
    "', voice:" <<
    endl <<
    fMeasuresRepeatVoiceUplink->getVoiceName () <<
    " (" << context << ")" <<
    ", line " << inputLineNumber <<
    " contains:" <<
    endl;

  gIndenter++;
  print (gLogIOstream);
  gIndenter--;

  gLogIOstream <<
    "<<*********" <<
    endl <<
    endl;
}

void msrMeasuresRepeat::print (ostream& os)
{
  os <<
    "MeasuresRepeat" <<
    ", line " << fInputLineNumber <<
    /* JMI ???
    " (" <<
    singularOrPlural (
      fMeasuresRepeatPattern
        ? measuresRepeatPatternMeasuresNumber ()
        : 0,
      "pattern measure",
      "pattern measures") <<
    ", " <<
    singularOrPlural (
      fMeasuresRepeatPattern
        ? measuresRepeatReplicasMeasuresNumber ()
        : 0,
      "replica measure",
      "replicas measures") <<
    ")" <<
    */
    endl;

  gIndenter++;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceMeasuresRepeats) {
    // print the current measures repeat build phase
    const int fieldWidth = 36;

    os <<
      setw (fieldWidth) <<
      "currentMeasuresRepeatBuildPhaseKind" << " : " <<
      measuresRepeatBuildPhaseKindAsString (
        fCurrentMeasuresRepeatBuildPhaseKind) <<
      endl;
  }
#endif

  // print the measures repeat pattern
  if (! fMeasuresRepeatPattern) {
    os <<
      "measures repeat pattern: none" <<
      endl;
  }

  else {
    os <<
      fMeasuresRepeatPattern;
  }

  // print the measures repeat replicas
  if (! fMeasuresRepeatReplicas) {
    os <<
      "measures repeat replicas: none" <<
      endl;
  }

  else {
    os <<
      fMeasuresRepeatReplicas;
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrMeasuresRepeat& elt)
{
  elt->print (os);
  return os;
}


}
