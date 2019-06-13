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
  rational positionInMeasure)
{
  fPositionInMeasure =
    positionInMeasure;

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTracePositionsInMeasures || gTraceOptions->fTraceMeasures) {
    gLogIOstream <<
      "Setting position of measure element '" <<
      asString () <<
      "' in measure to '" << fPositionInMeasure <<
      "' in measure '" <<
      fMeasureNumber <<
      "'" <<
      endl;
  }
#endif

  if (fPositionInMeasure == K_NO_POSITION_MEASURE_NUMBER) {
    abort ();
  }
}

void msrMeasureElement::acceptIn (basevisitor* v)
{
#ifdef TRACE_OPTIONS
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
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
          gLogIOstream <<
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
    gLogIOstream <<
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
          gLogIOstream <<
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
