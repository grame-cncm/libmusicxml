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
#include <iomanip>      // setw, setprecision, ...

#include "msrPageGeometry.h"

#include "msrOptions.h"
#include "traceOptions.h"
#include "xml2lyOptionsHandling.h"

#include "messagesHandling.h"


using namespace std;

namespace MusicXML2 {

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
  fPaperWidth   = 21.0; // cm
  fPaperHeight  = 29.7; // cm
  
  fTopMargin    = -1.0; // cm
  fBottomMargin = -1.0; // cm
  fLeftMargin   = -1.0; // cm
  fRightMargin  = -1.0; // cm
    
  fBetweenSystemSpace = -1.0; // cm
  fPageTopSpace       = -1.0; // cm

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

float msrPageGeometry::globalStaffSize () const
{  
  const float lilyPondDefaultStaffSize = 20.0;
  const float defaultTenthsToMillimetersRatio = 0.175;

  /*
        mm = scaling.get_named_child('millimeters')
        mm = float(mm.get_text ())
        tn = scaling.get_maybe_exist_named_child('tenths')
        tn = float(tn.get_text ())
        # The variable 'tenths' is actually a ratio, NOT the value of <tenths>.
        # TODO: rename and replace.
        tenths = mm / tn
        ratio = tenths / default_tenths_to_millimeters_ratio
        staff_size = default_staff_size * ratio

        if 1 < staff_size < 100:
            paper.global_staff_size = staff_size
        else:
            msg = "paper.global_staff_size {} is too large, using defaults=20".format(
                staff_size)
            warnings.warn(msg)
            paper.global_staff_size = 20
   */

  float millimetersOverTenths = fMillimeters / fTenths;
  float ratio = millimetersOverTenths / defaultTenthsToMillimetersRatio;
  float staffSize = lilyPondDefaultStaffSize * ratio;

  if (gTraceOptions->fTraceGeometry) {
    gLogIOstream <<
      "globalStaffSize ():" <<
      endl;

    gIndenter++;

    gLogIOstream <<
      "lilyPondDefaultStaffSize" << " : " <<
      lilyPondDefaultStaffSize <<
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
  
  if (staffSize < 1.0 || staffSize > 100.0) {
    if (gTraceOptions->fTraceGeometry) {
      stringstream s;

      s <<
        "staffSize " << staffSize <<
        " is not between 1.0 and 100.0, replaced by 20.0:" <<
        endl;

      msrMusicXMLWarning (
        gXml2lyOptions->fInputSourceName,
        fInputLineNumber,
        s.str ());
    }

    staffSize = lilyPondDefaultStaffSize;
  }
    
  return staffSize;
  
  /* JMI ???
  if (fMillimeters > 0)
    return fMillimeters * 72.27 / 25.4;
  else
    return 20.0; // LilyPond default
    */
}

void msrPageGeometry::acceptIn (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPageGeometry::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_msrPageGeometry>*
    p =
      dynamic_cast<visitor<S_msrPageGeometry>*> (v)) {
        S_msrPageGeometry elem = this;
        
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPageGeometry::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrPageGeometry::acceptOut (basevisitor* v)
{
  if (gMsrOptions->fTraceMsrVisitors) {
    gLogIOstream <<
      "% ==> msrPageGeometry::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrPageGeometry>*
    p =
      dynamic_cast<visitor<S_msrPageGeometry>*> (v)) {
        S_msrPageGeometry elem = this;
      
        if (gMsrOptions->fTraceMsrVisitors) {
          gLogIOstream <<
            "% ==> Launching msrPageGeometry::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrPageGeometry::browseData (basevisitor* v)
{
  /* JMI
  if (fPaperWidth) {
    // browse identification
    msrBrowser<fPaperWidth> browser (v);
    browser.browse (*fPaperWidth);
  }

  if (fPaperHeight) {
    // browse identification
    msrBrowser<fPaperWidth> browser (v);
    browser.browse (*fPaperHeight);
  }

  if (fTopMargin) {
    // browse identification
    msrBrowser<fPaperWidth> browser (v);
    browser.browse (*fTopMargin);
  }

  if (fBottomMargin) {
    // browse identification
    msrBrowser<fPaperWidth> browser (v);
    browser.browse (*fPaperWidth);
  }

  if (fLeftMargin) {
    // browse identification
    msrBrowser<fPaperWidth> browser (v);
    browser.browse (*fPaperWidth);
  }

  if (fRightMargin) {
    // browse identification
    msrBrowser<fPaperWidth> browser (v);
    browser.browse (*fRightMargin);
  }
  */
}

ostream& operator<< (ostream& os, const S_msrPageGeometry& elt) {
  elt->print (os);
  return os;
}

void msrPageGeometry::print (ostream& os) {
  os <<
    "PageGeometry" <<
    endl;

  bool emptyGeometry = true;
  
  gIndenter++;
  
  const int fieldWidth = 12;

  if (fPaperWidth > 0) {
    os << left <<
      setw (fieldWidth) <<
      "PaperWidth" << " = " <<
      setprecision (2) << fPaperWidth << " cm" <<
      endl;

    emptyGeometry = false;
  }
  
  if (fPaperHeight > 0) {
    os << left <<
      setw (fieldWidth) <<
      "PaperHeight" << " = " <<
      setprecision (2) << fPaperHeight << " cm" <<
      endl;

    emptyGeometry = false;
  }
  
  if (fTopMargin > 0) {
    os << left <<
      setw (fieldWidth) <<
      "TopMargin" << " = " <<
      setprecision (2) << fTopMargin << " cm" <<
      endl;

    emptyGeometry = false;
  }
  
  if (fBottomMargin > 0) {
    os << left <<
      setw (fieldWidth) <<
      "BottomMargin" << " = " <<
      setprecision (2) << fBottomMargin << " cm" <<
      endl;

    emptyGeometry = false;
  }
  
  if (fLeftMargin > 0) {
    os << left <<
      setw (fieldWidth) <<
      "LeftMargin" << " = " <<
      setprecision (2) << fLeftMargin << " cm" <<
      endl;

    emptyGeometry = false;
  }
  
  if (fRightMargin > 0) {
    os << left <<
      setw (fieldWidth) <<
      "RightMargin" << " = " <<
      setprecision (2) << fRightMargin << " cm" <<
      endl;

    emptyGeometry = false;
  }


  if (fMillimeters > 0) {
    os << left <<
      setw (fieldWidth) <<
      "Millimeters" << " = " <<
      setprecision (2) << fMillimeters <<
      endl;

    emptyGeometry = false;
  }

  if (fTenths > 0) {
    os << left <<
      setw (fieldWidth) <<
      "Tenths" << " = " <<
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
    os <<
      endl;
  }
  
  gIndenter--;
}

/*
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
