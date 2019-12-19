/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iostream>
#include <sstream>
#include <iomanip>      // setw, setprecision, ...

#include "msrBars.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrBarCheck msrBarCheck::create (
  int inputLineNumber)
{
  msrBarCheck* o =
    new msrBarCheck (
      inputLineNumber);
  assert(o!=0);
  return o;
}

S_msrBarCheck msrBarCheck::createWithNextBarPuristNumber (
  int    inputLineNumber,
  string nextBarOriginalNumber,
  int    nextBarPuristNumber)
{
  msrBarCheck* o =
    new msrBarCheck (
      inputLineNumber,
      nextBarOriginalNumber,
      nextBarPuristNumber);
  assert(o!=0);
  return o;
}

msrBarCheck::msrBarCheck (
  int inputLineNumber)
    : msrMeasureElement (inputLineNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresNumbers) {
    gLogOstream <<
      "Creating a bar check without next bar number" <<
      endl;
  }
#endif
}

msrBarCheck::msrBarCheck (
  int    inputLineNumber,
  string nextBarOriginalNumber,
  int    nextBarPuristNumber)
    : msrMeasureElement (inputLineNumber)
{
  fNextBarOriginalNumber = nextBarOriginalNumber;
  fNextBarPuristNumber   = nextBarPuristNumber;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresNumbers) {
    gLogOstream <<
      "Creating a bar check" <<
      " with next bar original number '" <<
      nextBarOriginalNumber <<
      " and next bar purist number '" <<
      fNextBarPuristNumber <<
      "'" <<
      endl;
  }
#endif
}

msrBarCheck::~msrBarCheck ()
{}

void msrBarCheck::setNextBarPuristNumber (int puristNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresNumbers) {
    gLogOstream <<
      "Setting bar check next bar number to '" <<
      puristNumber <<
      "'" <<
      endl;
  }
#endif

  fNextBarPuristNumber = puristNumber;
}

void msrBarCheck::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrBarCheck::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrBarCheck>*
    p =
      dynamic_cast<visitor<S_msrBarCheck>*> (v)) {
        S_msrBarCheck elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrBarCheck::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrBarCheck::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrBarCheck::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrBarCheck>*
    p =
      dynamic_cast<visitor<S_msrBarCheck>*> (v)) {
        S_msrBarCheck elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrBarCheck::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrBarCheck::browseData (basevisitor* v)
{}

string msrBarCheck::asString () const
{
  stringstream s;

  s <<
    "BarCheck" <<
    ", nextBarOriginalNumber = \"" << fNextBarOriginalNumber << "\"" <<
    ", nextBarPuristNumber = \"" << fNextBarPuristNumber << "\"" <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrBarCheck::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrBarCheck& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrBarNumberCheck msrBarNumberCheck::create (
  int    inputLineNumber,
  string nextBarOriginalNumber,
  int    nextBarPuristNumber)
{
  msrBarNumberCheck* o =
    new msrBarNumberCheck (
      inputLineNumber,
      nextBarOriginalNumber,
      nextBarPuristNumber);
  assert(o!=0);
  return o;
}

msrBarNumberCheck::msrBarNumberCheck (
  int    inputLineNumber,
  string nextBarOriginalNumber,
  int    nextBarPuristNumber)
    : msrMeasureElement (inputLineNumber)
{
  fNextBarOriginalNumber = nextBarOriginalNumber;
  fNextBarPuristNumber   = nextBarPuristNumber;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceMeasuresNumbers) {
    gLogOstream <<
      "Creating a bar number check" <<
      " with next bar original number '" <<
      nextBarOriginalNumber <<
      " and next bar purist number '" <<
      fNextBarPuristNumber <<
      "'" <<
      endl;
  }
#endif
}

msrBarNumberCheck::~msrBarNumberCheck ()
{}

void msrBarNumberCheck::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrBarNumberCheck::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrBarNumberCheck>*
    p =
      dynamic_cast<visitor<S_msrBarNumberCheck>*> (v)) {
        S_msrBarNumberCheck elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrBarNumberCheck::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrBarNumberCheck::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrBarNumberCheck::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrBarNumberCheck>*
    p =
      dynamic_cast<visitor<S_msrBarNumberCheck>*> (v)) {
        S_msrBarNumberCheck elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrBarNumberCheck::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrBarNumberCheck::browseData (basevisitor* v)
{}

string msrBarNumberCheck::asString () const
{
  stringstream s;

  s <<
    "BarNumberCheck" <<
    ", nextBarOriginalNumber = \"" << fNextBarOriginalNumber << "\"" <<
    ", nextBarPuristNumber = \"" << fNextBarPuristNumber << "\"";

  return s.str ();
}

void msrBarNumberCheck::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrBarNumberCheck& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrBarline msrBarline::create (
  int                           inputLineNumber,
  msrBarlineCategoryKind        barlineCategoryKind,
  msrBarlineHasSegnoKind        barlineHasSegnoKind,
  msrBarlineHasCodaKind         barlineHasCodaKind,
  msrBarlineLocationKind        barlineLocationKind,
  msrBarlineStyleKind           barlineStyleKind,
  msrBarlineEndingTypeKind      barlineEndingTypeKind,
  string                        endingNumber,
  msrBarlineRepeatDirectionKind barlineRepeatDirectionKind,
  msrBarlineRepeatWingedKind    barlineRepeatWingedKind,
  int                           barlineTimes)
{
  msrBarline* o =
    new msrBarline (
      inputLineNumber,
      barlineCategoryKind,
      barlineHasSegnoKind,
      barlineHasCodaKind,
      barlineLocationKind,
      barlineStyleKind,
      barlineEndingTypeKind,
      endingNumber,
      barlineRepeatDirectionKind,
      barlineRepeatWingedKind,
      barlineTimes);
  assert(o!=0);
  return o;
}

msrBarline::msrBarline (
  int                           inputLineNumber,
  msrBarlineCategoryKind        barlineCategoryKind,
  msrBarlineHasSegnoKind        barlineHasSegnoKind,
  msrBarlineHasCodaKind         barlineHasCodaKind,
  msrBarlineLocationKind        barlineLocationKind,
  msrBarlineStyleKind           barlineStyleKind,
  msrBarlineEndingTypeKind      barlineEndingTypeKind,
  string                        endingNumber,
  msrBarlineRepeatDirectionKind barlineRepeatDirectionKind,
  msrBarlineRepeatWingedKind    barlineRepeatWingedKind,
  int                           barlineTimes)
    : msrMeasureElement (inputLineNumber)
{
  fBarlineCategoryKind = barlineCategoryKind;

  fBarlineHasSegnoKind = barlineHasSegnoKind;
  fBarlineHasCodaKind  = barlineHasCodaKind;

  fLocationKind        = barlineLocationKind;
  fStyleKind           = barlineStyleKind;
  fEndingTypeKind      = barlineEndingTypeKind;
  fEndingNumber        = endingNumber;
  fRepeatDirectionKind = barlineRepeatDirectionKind;
  fRepeatWingedKind    = barlineRepeatWingedKind;

  fBarlineTimes = barlineTimes;

  // JMI gLogOstream << "fEndingNumber = " << fEndingNumber << endl;

  // extract individual numbers from fEndingNumber
  // that may contain "1, 2"
  fEndingNumbersList =
    extractNumbersFromString (
      fEndingNumber, false); // 'true' to debug it
}

msrBarline::~msrBarline ()
{}

void msrBarline::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrBarline::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrBarline>*
    p =
      dynamic_cast<visitor<S_msrBarline>*> (v)) {
        S_msrBarline elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrBarline::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrBarline::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrBarline::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrBarline>*
    p =
      dynamic_cast<visitor<S_msrBarline>*> (v)) {
        S_msrBarline elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrBarline::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrBarline::browseData (basevisitor* v)
{}

string msrBarline::barlineLocationKindAsString (
  msrBarlineLocationKind barlineLocationKind)
{
  string result;

  switch (barlineLocationKind) {
    case kBarlineLocationNone:
      result = "barlineLocationNone";
      break;
    case kBarlineLocationLeft:
      result = "barlineLocationLeft";
      break;
    case kBarlineLocationMiddle:
      result = "barlineLocationMiddle";
      break;
    case kBarlineLocationRight:
      result = "barlineLocationRight";
      break;
  } // switch

  return result;
}

string msrBarline::barlineCategoryKindAsString (
  msrBarlineCategoryKind barlineCategoryKind)
{
  string result;

  switch (barlineCategoryKind) {
    case k_NoBarlineCategory:
      result = "noBarlineCategory";
      break;
    case kBarlineCategoryStandalone:
      result = "barlineCategoryStandalone";
      break;
    case kBarlineCategoryRepeatStart:
      result = "barlineCategoryRepeatStart";
      break;
    case kBarlineCategoryRepeatEnd:
      result = "barlineCategoryRepeatEnd";
      break;
    case kBarlineCategoryHookedEndingStart:
      result = "barlineCategoryHookedEndingStart";
      break;
    case kBarlineCategoryHookedEndingEnd:
      result = "kBarlineCategoryHookedEndingEnd";
      break;
    case kBarlineCategoryHooklessEndingStart:
      result = "barlineCategoryHooklessEndingStart";
      break;
    case kBarlineCategoryHooklessEndingEnd:
      result = "barlineCategoryHooklessEndingEnd";
      break;
  } // switch

  return result;
}

string msrBarline::barlineHasSegnoKindAsString (
  msrBarlineHasSegnoKind barlineHasSegnoKind)
{
  string result;

  switch (barlineHasSegnoKind) {
    case msrBarline::kBarlineHasSegnoYes:
      result = "barlineHasSegnoYes";
      break;
    case msrBarline::kBarlineHasSegnoNo:
      result = "barlineHasSegnoNo";
      break;
  } // switch

  return result;
}

string msrBarline::barlineHasCodaKindAsString (
  msrBarlineHasCodaKind barlineHasCodaKind)
{
  string result;

  switch (barlineHasCodaKind) {
    case msrBarline::kBarlineHasCodaYes:
      result = "barlineHasCodaYes";
      break;
    case msrBarline::kBarlineHasCodaNo:
      result = "barlineHasCodaNo";
      break;
  } // switch

  return result;
}

string msrBarline::barlineStyleKindAsString (
  msrBarlineStyleKind barlineStyleKind)
{
  string result;

  switch (barlineStyleKind) {
    case msrBarline::kBarlineStyleNone:
      result = "barlineStyleNone";
      break;
    case msrBarline::kBarlineStyleRegular:
      result = "barlineStyleRegular";
      break;
    case msrBarline::kBarlineStyleDotted:
      result = "barlineStyleDotted";
      break;
    case msrBarline::kBarlineStyleDashed:
      result = "barlineStyleDashed";
      break;
    case msrBarline::kBarlineStyleHeavy:
      result = "barlineStyleHeavy";
      break;
    case msrBarline::kBarlineStyleLightLight:
      result = "barlineStyleLightLight";
      break;
    case msrBarline::kBarlineStyleLightHeavy:
      result = "barlineStyleLightHeavy";
      break;
    case msrBarline::kBarlineStyleHeavyLight:
      result = "barlineStyleHeavyLight";
      break;
    case msrBarline::kBarlineStyleHeavyHeavy:
      result = "barlineStyleHeavyHeavy";
      break;
    case msrBarline::kBarlineStyleTick:
      result = "barlineStyleTick";
      break;
    case msrBarline::kBarlineStyleShort:
      result = "barlineStyleShort";
      break;
  } // switch

  return result;
}

string msrBarline::barlineEndingTypeKindAsString (
  msrBarlineEndingTypeKind barlineEndingTypeKind)
{
  string result;

  switch (barlineEndingTypeKind) {
    case msrBarline::kBarlineEndingNone:
      result = "barlineEndingNone";
      break;
    case msrBarline::kBarlineEndingTypeStart:
      result = "barlineEndingTypeStart";
      break;
    case msrBarline::kBarlineEndingTypeStop:
      result = "barlineEndingTypeStop";
      break;
    case msrBarline::kBarlineEndingTypeDiscontinue:
      result = "barlineEndingTypeDiscontinue";
      break;
  } // switch

  return result;
}

string msrBarline::barlineRepeatDirectionKindAsString (
  msrBarlineRepeatDirectionKind barlineRepeatDirectionKind)
{
  string result;

  switch (barlineRepeatDirectionKind) {
    case msrBarline::kBarlineRepeatDirectionNone:
      result ="barlineRepeatDirectionNone";
      break;
    case msrBarline::kBarlineRepeatDirectionForward:
      result ="barlineRepeatDirectionForward";
      break;
    case msrBarline::kBarlineRepeatDirectionBackward:
      result ="barlineRepeatDirectionBackward";
      break;
  } // switch

  return result;
}

string msrBarline::barlineRepeatWingedKindAsString (
  msrBarlineRepeatWingedKind barlineRepeatWingedKind)
{
  string result;

  switch (barlineRepeatWingedKind) {
    case msrBarline::kBarlineRepeatWingedNone:
      result = "barlineRepeatWingedNone";
      break;
    case msrBarline::kBarlineRepeatWingedStraight:
      result = "barlineRepeatWingedStraight";
      break;
    case msrBarline::kBarlineRepeatWingedCurved:
      result = "barlineRepeatWingedCurved";
      break;
    case msrBarline::kBarlineRepeatWingedDoubleStraight:
      result = "barlineRepeatWingedDoubleStraight";
      break;
    case msrBarline::kBarlineRepeatWingedDoubleCurved:
      result = "barlineRepeatWingedDoubleCurved";
      break;
  } // switch

  return result;
}

string msrBarline::endingNumbersListAsString () const
{
  stringstream s;

  if (fEndingNumbersList.size ()) {
    list<int>::const_iterator
      iBegin = fEndingNumbersList.begin (),
      iEnd   = fEndingNumbersList.end (),
      i      = iBegin;
    for ( ; ; ) {
      s << (*i);
      if (++i == iEnd) break;
      s << " ";
    } // for
  }

  return s.str ();
}

string msrBarline::asShortString () const
{
  stringstream s;

  s <<
    "["
    "Barline " <<
    barlineCategoryKindAsString (fBarlineCategoryKind) <<
    ", measureNumber " << fMeasureNumber <<
    ", positionInMeasure " << fPositionInMeasure <<

/* JMI
    ", " <<
    barlineHasSegnoKindAsString (
      fBarlineHasSegnoKind) <<
    ", " <<
    barlineHasCodaKindAsString (
      fBarlineHasCodaKind) <<

    ", " <<
    barlineLocationKindAsString (fLocationKind) <<
    ", " <<
    barlineStyleKindAsString (fStyleKind) <<
    ", " <<
    barlineEndingTypeKindAsString (fEndingTypeKind) <<
    ", " <<
    endingNumbersListAsString () <<

    ", " <<
    barlineRepeatDirectionKindAsString (fRepeatDirectionKind) <<
    ", " <<
    barlineRepeatWingedKindAsString (fRepeatWingedKind) <<
    */
    ", " <<
    fBarlineTimes << " times" <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

string msrBarline::asString () const
{
  stringstream s;

  s <<
    "["
    "Barline " <<
    barlineCategoryKindAsString (fBarlineCategoryKind) <<
    ", measureNumber " << fMeasureNumber <<
    ", positionInMeasure " << fPositionInMeasure <<
    ", " <<
    barlineHasSegnoKindAsString (
      fBarlineHasSegnoKind) <<
    ", " <<
    barlineHasCodaKindAsString (
      fBarlineHasCodaKind) <<

    ", " <<
    barlineLocationKindAsString (fLocationKind) <<
    ", " <<
    barlineStyleKindAsString (fStyleKind) <<

    ", " <<
    barlineEndingTypeKindAsString (fEndingTypeKind) <<
    ", " <<
    endingNumbersListAsString () <<

    ", " <<
    barlineRepeatDirectionKindAsString (fRepeatDirectionKind) <<
    ", " <<
    barlineRepeatWingedKindAsString (fRepeatWingedKind) <<

    ", " <<
    fBarlineTimes << " times" <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrBarline::print (ostream& os) const
{
  os <<
    "Barline, " <<
    barlineCategoryKindAsString (
      fBarlineCategoryKind) <<
    ", measureNumber " << fMeasureNumber <<
    ", positionInMeasure " << fPositionInMeasure <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 24;

  os << left <<
    setw (fieldWidth) <<
    "barlineHasSegnoKind" << " : " <<
    barlineHasSegnoKindAsString (
      fBarlineHasSegnoKind) <<
    endl <<
    setw (fieldWidth) <<
    "barlineHasCodaKind" << " : " <<
    barlineHasCodaKindAsString (
      fBarlineHasCodaKind) <<
    endl <<

    setw (fieldWidth) <<
    "locationKind" << " : " <<
    barlineLocationKindAsString (fLocationKind) <<
    endl <<
    setw (fieldWidth) <<
    "styleKind" << " : " <<
    barlineStyleKindAsString (fStyleKind) <<
    endl <<
    setw (fieldWidth) <<
    "endingTypeKind" << " : " <<
    barlineEndingTypeKindAsString (fEndingTypeKind) <<
    endl <<

    setw (fieldWidth) <<
    "endingNumber" << " : " <<
    fEndingNumber <<
    endl <<
    setw (fieldWidth) <<
    "endingNumbersList" << " : " <<
    endingNumbersListAsString () <<
    endl <<

    setw (fieldWidth) <<
    "repeatDirectionKind" << " : " <<
    barlineRepeatDirectionKindAsString (fRepeatDirectionKind) <<
    endl <<

    setw (fieldWidth) <<
    "repeatWingedKind" << " : " <<
    barlineRepeatWingedKindAsString (fRepeatWingedKind) <<
    endl <<

    setw (fieldWidth) <<
    "barlineTimes" << " : " <<
    fBarlineTimes <<
    endl <<

    setw (fieldWidth) <<
    "barlineCategoryKind" << " : " <<
    barlineCategoryKindAsString (
      fBarlineCategoryKind) <<
    endl <<

    setw (fieldWidth) <<
    "measureNumber" << " : " <<
    fMeasureNumber <<
    endl <<
    setw (fieldWidth) <<
    "positionInMeasure" << " : " <<
    fPositionInMeasure <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrBarline& elt)
{
  elt->print (os);
  return os;
}


}
