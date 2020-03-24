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
  // JMI ???
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
  // JMI ???
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


}
