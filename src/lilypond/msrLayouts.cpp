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
/* JMI ???
  int n1 = fVarValAssocs.size ();

  for (int i = 0; i < n1; i++ ) {
    // browse the stanza
    msrBrowser<msrVarValAssoc> browser (v);
    browser.browse (*fVarValAssocs [i]);
  } // for
  */
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

  os << left <<
    setw (fieldWidth) <<
    "topMargin" << " : ";
    if (fTopMargin) {
      os << fTopMargin;
    }
    else {
      os << "none";
    }
  os << endl;

  os << left <<
    setw (fieldWidth) <<
    "bottomMargin" << " : ";
    if (fBottomMargin) {
      os << fBottomMargin;
    }
    else {
      os << "none";
    }
  os << endl;

    /* JMI
  int n1 = fVarValAssocs.size ();

  for (int i = 0; i < n1; i++ ) {
    os << fVarValAssocs [i];
  } // for

  int n2 = fMsrSchemeVarValAssocs.size ();
  for (int i = 0; i < n2; i++ ) {
    os << fMsrSchemeVarValAssocs[i];
  } // for
  */

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_msrPageLayout& elt)
{
  elt->print (os);
  return os;
}


}
