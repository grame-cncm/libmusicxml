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

#include "msrLayouts.h"

#include "msrOah.h"

#include "messagesHandling.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "oahOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_msrPageLayout msrPageLayout::create (
  int inputLineNumber)
{
  msrPageLayout* o =
    new msrPageLayout (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrPageLayout::msrPageLayout (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrPageLayout::~msrPageLayout ()
{}

void msrPageLayout::setOddMarginsGroup (
  int               inputLineNumber,
  S_msrMarginsGroup val)
{
  if (fBothMarginsGroup) {
    stringstream s;

    s <<
      "setting an odd margins group when there is already a both margins group in " <<
      this->asString ();

    msrMusicXMLError (
      gOahOah->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fOddMarginsGroup = val;
}

void msrPageLayout::setEvenMarginsGroup (
  int               inputLineNumber,
  S_msrMarginsGroup val)
{
  if (fBothMarginsGroup) {
    stringstream s;

    s <<
      "setting an even margins group when there is already a both margins group in " <<
      this->asString ();

    msrMusicXMLError (
      gOahOah->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fEvenMarginsGroup = val;
}

void msrPageLayout::setBothMarginsGroup (
  int               inputLineNumber,
  S_msrMarginsGroup val)
{
  if (fOddMarginsGroup || fEvenMarginsGroup) {
    stringstream s;

    s <<
      "setting a both margins group when there is already an odd or even margins group in " <<
      this->asString ();

    msrMusicXMLError (
      gOahOah->fInputSourceName,
      inputLineNumber,
      __FILE__, __LINE__,
      s.str ());
  }

  fBothMarginsGroup = val;
}


S_msrMargin msrPageLayout::getSingleLeftMargin ()
{
  // heuristic, could be refined with suitable options JMI
  S_msrMargin result;

  if (fBothMarginsGroup) {
    result = fBothMarginsGroup->getLeftMargin ();
  }
  else if (fOddMarginsGroup) {
    result = fOddMarginsGroup->getLeftMargin ();
  }
  else if (fEvenMarginsGroup) {
    result = fEvenMarginsGroup->getLeftMargin ();
  }

  return result;
}

S_msrMargin msrPageLayout::getSingleRightMargin ()
{
  // heuristic, could be refined with suitable options JMI
  S_msrMargin result;

  if (fBothMarginsGroup) {
    result = fBothMarginsGroup->getRightMargin ();
  }
  else if (fOddMarginsGroup) {
    result = fOddMarginsGroup->getRightMargin ();
  }
  else if (fEvenMarginsGroup) {
    result = fEvenMarginsGroup->getRightMargin ();
  }

  return result;
}

S_msrMargin msrPageLayout::getSingleTopMargin ()
{
  // heuristic, could be refined with suitable options JMI
  S_msrMargin result;

  if (fBothMarginsGroup) {
    result = fBothMarginsGroup->getTopMargin ();
  }
  else if (fOddMarginsGroup) {
    result = fOddMarginsGroup->getTopMargin ();
  }
  else if (fEvenMarginsGroup) {
    result = fEvenMarginsGroup->getTopMargin ();
  }

  return result;
}

S_msrMargin msrPageLayout::getSingleBottomMargin ()
{
  // heuristic, could be refined with suitable options JMI
  S_msrMargin result;

  if (fBothMarginsGroup) {
    result = fBothMarginsGroup->getBottomMargin ();
  }
  else if (fOddMarginsGroup) {
    result = fOddMarginsGroup->getBottomMargin ();
  }
  else if (fEvenMarginsGroup) {
    result = fEvenMarginsGroup->getBottomMargin ();
  }

  return result;
}

void msrPageLayout::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrPageLayout::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrPageLayout>*
    p =
      dynamic_cast<visitor<S_msrPageLayout>*> (v)) {
        S_msrPageLayout elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrPageLayout::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrPageLayout::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrPageLayout::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrPageLayout>*
    p =
      dynamic_cast<visitor<S_msrPageLayout>*> (v)) {
        S_msrPageLayout elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrPageLayout::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrPageLayout::browseData (basevisitor* v)
{}

string msrPageLayout::asString () const
{
  stringstream s;

  s <<
    "[PageLayout" <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrPageLayout::print (ostream& os) const
{
  os << "PageLayout" << endl;

  const int fieldWidth = 13;

  gIndenter++;

  // page size
  os << left <<
    setw (fieldWidth) <<
    "pageHeight" << " : ";
    if (fPageHeight) {
      os << fPageHeight;
    }
    else {
      os << "none";
    }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "pageWidth" << " : ";
    if (fPageWidth) {
      os << fPageWidth;
    }
    else {
      os << "none";
    }
  os << endl;

  // margins groups
  os << left <<
    setw (fieldWidth) <<
    "oddMarginsGroup" << " : ";
    if (fOddMarginsGroup) {
      os << fOddMarginsGroup;
    }
    else {
      os << "none" << endl;
    }

  os << left <<
    setw (fieldWidth) <<
    "evenMarginsGroup" << " : ";
    if (fEvenMarginsGroup) {
      os << fEvenMarginsGroup;
    }
    else {
      os << "none" << endl;
    }

  os << left <<
    setw (fieldWidth) <<
    "bothMarginsGroup" << " : ";
    if (fBothMarginsGroup) {
      os << fBothMarginsGroup;
    }
    else {
      os << "none" << endl;
    }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrPageLayout& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrSystemLayout msrSystemLayout::create (
  int inputLineNumber)
{
  msrSystemLayout* o =
    new msrSystemLayout (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrSystemLayout::msrSystemLayout (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrSystemLayout::~msrSystemLayout ()
{}

void msrSystemLayout::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrSystemLayout::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrSystemLayout>*
    p =
      dynamic_cast<visitor<S_msrSystemLayout>*> (v)) {
        S_msrSystemLayout elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrSystemLayout::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSystemLayout::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrSystemLayout::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrSystemLayout>*
    p =
      dynamic_cast<visitor<S_msrSystemLayout>*> (v)) {
        S_msrSystemLayout elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrSystemLayout::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrSystemLayout::browseData (basevisitor* v)
{}

string msrSystemLayout::asString () const
{
  stringstream s;

  s <<
    "[SystemLayout" <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrSystemLayout::print (ostream& os) const
{
  os << "SystemLayout" << endl;

  const int fieldWidth = 13;

  gIndenter++;

  // margins
  os << left <<
    setw (fieldWidth) <<
    "leftMargin" << " : ";
    if (fLeftMargin) {
      os << fLeftMargin;
    }
    else {
      os << "none";
    }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "rightMargin" << " : ";
    if (fRightMargin) {
      os << fRightMargin;
    }
    else {
      os << "none";
    }
  os << endl;

  // distances
  os << left <<
    setw (fieldWidth) <<
    "systemDistance" << " : ";
    if (fSystemDistance) {
      os << fSystemDistance;
    }
    else {
      os << "none";
    }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "topSystemDistance" << " : ";
    if (fTopSystemDistance) {
      os << fTopSystemDistance;
    }
    else {
      os << "none";
    }
  os << endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrSystemLayout& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrSystemDividers msrSystemDividers::create (
  int inputLineNumber)
{
  msrSystemDividers* o =
    new msrSystemDividers (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrSystemDividers::msrSystemDividers (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
  fLeftDivider  = false;
  fRightDivider = false;
}

msrSystemDividers::~msrSystemDividers ()
{}

void msrSystemDividers::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrSystemDividers::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrSystemDividers>*
    p =
      dynamic_cast<visitor<S_msrSystemDividers>*> (v)) {
        S_msrSystemDividers elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrSystemDividers::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrSystemDividers::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrSystemDividers::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrSystemDividers>*
    p =
      dynamic_cast<visitor<S_msrSystemDividers>*> (v)) {
        S_msrSystemDividers elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrSystemDividers::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrSystemDividers::browseData (basevisitor* v)
{}

string msrSystemDividers::asString () const
{
  stringstream s;

  s <<
    "[SystemDividers" <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrSystemDividers::print (ostream& os) const
{
  os << "SystemDividers" << endl;

  const int fieldWidth = 13;

  gIndenter++;

  os << left <<
    setw (fieldWidth) <<
    "leftDivider" << " : " << booleanAsString (fLeftDivider) <<
    endl <<
    setw (fieldWidth) <<
    "rightDivider" << " : " << booleanAsString (fRightDivider) <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrSystemDividers& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrStaffLayout msrStaffLayout::create (
  int inputLineNumber)
{
  msrStaffLayout* o =
    new msrStaffLayout (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrStaffLayout::msrStaffLayout (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrStaffLayout::~msrStaffLayout ()
{}

void msrStaffLayout::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrStaffLayout::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrStaffLayout>*
    p =
      dynamic_cast<visitor<S_msrStaffLayout>*> (v)) {
        S_msrStaffLayout elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrStaffLayout::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrStaffLayout::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrStaffLayout::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrStaffLayout>*
    p =
      dynamic_cast<visitor<S_msrStaffLayout>*> (v)) {
        S_msrStaffLayout elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrStaffLayout::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrStaffLayout::browseData (basevisitor* v)
{}

string msrStaffLayout::asString () const
{
  stringstream s;

  s <<
    "[StaffLayout" <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrStaffLayout::print (ostream& os) const
{
  os << "StaffLayout" << endl;

  const int fieldWidth = 14;

  gIndenter++;

  // staff number
  os << left <<
    setw (fieldWidth) <<
    "staffNumber" << " : " << fStaffNumber <<
    endl;

  // staff distance
  os << left <<
    setw (fieldWidth) <<
    "staffDistance" << " : " << fStaffDistance <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrStaffLayout& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrPrintLayout msrPrintLayout::create (
  int inputLineNumber)
{
  msrPrintLayout* o =
    new msrPrintLayout (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrPrintLayout::msrPrintLayout (
  int inputLineNumber)
    : msrMeasureElement (inputLineNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTracePrintLayouts) {
    gLogOstream <<
      "Creating a print layout " <<
      endl;
  }
#endif
}

msrPrintLayout::~msrPrintLayout ()
{}

void msrPrintLayout::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrPrintLayout::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrPrintLayout>*
    p =
      dynamic_cast<visitor<S_msrPrintLayout>*> (v)) {
        S_msrPrintLayout elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrPrintLayout::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrPrintLayout::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrPrintLayout::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrPrintLayout>*
    p =
      dynamic_cast<visitor<S_msrPrintLayout>*> (v)) {
        S_msrPrintLayout elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrPrintLayout::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrPrintLayout::browseData (basevisitor* v)
{
  if (fPageLayout) {
    // browse the page layout
    msrBrowser<msrPageLayout> browser (v);
    browser.browse (*fPageLayout);
  }

  if (fSystemLayout) {
    // browse the system layout
    msrBrowser<msrSystemLayout> browser (v);
    browser.browse (*fSystemLayout);
  }

  if (fSystemDividers) {
    // browse the system dividers
    msrBrowser<msrSystemDividers> browser (v);
    browser.browse (*fSystemDividers);
  }
}

string msrPrintLayout::asString () const
{
  stringstream s;

  s <<
    "[PrintLayout" <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrPrintLayout::print (ostream& os) const
{
  os <<
    "PrintLayout" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 15;

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

  os << left <<
    setw (fieldWidth) <<
    "systemLayout" << " : ";
    if (fSystemLayout) {
      os << fSystemLayout;
    }
    else {
      os << "none";
    }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "systemDividers" << " : ";
    if (fSystemDividers) {
      os << fSystemDividers;
    }
    else {
      os << "none";
    }
  os << endl;

  gIndenter++;
}

ostream& operator<< (ostream& os, const S_msrPrintLayout& elt)
{
  elt->print (os);
  return os;
}


}
