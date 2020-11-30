/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <climits>      // INT_MIN, INT_MAX

#include "utilities.h"
#include "messagesHandling.h"

#include "msrSegnosAndCodas.h"

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
msrMeasureElement::msrMeasureElement (
  int inputLineNumber)
    : msrElement (inputLineNumber),
      fMeasureElementMomentInMeasure (
        K_NO_POSITION, K_NO_POSITION),
      fMeasureElementMomentInVoice (
        K_NO_POSITION, K_NO_POSITION)
{
  fMeasureElementSoundingWholeNotes = rational (0, 1);

  fMeasureElementMeasureNumber = K_NO_MEASURE_NUMBER;

  fMeasureElementPositionInMeasure = K_NO_POSITION;
  fMeasureElementPositionInVoice   = K_NO_POSITION;
}

msrMeasureElement::~msrMeasureElement ()
{}

void msrMeasureElement::setMeasureElementSoundingWholeNotes (
  rational wholeNotes,
  string   context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePositionsInMeasures () || gGlobalTraceOahGroup->getTraceMeasures ()) {
    gLogStream <<
      "Setting measure element sounding whole notes of " <<
      asString () <<
      " to '" << wholeNotes <<
      "' in measure '" <<
      fMeasureElementMeasureNumber <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
    wholeNotes != K_NO_WHOLE_NOTES,
    "wholeNotes == K_NO_WHOLE_NOTES");

  fMeasureElementSoundingWholeNotes = wholeNotes;
}

void msrMeasureElement::setMeasureElementPositionInMeasure (
  rational positionInMeasure,
  string   context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Setting measure element position in measure of " <<
      asString () <<
      " to '" << positionInMeasure <<
      "' (was '" <<
      fMeasureElementPositionInMeasure <<
      "') in measure '" <<
      fMeasureElementMeasureNumber <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif
/* JMI
  if (positionInMeasure == K_NO_POSITION) abort();

  // sanity check
  msgAssert (
    positionInMeasure != K_NO_POSITION,
    "positionInMeasure == K_NO_POSITION");
*/

  fMeasureElementPositionInMeasure = positionInMeasure;
}

void msrMeasureElement::setMeasureElementPositionInVoice (
  rational positionInVoice,
  string   context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Setting measure element position in voice of " <<
      asString () <<
      " to '" << positionInVoice <<
      "' in measure '" <<
      fMeasureElementMeasureNumber <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
    positionInVoice != K_NO_POSITION,
    "positionInVoice == K_NO_POSITION");

  fMeasureElementPositionInVoice = positionInVoice;
}

void msrMeasureElement::setMeasureElementMomentInMeasure (
  const msrMoment& momentInMeasure,
  string           context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMomentsInMeasures ()) {
    gLogStream <<
      "Setting measure element moment in measure of " <<
      asString () <<
      " to '" << momentInMeasure <<
      "' (was '" <<
      fMeasureElementMomentInMeasure.asString () <<
      "') in measure '" <<
      fMeasureElementMeasureNumber <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

  fMeasureElementMomentInMeasure = momentInMeasure;
}

void msrMeasureElement::setMeasureElementMomentInVoice (
  const msrMoment& momentInVoice,
  string           context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceMomentsInMeasures ()) {
    gLogStream <<
      "Setting measure element moment in voice of " <<
      asString () <<
      " to '" << momentInVoice <<
      "' in measure '" <<
      fMeasureElementMeasureNumber <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
    momentInVoice != K_NO_MOMENT,
    "momentInVoice == K_NO_MOMENT");

  fMeasureElementMomentInVoice = momentInVoice;
}

bool msrMeasureElement::compareMeasureElementsByIncreasingPositionInMeasure (
  const SMARTP<msrMeasureElement>& first,
  const SMARTP<msrMeasureElement>& second)
{
  return
    first->getMeasureElementPositionInMeasure ()
      <
    second->getMeasureElementPositionInMeasure ();
}

void msrMeasureElement::assignMeasureElementPositionInVoice (
  rational& positionInVoice,
  string    context)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Assigning measure element position in voice of " <<
      asString () <<
      " to '" << positionInVoice <<
      "' in measure '" <<
      fMeasureElementMeasureNumber <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msgAssert (
    positionInVoice != K_NO_POSITION,
    "positionInVoice == K_NO_POSITION");

  // set measure element position in voice
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Setting measure element position in voice of " <<
      asString () <<
      " to '" << positionInVoice <<
      "' in measure '" <<
      fMeasureElementMeasureNumber <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

  fMeasureElementPositionInVoice = positionInVoice;

  // account for it in positionInVoice
  positionInVoice +=
    fMeasureElementSoundingWholeNotes;
  positionInVoice.rationalise ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePositionsInMeasures ()) {
    gLogStream <<
      "Position in voice becomes " <<
      positionInVoice <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif
}

void msrMeasureElement::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasureElement::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrMeasureElement>*
    p =
      dynamic_cast<visitor<S_msrMeasureElement>*> (v)) {
        S_msrMeasureElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasureElement::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrMeasureElement::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrMeasureElement::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrMeasureElement>*
    p =
      dynamic_cast<visitor<S_msrMeasureElement>*> (v)) {
        S_msrMeasureElement elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrMeasureElement::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

string msrMeasureElement::asString () const
{
  // this is overriden all in actual elements
  return "??? msrMeasureElement::asString () ???";
}

string msrMeasureElement::asShortString () const
{
  // this can be overriden in actual elements
  return asString ();
}

void msrMeasureElement::print (ostream& os) const
{
  os << asString () << endl;
}

ostream& operator<< (ostream& os, const S_msrMeasureElement& elt)
{
  elt->print (os);
  return os;
}


}
