/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <iostream>
#include <sstream>

#include "msrBeams.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "msrOptions.h"


using namespace std;

namespace MusicXML2 {

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

#ifdef TRACE_OPTIONS
  if (gTraceOptions->fTraceBeams) {
    gLogIOstream <<
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
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrBeam::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrBeam>*
    p =
      dynamic_cast<visitor<S_msrBeam>*> (v)) {
        S_msrBeam elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrBeam::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrBeam::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrBeam::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrBeam>*
    p =
      dynamic_cast<visitor<S_msrBeam>*> (v)) {
        S_msrBeam elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrBeam::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrBeam::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrBeam& elt)
{
  elt->print (os);
  return os;
}

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

void msrBeam::print (ostream& os)
{  
  os <<
    asString () <<
    endl;
}


}
