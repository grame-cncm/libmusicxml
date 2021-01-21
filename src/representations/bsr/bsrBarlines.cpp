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

#include "bsrBarlines.h"

#include "utilities.h"
#include "messagesHandling.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

//#include "oahOah.h"

#include "bsrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_bsrBarline bsrBarline::create (
  int            inputLineNumber,
  bsrBarlineKind barlineKind)
{
  bsrBarline* o =
    new bsrBarline (
      inputLineNumber, barlineKind);
  assert (o != nullptr);

  return o;
}

bsrBarline::bsrBarline (
  int            inputLineNumber,
  bsrBarlineKind barlineKind)
    : bsrLineContentsElement (inputLineNumber)
{
  fBarlineKind = barlineKind;

  fBarlineCellsList = buildCellsList ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceBarlines ()) {
    gLogStream <<
      "Creating bsrBarline '" <<
      asString () <<
      "', line " <<
      fInputLineNumber <<
      endl;
  }
#endif
}

bsrBarline::~bsrBarline ()
{}

S_bsrCellsList bsrBarline::buildCellsList () const
{
  S_bsrCellsList
    result =
      bsrCellsList::create (
        fInputLineNumber);

  switch (fBarlineKind) {
    case bsrBarline::kBarlineKindNone:
      break;
    case bsrBarline::kBarlineKindSpecial:
      result->
        appendCellKindToCellsList (
          kDots13);
      break;
    case bsrBarline::kBarlineKindUnusual:
      result->
        appendCellKindToCellsList (
          kDots123);
      break;
    case bsrBarline::kBarlineKindFinalDouble:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber,
            kDots126, kDots13));
      break;
    case bsrBarline::kBarlineKindSectionalDouble:
      result->
        appendCellsListToCellsList (
          bsrCellsList::create (
            fInputLineNumber,
            kDots126, kDots13, kDots3));
      break;
  } // switch

  return result;
}

int bsrBarline::fetchCellsNumber() const
{
  return fBarlineCellsList->fetchCellsNumber();
}

void bsrBarline::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrBarline::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrBarline>*
    p =
      dynamic_cast<visitor<S_bsrBarline>*> (v)) {
        S_bsrBarline elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrBarline::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrBarline::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrBarline::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrBarline>*
    p =
      dynamic_cast<visitor<S_bsrBarline>*> (v)) {
        S_bsrBarline elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrBarline::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrBarline::browseData (basevisitor* v)
{}

string bsrBarline::barlineKindAsString (
  bsrBarlineKind barlineKind)
{
  string result;

  switch (barlineKind) {
    case bsrBarline::kBarlineKindNone:
      result = "barlineKindNone";
      break;
    case bsrBarline::kBarlineKindSpecial:
      result = "barlineKindSpecial";
      break;
    case bsrBarline::kBarlineKindUnusual:
      result = "barlineKindUnusual";
      break;
    case bsrBarline::kBarlineKindFinalDouble:
      result = "barlineKindFinalDouble";
      break;
    case bsrBarline::kBarlineKindSectionalDouble:
      result = "barlineKindSectionalDouble";
      break;
  } // switch

  return result;
}

string bsrBarline::barlineKindAsDebugString (
  bsrBarlineKind barlineKind)
{
  string result;

  switch (barlineKind) {
    case bsrBarline::kBarlineKindNone:
      result = "***None***";
      break;
    case bsrBarline::kBarlineKindSpecial:
      result = "Special";
      break;
    case bsrBarline::kBarlineKindUnusual:
      result = "Unusual";
      break;
    case bsrBarline::kBarlineKindFinalDouble:
      result = "FinalDouble";
      break;
    case bsrBarline::kBarlineKindSectionalDouble:
      result = "||"; // L'\u2016' ??? JMI
//      result = L'\u1D101';
      break;
  } // switch

  return result;
}

string bsrBarline::asString () const
{
  stringstream s;

  s <<
    "Barline" <<
    ", " << barlineKindAsString (fBarlineKind) <<
    ", barlineCellsList: " << fBarlineCellsList->asShortString () <<
    ", line " << fInputLineNumber;

  return s.str ();
}

string bsrBarline::asDebugString () const
{
  stringstream s;

  s <<
    "[BARLINE " <<
    barlineKindAsDebugString (fBarlineKind) <<
    "]";

  return s.str ();
}

void bsrBarline::print (ostream& os) const
{
  os <<
    "Barline" <<
    ", line " << fInputLineNumber <<
    endl;

  ++gIndenter;

  const unsigned int fieldWidth = 17;

  os <<
    setw (fieldWidth) <<
    "barlineKind" << " : " << barlineKindAsString (fBarlineKind) <<
    endl <<
    setw (fieldWidth) <<
    "barlineCellsList" << " : " << fBarlineCellsList->asShortString () <<
    endl;

  --gIndenter;
}

ostream& operator<< (ostream& os, const S_bsrBarline& elt)
{
  elt->print (os);
  return os;
}


}
