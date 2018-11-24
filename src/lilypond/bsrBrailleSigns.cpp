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

#include "bsrBrailleSigns.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
//  #include "bsrTraceOptions.h"
#endif

#include "bsrOptions.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
S_bsrBrailleSign bsrBrailleSign::create (
  int         inputLineNumber)
{
  bsrBrailleSign* o =
    new bsrBrailleSign (
      inputLineNumber);
  assert(o!=0);
  return o;
}

S_bsrBrailleSign bsrBrailleSign::create (
  int         inputLineNumber,
  bsrCellKind cellKind1)
{
  bsrBrailleSign* o =
    new bsrBrailleSign (
      inputLineNumber,
      cellKind1);
  assert(o!=0);
  return o;
}

S_bsrBrailleSign bsrBrailleSign::create (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2)
{
  bsrBrailleSign* o =
    new bsrBrailleSign (
      inputLineNumber,
      cellKind1, cellKind2);
  assert(o!=0);
  return o;
}

S_bsrBrailleSign bsrBrailleSign::create (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2,
  bsrCellKind cellKind3)
{
  bsrBrailleSign* o =
    new bsrBrailleSign (
      inputLineNumber,
      cellKind1, cellKind2, cellKind3);
  assert(o!=0);
  return o;
}

S_bsrBrailleSign bsrBrailleSign::create (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2,
  bsrCellKind cellKind3,
  bsrCellKind cellKind4)
{
  bsrBrailleSign* o =
    new bsrBrailleSign (
      inputLineNumber,
      cellKind1, cellKind2, cellKind3, cellKind4);
  assert(o!=0);
  return o;
}

S_bsrBrailleSign bsrBrailleSign::create (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2,
  bsrCellKind cellKind3,
  bsrCellKind cellKind4,
  bsrCellKind cellKind5)
{
  bsrBrailleSign* o =
    new bsrBrailleSign (
      inputLineNumber,
      cellKind1, cellKind2, cellKind3, cellKind4, cellKind5);
  assert(o!=0);
  return o;
}

S_bsrBrailleSign bsrBrailleSign::create (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2,
  bsrCellKind cellKind3,
  bsrCellKind cellKind4,
  bsrCellKind cellKind5,
  bsrCellKind cellKind6)
{
  bsrBrailleSign* o =
    new bsrBrailleSign (
      inputLineNumber,
      cellKind1, cellKind2, cellKind3, cellKind4, cellKind5, cellKind6);
  assert(o!=0);
  return o;
}

bsrBrailleSign::bsrBrailleSign (
  int         inputLineNumber)
    : bsrElement (inputLineNumber)
{}

bsrBrailleSign::bsrBrailleSign (
  int         inputLineNumber,
  bsrCellKind cellKind1)
    : bsrElement (inputLineNumber)
{
  fCellsList.push_back (cellKind1);
}

bsrBrailleSign::bsrBrailleSign (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2)
    : bsrElement (inputLineNumber)
{
  fCellsList.push_back (cellKind1);
  fCellsList.push_back (cellKind2);
}

bsrBrailleSign::bsrBrailleSign (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2,
  bsrCellKind cellKind3)
    : bsrElement (inputLineNumber)
{
  fCellsList.push_back (cellKind1);
  fCellsList.push_back (cellKind2);
  fCellsList.push_back (cellKind3);
}

bsrBrailleSign::bsrBrailleSign (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2,
  bsrCellKind cellKind3,
  bsrCellKind cellKind4)
    : bsrElement (inputLineNumber)
{
  fCellsList.push_back (cellKind1);
  fCellsList.push_back (cellKind2);
  fCellsList.push_back (cellKind3);
  fCellsList.push_back (cellKind4);
}

bsrBrailleSign::bsrBrailleSign (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2,
  bsrCellKind cellKind3,
  bsrCellKind cellKind4,
  bsrCellKind cellKind5)
    : bsrElement (inputLineNumber)
{
  fCellsList.push_back (cellKind1);
  fCellsList.push_back (cellKind2);
  fCellsList.push_back (cellKind3);
  fCellsList.push_back (cellKind4);
  fCellsList.push_back (cellKind5);
}

bsrBrailleSign::bsrBrailleSign (
  int         inputLineNumber,
  bsrCellKind cellKind1,
  bsrCellKind cellKind2,
  bsrCellKind cellKind3,
  bsrCellKind cellKind4,
  bsrCellKind cellKind5,
  bsrCellKind cellKind6)
    : bsrElement (inputLineNumber)
{
  fCellsList.push_back (cellKind1);
  fCellsList.push_back (cellKind2);
  fCellsList.push_back (cellKind3);
  fCellsList.push_back (cellKind4);
  fCellsList.push_back (cellKind5);
  fCellsList.push_back (cellKind6);
}

bsrBrailleSign::~bsrBrailleSign ()
{}

void bsrBrailleSign::appendBrailleSignToBrailleSign (
  S_bsrBrailleSign otherBrailleSign)
{
  if (otherBrailleSign) {
    if (otherBrailleSign->fCellsList.size ()) {
      list<bsrCellKind>::const_iterator
        iBegin = otherBrailleSign->fCellsList.begin (),
        iEnd   = otherBrailleSign->fCellsList.end (),
        i      = iBegin;
      for ( ; ; ) {
        fCellsList.push_back ((*i));
        if (++i == iEnd) break;
   // JMI      os << " ";
      } // for  
    }
  }
}

void bsrBrailleSign::prependBrailleSignToBrailleSign (
  S_bsrBrailleSign otherBrailleSign)
{
  if (otherBrailleSign) {
    if (otherBrailleSign->fCellsList.size ()) {
      list<bsrCellKind>::const_reverse_iterator
        iBegin = otherBrailleSign->fCellsList.rbegin (),
        iEnd   = otherBrailleSign->fCellsList.rend (),
        i      = iBegin;
      for ( ; ; ) {
        fCellsList.push_front ((*i));
        if (++i == iEnd) break;
   // JMI      os << " ";
      } // for  
    }
  }
}

void bsrBrailleSign::generateBrailleCode (ostream& os)
{
  if (fCellsList.size ()) {
    list<bsrCellKind>::const_iterator
      iBegin = fCellsList.begin (),
      iEnd   = fCellsList.end (),
      i      = iBegin;
    for ( ; ; ) {
      os <<
        bsrCellKindAsShortString ((*i));
      if (++i == iEnd) break;
       os << " ";
    } // for  
  }
}

void bsrBrailleSign::acceptIn (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrBrailleSign::acceptIn ()" <<
      endl;
  }
  
  if (visitor<S_bsrBrailleSign>*
    p =
      dynamic_cast<visitor<S_bsrBrailleSign>*> (v)) {
        S_bsrBrailleSign elem = this;
        
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrBrailleSign::visitStart ()" <<
            endl;
        }
        p->visitStart (elem);
  }
}

void bsrBrailleSign::acceptOut (basevisitor* v)
{
  if (gBsrOptions->fTraceBsrVisitors) {
    gLogIOstream <<
      "% ==> bsrBrailleSign::acceptOut ()" <<
      endl;
  }

  if (visitor<S_bsrBrailleSign>*
    p =
      dynamic_cast<visitor<S_bsrBrailleSign>*> (v)) {
        S_bsrBrailleSign elem = this;
      
        if (gBsrOptions->fTraceBsrVisitors) {
          gLogIOstream <<
            "% ==> Launching bsrBrailleSign::visitEnd ()" <<
            endl;
        }
        p->visitEnd (elem);
  }
}

void bsrBrailleSign::browseData (basevisitor* v)
{}


string bsrBrailleSign::asString () const
{
  stringstream s;
  
  s <<
    "bsrBrailleSign [";
    
  if (fCellsList.size ()) {
    list<bsrCellKind>::const_iterator
      iBegin = fCellsList.begin (),
      iEnd   = fCellsList.end (),
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

string bsrBrailleSign::asShortString () const
{
  return asString ();
}

void bsrBrailleSign::print (ostream& os)
{
  os <<
    "bsrBrailleSign [";
    
  if (fCellsList.size ()) {
    list<bsrCellKind>::const_iterator
      iBegin = fCellsList.begin (),
      iEnd   = fCellsList.end (),
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

ostream& operator<< (ostream& os, const S_bsrBrailleSign& elt)
{
  elt->print (os);
  return os;
}

bsrBrailleSign& operator<< (bsrBrailleSign& brailleSign, const bsrCellKind cellKind)
{
  brailleSign.appendCellKindToBrailleSign (cellKind);
  return brailleSign;
}


}
