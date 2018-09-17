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

#include "lpsrOptions.h"


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
  fStaffSize = 20; // LilyPond default // JMI
}

lpsrLayout::~lpsrLayout ()
{}

void lpsrLayout::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrLayout::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_lpsrLayout>*
    p =
      dynamic_cast<visitor<S_lpsrLayout>*> (v)) {
        S_lpsrLayout elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrLayout::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrLayout::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrLayout::acceptOut ()" <<
      endl;
  }

  if (visitor<S_lpsrLayout>*
    p =
      dynamic_cast<visitor<S_lpsrLayout>*> (v)) {
        S_lpsrLayout elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrLayout::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrLayout::browseData (basevisitor* v)
{

  int lilypondAssocs = flpsrVarValAssocs.size ();

  for (int i = 0; i < lilypondAssocs; i++ ) {
    // browse the variable/value association
    msrBrowser<lpsrVarValAssoc> browser (v);
    browser.browse (*flpsrVarValAssocs [i]);
  } // for
  
  int schemeAssocs = fLpsrSchemeVariables.size ();
  
  for (int i = 0; i < schemeAssocs; i++ ) {
    // browse the Scheme variable/value association
    msrBrowser<lpsrSchemeVariable> browser (v);
    browser.browse (*fLpsrSchemeVariables [i]);
  } // for
}

void lpsrLayout::print (ostream& os)
{
  os <<
    "Layout" <<
    endl;

  gIndenter++;

  const int fieldWidth = 9;

  os << left <<
    setw (fieldWidth) <<
   "StaffSize" << " : " << fStaffSize <<
    endl;
    
  int lilypondAssocs = flpsrVarValAssocs.size ();
  
  for (int i = 0; i < lilypondAssocs; i++ ) {
    os << flpsrVarValAssocs [i];
  } // for

  int schemeAssocs = fLpsrSchemeVariables.size ();
  
  for (int i = 0; i < schemeAssocs; i++ ) {
    os << fLpsrSchemeVariables[i];
  } // for
  
  gIndenter--;
}

ostream& operator<< (ostream& os, const S_lpsrLayout& lay)
{
  lay->print (os);
  return os;
}


}
