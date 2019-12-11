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

#include "msrGeometry.h"

#include "generalOah.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "msrOah.h"
#include "lpsrOah.h"

#include "messagesHandling.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrGeometry msrGeometry::create (
  int inputLineNumber)
{
  msrGeometry* o =
    new msrGeometry (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrGeometry::msrGeometry (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
  fMillimeters = -1;
  fTenths      = -1;
}

msrGeometry::~msrGeometry ()
{}

S_msrGeometry msrGeometry::createMsrGeometryNewbornClone ()
{
  S_msrGeometry
    newbornClone =
      msrGeometry::create (
        fInputLineNumber);

  newbornClone->fMillimeters =
    fMillimeters;
  newbornClone->fTenths =
    fTenths;

  newbornClone->fPageLayout =
    fPageLayout;

  return newbornClone;
}

float msrGeometry::fetchGlobalStaffSize () const
{
  const float optionsGlobalStaffSize =
    gLpsrOah->fGlobalStaffSize;

  const float defaultTenthsToMillimetersRatio = 0.175;

  float millimetersOverTenths =
    fMillimeters / fTenths;
  float ratio =
    millimetersOverTenths / defaultTenthsToMillimetersRatio;

  float staffSize =
    optionsGlobalStaffSize * ratio;

#ifdef TRACE_OAH
  if (gTraceOah->fTraceGeometry) {
    gLogOstream <<
      "fetchGlobalStaffSize():" <<
      endl;

    gIndenter++;

    gLogOstream <<
      "optionsGlobalStaffSize" << " : " <<
      optionsGlobalStaffSize <<
      endl <<
      "defaultTenthsToMillimetersRatio" << " : " <<
      defaultTenthsToMillimetersRatio <<
      endl <<
      "millimetersOverTenths" << " : " <<
      millimetersOverTenths <<
      endl <<
      "ratio" << " : " <<
      ratio <<
      endl <<
      "staffSize" << " : " <<
      staffSize <<
      endl;

    gIndenter--;
  }
#endif

  if (staffSize < 1.0 || staffSize > 100.0) {
#ifdef TRACE_OAH
    if (gTraceOah->fTraceGeometry) {
      stringstream s;

      s <<
        "staffSize " << staffSize <<
        " is not between 1.0 and 100.0, replaced by 20.0:" <<
        endl;

      msrMusicXMLWarning (
        gOahOah->fInputSourceName,
        fInputLineNumber,
        s.str ());
    }
#endif

    staffSize = optionsGlobalStaffSize;
  }

  return staffSize;
}

void msrGeometry::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrGeometry::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrGeometry>*
    p =
      dynamic_cast<visitor<S_msrGeometry>*> (v)) {
        S_msrGeometry elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrGeometry::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrGeometry::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrGeometry::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrGeometry>*
    p =
      dynamic_cast<visitor<S_msrGeometry>*> (v)) {
        S_msrGeometry elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrGeometry::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrGeometry::browseData (basevisitor* v)
{}

void msrGeometry::print (ostream& os) const
{
  os <<
    "Geometry" <<
    endl;

  gIndenter++;

  const int fieldWidth = 13;

  // relative to absolute lengths conversion
  os << left <<
    setw (fieldWidth) <<
    "millimeters" << " : " <<
    setprecision (2) << fMillimeters <<
    endl <<
    setw (fieldWidth) <<
    "tenths" << " : " <<
    setprecision (2) << fTenths <<
    endl;

  // page layout
  os << left <<
    setw (fieldWidth) <<
    "pageLayout" << " : ";
    if (fPageLayout) {
      os << fPageLayout;
    }
    else {
      os << "none";
    }
  os << endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrGeometry& elt) {
  elt->print (os);
  return os;
}


}
