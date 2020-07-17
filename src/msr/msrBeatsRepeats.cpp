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

/* JMI
//______________________________________________________________________________
S_msrBeatsRepeatElement msrBeatsRepeatElement::create (
  int                 inputLineNumber,
  S_msrBeatsRepeat beatsRepeatUpLink)
{
  msrBeatsRepeatElement* o =
    new msrBeatsRepeatElement (
      inputLineNumber,
      beatsRepeatUpLink);
  assert(o!=0);
  return o;
}

msrBeatsRepeatElement::msrBeatsRepeatElement (
  int                 inputLineNumber,
  S_msrBeatsRepeat beatsRepeatUpLink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert (
    beatsRepeatUpLink != nullptr,
    "beatsRepeatUpLink is null");

  fBeatsRepeatElementBeatsRepeatUpLink = beatsRepeatUpLink;
}

msrBeatsRepeatElement::~msrBeatsRepeatElement ()
{}

void msrBeatsRepeatElement::appendSegmentToBeatsRepeatElementsList ( // JMI ???
  int          inputLineNumber,
  S_msrSegment segment,
  string       context)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
      "Appending segment '" << segment <<
      "' to beatsRepeat element elements list '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    segment != nullptr,
    "segment is null");

  fBeatsRepeatElementElementsList.push_back (segment);
}

void msrBeatsRepeatElement::appendBeatsRepeatToBeatsRepeatElementsList (
  int                  inputLineNumber,
  S_msrBeatsRepeat  beatsRepeat,
  string               context)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
      "Appending beatsRepeat '" << beatsRepeat <<
      "' to beatsRepeat element elements list '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    beatsRepeat != nullptr,
    "beatsRepeat is null");

  fBeatsRepeatElementElementsList.push_back (beatsRepeat);
}

void msrBeatsRepeatElement::appendVoiceElementToBeatsRepeatElementsList (
  int               inputLineNumber,
  S_msrVoiceElement voiceElement,
  string            context)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
      "Appending voice element '" << voiceElement <<
      "' to beatsRepeat element elements list '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    voiceElement != nullptr,
    "voiceElement is null");

  fBeatsRepeatElementElementsList.push_back (voiceElement);
}

S_msrNote msrBeatsRepeatElement::fetchBeatsRepeatElementFirstNonGraceNote () const
{

  S_msrNote result;

  // fetch the first note in the first measure to which
  // a grace notes group can be attached
  // i.e. one not in a grace notes group itself,
  // possibly inside a chord or tuplet

  if (fBeatsRepeatElementElementsList.size ()) {
    list<S_msrVoiceElement>::const_iterator
      iBegin = fBeatsRepeatElementElementsList.begin (),
      iEnd   = fBeatsRepeatElementElementsList.end (),
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
          gOahOah->fInputSourceName,
          fInputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }

      if (++i == iEnd) break;
    } // for
  }

  return result;
}

void msrBeatsRepeatElement::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrBeatsRepeatElement::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrBeatsRepeatElement>*
    p =
      dynamic_cast<visitor<S_msrBeatsRepeatElement>*> (v)) {
        S_msrBeatsRepeatElement elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrBeatsRepeatElement::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrBeatsRepeatElement::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrBeatsRepeatElement::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrBeatsRepeatElement>*
    p =
      dynamic_cast<visitor<S_msrBeatsRepeatElement>*> (v)) {
        S_msrBeatsRepeatElement elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrBeatsRepeatElement::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrBeatsRepeatElement::browseData (basevisitor* v)
{
  // browse the elements
  if (fBeatsRepeatElementElementsList.size ()) {
    for (
      list<S_msrVoiceElement>::const_iterator i = fBeatsRepeatElementElementsList.begin ();
      i != fBeatsRepeatElementElementsList.end ();
      i++
  ) {
      // browse the element
      msrBrowser<msrVoiceElement> browser (v);
      browser.browse (*(*i));
    } // for
  }
}

string msrBeatsRepeatElement::asString () const
{
  stringstream s;

  s <<
    "BeatsRepeatElement" <<
    ", beatsRepeat upLink: '" <<
    fBeatsRepeatElementBeatsRepeatUpLink->
      asShortString () <<
    "', line " << fInputLineNumber <<
    endl;

  return s.str ();
}

void msrBeatsRepeatElement::print (ostream& os) const
{
  os <<
    "BeatsRepeatElement" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  os <<
    "beatsRepeat upLink: '" <<
    fBeatsRepeatElementBeatsRepeatUpLink->
      asShortString () <<
      "'" <<
    endl <<
    endl;

  // print the elements
  int elementsNumber =
    fBeatsRepeatElementElementsList.size ();

  os <<
    "beatsRepeatElementElementsList: ";
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
      iBegin = fBeatsRepeatElementElementsList.begin (),
      iEnd   = fBeatsRepeatElementElementsList.end (),
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

void msrBeatsRepeatElement::printShort (ostream& os)
{
  os <<
    "BeatsRepeatElement" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

/ / * JMI
  os <<
    "beatsRepeat upLink: '" <<
    fBeatsRepeatElementBeatsRepeatUpLink->
      asShortString () <<
      "'" <<
    endl;
* /

  // print the elements
  int elementsNumber =
    fBeatsRepeatElementElementsList.size ();

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
      iBegin = fBeatsRepeatElementElementsList.begin (),
      iEnd   = fBeatsRepeatElementElementsList.end (),
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

ostream& operator<< (ostream& os, const S_msrBeatsRepeatElement& elt)
{
  elt->print (os);
  return os;
}
*/

//______________________________________________________________________________
S_msrBeatsRepeatPattern msrBeatsRepeatPattern::create (
  int                 inputLineNumber,
  S_msrBeatsRepeat beatsRepeatUpLink)
{
  msrBeatsRepeatPattern* o =
    new msrBeatsRepeatPattern (
      inputLineNumber,
      beatsRepeatUpLink);
  assert(o!=0);
  return o;
}

msrBeatsRepeatPattern::msrBeatsRepeatPattern (
  int                 inputLineNumber,
  S_msrBeatsRepeat beatsRepeatUpLink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert (
    beatsRepeatUpLink != nullptr,
    "beatsRepeatUpLink is null");

  fBeatsRepeatUpLink = beatsRepeatUpLink;
}

msrBeatsRepeatPattern::~msrBeatsRepeatPattern ()
{}

void msrBeatsRepeatPattern::setBeatsRepeatPatternSegment (
    S_msrSegment beatsRepeatPatternSegment)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceBeatsRepeats) {
    gLogOstream <<
      "Setting measures repeat pattern segment containing " <<
      singularOrPlural (
        fetchMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    beatsRepeatPatternSegment != nullptr,
    "beatsRepeatPatternSegment is null");

  fBeatsRepeatPatternSegment =
    beatsRepeatPatternSegment;
}

int msrBeatsRepeatPattern::fetchMeasuresNumber () const
{
  int result;

  if (fBeatsRepeatPatternSegment) {
    result =
      fBeatsRepeatPatternSegment->
        getSegmentMeasuresList ().size ();
  }
  else {
    result = 0;
  }

  return result;
}

void msrBeatsRepeatPattern::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrBeatsRepeatPattern::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrBeatsRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrBeatsRepeatPattern>*> (v)) {
        S_msrBeatsRepeatPattern elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrBeatsRepeatPattern::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrBeatsRepeatPattern::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrBeatsRepeatPattern::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrBeatsRepeatPattern>*
    p =
      dynamic_cast<visitor<S_msrBeatsRepeatPattern>*> (v)) {
        S_msrBeatsRepeatPattern elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrBeatsRepeatPattern::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrBeatsRepeatPattern::browseData (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrBeatsRepeatPattern::browseData ()" <<
      endl;
  }

  if (fBeatsRepeatPatternSegment) {
    // browse the pattern segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fBeatsRepeatPatternSegment);
  }
}

string msrBeatsRepeatPattern::asString () const
{
  stringstream s;

  s <<

    "BeatsRepeatPattern" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      fetchMeasuresNumber (),
      "repeated measure",
      "repeated measures") <<
    ")";

  return s.str ();
}

void msrBeatsRepeatPattern::print (ostream& os) const
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

  if (! fBeatsRepeatPatternSegment) {
    os <<
      " none" <<
      endl;
  }
  else {
    os << endl;

    gIndenter++;

    os <<
      fBeatsRepeatPatternSegment;

    gIndenter--;
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrBeatsRepeatPattern& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrBeatsRepeatReplicas msrBeatsRepeatReplicas::create (
  int                 inputLineNumber,
  S_msrBeatsRepeat beatsRepeatUpLink)
{
  msrBeatsRepeatReplicas* o =
    new msrBeatsRepeatReplicas (
      inputLineNumber,
      beatsRepeatUpLink);
  assert(o!=0);
  return o;
}

msrBeatsRepeatReplicas::msrBeatsRepeatReplicas (
  int                 inputLineNumber,
  S_msrBeatsRepeat beatsRepeatUpLink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert (
    beatsRepeatUpLink != nullptr,
    "beatsRepeatUpLink is null");

  fBeatsRepeatUpLink = beatsRepeatUpLink;
}

msrBeatsRepeatReplicas::~msrBeatsRepeatReplicas ()
{}

void msrBeatsRepeatReplicas::setBeatsRepeatReplicasSegment (
  S_msrSegment beatsRepeatReplicasSegment)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceBeatsRepeats) {
    gLogOstream <<
      "Setting measures repeat replicas segment containing " <<
      singularOrPlural (
        fetchMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    beatsRepeatReplicasSegment != nullptr,
    "beatsRepeatReplicasSegment is null");

  fBeatsRepeatReplicasSegment =
    beatsRepeatReplicasSegment;
}

int msrBeatsRepeatReplicas::fetchMeasuresNumber () const
{
  int result;

  if (fBeatsRepeatReplicasSegment) {
    result =
      fBeatsRepeatReplicasSegment->
        getSegmentMeasuresList ().size ();
  }
  else {
    result = 0;
  }

  return result;
}

void msrBeatsRepeatReplicas::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrBeatsRepeatReplicas::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrBeatsRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrBeatsRepeatReplicas>*> (v)) {
        S_msrBeatsRepeatReplicas elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrBeatsRepeatReplicas::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrBeatsRepeatReplicas::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrBeatsRepeatReplicas::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrBeatsRepeatReplicas>*
    p =
      dynamic_cast<visitor<S_msrBeatsRepeatReplicas>*> (v)) {
        S_msrBeatsRepeatReplicas elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrBeatsRepeatReplicas::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrBeatsRepeatReplicas::browseData (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrBeatsRepeatReplicas::browseData ()" <<
      endl;
  }

  if (fBeatsRepeatReplicasSegment) {
  // browse the replicas segment
    msrBrowser<msrSegment> browser (v);
    browser.browse (*fBeatsRepeatReplicasSegment);
  }
}

string msrBeatsRepeatReplicas::asString () const
{
  stringstream s;

  s <<

    "BeatsRepeatReplicas" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      fetchMeasuresNumber (),
      "replicas measure",
      "replicas measures") <<
    ")";

  return s.str ();
}

void msrBeatsRepeatReplicas::print (ostream& os) const
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

  if (! fBeatsRepeatReplicasSegment) {
    os <<
      " none" <<
      endl;
  }
  else {
    os << endl;

    gIndenter++;

    os <<
      fBeatsRepeatReplicasSegment;

    gIndenter--;
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrBeatsRepeatReplicas& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrBeatsRepeat msrBeatsRepeat::create (
  int        inputLineNumber,
  int        beatsRepeatMeasuresNumber,
  int        beatsRepeatSlashesNumber,
  S_msrVoice voiceUpLink)
{
  msrBeatsRepeat* o =
    new msrBeatsRepeat (
      inputLineNumber,
      beatsRepeatMeasuresNumber,
      beatsRepeatSlashesNumber,
      voiceUpLink);
  assert(o!=0);
  return o;
}

msrBeatsRepeat::msrBeatsRepeat (
  int        inputLineNumber,
  int        beatsRepeatMeasuresNumber,
  int        beatsRepeatSlashesNumber,
  S_msrVoice voiceUpLink)
    : msrVoiceElement (inputLineNumber)
{
  // sanity check
  msrAssert (
    beatsRepeatMeasuresNumber > 0,
    "beatsRepeatMeasuresNumber is not positive");

  fBeatsRepeatMeasuresNumber = beatsRepeatMeasuresNumber;

  // sanity check
  msrAssert (
    beatsRepeatSlashesNumber > 0,
    "beatsRepeatSlashesNumber is not positive");

  fBeatsRepeatSlashesNumber  = beatsRepeatSlashesNumber;

  fBeatsRepeatVoiceUpLink = voiceUpLink;

  // measures repeat build phase
  fCurrentBeatsRepeatBuildPhaseKind =
    msrBeatsRepeat::kBeatsRepeatBuildPhaseJustCreated;
}

msrBeatsRepeat::~msrBeatsRepeat ()
{}

S_msrBeatsRepeat msrBeatsRepeat::createBeatsRepeatNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceBeatsRepeats) {
    gLogOstream <<
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

  S_msrBeatsRepeat
    newbornClone =
      msrBeatsRepeat::create (
        fInputLineNumber,
        fBeatsRepeatMeasuresNumber,
        fBeatsRepeatSlashesNumber,
        containingVoice);

  return newbornClone;
}

void msrBeatsRepeat::setBeatsRepeatPattern (
  S_msrBeatsRepeatPattern beatsRepeatPattern)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceBeatsRepeats) {
    gLogOstream <<
      "Setting measures repeat pattern containing " <<
      singularOrPlural (
        beatsRepeatPattern->
          fetchMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    beatsRepeatPattern != nullptr,
    "beatsRepeatPattern is null");

  fBeatsRepeatPattern = beatsRepeatPattern;

  // set currentRepeat's build phase
  fCurrentBeatsRepeatBuildPhaseKind =
    msrBeatsRepeat::kBeatsRepeatBuildPhaseInPattern;
}

void msrBeatsRepeat::setBeatsRepeatReplicas (
  S_msrBeatsRepeatReplicas beatsRepeatReplicas)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceBeatsRepeats) {
    gLogOstream <<
      "Setting measures repeat replicas containing " <<
      singularOrPlural (
        beatsRepeatReplicas->
          fetchMeasuresNumber (),
        "measure",
        "measures") <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    beatsRepeatReplicas != nullptr,
    "beatsRepeatReplicas is null");

  fBeatsRepeatReplicas = beatsRepeatReplicas;

  // set currentRepeat's build phase
  fCurrentBeatsRepeatBuildPhaseKind =
    msrBeatsRepeat::kBeatsRepeatBuildPhaseInReplicas;
}

int msrBeatsRepeat::fetchBeatsRepeatReplicasNumber () const
{
  int patternMeasuresNumber =
    fetchBeatsRepeatPatternMeasuresNumber ();

  // sanity check
  msrAssert (
    patternMeasuresNumber > 0,
    "patternMeasuresNumber is not positive");

  return
    fetchBeatsRepeatReplicasMeasuresNumber ()
      / // JMI ???
    patternMeasuresNumber;
}

int msrBeatsRepeat::fetchBeatsRepeatPatternMeasuresNumber () const
{
  // sanity check
  msrAssert (
    fBeatsRepeatPattern != nullptr,
    "fBeatsRepeatPattern is null");

  return
    fBeatsRepeatPattern->
      fetchMeasuresNumber ();
}

int msrBeatsRepeat::fetchBeatsRepeatReplicasMeasuresNumber () const
{
  // sanity check
  msrAssert (
    fBeatsRepeatReplicas != nullptr,
    "fBeatsRepeatReplicas is null");

  return
    fBeatsRepeatReplicas->
      fetchMeasuresNumber ();
}

void msrBeatsRepeat::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrBeatsRepeat::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrBeatsRepeat>*
    p =
      dynamic_cast<visitor<S_msrBeatsRepeat>*> (v)) {
        S_msrBeatsRepeat elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrBeatsRepeat::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrBeatsRepeat::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrBeatsRepeat::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrBeatsRepeat>*
    p =
      dynamic_cast<visitor<S_msrBeatsRepeat>*> (v)) {
        S_msrBeatsRepeat elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrBeatsRepeat::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrBeatsRepeat::browseData (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrBeatsRepeat::browseData ()" <<
      endl;
  }

  if (fBeatsRepeatPattern) {
  // browse the measdure repeat pattern
    msrBrowser<msrBeatsRepeatPattern> browser (v);
    browser.browse (*fBeatsRepeatPattern);
  }

  // fetch the score
  S_msrScore
    score =
      fBeatsRepeatVoiceUpLink->
        fetchVoicePartUpLink ()->
          getPartPartGroupUpLink ()->
            getPartGroupScoreUpLink ();

/* JMI
  bool inhibitBeatsRepeatReplicasBrowsing =
    score->getInhibitBeatsRepeatReplicasBrowsing ();

  if (inhibitBeatsRepeatReplicasBrowsing) {
#ifdef TRACE_OAH
    if (gMsrOah->fTraceMsrVisitors || gTraceOah->fTraceBeatsRepeats) {
      gLogOstream <<
        "% ==> visiting measures repeat replicas is inhibited" <<
        endl;
    }
#endif
  }

  if (fBeatsRepeatReplicas) {
    if (! inhibitBeatsRepeatReplicasBrowsing) {
      // browse the measures repeat replicas
      msrBrowser<msrBeatsRepeatReplicas> browser (v);
      browser.browse (*fBeatsRepeatReplicas);
    }
  }
  */
}

string msrBeatsRepeat::beatsRepeatBuildPhaseKindAsString (
  msrBeatsRepeatBuildPhaseKind beatsRepeatBuildPhaseKind)
{
  string result;

  switch (beatsRepeatBuildPhaseKind) {
    case msrBeatsRepeat::kBeatsRepeatBuildPhaseJustCreated:
      result = "beatsRepeatBuildPhaseJustCreated";
      break;
    case msrBeatsRepeat::kBeatsRepeatBuildPhaseInPattern:
      result = "beatsRepeatBuildPhaseInPattern";
      break;
    case msrBeatsRepeat::kBeatsRepeatBuildPhaseInReplicas:
      result = "beatsRepeatBuildPhaseInReplicas";
      break;
    case msrBeatsRepeat::kBeatsRepeatBuildPhaseCompleted:
      result = "beatsRepeatBuildPhaseCompleted";
      break;
  } // switch

  return result;
}

string msrBeatsRepeat::asString () const
{
  stringstream s;

  s <<

    "BeatsRepeat" <<
    ", line " << fInputLineNumber <<
    " (" <<
    singularOrPlural (
      fetchBeatsRepeatPatternMeasuresNumber (),
      "repeated measure",
      "repeated measures") <<
    ", " <<
    singularOrPlural (
      fetchBeatsRepeatReplicasMeasuresNumber (),
      "replicas measure",
      "replicas measures") <<
    ", " <<
    fetchBeatsRepeatReplicasNumber () << " replicas" <<
    ")";

  return s.str ();
}

void msrBeatsRepeat::displayBeatsRepeat (
  int    inputLineNumber,
  string context)
{
  gLogOstream <<
    endl <<
    "*********>> BeatsRepeat " <<
    ", beatsRepeatMeasuresNumber: '" <<
    fBeatsRepeatMeasuresNumber <<
    ", beatsRepeatSlashesNumber: '" <<
    fBeatsRepeatSlashesNumber <<
    "', voice:" <<
    endl <<
    fBeatsRepeatVoiceUpLink->getVoiceName () <<
    " (" << context << ")" <<
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

void msrBeatsRepeat::print (ostream& os) const
{
  os <<
    "BeatsRepeat" <<
    " (" <<
    singularOrPlural (
      fBeatsRepeatPattern
        ? fetchBeatsRepeatPatternMeasuresNumber ()
        : 0,
      "pattern measure",
      "pattern measures") <<
    ", " <<
    singularOrPlural (
      fBeatsRepeatReplicas
        ? fetchBeatsRepeatReplicasMeasuresNumber ()
        : 0,
      "replica measure",
      "replicas measures") <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceBeatsRepeats) {
    // print the current measures repeat build phase
    const int fieldWidth = 36;

    os <<
      setw (fieldWidth) <<
      "currentBeatsRepeatBuildPhaseKind" << " : " <<
      beatsRepeatBuildPhaseKindAsString (
        fCurrentBeatsRepeatBuildPhaseKind) <<
      endl;
  }
#endif

  // print the measures repeat pattern
  if (! fBeatsRepeatPattern) {
    os <<
      "measures repeat pattern: none" <<
      endl;
  }

  else {
    os <<
      fBeatsRepeatPattern;
  }

  // print the measures repeat replicas
  if (! fBeatsRepeatReplicas) {
    os <<
      "measures repeat replicas: none" <<
      endl;
  }

  else {
    os <<
      fBeatsRepeatReplicas;
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrBeatsRepeat& elt)
{
  elt->print (os);
  return os;
}


}
