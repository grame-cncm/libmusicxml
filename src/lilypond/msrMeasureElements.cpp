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

#include "msrMeasureElements.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "msrOptions.h"

#include "messagesHandling.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
msrMeasureElement::msrMeasureElement (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
  fMeasureNumber     = K_NO_MEASURE_NUMBER;
  fPositionInMeasure = K_NO_POSITION_MEASURE_NUMBER;
}

msrMeasureElement::~msrMeasureElement ()
{}

void msrMeasureElement::setPositionInMeasure (
  rational positionInMeasure,
  string   context)
{
  // sanity check
  msrAssert (
    positionInMeasure != K_NO_POSITION_MEASURE_NUMBER,
    "positionInMeasure == K_NO_POSITION_MEASURE_NUMBER");

  fPositionInMeasure =
    positionInMeasure;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePositionsInMeasures || gTraceOptions->fTraceMeasures) {
    glogIOstream <<
      "Setting position of measure element '" <<
      asString () <<
      "' in measure to '" << fPositionInMeasure <<
      "' in measure '" <<
      fMeasureNumber <<
      "', context: \"" <<
      context <<
      "\"" <<
      endl;
  }
#endif

/* JMI
  if (
    // boolean item?
    S_msrNote
      note =
        dynamic_cast<msrNote*>(&(*this))
  ) {
    if (positionInMeasure == rational (1, 1)) {
      abort (); // JMI
    }
  }
  */
}

bool msrMeasureElement::compareMeasureElementsByIncreasingPositionInMeasure (
  const SMARTP<msrMeasureElement>& first,
  const SMARTP<msrMeasureElement>& second)
{
  return
    first->getPositionInMeasure ()
      <
    second->getPositionInMeasure ();
}

void msrMeasureElement::acceptIn (basevisitor* v)
{
#ifdef TRACE_OPTIONS
  if (gMsrOptions->fTraceMsrVisitors) {
    glogIOstream <<
      "% ==> msrMeasureElement::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_msrMeasureElement>*
    p =
      dynamic_cast<visitor<S_msrMeasureElement>*> (v)) {
        S_msrMeasureElement elem = this;

#ifdef TRACE_OPTIONS
        if (gMsrOptions->fTraceMsrVisitors) {
          glogIOstream <<
            "% ==> Launching msrMeasureElement::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void msrMeasureElement::acceptOut (basevisitor* v)
{
#ifdef TRACE_OPTIONS
  if (gMsrOptions->fTraceMsrVisitors) {
    glogIOstream <<
      "% ==> msrMeasureElement::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_msrMeasureElement>*
    p =
      dynamic_cast<visitor<S_msrMeasureElement>*> (v)) {
        S_msrMeasureElement elem = this;

#ifdef TRACE_OPTIONS
        if (gMsrOptions->fTraceMsrVisitors) {
          glogIOstream <<
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

void msrMeasureElement::print (ostream& os)
{
  os << asString () << endl;
}

ostream& operator<< (ostream& os, const S_msrMeasureElement& elt)
{
  elt->print (os);
  return os;
}


}
