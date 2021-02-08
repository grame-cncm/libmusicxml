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

#include <regex>

#include "utilities.h"

#include "messagesHandling.h"

#include "msrClefs.h"

#include "oahOah.h"

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrClef msrClef::create (
  int         inputLineNumber,
  msrClefKind clefKind,
  int         clefStaffNumber)
{
  msrClef* o =
    new msrClef (
      inputLineNumber, clefKind, clefStaffNumber);
  assert (o != nullptr);
  return o;
}

msrClef::msrClef (
  int         inputLineNumber,
  msrClefKind clefKind,
  int         clefStaffNumber)
    : msrMeasureElement (inputLineNumber)
{
  fClefKind = clefKind;
  fClefStaffNumber = clefStaffNumber;
}

msrClef::~msrClef ()
{}

//________________________________________________________________________
S_msrClef msrClef::createClefFromString (
  int    inputLineNumber,
  string clefString,
  int    clefLineNumber)
{
  /*
    Handles clefString à la LilyPond, such as 'treble' or 'baritone'
  */

  S_msrClef result;

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceNotes ()) {
    gLogStream <<
      "Creating clef from string \"" <<
      clefString <<
      "', line " << inputLineNumber <<
      endl;
  }
#endif

  string regularExpression (
    "[[:space:]]*"
    "([[:alpha:]]+)" // clefName
    "[[:space:]]*"
    );

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "regularExpression = " <<
      regularExpression <<
      endl;
  }
#endif

  regex  e (regularExpression);
  smatch sm;

  regex_match (clefString, sm, e);

  unsigned int smSize = sm.size ();

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "There are " << smSize << " matches" <<
      " for clef string \"" << clefString <<
      "\" with regex \"" << regularExpression <<
      "\":" <<
      endl;

    ++gIndenter;

    for (unsigned i = 0; i < smSize; ++i) {
      gLogStream <<
        i << ": " << "\"" << sm [i] << "\"" <<
        endl;
    } // for
    gLogStream << endl;

    --gIndenter;
  }
#endif

  //  Handles clefString à la LilyPond, such as c [major] or bes minor

  if (smSize != 2) {
    stringstream s;

    s <<
      "clefString \"" << clefString <<
      "\" is ill-formed";

    msdlError (
      gGlobalOahOahGroup->getInputSourceName (),
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  string clefName = sm [1];

  // compute the clefKind from the clefName
  msrClefKind
    clefKind =
      clefKindFromString (
        inputLineNumber,
        clefName);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    gLogStream <<
      "clefName = \"" <<
      beatsNumber <<
      "\"" <<
      endl;
  }
#endif

  // create the clef
  result =
    msrClef::create (
      inputLineNumber,
      clefKind,
      clefLineNumber);

  return result;
}

bool msrClef::clefIsATablatureClef () const
{
  switch (fClefKind) {
    case kTablature4Clef:
    case kTablature5Clef:
    case kTablature6Clef:
    case kTablature7Clef:
      return true;
      break;
    default:
      return false;
  } // switch
}

bool msrClef::clefIsAPercussionClef () const
{
  switch (fClefKind) {
    case kPercussionClef:
      return true;
      break;
    default:
      return false;
  } // switch
}

bool msrClef::isEqualTo (S_msrClef otherClef) const
{
  if (! otherClef) {
    return false;
  }

  return
    fClefKind == otherClef->fClefKind
      &&
    fClefStaffNumber == otherClef->fClefStaffNumber;
}

void msrClef::acceptIn (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrClef::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrClef>*
    p =
      dynamic_cast<visitor<S_msrClef>*> (v)) {
        S_msrClef elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrClef::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrClef::acceptOut (basevisitor* v)
{
  if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
    gLogStream <<
      "% ==> msrClef::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrClef>*
    p =
      dynamic_cast<visitor<S_msrClef>*> (v)) {
        S_msrClef elem = this;

        if (gGlobalMsrOahGroup->getTraceMsrVisitors ()) {
          gLogStream <<
            "% ==> Launching msrClef::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrClef::browseData (basevisitor* v)
{}

string msrClef::asString () const
{
  stringstream s;

  s <<
    "[Clef " <<
    msrClefKindAsString (fClefKind) <<
    ", clefStaffNumber: " << fClefStaffNumber <<
    "], line " << fInputLineNumber;

  return s.str ();
}

void msrClef::print (ostream& os) const
{
  os <<
    "Clef " <<
    msrClefKindAsString (fClefKind) <<
    ", clefStaffNumber: " << fClefStaffNumber <<
    ", line " << fInputLineNumber <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrClef& elt)
{
  elt->print (os);
  return os;
}


}
