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

#include "msrBeams.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "msrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrBeam msrBeam::create (
  int         inputLineNumber,
  int         number,
  msrBeamKind beamKind)
{
  msrBeam* o =
    new msrBeam (
      inputLineNumber, number, beamKind);
  assert(o!=0);
  return o;
}

msrBeam::msrBeam (
  int         inputLineNumber,
  int         number,
  msrBeamKind beamKind)
    : msrElement (inputLineNumber)
{
  fBeamNumber = number;
  fBeamKind   = beamKind;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceBeams) {
    gLogOstream <<
      "Creating beam '" <<
      this->asString () <<
      "'" <<
      endl;
  }
#endif
}

msrBeam::~msrBeam ()
{}

void msrBeam::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrBeam::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrBeam>*
    p =
      dynamic_cast<visitor<S_msrBeam>*> (v)) {
        S_msrBeam elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrBeam::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrBeam::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrBeam::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrBeam>*
    p =
      dynamic_cast<visitor<S_msrBeam>*> (v)) {
        S_msrBeam elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrBeam::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrBeam::browseData (basevisitor* v)
{}

string msrBeam::beamKindAsString (
  msrBeamKind beamKind)
{
  string result;

  switch (beamKind) {
    case kBeginBeam:
      result = "begin";
      break;
    case kContinueBeam:
      result = "continue";
      break;
    case kEndBeam:
      result = "end";
      break;
    case kForwardHookBeam:
      result = "forward";
      break;
    case kBackwardHookBeam:
      result = "backward";
      break;
    case k_NoBeam:
      result = "### none ###";
      break;
  } // switch

  return result;
}

string msrBeam::asString () const
{
  stringstream s;

  s <<
    "Beam" <<
    " " << beamKindAsString (fBeamKind) <<
    ", number " << fBeamNumber <<
    ", line " << fInputLineNumber;

  return s.str ();
}

void msrBeam::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_msrBeam& elt)
{
  elt->print (os);
  return os;
}


}
