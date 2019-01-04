/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "lpsrPartGroups.h"

#include "lpsrOptions.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
S_lpsrPartGroupBlock lpsrPartGroupBlock::create (
  S_msrPartGroup partGroup)
{
  lpsrPartGroupBlock* o = new lpsrPartGroupBlock (
    partGroup);
  assert(o!=0);
  return o;
}

lpsrPartGroupBlock::lpsrPartGroupBlock (
  S_msrPartGroup partGroup)
    : lpsrElement (0) // JMI
{
  fPartGroup = partGroup;
}

lpsrPartGroupBlock::~lpsrPartGroupBlock ()
{}

void lpsrPartGroupBlock::acceptIn (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrPartGroupBlock::acceptIn ()" <<
      endl;
  }
      
  if (visitor<S_lpsrPartGroupBlock>*
    p =
      dynamic_cast<visitor<S_lpsrPartGroupBlock>*> (v)) {
        S_lpsrPartGroupBlock elem = this;
        
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrPartGroupBlock::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void lpsrPartGroupBlock::acceptOut (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrPartGroupBlock::acceptOut ()" <<
      endl;
  }

  if (visitor<S_lpsrPartGroupBlock>*
    p =
      dynamic_cast<visitor<S_lpsrPartGroupBlock>*> (v)) {
        S_lpsrPartGroupBlock elem = this;
      
        if (gLpsrOptions->fTraceLpsrVisitors) {
          gLogIOstream <<
            "% ==> Launching lpsrPartGroupBlock::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void lpsrPartGroupBlock::browseData (basevisitor* v)
{
  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% ==> lpsrPartGroupBlock::browseData ()" <<
      endl;
  }

  for (
    list<S_msrElement>::const_iterator i = fPartGroupBlockElements.begin ();
    i != fPartGroupBlockElements.end ();
    i++) {
    // browse the element
    msrBrowser<msrElement> browser (v);
    browser.browse (*(*i));
  } // for

  if (gLpsrOptions->fTraceLpsrVisitors) {
    gLogIOstream <<
      "% <== lpsrPartGroupBlock::browseData ()" <<
      endl;
  }
}

ostream& operator<< (ostream& os, const S_lpsrPartGroupBlock& scr)
{
  scr->print (os);
  return os;
}

void lpsrPartGroupBlock::print (ostream& os)
{
  os <<
    endl <<
    "PartGroupBlock for partGroup \"" <<
    fPartGroup->getPartGroupCombinedName () <<
    "\", " << fPartGroup->partGroupSymbolKindAsString () <<
    ", " <<
    singularOrPlural (
      fPartGroupBlockElements.size (), "element", "elements") <<
    endl <<
    endl;

  gIndenter++;

  if (fPartGroupBlockElements.size ()) {  
    list<S_msrElement>::const_iterator
      iBegin = fPartGroupBlockElements.begin (),
      iEnd   = fPartGroupBlockElements.end (),
      i      = iBegin;
    for ( ; ; ) {
      os << (*i);
      if (++i == iEnd) break;
      os << endl;
    } // for
  }

  gIndenter--;
}


}
