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

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
msrMeasureElement::msrMeasureElement (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
  fMeasureElementMeasureNumber      = K_NO_MEASURE_NUMBER;
  fMeasureElementPositionInMeasure  = K_NO_POSITION_MEASURE_NUMBER;
  fMeasureElementSoundingWholeNotes = rational (0, 1);
}

msrMeasureElement::~msrMeasureElement ()
{}

void msrMeasureElement::setMeasureElementPositionInMeasure (
  rational positionInMeasure,
  string   context)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTracePositionsInMeasures || gTraceOah->fTraceMeasures) {
    gLogOstream <<
      "Setting measure element position in measure of " <<
      asString () <<
      " to '" << positionInMeasure <<
      "' in measure '" <<
      fMeasureElementMeasureNumber <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

  // sanity check
  msrAssert (
    positionInMeasure != K_NO_POSITION_MEASURE_NUMBER,
    "positionInMeasure == K_NO_POSITION_MEASURE_NUMBER");

  fMeasureElementPositionInMeasure = positionInMeasure;
}

void msrMeasureElement::setMeasureElementSoundingWholeNotes (
  rational wholeNotes,
  string   context)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTracePositionsInMeasures || gTraceOah->fTraceMeasures) {
    gLogOstream <<
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
  msrAssert (
    wholeNotes != K_NO_WHOLE_NOTES,
    "wholeNotes == K_NO_WHOLE_NOTES");

  fMeasureElementSoundingWholeNotes = wholeNotes;
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

void msrMeasureElement::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrMeasureElement::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrMeasureElement>*
    p =
      dynamic_cast<visitor<S_msrMeasureElement>*> (v)) {
        S_msrMeasureElement elem = this;

#ifdef TRACE_OAH
        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrMeasureElement::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrMeasureElement::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrMeasureElement::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrMeasureElement>*
    p =
      dynamic_cast<visitor<S_msrMeasureElement>*> (v)) {
        S_msrMeasureElement elem = this;

#ifdef TRACE_OAH
        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
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
