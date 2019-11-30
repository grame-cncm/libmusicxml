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

#include "msrPageGeometry.h"

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
S_msrPageGeometry msrPageGeometry::create (
  int inputLineNumber)
{
  msrPageGeometry* o =
    new msrPageGeometry (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrPageGeometry::msrPageGeometry (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
  fPaperUnitKind = kMillimeterUnit;

  fPaperWidth   = 210; // mm
  fPaperHeight  = 297; // mm

  fTopMargin    = -1.0; // mm
  fBottomMargin = -1.0; // mm
  fLeftMargin   = -1.0; // mm
  fRightMargin  = -1.0; // mm

  fBetweenSystemSpace = -1.0; // mm
  fPageTopSpace       = -1.0; // mm

  fMillimeters = -1;
  fTenths      = -1;
}

msrPageGeometry::~msrPageGeometry ()
{}

S_msrPageGeometry msrPageGeometry::createGeometryNewbornClone ()
{
  S_msrPageGeometry
    newbornClone =
      msrPageGeometry::create (
        fInputLineNumber);

  newbornClone->fPaperUnitKind =
    fPaperUnitKind;

  newbornClone->fPaperWidth =
    fPaperWidth;
  newbornClone->fPaperHeight =
    fPaperHeight;

  newbornClone->fTopMargin =
    fTopMargin;
  newbornClone->fBottomMargin =
    fBottomMargin;
  newbornClone->fLeftMargin =
    fLeftMargin;
  newbornClone->fRightMargin =
    fRightMargin;

  newbornClone->fBetweenSystemSpace =
    fBetweenSystemSpace;
  newbornClone->fPageTopSpace =
    fPageTopSpace;

  newbornClone->fMillimeters =
    fMillimeters;
  newbornClone->fTenths =
    fTenths;

  return newbornClone;
}

float msrPageGeometry::fetchGlobalStaffSize () const
{
  const float optionsGlobalStaffSize =
    gLpsrOah->fGlobalStaffSize;

  const float defaultTenthsToMillimetersRatio = 0.175;

  float millimetersOverTenths = fMillimeters / fTenths;
  float ratio = millimetersOverTenths / defaultTenthsToMillimetersRatio;

  float staffSize = optionsGlobalStaffSize * ratio;

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

void msrPageGeometry::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrPageGeometry::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrPageGeometry>*
    p =
      dynamic_cast<visitor<S_msrPageGeometry>*> (v)) {
        S_msrPageGeometry elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrPageGeometry::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrPageGeometry::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrPageGeometry::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrPageGeometry>*
    p =
      dynamic_cast<visitor<S_msrPageGeometry>*> (v)) {
        S_msrPageGeometry elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrPageGeometry::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrPageGeometry::browseData (basevisitor* v)
{}

ostream& operator<< (ostream& os, const S_msrPageGeometry& elt) {
  elt->print (os);
  return os;
}

void msrPageGeometry::print (ostream& os) {
  os <<
    "PageGeometry" <<
    endl;

  bool emptyGeometry = true; // JMI

  gIndenter++;

  const int fieldWidth = 13;

  os << left <<
    setw (fieldWidth) <<
    "paperUnitKind" << " : " <<
    msrPaperUnitKindAsString (fPaperUnitKind) <<
    endl;

  if (fPaperWidth > 0) {
    os << left <<
      setw (fieldWidth) <<
      "paperWidth" << " : " <<
      setprecision (2) << fPaperWidth <<
      msrPaperUnitKindAsString (fPaperUnitKind) <<
      endl;

    emptyGeometry = false;
  }

  if (fPaperHeight > 0) {
    os << left <<
      setw (fieldWidth) <<
      "paperHeight" << " : " <<
      setprecision (2) << fPaperHeight <<
      msrPaperUnitKindAsString (fPaperUnitKind) <<
      endl;

    emptyGeometry = false;
  }

  if (fTopMargin > 0) {
    os << left <<
      setw (fieldWidth) <<
      "topMargin" << " : " <<
      setprecision (2) << fTopMargin <<
      msrPaperUnitKindAsString (fPaperUnitKind) <<
      endl;

    emptyGeometry = false;
  }

  if (fBottomMargin > 0) {
    os << left <<
      setw (fieldWidth) <<
      "bottomMargin" << " : " <<
      setprecision (2) << fBottomMargin <<
      msrPaperUnitKindAsString (fPaperUnitKind) <<
      endl;

    emptyGeometry = false;
  }

  if (fLeftMargin > 0) {
    os << left <<
      setw (fieldWidth) <<
      "leftMargin" << " : " <<
      setprecision (2) << fLeftMargin <<
      msrPaperUnitKindAsString (fPaperUnitKind) <<
      endl;

    emptyGeometry = false;
  }

  if (fRightMargin > 0) {
    os << left <<
      setw (fieldWidth) <<
      "rightMargin" << " ; " <<
      setprecision (2) << fRightMargin <<
      msrPaperUnitKindAsString (fPaperUnitKind) <<
      endl;

    emptyGeometry = false;
  }

  // millimeters
  if (fMillimeters > 0) {
    os << left <<
      setw (fieldWidth) <<
      "millimeters" << " : " <<
      setprecision (2) << fMillimeters <<
      endl;

    emptyGeometry = false;
  }

  // tenths
  if (fTenths > 0) {
    os << left <<
      setw (fieldWidth) <<
      "tenths" << " : " <<
      setprecision (2) << fTenths <<
      endl;

    emptyGeometry = false;
  }

  if (emptyGeometry) {
    os <<
      " " << "nothing specified" <<
      endl <<
      endl;
  }
  else {
    os << endl;
  }

  gIndenter--;
}

/* JMI
  if (fBetweenSystemSpace > 0) {
    os <<
      "between-system-space = " <<
      setprecision (2) << fBetweenSystemSpace << "\\cm" << endl;
  }

  if (fPageTopSpace > 0) {
    os <<
      "page-top-space = " <<
      setprecision (2) << fPageTopSpace << "\\cm" << endl;
  }
*/


}
