/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iomanip>      // setw, setprecision, ...

#include "lpsrLayouts.h"

#include "lpsrScheme.h"

#include "lpsrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
S_lpsrLayout lpsrLayout::create (
  int inputLineNumber)
{
  lpsrLayout* o =
    new lpsrLayout (
      inputLineNumber);
  assert(o!=0);
  return o;
}

lpsrLayout::lpsrLayout (
  int inputLineNumber)
    : lpsrElement (inputLineNumber)
{
  fLayoutGlobalStaffSize = 20; // LilyPond default // JMI
}

lpsrLayout::~lpsrLayout ()
{}

void lpsrLayout::acceptIn (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    gLogOstream <<
      "% ==> lpsrLayout::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrLayout>*
    p =
      dynamic_cast<visitor<S_lpsrLayout>*> (v)) {
        S_lpsrLayout elem = this;

#ifdef TRACE_OAH
        if (gLpsrOah->fTraceLpsrVisitors) {
          gLogOstream <<
            "% ==> Launching lpsrLayout::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void lpsrLayout::acceptOut (basevisitor* v)
{
#ifdef TRACE_OAH
  if (gLpsrOah->fTraceLpsrVisitors) {
    gLogOstream <<
      "% ==> lpsrLayout::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_lpsrLayout>*
    p =
      dynamic_cast<visitor<S_lpsrLayout>*> (v)) {
        S_lpsrLayout elem = this;

#ifdef TRACE_OAH
        if (gLpsrOah->fTraceLpsrVisitors) {
          gLogOstream <<
            "% ==> Launching lpsrLayout::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void lpsrLayout::browseData (basevisitor* v)
{
/* JMI
  int lilypondAssocs = flpsrVarValAssocsVector.size ();

  for (int i = 0; i < lilypondAssocs; i++ ) {
    // browse the variable/value association
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*flpsrVarValAssocsVector [i]);
  } // for

  int schemeAssocs = fLpsrSchemeVariablesVector.size ();

  for (int i = 0; i < schemeAssocs; i++ ) {
    // browse the Scheme variable/value association
    msrBrowser<lpsrSchemeVariable> browser (v);
    browser.browse (*fLpsrSchemeVariablesVector [i]);
  } // for
  */
}

void lpsrLayout::print (ostream& os) const
{
  os <<
    "Layout" <<
    endl;

  gIndenter++;

  const int fieldWidth = 9;

  os << left <<
    setw (fieldWidth) <<
   "layoutGlobalStaffSize" << " : " << fLayoutGlobalStaffSize <<
    endl;

/* JMI
  int lilypondAssocs = fLpsrVarValAssocsVector.size ();

  for (int i = 0; i < lilypondAssocs; i++ ) {
    os << fLpsrVarValAssocsVector [i];
  } // for

  int schemeAssocs = fLpsrSchemeVariablesVector.size ();

  for (int i = 0; i < schemeAssocs; i++ ) {
    os << fLpsrSchemeVariablesVector[i];
  } // for
*/

  gIndenter--;
}

ostream& operator<< (ostream& os, const S_lpsrLayout& lay)
{
  lay->print (os);
  return os;
}


}
