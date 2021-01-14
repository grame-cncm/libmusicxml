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

#include "utilities.h"

#include "bsrCellsLists.h"

#include "oahOah.h"

#include "bsrOah.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
S_bsrCellsList bsrCellsList::create (
  int         inputLineNumber)
{
  bsrCellsList* o =
    new bsrCellsList (
      inputLineNumber);
  assert (o != nullptr);
  return o;
}

S_bsrCellsList bsrCellsList::create (
  int         inputLineNumber,
  bsrCellKind cellKind1)
{
  bsrCellsList* o =
    new bsrCellsList (
      inputLineNumber,
      cellKind1);
  assert (o != nullptr);
  return o;
}

S_bsrCellsList bsrCellsList::create (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2)
{
  bsrCellsList* o =
    new bsrCellsList (
      inputLineNumber,
      cellKind1, cellKind2);
  assert (o != nullptr);
  return o;
}

S_bsrCellsList bsrCellsList::create (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2,
  bsrCellKind cellKind3)
{
  bsrCellsList* o =
    new bsrCellsList (
      inputLineNumber,
      cellKind1, cellKind2, cellKind3);
  assert (o != nullptr);
  return o;
}

S_bsrCellsList bsrCellsList::create (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2,
  bsrCellKind cellKind3,
  bsrCellKind cellKind4)
{
  bsrCellsList* o =
    new bsrCellsList (
      inputLineNumber,
      cellKind1, cellKind2, cellKind3, cellKind4);
  assert (o != nullptr);
  return o;
}

S_bsrCellsList bsrCellsList::create (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2,
  bsrCellKind cellKind3,
  bsrCellKind cellKind4,
  bsrCellKind cellKind5)
{
  bsrCellsList* o =
    new bsrCellsList (
      inputLineNumber,
      cellKind1, cellKind2, cellKind3, cellKind4, cellKind5);
  assert (o != nullptr);
  return o;
}

S_bsrCellsList bsrCellsList::create (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2,
  bsrCellKind cellKind3,
  bsrCellKind cellKind4,
  bsrCellKind cellKind5,
  bsrCellKind cellKind6)
{
  bsrCellsList* o =
    new bsrCellsList (
      inputLineNumber,
      cellKind1, cellKind2, cellKind3, cellKind4, cellKind5, cellKind6);
  assert (o != nullptr);
  return o;
}

bsrCellsList::bsrCellsList (
  int         inputLineNumber)
 //   : bsrElement (inputLineNumber)
{}

bsrCellsList::bsrCellsList (
  int         inputLineNumber,
  bsrCellKind cellKind1)
 //   : bsrElement (inputLineNumber)
{
  fCellsListElements.push_back (cellKind1);
}

bsrCellsList::bsrCellsList (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2)
 //   : bsrElement (inputLineNumber)
{
  fCellsListElements.push_back (cellKind1);
  fCellsListElements.push_back (cellKind2);
}

bsrCellsList::bsrCellsList (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2,
  bsrCellKind cellKind3)
 //   : bsrElement (inputLineNumber)
{
  fCellsListElements.push_back (cellKind1);
  fCellsListElements.push_back (cellKind2);
  fCellsListElements.push_back (cellKind3);
}

bsrCellsList::bsrCellsList (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2,
  bsrCellKind cellKind3,
  bsrCellKind cellKind4)
 //   : bsrElement (inputLineNumber)
{
  fCellsListElements.push_back (cellKind1);
  fCellsListElements.push_back (cellKind2);
  fCellsListElements.push_back (cellKind3);
  fCellsListElements.push_back (cellKind4);
}

bsrCellsList::bsrCellsList (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2,
  bsrCellKind cellKind3,
  bsrCellKind cellKind4,
  bsrCellKind cellKind5)
 //   : bsrElement (inputLineNumber)
{
  fCellsListElements.push_back (cellKind1);
  fCellsListElements.push_back (cellKind2);
  fCellsListElements.push_back (cellKind3);
  fCellsListElements.push_back (cellKind4);
  fCellsListElements.push_back (cellKind5);
}

bsrCellsList::bsrCellsList (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2,
  bsrCellKind cellKind3,
  bsrCellKind cellKind4,
  bsrCellKind cellKind5,
  bsrCellKind cellKind6)
 //   : bsrElement (inputLineNumber)
{
  fCellsListElements.push_back (cellKind1);
  fCellsListElements.push_back (cellKind2);
  fCellsListElements.push_back (cellKind3);
  fCellsListElements.push_back (cellKind4);
  fCellsListElements.push_back (cellKind5);
  fCellsListElements.push_back (cellKind6);
}

bsrCellsList::~bsrCellsList ()
{}

void bsrCellsList::appendCellsListToCellsList (
  S_bsrCellsList otherCellsList)
{
  if (otherCellsList) {
    if (otherCellsList->fCellsListElements.size ()) {
      list<bsrCellKind>::const_iterator
        iBegin = otherCellsList->fCellsListElements.begin (),
        iEnd   = otherCellsList->fCellsListElements.end (),
        i      = iBegin;
      for ( ; ; ) {
        fCellsListElements.push_back ((*i));
        if (++i == iEnd) break;
   // JMI      os << " ";
      } // for
    }
  }
}

void bsrCellsList::prependCellsListToCellsList (
  S_bsrCellsList otherCellsList)
{
  if (otherCellsList) {
    if (otherCellsList->fCellsListElements.size ()) {
      list<bsrCellKind>::const_reverse_iterator
        iBegin = otherCellsList->fCellsListElements.rbegin (),
        iEnd   = otherCellsList->fCellsListElements.rend (),
        i      = iBegin;
      for ( ; ; ) {
        fCellsListElements.push_front ((*i));
        if (++i == iEnd) break;
   // JMI      os << " ";
      } // for
    }
  }
}

/* JMI
void bsrCellsList::generateBrailleCode (ostream& os)
{
  if (fCellsListElements.size ()) {
    list<bsrCellKind>::const_iterator
      iBegin = fCellsListElements.begin (),
      iEnd   = fCellsListElements.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        bsrCellKindAsShortString ((*i));
      if (++i == iEnd) break;
       os << " ";
    } // for
  }
}
*/

void bsrCellsList::acceptIn (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrCellsList::acceptIn ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrCellsList>*
    p =
      dynamic_cast<visitor<S_bsrCellsList>*> (v)) {
        S_bsrCellsList elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrCellsList::visitStart ()" <<
            endl;
        }
#endif
        p->visitStart (elem);
  }
}

void bsrCellsList::acceptOut (basevisitor* v)
{
#ifdef TRACING_IS_ENABLED
  if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
    gLogStream <<
      "% ==> bsrCellsList::acceptOut ()" <<
      endl;
  }
#endif

  if (visitor<S_bsrCellsList>*
    p =
      dynamic_cast<visitor<S_bsrCellsList>*> (v)) {
        S_bsrCellsList elem = this;

#ifdef TRACING_IS_ENABLED
        if (gGlobalBsrOahGroup->getTraceBsrVisitors ()) {
          gLogStream <<
            "% ==> Launching bsrCellsList::visitEnd ()" <<
            endl;
        }
#endif
        p->visitEnd (elem);
  }
}

void bsrCellsList::browseData (basevisitor* v)
{}


string bsrCellsList::asString () const
{
  stringstream s;

  s <<
    "cellsListElements [";

  if (fCellsListElements.size ()) {
    list<bsrCellKind>::const_iterator
      iBegin = fCellsListElements.begin (),
      iEnd   = fCellsListElements.end (),
      i      = iBegin;
    for ( ; ; ) {
      s <<
        bsrCellKindAsShortString ((*i));
      if (++i == iEnd) break;
       s << " ";
    } // for
  }

  s <<
    "]";

  return s.str ();
}

string bsrCellsList::asShortString () const
{
  return asString ();
}

void bsrCellsList::print (ostream& os) const
{
  os <<
    "cellsListElements [";

  if (fCellsListElements.size ()) {
    list<bsrCellKind>::const_iterator
      iBegin = fCellsListElements.begin (),
      iEnd   = fCellsListElements.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        bsrCellKindAsShortString ((*i));
      if (++i == iEnd) break;
       os << " ";
    } // for
  }

  os <<
    "]";
}

ostream& operator<< (ostream& os, const S_bsrCellsList& elt)
{
  elt->print (os);
  return os;
}

bsrCellsList& operator<< (bsrCellsList& cellsList, const bsrCellKind cellKind)
{
  cellsList.appendCellKindToCellsList (cellKind);
  return cellsList;
}


}
