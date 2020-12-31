/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iomanip>

#include "messagesHandling.h"

#include "msrMeasuresRepeats.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

/* JMI
//______________________________________________________________________________
S_msrMeasuresRepeatElement msrMeasuresRepeatElement::create (
  int                 inputLineNumber,
  S_msrMeasuresRepeat measuresRepeatUpLink)
{
  msrMeasuresRepeatElement* o =
    new msrMeasuresRepeatElement (
      inputLineNumber,
      measuresRepeatUpLink);
  assert (o!=0);
  return o;
}

msrMeasuresRepeatElement::msrMeasuresRepeatElement (
  int                 inputLineNumber,
  S_msrMeasuresRepeat measuresRepeatUpLink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msgAssert (
    measuresRepeatUpLink != nullptr,
    "measuresRepeatUpLink is null");

  fMeasuresRepeatElementMeasuresRepeatUpLink = measuresRepeatUpLink;
}

msrMeasuresRepeatElement::~msrMeasuresRepeatElement ()
{}

void msrMeasuresRepeatElement::appendSegmentToMeasuresRepeatElementsList ( // JMI ???
  int          inputLineNumber,
  S_msrSegment segment,
  string       context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Appending segment '" << segment <<
      "' to measuresRepeat element elements list '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
    segment != nullptr,
    "segment is null");

  fMeasuresRepeatElementElementsList.push_back (segment);
}

void msrMeasuresRepeatElement::appendMeasuresRepeatToMeasuresRepeatElementsList (
  int                  inputLineNumber,
  S_msrMeasuresRepeat  measuresRepeat,
  string               context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Appending measuresRepeat '" << measuresRepeat <<
      "' to measuresRepeat element elements list '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
    measuresRepeat != nullptr,
    "measuresRepeat is null");

  fMeasuresRepeatElementElementsList.push_back (measuresRepeat);
}

void msrMeasuresRepeatElement::appendVoiceElementToMeasuresRepeatElementsList (
  int               inputLineNumber,
  S_msrVoiceElement voiceElement,
  string            context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceVoices ()) {
    gLogStream <<
      "Appending voice element '" << voiceElement <<
      "' to measuresRepeat element elements list '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
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
          gGlobalOahOahGroup->getInputSourceName (),
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
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasuresRepeatElement::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrMeasuresRepeatElement>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeatElement>*> (v)) {
        S_msrMeasuresRepeatElement elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasuresRepeatElement::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasuresRepeatElement::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasuresRepeatElement::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMeasuresRepeatElement>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeatElement>*> (v)) {
        S_msrMeasuresRepeatElement elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
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
    ", measuresRepeat upLink: '" <<
    fMeasuresRepeatElementMeasuresRepeatUpLink->
      asShortString () <<
    "', line " << fInputLineNumber <<
    endl;

  return s.str ();
}

void msrMeasuresRepeatElement::print (ostream& os) const
{
  os <<
    "MeasuresRepeatElement" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  os <<
    "measuresRepeat upLink: '" <<
    fMeasuresRepeatElementMeasuresRepeatUpLink->
      asShortString () <<
      "'" <<
    endl << endl;

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
  os << endl;

  if (elementsNumber) {
    os << endl;

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

void msrMeasuresRepeatElement::printShort (ostream& os) const
{
  os <<
    "MeasuresRepeatElement" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

/ / * JMI
  os <<
    "measuresRepeat upLink: '" <<
    fMeasuresRepeatElementMeasuresRepeatUpLink->
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
  os << endl;

  if (elementsNumber) {
    os << endl;

    gIndenter++;

    list<S_msrVoiceElement>::const_iterator
      iBegin = fMeasuresRepeatElementElementsList.begin (),
      iEnd   = fMeasuresRepeatElementElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the element
      (*i)->printShort (os);
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
  S_msrMeasuresRepeat measuresRepeatUpLink)
{
  msrMeasuresRepeatPattern* o =
    new msrMeasuresRepeatPattern (
      inputLineNumber,
      measuresRepeatUpLink);
  assert (o!=0);
  return o;
}

msrMeasuresRepeatPattern::msrMeasuresRepeatPattern (
  int                 inputLineNumber,
  S_msrMeasuresRepeat measuresRepeatUpLink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msgAssert (
    measuresRepeatUpLink != nullptr,
    "measuresRepeatUpLink is null");

  fMeasuresRepeatUpLink = measuresRepeatUpLink;
}

msrMeasuresRepeatPattern::~msrMeasuresRepeatPattern ()
{}

void msrMeasuresRepeatPattern::setMeasuresRepeatPatternSegment (
    S_msrSegment measuresRepeatPatternSegment)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Setting measures repeat pattern segment containing " <<
      singularOrPlural (
        fetchMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
    measuresRepeatPatternSegment != nullptr,
    "measuresRepeatPatternSegment is null");

  fMeasuresRepeatPatternSegment =
    measuresRepeatPatternSegment;
}

int msrMeasuresRepeatPattern::fetchMeasuresNumber () const
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
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasuresRepeatPattern::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrMeasuresRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeatPattern>*> (v)) {
        S_msrMeasuresRepeatPattern elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasuresRepeatPattern::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasuresRepeatPattern::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasuresRepeatPattern::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMeasuresRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeatPattern>*> (v)) {
        S_msrMeasuresRepeatPattern elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasuresRepeatPattern::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasuresRepeatPattern::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
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
      fetchMeasuresNumber (),
      "repeated measure",
      "repeated measures") <<
    ")";

  return s.str ();
}

void msrMeasuresRepeatPattern::print (ostream& os) const
{
  os <<
    endl <<
    asString () <<
    endl << endl;

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
    os << endl;

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
  S_msrMeasuresRepeat measuresRepeatUpLink)
{
  msrMeasuresRepeatReplicas* o =
    new msrMeasuresRepeatReplicas (
      inputLineNumber,
      measuresRepeatUpLink);
  assert (o!=0);
  return o;
}

msrMeasuresRepeatReplicas::msrMeasuresRepeatReplicas (
  int                 inputLineNumber,
  S_msrMeasuresRepeat measuresRepeatUpLink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msgAssert (
    measuresRepeatUpLink != nullptr,
    "measuresRepeatUpLink is null");

  fMeasuresRepeatUpLink = measuresRepeatUpLink;
}

msrMeasuresRepeatReplicas::~msrMeasuresRepeatReplicas ()
{}

void msrMeasuresRepeatReplicas::setMeasuresRepeatReplicasSegment (
  S_msrSegment measuresRepeatReplicasSegment)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Setting measures repeat replicas segment containing " <<
      singularOrPlural (
        fetchMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
    measuresRepeatReplicasSegment != nullptr,
    "measuresRepeatReplicasSegment is null");

  fMeasuresRepeatReplicasSegment =
    measuresRepeatReplicasSegment;
}

int msrMeasuresRepeatReplicas::fetchMeasuresNumber () const
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
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasuresRepeatReplicas::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrMeasuresRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeatReplicas>*> (v)) {
        S_msrMeasuresRepeatReplicas elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasuresRepeatReplicas::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasuresRepeatReplicas::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasuresRepeatReplicas::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMeasuresRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeatReplicas>*> (v)) {
        S_msrMeasuresRepeatReplicas elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasuresRepeatReplicas::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasuresRepeatReplicas::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
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
      fetchMeasuresNumber (),
      "replicas measure",
      "replicas measures") <<
    ")";

  return s.str ();
}

void msrMeasuresRepeatReplicas::print (ostream& os) const
{
  os <<
    endl <<
    asString () <<
    endl << endl;

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
    os << endl;

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
  S_msrVoice voiceUpLink)
{
  msrMeasuresRepeat* o =
    new msrMeasuresRepeat (
      inputLineNumber,
      measuresRepeatMeasuresNumber,
      measuresRepeatSlashesNumber,
      voiceUpLink);
  assert (o!=0);
  return o;
}

msrMeasuresRepeat::msrMeasuresRepeat (
  int        inputLineNumber,
  int        measuresRepeatMeasuresNumber,
  int        measuresRepeatSlashesNumber,
  S_msrVoice voiceUpLink)
    : msrVoiceElement (inputLineNumber)
{
  // sanity check
  msgAssert (
    measuresRepeatMeasuresNumber > 0,
    "measuresRepeatMeasuresNumber is not positive");

  fMeasuresRepeatMeasuresNumber = measuresRepeatMeasuresNumber;

  // sanity check
  msgAssert (
    measuresRepeatSlashesNumber > 0,
    "measuresRepeatSlashesNumber is not positive");

  fMeasuresRepeatSlashesNumber  = measuresRepeatSlashesNumber;

  fMeasuresRepeatVoiceUpLink = voiceUpLink;

  // measures repeat build phase
  fCurrentMeasuresRepeatBuildPhaseKind =
    msrMeasuresRepeat::kMeasuresRepeatBuildPhaseJustCreated;
}

msrMeasuresRepeat::~msrMeasuresRepeat ()
{}

S_msrMeasuresRepeat msrMeasuresRepeat::createMeasuresRepeatNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Creating a newborn clone of measures repeat '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  // sanity check
  msgAssert(
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Setting measures repeat pattern containing " <<
      singularOrPlural (
        measuresRepeatPattern->
          fetchMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
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
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    gLogStream <<
      "Setting measures repeat replicas containing " <<
      singularOrPlural (
        measuresRepeatReplicas->
          fetchMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
    measuresRepeatReplicas != nullptr,
    "measuresRepeatReplicas is null");

  fMeasuresRepeatReplicas = measuresRepeatReplicas;

  // set currentRepeat's build phase
  fCurrentMeasuresRepeatBuildPhaseKind =
    msrMeasuresRepeat::kMeasuresRepeatBuildPhaseInReplicas;
}

int msrMeasuresRepeat::fetchMeasuresRepeatReplicasNumber () const
{
  int patternMeasuresNumber =
    fetchMeasuresRepeatPatternMeasuresNumber ();

  // sanity check
  msgAssert (
    patternMeasuresNumber > 0,
    "patternMeasuresNumber is not positive");

  return
    fetchMeasuresRepeatReplicasMeasuresNumber ()
      / // JMI ???
    patternMeasuresNumber;
}

int msrMeasuresRepeat::fetchMeasuresRepeatPatternMeasuresNumber () const
{
  // sanity check
  msgAssert (
    fMeasuresRepeatPattern != nullptr,
    "fMeasuresRepeatPattern is null");

  return
    fMeasuresRepeatPattern->
      fetchMeasuresNumber ();
}

int msrMeasuresRepeat::fetchMeasuresRepeatReplicasMeasuresNumber () const
{
  // sanity check
  msgAssert (
    fMeasuresRepeatReplicas != nullptr,
    "fMeasuresRepeatReplicas is null");

  return
    fMeasuresRepeatReplicas->
      fetchMeasuresNumber ();
}

void msrMeasuresRepeat::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasuresRepeat::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrMeasuresRepeat>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeat>*> (v)) {
        S_msrMeasuresRepeat elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasuresRepeat::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasuresRepeat::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasuresRepeat::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMeasuresRepeat>*
    p =
      dynamic_cast<visitor<S_msrMeasuresRepeat>*> (v)) {
        S_msrMeasuresRepeat elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasuresRepeat::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasuresRepeat::browseData (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
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
      fMeasuresRepeatVoiceUpLink->
        fetchVoiceScoreUpLink ();

  bool inhibitMeasuresRepeatReplicasBrowsing =
    score->getInhibitMeasuresRepeatReplicasBrowsing ();

  if (inhibitMeasuresRepeatReplicasBrowsing) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalMsrOahGroup->getTraceMsrVisitors () || gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
      gLogStream <<
        "% ==> visiting measures repeat replicas is inhibited" <<
        endl;
    }
#endif
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
      fetchMeasuresRepeatPatternMeasuresNumber (),
      "repeated measure",
      "repeated measures") <<
    ", " <<
    singularOrPlural (
      fetchMeasuresRepeatReplicasMeasuresNumber (),
      "replicas measure",
      "replicas measures") <<
    ", " <<
    fetchMeasuresRepeatReplicasNumber () << " replicas" <<
    ")";

  return s.str ();
}

void msrMeasuresRepeat::displayMeasuresRepeat (
  int    inputLineNumber,
  string context)
{
  gLogStream <<
    endl <<
    "*********>> MeasuresRepeat " <<
    ", measuresRepeatMeasuresNumber: '" <<
    fMeasuresRepeatMeasuresNumber <<
    ", measuresRepeatSlashesNumber: '" <<
    fMeasuresRepeatSlashesNumber <<
    "', voice:" <<
    endl <<
    fMeasuresRepeatVoiceUpLink->getVoiceName () <<
    " (" << context << ")" <<
    ", line " << inputLineNumber <<
    " contains:" <<
    endl;

  gIndenter++;
  print (gLogStream);
  gIndenter--;

  gLogStream <<
    " <<*********" <<
    endl << endl;
}

void msrMeasuresRepeat::print (ostream& os) const
{
  os <<
    "MeasuresRepeat" <<
    " (" <<
    singularOrPlural (
      fMeasuresRepeatPattern
        ? fetchMeasuresRepeatPatternMeasuresNumber ()
        : 0,
      "pattern measure",
      "pattern measures") <<
    ", " <<
    singularOrPlural (
      fMeasuresRepeatReplicas
        ? fetchMeasuresRepeatReplicasMeasuresNumber ()
        : 0,
      "replica measure",
      "replicas measures") <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMeasuresRepeats ()) {
    // print the current measures repeat build phase
    const unsigned int fieldWidth = 36;

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
