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
/*
  Repeats in MusicXML are applied to all voices in all staves of the current part

  The currentRepeat in each voice is the top of the voice repeats stack

  A repeat is recognized in MusicXML either by:

    - it's start: handleRepeatStart
    - it's first hooked ending (45b): handleRepeatEndingStart
        the elements before is moved to the new repeat's common part
    - it's end: handleRepeatEnd

  Hooked endings following the first one are added to currentRepeat handleRepeatHookedEndingEnd

  A hookless ending terminates currentRepeat: handleRepeatHooklessEndingEnd
    finalize currentRepeat
*/

//______________________________________________________________________________
/* JMI
S_msrRepeatElement msrRepeatElement::create (
  int                 inputLineNumber,
  S_msrRepeat         repeatUpLink)
{
  msrRepeatElement* o =
    new msrRepeatElement (
      inputLineNumber,
      repeatUpLink);
  assert(o!=0);
  return o;
}

msrRepeatElement::msrRepeatElement (
  int                 inputLineNumber,
  S_msrRepeat         repeatUpLink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert (
    repeatUpLink != nullptr,
    "repeatUpLink is null");

  fRepeatElementRepeatUpLink = repeatUpLink;
}

msrRepeatElement::~msrRepeatElement ()
{}

void msrRepeatElement::appendSegmentToRepeatElementsList ( // JMI ???
  int          inputLineNumber,
  S_msrSegment segment,
  string       context)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
      "Appending segment '" << segment <<
      "' to repeat element elements list '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    segment != nullptr,
    "segment is null");

  fRepeatElementElementsList.push_back (segment);
}

void msrRepeatElement::appendRepeatToRepeatElementsList (
  int          inputLineNumber,
  S_msrRepeat  repeat,
  string       context)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
      "Appending repeat '" << repeat <<
      "' to repeat element elements list '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    repeat != nullptr,
    "repeat is null");

  fRepeatElementElementsList.push_back (repeat);
}

void msrRepeatElement::appendVoiceElementToRepeatElementsList (
  int               inputLineNumber,
  S_msrVoiceElement voiceElement,
  string            context)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
      "Appending voice element '" << voiceElement <<
      "' to repeat element elements list '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    voiceElement != nullptr,
    "voiceElement is null");

  fRepeatElementElementsList.push_back (voiceElement);
}

S_msrNote msrRepeatElement::fetchRepeatElementFirstNonGraceNote () const
{

  S_msrNote result;

  // fetch the first note in the first measure to which
  // a grace notes group can be attached
  // i.e. one not in a grace notes group itself,
  // possibly inside a chord or tuplet

  if (fRepeatElementElementsList.size ()) {
    list<S_msrVoiceElement>::const_iterator
      iBegin = fRepeatElementElementsList.begin (),
      iEnd   = fRepeatElementElementsList.end (),
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

void msrRepeatElement::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrRepeatElement::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrRepeatElement>*
    p =
      dynamic_cast<visitor<S_msrRepeatElement>*> (v)) {
        S_msrRepeatElement elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrRepeatElement::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrRepeatElement::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrRepeatElement::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrRepeatElement>*
    p =
      dynamic_cast<visitor<S_msrRepeatElement>*> (v)) {
        S_msrRepeatElement elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrRepeatElement::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrRepeatElement::browseData (basevisitor* v)
{
  // browse the elements
  if (fRepeatElementElementsList.size ()) {
    for (
      list<S_msrVoiceElement>::const_iterator i = fRepeatElementElementsList.begin ();
      i != fRepeatElementElementsList.end ();
      i++
  ) {
      // browse the element
      msrBrowser<msrVoiceElement> browser (v);
      browser.browse (*(*i));
    } // for
  }
}

string msrRepeatElement::asString () const
{
  stringstream s;

  s <<
    "RepeatElement" <<
    ", repeat upLink: '" <<
    fRepeatElementRepeatUpLink->
      asShortString () <<
    "', line " << fInputLineNumber <<
    endl;

  return s.str ();
}

void msrRepeatElement::print (ostream& os) const
{
  os <<
    "RepeatElement" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  os <<
    "repeat upLink: '" <<
    fRepeatElementRepeatUpLink->
      asShortString () <<
      "'" <<
    endl <<
    endl;

  // print the elements
  int elementsNumber =
    fRepeatElementElementsList.size ();

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
      iBegin = fRepeatElementElementsList.begin (),
      iEnd   = fRepeatElementElementsList.end (),
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

void msrRepeatElement::shortPrint (ostream& os) const
{
  os <<
    "RepeatElement" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

/ * JMI
  os <<
    "repeat upLink: '" <<
    fRepeatElementRepeatUpLink->
      asShortString () <<
      "'" <<
    endl;
* /

  // print the elements
  int elementsNumber =
    fRepeatElementElementsList.size ();

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
      iBegin = fRepeatElementElementsList.begin (),
      iEnd   = fRepeatElementElementsList.end (),
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

ostream& operator<< (ostream& os, const S_msrRepeatElement& elt)
{
  elt->print (os);
  return os;
}
*/

//______________________________________________________________________________
S_msrRepeatCommonPart msrRepeatCommonPart::create (
  int                 inputLineNumber,
  S_msrRepeat         repeatUpLink)
{
  msrRepeatCommonPart* o =
    new msrRepeatCommonPart (
      inputLineNumber,
      repeatUpLink);
  assert(o!=0);
  return o;
}

msrRepeatCommonPart::msrRepeatCommonPart (
  int                 inputLineNumber,
  S_msrRepeat         repeatUpLink)
    : msrElement (inputLineNumber)
{
  // sanity check
  msrAssert (
    repeatUpLink != nullptr,
    "repeatUpLink is null");

  fRepeatCommonPartRepeatUpLink = repeatUpLink;
}

msrRepeatCommonPart::~msrRepeatCommonPart ()
{}

void msrRepeatCommonPart::appendSegmentToRepeatCommonPart (
  int          inputLineNumber,
  S_msrSegment segment,
  string       context)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceSegments) {
    gLogOstream <<
      "Appending segment '" <<
      segment->asString () <<
      "' to repeat common part '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    segment != nullptr,
    "segment is null");

  fRepeatCommonPartElementsList.push_back (segment);
}

void msrRepeatCommonPart::appendRepeatToRepeatCommonPart (
  int          inputLineNumber,
  S_msrRepeat  repeat,
  string       context)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Appending repeat '" <<
    // JMI  repeat->asString () <<
      repeat <<
      "' to repeat common part '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    repeat != nullptr,
    "repeat is null");

  fRepeatCommonPartElementsList.push_back (repeat);
}

void msrRepeatCommonPart::appendMeasuresRepeatToRepeatCommonPart (
  int                 inputLineNumber,
  S_msrMeasuresRepeat measuresRepeat,
  string              context)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresRepeats) {
    gLogOstream <<
      "Appending measures repeat '" <<
      measuresRepeat->asString () <<
      "' to repeat common part '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    measuresRepeat != nullptr,
    "measuresRepeat is null");

  fRepeatCommonPartElementsList.push_back (measuresRepeat);
}

void msrRepeatCommonPart::appendRestMeasuresToRepeatCommonPart (
  int               inputLineNumber,
  S_msrRestMeasures restMeasures,
  string            context)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRestMeasures) {
    gLogOstream <<
      "Appending rest measures '" <<
      restMeasures->asString () <<
      "' to repeat common part '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    restMeasures != nullptr,
    "restMeasures is null");

  fRepeatCommonPartElementsList.push_back (restMeasures);
}

void msrRepeatCommonPart::appendVoiceElementToRepeatCommonPart (
  int               inputLineNumber,
  S_msrVoiceElement voiceElement,
  string            context)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Appending voice element '" <<
      voiceElement->asString () <<
      "' to repeat common part '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    voiceElement != nullptr,
    "voiceElement is null");

  fRepeatCommonPartElementsList.push_back (voiceElement);
}

S_msrNote msrRepeatCommonPart::fetchRepeatCommonPartFirstNonGraceNote () const
{

  S_msrNote result;

  // fetch the first note in the first measure to which
  // a grace notes group can be attached
  // i.e. one not in a grace notes group itself,
  // possibly inside a chord or tuplet

  if (fRepeatCommonPartElementsList.size ()) {
    list<S_msrVoiceElement>::const_iterator
      iBegin = fRepeatCommonPartElementsList.begin (),
      iEnd   = fRepeatCommonPartElementsList.end (),
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

void msrRepeatCommonPart::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrRepeatCommonPart::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrRepeatCommonPart>*
    p =
      dynamic_cast<visitor<S_msrRepeatCommonPart>*> (v)) {
        S_msrRepeatCommonPart elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrRepeatCommonPart::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrRepeatCommonPart::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrRepeatCommonPart::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrRepeatCommonPart>*
    p =
      dynamic_cast<visitor<S_msrRepeatCommonPart>*> (v)) {
        S_msrRepeatCommonPart elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrRepeatCommonPart::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrRepeatCommonPart::browseData (basevisitor* v)
{
  // browse the elements
  if (fRepeatCommonPartElementsList.size ()) {
    for (
      list<S_msrVoiceElement>::const_iterator i = fRepeatCommonPartElementsList.begin ();
      i != fRepeatCommonPartElementsList.end ();
      i++
  ) {
      // browse the element
      msrBrowser<msrVoiceElement> browser (v);
      browser.browse (*(*i));
    } // for
  }
}

string msrRepeatCommonPart::asString () const
{
  stringstream s;

  s <<
    "RepeatCommonPart" <<
    ", repeat upLink: '" <<
    fRepeatCommonPartRepeatUpLink->
      asShortString () <<
    "', line " << fInputLineNumber <<
    endl;

  return s.str ();
}

void msrRepeatCommonPart::print (ostream& os) const
{
  os <<
    "RepeatCommonPart" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  os <<
    "repeat upLink: '" <<
    fRepeatCommonPartRepeatUpLink->
      asShortString () <<
      "'" <<
    endl <<
    endl;

  // print the elements
  int elementsNumber =
    fRepeatCommonPartElementsList.size ();

  os <<
    "repeatCommonPartElementsList: ";
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
      iBegin = fRepeatCommonPartElementsList.begin (),
      iEnd   = fRepeatCommonPartElementsList.end (),
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

void msrRepeatCommonPart::shortPrint (ostream& os) const
{
  os <<
    "RepeatCommonPart" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

/* JMI
  os <<
    "repeat upLink: '" <<
    fRepeatCommonPartRepeatUpLink->
      asShortString () <<
      "'" <<
    endl;
*/

  // print the elements
  int elementsNumber =
    fRepeatCommonPartElementsList.size ();

  os <<
    "repeatCommonPartElementsList: ";
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
      iBegin = fRepeatCommonPartElementsList.begin (),
      iEnd   = fRepeatCommonPartElementsList.end (),
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

ostream& operator<< (ostream& os, const S_msrRepeatCommonPart& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrRepeatEnding msrRepeatEnding::create (
  int                 inputLineNumber,
  string              repeatEndingNumber, // may be "1, 2"
  msrRepeatEndingKind repeatEndingKind,
  S_msrRepeat         repeatUpLink)
{
  msrRepeatEnding* o =
    new msrRepeatEnding (
      inputLineNumber,
      repeatEndingNumber,
      repeatEndingKind,
      repeatUpLink);
  assert(o!=0);
  return o;
}

msrRepeatEnding::msrRepeatEnding (
  int                 inputLineNumber,
  string              repeatEndingNumber, // may be "1, 2"
  msrRepeatEndingKind repeatEndingKind,
  S_msrRepeat         repeatUpLink)
    : msrElement (inputLineNumber)
{
  fRepeatEndingNumber = repeatEndingNumber;

  fRepeatEndingInternalNumber = 0;
    // will be set by msrRepeat::addRepeatEnding ()

  fRepeatEndingKind = repeatEndingKind;

  fRepeatEndingRepeatUpLink = repeatUpLink;
}

msrRepeatEnding::~msrRepeatEnding ()
{}

void msrRepeatEnding::appendSegmentToRepeatEnding (
  int          inputLineNumber,
  S_msrSegment segment,
  string       context)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
      "Appending segment '" << segment <<
      "' to repeat ending '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    segment != nullptr,
    "segment is null");

  fRepeatEndingElementsList.push_back (segment);
}

void msrRepeatEnding::appendRepeatToRepeatEnding (
  int          inputLineNumber,
  S_msrRepeat  repeat,
  string       context)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
      "Appending repeat '" << repeat <<
      "' to repeat ending '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    repeat != nullptr,
    "repeat is null");

  fRepeatEndingElementsList.push_back (repeat);
}

void msrRepeatEnding::appendMeasuresRepeatToRepeatEnding (
  int                 inputLineNumber,
  S_msrMeasuresRepeat measuresRepeat,
  string              context)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
      "Appending measures repeat '" <<
      measuresRepeat->asShortString () <<
      "' to repeat ending '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    measuresRepeat != nullptr,
    "measuresRepeat is null");

  fRepeatEndingElementsList.push_back (measuresRepeat);
}

void msrRepeatEnding::appendRestMeasuresToRepeatEnding (
  int                       inputLineNumber,
  S_msrRestMeasures restMeasures,
  string                    context)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
      "Appending rest measures '" <<
      restMeasures->asShortString () <<
      "' to repeat ending '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    restMeasures != nullptr,
    "restMeasures is null");

  fRepeatEndingElementsList.push_back (restMeasures);
}

void msrRepeatEnding::appendVoiceElementToRepeatEnding (
  int               inputLineNumber,
  S_msrVoiceElement voiceElement,
  string            context)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceVoices) {
    gLogOstream <<
      "Appending voice element '" << voiceElement <<
      "' to repeat ending '" << asString () <<
      "' (" << context << ")" <<
      ", line " << inputLineNumber <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    voiceElement != nullptr,
    "voiceElement is null");

  fRepeatEndingElementsList.push_back (voiceElement);
}

void msrRepeatEnding::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrRepeatEnding::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrRepeatEnding>*
    p =
      dynamic_cast<visitor<S_msrRepeatEnding>*> (v)) {
        S_msrRepeatEnding elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrRepeatEnding::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrRepeatEnding::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrRepeatEnding::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrRepeatEnding>*
    p =
      dynamic_cast<visitor<S_msrRepeatEnding>*> (v)) {
        S_msrRepeatEnding elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrRepeatEnding::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrRepeatEnding::browseData (basevisitor* v)
{
  // browse the elements
  if (fRepeatEndingElementsList.size ()) {
    for (
      list<S_msrVoiceElement>::const_iterator i = fRepeatEndingElementsList.begin ();
      i != fRepeatEndingElementsList.end ();
      i++
  ) {
      // browse the element
      msrBrowser<msrVoiceElement> browser (v);
      browser.browse (*(*i));
    } // for
  }
}

string msrRepeatEnding::repeatEndingKindAsString (
  msrRepeatEndingKind repeatEndingKind)
{
  string result;

  switch (repeatEndingKind) {
    case msrRepeatEnding::kHookedEnding:
      result = "hooked";
      break;
    case msrRepeatEnding::kHooklessEnding:
      result = "hookless";
      break;
  } // switch

  return result;
}

string msrRepeatEnding::asString () const
{
  stringstream s;

  s <<
    "RepeatEnding" <<
    ", " <<
    repeatEndingKindAsString (
      fRepeatEndingKind) <<
    ", repeatEndingRepeatUpLink: '";

  if (fRepeatEndingRepeatUpLink) {
    s <<
      fRepeatEndingRepeatUpLink->
        asShortString ();
  }
  else {
    s <<
      "none";
  }

  s <<
    ", repeatEndingNumber: " << fRepeatEndingNumber <<
    ", repeatEndingInternalNumber: " << fRepeatEndingInternalNumber <<
    "', line " << fInputLineNumber <<
    endl;

  return s.str ();
}

void msrRepeatEnding::print (ostream& os) const
{
  os <<
    endl <<
    "RepeatEnding" <<
    ", repeatEndingKind" << " : " <<
    repeatEndingKindAsString (
      fRepeatEndingKind) <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

/* JMI
  os <<
    "repeat upLink: '" <<
    fRepeatEndingRepeatUpLink->
      asShortString () <<
      "'" <<
    endl;
*/

  const int fieldWidth = 27;

  os << left <<
    setw (fieldWidth) <<
    "repeatEndingNumber" <<  " : " <<fRepeatEndingNumber <<
    endl <<
    setw (fieldWidth) <<
    "repeatEndingInternalNumber" <<  " : " <<fRepeatEndingInternalNumber <<
    endl <<
    setw (fieldWidth) <<
    "repeat upLink" << " : '" <<
    fRepeatEndingRepeatUpLink->
      asShortString () <<
    "'" <<
    endl <<
    endl;

  // print the elements
  int elementsNumber =
    fRepeatEndingElementsList.size ();

  os <<
    "repeatEndingElementsList: ";
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
      iBegin = fRepeatEndingElementsList.begin (),
      iEnd   = fRepeatEndingElementsList.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the element
      (*i)->print (os);
      if (++i == iEnd) break;
      os << endl;
    } // for

    gIndenter--;
  }

  gIndenter--;
}

void msrRepeatEnding::shortPrint (ostream& os) const
{
  os <<
    endl <<
    "RepeatEnding" <<
// JMI    ", repeatEndingKind" << " : " <<
    ", " <<
    repeatEndingKindAsString (
      fRepeatEndingKind) <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 27;

  os << left <<
    setw (fieldWidth) <<
    "repeatEndingNumber" <<  " : " <<fRepeatEndingNumber <<
    /* JMI
    endl <<
    setw (fieldWidth) <<
    "repeatEndingInternalNumber" <<  " : " <<fRepeatEndingInternalNumber <<
    endl <<
    setw (fieldWidth) <<
    "repeat upLink: " << " : '" <<
    fRepeatEndingRepeatUpLink->
      asShortString () <<
    "'" <<
    endl <<
    */
    endl;

/* JMI
  os <<
    "repeat upLink: '" <<
    fRepeatEndingRepeatUpLink->
      asShortString () <<
      "'" <<
    endl;
*/

  // print the elements
  int elementsNumber =
    fRepeatEndingElementsList.size ();

  os <<
    "repeatEndingElementsList: ";
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
      iBegin = fRepeatEndingElementsList.begin (),
      iEnd   = fRepeatEndingElementsList.end (),
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

ostream& operator<< (ostream& os, const S_msrRepeatEnding& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrRepeat msrRepeat::create (
  int        inputLineNumber,
  int        repeatTimes,
  S_msrVoice voiceUpLink)
{
  msrRepeat* o =
    new msrRepeat (
      inputLineNumber,
      repeatTimes,
      voiceUpLink);
  assert(o!=0);
  return o;
}

msrRepeat::msrRepeat (
  int        inputLineNumber,
  int        repeatTimes,
  S_msrVoice voiceUpLink)
    : msrVoiceElement (inputLineNumber)
{
  fRepeatEndingsInternalCounter = 0;

  // sanity check
  msrAssert(
    voiceUpLink != nullptr,
    "voiceUpLink is null");

  fRepeatVoiceUpLink = voiceUpLink;

  fRepeatTimes = repeatTimes;

  fRepeatExplicitStartKind = kRepeatExplicitStartNo; // default value

  // repeat build phase
  fCurrentRepeatBuildPhaseKind =
    msrRepeat::kRepeatBuildPhaseJustCreated;
}

S_msrRepeat msrRepeat::createRepeatNewbornClone (
  S_msrVoice containingVoice)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Creating a newborn clone of a repeat" <<
      endl;
  }
#endif

  // sanity check
  msrAssert(
    containingVoice != nullptr,
    "containingVoice is null");

  S_msrRepeat
    newbornClone =
      msrRepeat::create (
        fInputLineNumber,
        fRepeatTimes,
        containingVoice);

  return newbornClone;
}

msrRepeat::~msrRepeat ()
{}

void msrRepeat::setRepeatCommonPart (
  S_msrRepeatCommonPart repeatCommonPart)
{
  // sanity check
  msrAssert(
    repeatCommonPart != nullptr,
    "repeatCommonPart is null");

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Setting repeat common part containing " <<
      singularOrPlural (
        repeatCommonPart->
          getRepeatCommonPartElementsList ().size (),
        "element",
        "elements") <<
      endl;
  }
#endif

  fRepeatCommonPart = repeatCommonPart;

  // set repeat's build phase
  fCurrentRepeatBuildPhaseKind =
    msrRepeat::kRepeatBuildPhaseInCommonPart;
}

void msrRepeat::addRepeatEndingToRepeat (
  int               inputLineNumber,
  S_msrRepeatEnding repeatEnding)
{
  // sanity check
  msrAssert(
    repeatEnding != nullptr,
    "repeatEnding is null");

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Adding ending '" <<
      repeatEnding->asString () <<
      "' to repeat" <<
      "' to repeat '" <<
      asShortString () <<
      "'" <<
      endl;
  }
#endif

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeatsDetails) {
    displayRepeat (
      inputLineNumber,
      "addRepeatEndingToRepeat() 1");
  }
#endif

  // get repeat ending kind
  msrRepeatEnding::msrRepeatEndingKind
    repeatEndingKind =
      repeatEnding->
        getRepeatEndingKind ();

  // consistency check
  switch (repeatEndingKind) {
    case msrRepeatEnding::kHookedEnding:
      switch (fCurrentRepeatBuildPhaseKind) {
        case msrRepeat::kRepeatBuildPhaseJustCreated:
          {
            stringstream s;

            s <<
              "cannot add hooked ending '" <<
              repeatEnding->asShortString () <<
              "' right after a repeat has been created";

            msrMusicXMLError (
              gOahOah->fInputSourceName,
              fInputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
        case msrRepeat::kRepeatBuildPhaseInCommonPart:
          fCurrentRepeatBuildPhaseKind =
            msrRepeat::kRepeatBuildPhaseInEndings;
          break;
        case msrRepeat::kRepeatBuildPhaseInEndings:
          // there can be several successive endings
          break;
        case msrRepeat::kRepeatBuildPhaseCompleted:
          {
            stringstream s;

            s <<
              "cannot add hooked ending '" <<
              repeatEnding->asShortString () <<
              "' after a repeat has been completed";

            msrMusicXMLError (
              gOahOah->fInputSourceName,
              fInputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
      } // switch
      break;

    case msrRepeatEnding::kHooklessEnding:
      switch (fCurrentRepeatBuildPhaseKind) {
        case msrRepeat::kRepeatBuildPhaseJustCreated:
          {
            stringstream s;

            s <<
              "cannot add hookless repeat ending '" <<
              repeatEnding->asShortString () <<
              "' right after the repeat has been created";

            msrInternalError (
              gOahOah->fInputSourceName,
              fInputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
        case msrRepeat::kRepeatBuildPhaseInCommonPart:
          {
            stringstream s;

            s <<
              "cannot add hookless repeat ending '" <<
              repeatEnding->asShortString () <<
              "' right after the repeat common part";

            msrInternalError (
              gOahOah->fInputSourceName,
              fInputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
          break;
        case msrRepeat::kRepeatBuildPhaseInEndings:
          // there can be several successive endings
          break;
        case msrRepeat::kRepeatBuildPhaseCompleted:
          {
            stringstream s;

            s <<
              "cannot add hookless ending '" <<
              repeatEnding->asShortString () <<
              "' after a repeat has been completed";

            msrMusicXMLError (
              gOahOah->fInputSourceName,
              fInputLineNumber,
              __FILE__, __LINE__,
              s.str ());
          }
      } // switch
      break;
  } // switch

  fRepeatEndings.push_back (repeatEnding);

  // set repeat ending internal number
  repeatEnding->
    setRepeatEndingInternalNumber (
      ++ fRepeatEndingsInternalCounter);

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeatsDetails) {
    displayRepeat (
      inputLineNumber,
      "addRepeatEndingToRepeat() 2");
  }
#endif
}

void msrRepeat::appendSegmentToRepeat (
  int          inputLineNumber,
  S_msrSegment segment,
  string       context)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Appending segment '" <<
      segment->asString () <<
      "' to repeat '" <<
      asShortString () <<
      "'" <<
      endl;

    displayRepeat (
      inputLineNumber,
      "appendSegmentToRepeat() 1");
  }
#endif

  switch (fCurrentRepeatBuildPhaseKind) {
    case msrRepeat::kRepeatBuildPhaseJustCreated:
      {
        stringstream s;

        s <<
          "segment '" <<
          segment->asShortString () <<
          "'cannot be added to a just created repeat" <<
          " (" << context << ")";

        msrMusicXMLError (
          gOahOah->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;

    case msrRepeat::kRepeatBuildPhaseInCommonPart:
        fRepeatCommonPart->
          appendSegmentToRepeatCommonPart (
            inputLineNumber,
            segment,
            context);
      break;

    case msrRepeat::kRepeatBuildPhaseInEndings:
      fRepeatEndings.back ()->
        appendSegmentToRepeatEnding (
          inputLineNumber,
          segment,
          context);
      break;

    case msrRepeat::kRepeatBuildPhaseCompleted:
      {
        stringstream s;

        s <<
          "segment '" <<
          segment->asShortString () <<
          "'cannot be added to a completed repeat" <<
          "(" << context << ")";

        msrMusicXMLError (
          gOahOah->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayRepeat (
      inputLineNumber,
      "appendSegmentToRepeat() 2");
  }
#endif
}

void msrRepeat::appendRepeatToRepeat (
  int         inputLineNumber,
  S_msrRepeat repeat,
  string      context)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Appending repeat '" <<
      repeat->asString () <<
      "' to repeat '" <<
      asShortString () <<
      "'" <<
      endl;

    displayRepeat (
      inputLineNumber,
      "appendRepeatToRepeat() 1");
  }
#endif

  switch (fCurrentRepeatBuildPhaseKind) {
    case msrRepeat::kRepeatBuildPhaseJustCreated:
      {
        stringstream s;

        s <<
          "repeat '" <<
          repeat->asShortString () <<
          "'cannot be added to a just created repeat" <<
          " (" << context << ")";

        msrMusicXMLError (
          gOahOah->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;

    case msrRepeat::kRepeatBuildPhaseInCommonPart:
        fRepeatCommonPart->
          appendRepeatToRepeatCommonPart (
            inputLineNumber,
            repeat,
            context);
      break;

    case msrRepeat::kRepeatBuildPhaseInEndings:
      fRepeatEndings.back ()->
        appendRepeatToRepeatEnding (
          inputLineNumber,
          repeat,
          context);
      break;

    case msrRepeat::kRepeatBuildPhaseCompleted:
      {
        stringstream s;

        s <<
          "repeat '" <<
          repeat->asShortString () <<
          "'cannot be added to a completed repeat" <<
          "(" << context << ")";

        msrMusicXMLError (
          gOahOah->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayRepeat (
      inputLineNumber,
      "appendRepeatToRepeat() 2");
  }
#endif
}

void msrRepeat::appendMeasuresRepeatToRepeat (
  int                 inputLineNumber,
  S_msrMeasuresRepeat measuresRepeat,
  string              context)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Appending measures repeat '" <<
      measuresRepeat->asString () <<
      "' to repeat '" <<
      asShortString () <<
      "'" <<
      endl;

    displayRepeat (
      inputLineNumber,
      "appendMeasuresRepeatToRepeat() 1");
  }
#endif

  switch (fCurrentRepeatBuildPhaseKind) {
    case msrRepeat::kRepeatBuildPhaseJustCreated:
      {
        stringstream s;

        s <<
          "measures repeat '" <<
          measuresRepeat->asShortString () <<
          "'cannot be added to a just created repeat" <<
          " (" << context << ")";

        msrMusicXMLError (
          gOahOah->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;

    case msrRepeat::kRepeatBuildPhaseInCommonPart:
        fRepeatCommonPart->
          appendMeasuresRepeatToRepeatCommonPart (
            inputLineNumber,
            measuresRepeat,
            context);
      break;

    case msrRepeat::kRepeatBuildPhaseInEndings:
      fRepeatEndings.back ()->
        appendMeasuresRepeatToRepeatEnding (
          inputLineNumber,
          measuresRepeat,
          context);
      break;

    case msrRepeat::kRepeatBuildPhaseCompleted:
      {
        stringstream s;

        s <<
          "measures repeat '" <<
          measuresRepeat->asShortString () <<
          "'cannot be added to a completed repeat" <<
          "(" << context << ")";

        msrMusicXMLError (
          gOahOah->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayRepeat (
      inputLineNumber,
      "appendMeasuresRepeatToRepeat() 2");
  }
#endif
}

void msrRepeat::appendRestMeasuresToRepeat (
  int                       inputLineNumber,
  S_msrRestMeasures restMeasures,
  string                    context)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    gLogOstream <<
      "Appending rest measures '" <<
      restMeasures->asString () <<
      "' to repeat '" <<
      asShortString () <<
      "'" <<
      endl;

    displayRepeat (
      inputLineNumber,
      "appendRestMeasuresToRepeat() 1");
  }
#endif

  switch (fCurrentRepeatBuildPhaseKind) {
    case msrRepeat::kRepeatBuildPhaseJustCreated:
      {
        stringstream s;

        s <<
          "rest measures '" <<
          restMeasures->asShortString () <<
          "'cannot be added to a just created repeat" <<
          " (" << context << ")";

        msrMusicXMLError (
          gOahOah->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;

    case msrRepeat::kRepeatBuildPhaseInCommonPart:
      fRepeatCommonPart->
        appendRestMeasuresToRepeatCommonPart (
          inputLineNumber,
          restMeasures,
          context);
      break;

    case msrRepeat::kRepeatBuildPhaseInEndings:
      fRepeatEndings.back ()->
        appendRestMeasuresToRepeatEnding (
          inputLineNumber,
          restMeasures,
          context);
      break;

    case msrRepeat::kRepeatBuildPhaseCompleted:
      {
        stringstream s;

        s <<
          "rest measures '" <<
          restMeasures->asShortString () <<
          "'cannot be added to a completed repeat" <<
          "(" << context << ")";

        msrMusicXMLError (
          gOahOah->fInputSourceName,
          inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;
  } // switch

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    displayRepeat (
      inputLineNumber,
      "appendRestMeasuresToRepeat() 2");
  }
#endif
}

S_msrNote msrRepeat::fetchRepeatFirstNonGraceNote () const
{
  S_msrNote result;

  if (fRepeatCommonPart) {
    result =
      fRepeatCommonPart->
        fetchRepeatCommonPartFirstNonGraceNote ();
  }

  return result;
}

void msrRepeat::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrRepeat::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrRepeat>*
    p =
      dynamic_cast<visitor<S_msrRepeat>*> (v)) {
        S_msrRepeat elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrRepeat::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrRepeat::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrRepeat::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrRepeat>*
    p =
      dynamic_cast<visitor<S_msrRepeat>*> (v)) {
        S_msrRepeat elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrRepeat::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrRepeat::browseData (basevisitor* v)
{
  if (fRepeatCommonPart) {
  // browse the common part
    msrBrowser<msrRepeatCommonPart> browser (v);
    browser.browse (*fRepeatCommonPart);
  }

  // browse the alternatives
  for (
    vector<S_msrRepeatEnding>::const_iterator i = fRepeatEndings.begin ();
    i != fRepeatEndings.end ();
    i++
  ) {
    // browse the alternative
    msrBrowser<msrRepeatEnding> browser (v);
    browser.browse (*(*i));
  } // for
}

string msrRepeat::repeatExplicitStartKindAsString (
  msrRepeatExplicitStartKind repeatExplicitStartKind)
{
  string result;

  switch (repeatExplicitStartKind) {
    case msrRepeat::kRepeatExplicitStartNo:
      result = "repeatExplicitStartNo";
      break;
    case msrRepeat::kRepeatExplicitStartYes:
      result = "repeatExplicitStartYes";
      break;
  } // switch

  return result;
}

string msrRepeat::repeatBuildPhaseKindAsString (
  msrRepeatBuildPhaseKind repeatBuildPhaseKind)
{
  string result;

  switch (repeatBuildPhaseKind) {
    case msrRepeat::kRepeatBuildPhaseJustCreated:
      result = "repeatBuildPhaseJustCreated";
      break;
    case msrRepeat::kRepeatBuildPhaseInCommonPart:
      result = "repeatBuildPhaseInCommonPart";
      break;
    case msrRepeat::kRepeatBuildPhaseInEndings:
      result = "repeatBuildPhaseInEndings";
      break;
    case msrRepeat::kRepeatBuildPhaseCompleted:
      result = "repeatBuildPhaseCompleted";
      break;
  } // switch

  return result;
}

string msrRepeat::asShortString () const
{
  stringstream s;

  s <<
    "Repeat" <<
    ", " << fRepeatTimes << " times" <<
   ", repeatExplicitStartKind: " <<
    repeatExplicitStartKindAsString (
      fRepeatExplicitStartKind) <<
    ", currentRepeatBuildPhaseKind: " <<
    repeatBuildPhaseKindAsString (
      fCurrentRepeatBuildPhaseKind) <<
    ", common part: ";

  if (fRepeatCommonPart) {
    s <<
      singularOrPlural (
        fRepeatCommonPart->
          getRepeatCommonPartElementsList ().size (),
        "element",
        "elements");
  }
  else {
    s <<
      "none";
  }

  int repeatEndingsNumber =
    fRepeatEndings.size ();

  s <<
    ", " <<
    singularOrPlural (
      repeatEndingsNumber, "repeat ending", "repeat endings") <<
    ", line " << fInputLineNumber;

  return s.str ();
}

string msrRepeat::asString () const
{
  stringstream s;

  s <<
    "Repeat" <<
    ", " << fRepeatTimes << " times" <<
   ", repeatExplicitStartKind:: " <<
    repeatExplicitStartKindAsString (
      fRepeatExplicitStartKind) <<
    ", currentRepeatBuildPhaseKind: " <<
    repeatBuildPhaseKindAsString (
      fCurrentRepeatBuildPhaseKind) <<
    ", common part: ";

  if (fRepeatCommonPart) {
    s <<
      fRepeatCommonPart->asString ();
  }
  else {
    s <<
      "none";
  }

  int endingsNumber =
    fRepeatEndings.size ();

  s <<
    ", " <<
    singularOrPlural (
      endingsNumber, "repeat ending", "repeat endings");

  if (endingsNumber) {
    vector<S_msrRepeatEnding>::const_iterator
      iBegin = fRepeatEndings.begin (),
      iEnd   = fRepeatEndings.end (),
      i      = iBegin;

    s <<
      ", ";

    for ( ; ; ) {
      // print the repeat ending
      s << (*i)->asShortString ();
      if (++i == iEnd) break;
      s << " ";
    } // for
  }

  s <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrRepeat::displayRepeat (
  int    inputLineNumber,
  string context)
{
  gLogOstream <<
    endl <<
    "*********>> Repeat '" <<
    asShortString () <<
    "' (" << context << ")" <<
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

void msrRepeat::print (ostream& os) const
{
  os <<
    "Repeat" <<
    ", " << fRepeatTimes << " times" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 28;

  os << left <<
    setw (fieldWidth) <<
   "repeatExplicitStartKind" << " : " <<
    repeatExplicitStartKindAsString (
      fRepeatExplicitStartKind) <<
    endl <<
    setw (fieldWidth) <<
    "repeat ending(s)" << " : " <<
    fRepeatEndings.size () <<
    endl;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    // print the current repeat build phase
    os << left <<
      setw (fieldWidth) <<
      "currentRepeatBuildPhaseKind" << " : " <<
      repeatBuildPhaseKindAsString (
        fCurrentRepeatBuildPhaseKind) <<
      endl;
  }
#endif

  os << endl;

  // print the repeat common part
  if (! fRepeatCommonPart) {
    os <<
      "Common part: none" <<
      endl;
  }
  else {
    os <<
      fRepeatCommonPart <<
      endl;
  }

  // print the repeat endings
  int endingsNumber =
    fRepeatEndings.size ();

  os <<
    "Repeat endings: ";
  if (endingsNumber) {
    os << "(" << endingsNumber << ")";
  }
  else {
    os << "none";
  }
  os << endl;

  if (endingsNumber) {
    gIndenter++;

    vector<S_msrRepeatEnding>::const_iterator
      iBegin = fRepeatEndings.begin (),
      iEnd   = fRepeatEndings.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the repeat ending
      os << (*i);
      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for

    gIndenter--;
  }

  gIndenter--;
}

void msrRepeat::shortPrint (ostream& os) const
{
  os <<
    "Repeat" <<
    ", " << fRepeatTimes << " times" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 28;

  os << left <<
    setw (fieldWidth) <<
   "repeatExplicitStartKind" << " : " <<
    repeatExplicitStartKindAsString (
      fRepeatExplicitStartKind) <<
    endl <<
    setw (fieldWidth) <<
    "repeat ending(s)" << " : " <<
    fRepeatEndings.size () <<
    endl;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceRepeats) {
    // print the current repeat build phase
    os <<
      setw (fieldWidth) <<
      "currentRepeatBuildPhaseKind" << " : " <<
      repeatBuildPhaseKindAsString (
        fCurrentRepeatBuildPhaseKind) <<
      endl;
  }
#endif

  gLogOstream << endl;

  // print the repeat common part
  if (! fRepeatCommonPart) {
    os <<
      "Common part: none" <<
      endl;
  }
  else {
    fRepeatCommonPart->shortPrint (os);
  }
  os << endl;

  // print the repeat endings
  int endingsNumber =
    fRepeatEndings.size ();

  os <<
    "Repeat endings: ";
  if (endingsNumber) {
    os << "(" << endingsNumber << ")";
  }
  else {
    os << "none";
  }
  os << endl;

  if (endingsNumber) {
    gIndenter++;

    vector<S_msrRepeatEnding>::const_iterator
      iBegin = fRepeatEndings.begin (),
      iEnd   = fRepeatEndings.end (),
      i      = iBegin;

    for ( ; ; ) {
      // print the repeat ending
      (*i)->shortPrint (os);
      if (++i == iEnd) break;
  // JMI    os << endl;
    } // for

    gIndenter--;
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrRepeat& elt)
{
  elt->print (os);
  return os;
}


}
