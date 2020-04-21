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

#include "msrMusicXMLSpecifics.h"

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
{
/* JMI
  if (fOddMarginsGroup) {
    // browse the odd margins group
    msrBrowser<msrMarginsGroup> browser (v);
    browser.browse (*fOddMarginsGroup);
  }

  if (fEvenMarginsGroup) {
    // browse the even margins group
    msrBrowser<msrMarginsGroup> browser (v);
    browser.browse (*fEvenMarginsGroup);
  }

  if (fBothMarginsGroup) {
    // browse the both margins group
    msrBrowser<msrMarginsGroup> browser (v);
    browser.browse (*fBothMarginsGroup);
  }
  */
}

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
{
/* JMI
    // margins
    S_msrMargin           fLeftMargin;
    S_msrMargin           fRightMargin;

    // distances
    S_msrLength           fSystemDistance;
    S_msrLength           fTopSystemDistance;
    */
}

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

  const int fieldWidth = 18;

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
  int inputLineNumber,
  int staffNumber)
{
  msrStaffLayout* o =
    new msrStaffLayout (
      inputLineNumber,
      staffNumber);
  assert(o!=0);
  return o;
}

msrStaffLayout::msrStaffLayout (
  int inputLineNumber,
  int staffNumber)
    : msrElement (inputLineNumber)
{
  fStaffNumber = staffNumber;
}

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
    ", staffDistance: " << fStaffDistance <<
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
S_msrMeasureLayout msrMeasureLayout::create (
  int inputLineNumber)
{
  msrMeasureLayout* o =
    new msrMeasureLayout (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrMeasureLayout::msrMeasureLayout (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{}

msrMeasureLayout::~msrMeasureLayout ()
{}

void msrMeasureLayout::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrMeasureLayout::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrMeasureLayout>*
    p =
      dynamic_cast<visitor<S_msrMeasureLayout>*> (v)) {
        S_msrMeasureLayout elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrMeasureLayout::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrMeasureLayout::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrMeasureLayout::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrMeasureLayout>*
    p =
      dynamic_cast<visitor<S_msrMeasureLayout>*> (v)) {
        S_msrMeasureLayout elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrMeasureLayout::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrMeasureLayout::browseData (basevisitor* v)
{}

string msrMeasureLayout::asString () const
{
  stringstream s;

  s <<
    "[MeasureLayout" <<
    ", measureDistance: " << fMeasureDistance <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrMeasureLayout::print (ostream& os) const
{
  os << "MeasureLayout" << endl;

  const int fieldWidth = 14;

  gIndenter++;

  // measure distance
  os << left <<
    setw (fieldWidth) <<
    "measureDistance" << " : " << fMeasureDistance <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrMeasureLayout& elt)
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

  fStaffSpacing = -1;

  fNewSystem = false;
  fNewPage   = false;

  fBlankPage = -1;

  fPageNumber = 0;
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

  for (
    list<S_msrStaffLayout>::const_iterator i = fStaffLayoutsList.begin ();
    i != fStaffLayoutsList.end ();
    i++
  ) {
    // browse staff layout
    msrBrowser<msrStaffLayout> browser (v);
    browser.browse (*(*i));
  } // for

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

  const int fieldWidth = 18;

  os << left <<
    setw (fieldWidth) <<
    "pageLayout" << " : ";
    if (fPageLayout) {
      os << fPageLayout;
    }
    else {
      os << "none" << endl;
    }

  os << left <<
    setw (fieldWidth) <<
    "systemLayout" << " : ";
    if (fSystemLayout) {
      os << fSystemLayout;
    }
    else {
      os << "none" << endl;
    }

  int
    staffLayoutsListSize =
      fStaffLayoutsList.size ();

  os <<
    setw (fieldWidth) <<
    "staffLayoutsList" << " : " <<
    singularOrPlural (
      staffLayoutsListSize, "element", "elements") <<
    endl;

  if (staffLayoutsListSize) {
    gIndenter++;

    list<S_msrStaffLayout>::const_iterator
      iBegin = fStaffLayoutsList.begin (),
      iEnd   = fStaffLayoutsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for

    gIndenter--;
  }

  os << left <<
    setw (fieldWidth) <<
    "systemDividers" << " : ";
    if (fSystemDividers) {
      os << fSystemDividers;
    }
    else {
      os << "none" << endl;
    }

  os << left <<
    setw (fieldWidth) <<
    "staffSpacing" << " : " << fStaffSpacing <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "newSystem" << " : " << booleanAsString (fNewSystem) <<
    endl;
  os << left <<
    setw (fieldWidth) <<
    "newPage" << " : " << booleanAsString (fNewPage) <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "blankPage" << " : " << fBlankPage <<
    endl;

  os << left <<
    setw (fieldWidth) <<
    "pageNumber" << " : " << fPageNumber <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrPrintLayout& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrLineWidth msrLineWidth::create (
  int                  inputLineNumber,
  msrLineWidthTypeKind lineWidthTypeKind,
  S_msrLength          lineWidthValue)
{
  msrLineWidth* o =
    new msrLineWidth (
      inputLineNumber,
      lineWidthTypeKind,
      lineWidthValue);
  assert(o!=0);
  return o;
}

msrLineWidth::msrLineWidth (
  int                  inputLineNumber,
  msrLineWidthTypeKind lineWidthTypeKind,
  S_msrLength          lineWidthValue)
    : msrElement (inputLineNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTracePrintLayouts) {
    gLogOstream <<
      "Creating a print layout " <<
      endl;
  }
#endif

  fLineWidthTypeKind = lineWidthTypeKind;
  fLineWidthValue    = lineWidthValue;
}

msrLineWidth::~msrLineWidth ()
{}

void msrLineWidth::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrLineWidth::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrLineWidth>*
    p =
      dynamic_cast<visitor<S_msrLineWidth>*> (v)) {
        S_msrLineWidth elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrLineWidth::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrLineWidth::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrLineWidth::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrLineWidth>*
    p =
      dynamic_cast<visitor<S_msrLineWidth>*> (v)) {
        S_msrLineWidth elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrLineWidth::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrLineWidth::browseData (basevisitor* v)
{}

string msrLineWidth::lineWidthTypeKindAsString (
  msrLineWidthTypeKind lineWidthTypeKind)
{
  string result;

  switch (lineWidthTypeKind) {
    case msrLineWidth::k_NoLineWidthTypeKind:
      result = "k_NoLineWidthTypeKind";
      break;
    case msrLineWidth::kBeamLineWidth:
      result = "beamLineWidth";
      break;
    case msrLineWidth::kBracketLineWidth:
      result = "bracketLineWidth";
      break;
    case msrLineWidth::kDashesLineWidth:
      result = "dashesLineWidth";
      break;
    case msrLineWidth::kEnclosureLineWidth:
      result = "enclosureLineWidth";
      break;
    case msrLineWidth::kEndingLineWidth:
      result = "endingLineWidth";
      break;
    case msrLineWidth::kExtendLineWidth:
      result = "extendLineWidth";
      break;
    case msrLineWidth::kHeavyBarLineLineWidth:
      result = "heavyBarLineLineWidth";
      break;
    case msrLineWidth::kLegerLineWidth:
      result = "legerLineWidth";
      break;
    case msrLineWidth::kLightBarLineLineWidthLineWidth:
      result = "lightBarLineLineWidthLineWidth";
      break;
    case msrLineWidth::kOctaveShiftLineWidth:
      result = "octaveShiftLineWidth";
      break;
    case msrLineWidth::kPedalLineWidth:
      result = "pedalLineWidth";
      break;
    case msrLineWidth::kSlurMiddleLineWidth:
      result = "slurMiddleLineWidth";
      break;
    case msrLineWidth::kSlurTipLineWidth:
      result = "slurTipLineWidth";
      break;
    case msrLineWidth::kStaffLineWidth:
      result = "staffLineWidth";
      break;
    case msrLineWidth::kStemLineWidthLineWidth:
      result = "stemLineWidthLineWidth";
      break;
    case msrLineWidth::kTieMiddleLineWidth:
      result = "tieMiddleLineWidth";
      break;
    case msrLineWidth::kTieTipLineWidth:
      result = "tieTipLineWidth";
      break;
    case msrLineWidth::kTupletBracketLineWidth:
      result = "tupletBracketLineWidth";
      break;
    case msrLineWidth::kWedgeLineWidth:
      result = "wedgeLineWidth";
      break;
  } // switch

  return result;
}

string msrLineWidth::asString () const
{
  stringstream s;

  s <<
    "[LineWidth" <<
    ", lineWidthTypeKind: " <<
    lineWidthTypeKindAsString (fLineWidthTypeKind) <<
     ", lineWidthValue: ";
    if (fLineWidthValue) {
      s << fLineWidthValue->asString();
    }
    else {
      s << "none";
    }
  s <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrLineWidth::print (ostream& os) const
{
  os <<
    "LineWidth" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 18;

  os << left <<
    setw (fieldWidth) <<
    "lineWidthTypeKind" << " : " <<
    lineWidthTypeKindAsString (fLineWidthTypeKind) <<
    endl <<

    setw (fieldWidth) <<
    "lineWidthValue" <<  " : ";
    if (fLineWidthValue) {
      os << fLineWidthValue;
    }
    else {
      os << "none";
    }
  os << endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrLineWidth& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrNoteSize msrNoteSize::create (
  int                 inputLineNumber,
  msrNoteSizeTypeKind noteSizeTypeKind,
  float               noteSizeValue)
{
  msrNoteSize* o =
    new msrNoteSize (
      inputLineNumber,
      noteSizeTypeKind,
      noteSizeValue);
  assert(o!=0);
  return o;
}

msrNoteSize::msrNoteSize (
  int                 inputLineNumber,
  msrNoteSizeTypeKind noteSizeTypeKind,
  float               noteSizeValue)
    : msrElement (inputLineNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTracePrintLayouts) {
    gLogOstream <<
      "Creating a print layout " <<
      endl;
  }
#endif

  fNoteSizeTypeKind = noteSizeTypeKind;
  fNoteSizeValue    = noteSizeValue;
}

msrNoteSize::~msrNoteSize ()
{}

void msrNoteSize::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrNoteSize::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrNoteSize>*
    p =
      dynamic_cast<visitor<S_msrNoteSize>*> (v)) {
        S_msrNoteSize elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrNoteSize::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrNoteSize::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrNoteSize::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrNoteSize>*
    p =
      dynamic_cast<visitor<S_msrNoteSize>*> (v)) {
        S_msrNoteSize elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrNoteSize::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrNoteSize::browseData (basevisitor* v)
{}

string msrNoteSize::noteSizeTypeKindAsString (
  msrNoteSizeTypeKind noteSizeTypeKind)
{
  string result;

  switch (noteSizeTypeKind) {
    case msrNoteSize::k_NoNoteSizeTypeKind:
      result = "k_NoLineWidthTypeKind";
      break;
    case msrNoteSize::kCueNoteSize:
      result = "kCueNoteSize";
      break;
    case msrNoteSize::kGraceNoteSize:
      result = "kGraceNoteSize";
      break;
    case msrNoteSize::kLargeNoteSize:
      result = "kLargeNoteSize";
      break;
  } // switch

  return result;
}

string msrNoteSize::asString () const
{
  stringstream s;

  s <<
    "[NoteSize" <<
    ", noteSizeTypeKind: " <<
    noteSizeTypeKindAsString (fNoteSizeTypeKind) <<
    ", noteSizeValue: " << fNoteSizeValue <<
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrNoteSize::print (ostream& os) const
{
  os <<
    "NoteSize" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 18;

  os << left <<
    setw (fieldWidth) <<
    "noteSizeTypeKind" << " : " <<
    noteSizeTypeKindAsString (fNoteSizeTypeKind) <<
    endl <<

    setw (fieldWidth) <<
    "noteSizeValue" <<  " : " << fNoteSizeValue <<
    endl;

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrNoteSize& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_msrAppearance msrAppearance::create (
  int inputLineNumber)
{
  msrAppearance* o =
    new msrAppearance (
      inputLineNumber);
  assert(o!=0);
  return o;
}

msrAppearance::msrAppearance (
  int inputLineNumber)
    : msrElement (inputLineNumber)
{
#ifdef TRACE_OAH
  if (gTraceOah->fTracePrintLayouts) {
    gLogOstream <<
      "Creating an appearance" <<
      endl;
  }
#endif
}

msrAppearance::~msrAppearance ()
{}

void msrAppearance::acceptIn (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrAppearance::acceptIn ()" <<
      endl;
  }

  if (visitor<S_msrAppearance>*
    p =
      dynamic_cast<visitor<S_msrAppearance>*> (v)) {
        S_msrAppearance elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrAppearance::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void msrAppearance::acceptOut (basevisitor* v)
{
  if (gMsrOah->fTraceMsrVisitors) {
    gLogOstream <<
      "% ==> msrAppearance::acceptOut ()" <<
      endl;
  }

  if (visitor<S_msrAppearance>*
    p =
      dynamic_cast<visitor<S_msrAppearance>*> (v)) {
        S_msrAppearance elem = this;

        if (gMsrOah->fTraceMsrVisitors) {
          gLogOstream <<
            "% ==> Launching msrAppearance::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void msrAppearance::browseData (basevisitor* v)
{
/*
    list<S_msrLineWidth>  fLineWidthsList;

    list<S_msrNoteSize>   fNoteSizesList;

    list<S_msrDistance>   fDistancesList;

    list<S_msrGlyph>      fGlyphsList;

    list<S_msrOtherAppearance>
                          fOtherAppearancesList;


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
  */
}

string msrAppearance::asString () const
{
  stringstream s;

  s <<
    "[Appearance" <<
    /*
    ", otherAppearanceTypeKind: " <<
    otherAppearanceTypeKindAsString (fOtherAppearanceTypeKind) <<
    ", otherAppearanceValue: " <<  fOtherAppearanceValue <<
    */
    ", line " << fInputLineNumber <<
    "]";

  return s.str ();
}

void msrAppearance::print (ostream& os) const
{
  os <<
    "Appearance" <<
    ", line " << fInputLineNumber <<
    endl;

  gIndenter++;

  const int fieldWidth = 18;

  // line widths
  int
    lineWidthsListSize =
      fLineWidthsList.size ();

  os <<
    setw (fieldWidth) <<
    "lineWidthsList" << " : " <<
    singularOrPlural (
      lineWidthsListSize, "element", "elements") <<
    endl;

  if (lineWidthsListSize) {
    gIndenter++;

    list<S_msrLineWidth>::const_iterator
      iBegin = fLineWidthsList.begin (),
      iEnd   = fLineWidthsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for

    gIndenter--;
  }

  // note sizes
  int
    noteSizesListSize =
      fNoteSizesList.size ();

  os <<
    setw (fieldWidth) <<
    "noteSizesList" << " : " <<
    singularOrPlural (
      noteSizesListSize, "element", "elements") <<
    endl;

  if (noteSizesListSize) {
    gIndenter++;

    list<S_msrNoteSize>::const_iterator
      iBegin = fNoteSizesList.begin (),
      iEnd   = fNoteSizesList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
 // JMI     os << endl;
    } // for

    gIndenter--;
  }

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrAppearance& elt)
{
  elt->print (os);
  return os;
}


}
