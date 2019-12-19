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
S_msrGlissando msrGlissando::create (
  int                  inputLineNumber,
  int                  glissandoNumber,
  msrGlissandoTypeKind glissandoTypeKind,
  msrLineTypeKind      glissandoLineTypeKind,
  string               glissandoTextValue)
{
  msrGlissando* o =
    new msrGlissando (
      inputLineNumber,
      glissandoNumber,
      glissandoTypeKind,
      glissandoLineTypeKind,
      glissandoTextValue);
  assert(o!=0);
  return o;
}

msrGlissando::msrGlissando (
  int                  inputLineNumber,
  int                  glissandoNumber,
  msrGlissandoTypeKind glissandoTypeKind,
  msrLineTypeKind      glissandoLineTypeKind,
  string               glissandoTextValue)
    : msrElement (inputLineNumber)
{
  fGlissandoNumber = glissandoNumber;

  fGlissandoTypeKind     = glissandoTypeKind;
  fGlissandoLineTypeKind = glissandoLineTypeKind;

  fGlissandoTextValue = glissandoTextValue;
}

msrGlissando::~msrGlissando ()
{}

S_msrGlissando msrGlissando::createGlissandoNewbornClone ()
{
#ifdef TRACE_OAH
  if (gTraceOah->fTraceGlissandos) {
    gLogOstream <<
      "Creating a newborn clone of glissando '" <<
      asString () <<
      "'" <<
      endl;
  }
#endif

  S_msrGlissando
    newbornClone =
      msrGlissando::create (
        fInputLineNumber,
        fGlissandoNumber,
        fGlissandoTypeKind,
        fGlissandoLineTypeKind,
        fGlissandoTextValue);

  return newbornClone;
}

string msrGlissando::glissandoTypeKindAsString (
  msrGlissandoTypeKind glissandoTypeKind)
{
  string result;

  switch (glissandoTypeKind) {
    case msrGlissando::kGlissandoTypeNone:
      result = "glissandoTypeNone";
      break;
    case msrGlissando::kGlissandoTypeStart:
      result = "glissandoTypeStart";
      break;
    case msrGlissando::kGlissandoTypeStop:
      result = "glissandoTypeStop";
      break;
  } // switch

  return result;
}

void msrGlissando::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrGlissando::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrGlissando>*
    p =
      dynamic_cast<visitor<S_msrGlissando>*> (v)) {
        S_msrGlissando elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrGlissando::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrGlissando::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrGlissando::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrGlissando>*
    p =
      dynamic_cast<visitor<S_msrGlissando>*> (v)) {
        S_msrGlissando elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrGlissando::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrGlissando::browseData (basevisitor* v)
{}

string msrGlissando::asString () const
{
  stringstream s;

  s <<
    "Glissando" <<
    ", fGlissandoNumber " << fGlissandoNumber <<
    ", " << glissandoTypeKindAsString (
      fGlissandoTypeKind) <<
    ", " << msrLineTypeKindAsString (
      fGlissandoLineTypeKind) <<
    ", \"" << fGlissandoTextValue <<
    "\", line " << fInputLineNumber;

  return s.str ();
}

void msrGlissando::print (ostream& os) const
{
  os <<
    "Glissando" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 22;

  os << left <<
    setw (fieldWidth) <<
    "glissandoNumber " <<
    fGlissandoNumber <<
    endl <<
    setw (fieldWidth) <<
    "glissandoTypeKind" <<
    glissandoTypeKindAsString (
      fGlissandoTypeKind) <<
    endl <<
    setw (fieldWidth) <<
    "glissandoLineTypeKind" <<
    msrLineTypeKindAsString (
      fGlissandoLineTypeKind) <<
    endl <<
    setw (fieldWidth) <<
    "fGlissandoTextValue" << " : \"" <<
    fGlissandoTextValue <<
     "\"" <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrGlissando& elt)
{
  elt->print (os);
  return os;
}


}
