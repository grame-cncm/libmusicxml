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

#include "bsrBrailleCodeGenerators.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "bsrOptions.h"

#include "xml2brlOptionsHandling.h"

#include "messagesHandling.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
/* JMI
S_bsrBrailleGenerator bsrBrailleGenerator::create (
  brailleOptions::bsrByteOrderingKind
                      byteOrderingKind,
  ostream&            brailleOutputStream)
{
  bsrBrailleGenerator* o =
    new bsrBrailleGenerator (
      byteOrderingKind,
      brailleOutputStream);
  assert (o!=0);
  return o;
}
*/

bsrBrailleGenerator::bsrBrailleGenerator (
  brailleOptions::bsrByteOrderingKind
                      byteOrderingKind,
  ostream&            brailleOutputStream)
    : fBrailleOutputStream (brailleOutputStream)
{
  fByteOrderingKind    = byteOrderingKind;
}

bsrBrailleGenerator::~bsrBrailleGenerator ()
{}

string bsrBrailleGenerator::asString () const
{
  stringstream s;

  s <<
    "BrailleGenerator" << 
    ", byteOrderingKind: " <<
    brailleOptions::byteOrderingKindAsString (
      fByteOrderingKind);

  return s.str ();
}

void bsrBrailleGenerator::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_bsrBrailleGenerator& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_bsrUTF8BrailleGenerator bsrUTF8BrailleGenerator::create (
  brailleOptions::bsrByteOrderingKind
                      byteOrderingKind,
  ostream&            brailleOutputStream)
{
  bsrUTF8BrailleGenerator* o =
    new bsrUTF8BrailleGenerator (
      byteOrderingKind,
      brailleOutputStream);
  assert (o!=0);
  return o;
}

bsrUTF8BrailleGenerator::bsrUTF8BrailleGenerator (
  brailleOptions::bsrByteOrderingKind
                      byteOrderingKind,
  ostream&            brailleOutputStream)
    : bsrBrailleGenerator (
        byteOrderingKind,
        brailleOutputStream)
{}

bsrUTF8BrailleGenerator::~bsrUTF8BrailleGenerator ()
{}

void bsrUTF8BrailleGenerator::generateCodeForBrailleCell (
  bsrCellKind cellKind)
{
}

void bsrUTF8BrailleGenerator::generateCodeForBrailleSign (
  S_bsrBrailleSign brailleSign)
{
}

string bsrUTF8BrailleGenerator::asString () const
{
  stringstream s;

  s <<
    "UTF8BrailleGenerator" << 
    ", byteOrderingKind: " <<
    brailleOptions::byteOrderingKindAsString (
      fByteOrderingKind);

  return s.str ();
}

void bsrUTF8BrailleGenerator::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_bsrUTF8BrailleGenerator& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_bsrUTF16BigEndianBrailleGenerator bsrUTF16BigEndianBrailleGenerator::create (
  brailleOptions::bsrByteOrderingKind
                      byteOrderingKind,
  ostream&            brailleOutputStream)
{
  bsrUTF16BigEndianBrailleGenerator* o =
    new bsrUTF16BigEndianBrailleGenerator (
      byteOrderingKind,
      brailleOutputStream);
  assert (o!=0);
  return o;
}

bsrUTF16BigEndianBrailleGenerator::bsrUTF16BigEndianBrailleGenerator (
  brailleOptions::bsrByteOrderingKind
                      byteOrderingKind,
  ostream&            brailleOutputStream)
    : bsrBrailleGenerator (
        byteOrderingKind,
        brailleOutputStream)
{}

bsrUTF16BigEndianBrailleGenerator::~bsrUTF16BigEndianBrailleGenerator ()
{}

void bsrUTF16BigEndianBrailleGenerator::generateCodeForBrailleCell (
  bsrCellKind cellKind)
{
}

void bsrUTF16BigEndianBrailleGenerator::generateCodeForBrailleSign (
  S_bsrBrailleSign brailleSign)
{
}

string bsrUTF16BigEndianBrailleGenerator::asString () const
{
  stringstream s;

  s <<
    "UTF16BigEndianBrailleGenerator" << 
    ", byteOrderingKind: " <<
    brailleOptions::byteOrderingKindAsString (
      fByteOrderingKind);

  return s.str ();
}

void bsrUTF16BigEndianBrailleGenerator::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_bsrUTF16BigEndianBrailleGenerator& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_bsrUTF16SmallEndianBrailleGenerator bsrUTF16SmallEndianBrailleGenerator::create (
  brailleOptions::bsrByteOrderingKind
                      byteOrderingKind,
  ostream&            brailleOutputStream)
{
  bsrUTF16SmallEndianBrailleGenerator* o =
    new bsrUTF16SmallEndianBrailleGenerator (
      byteOrderingKind,
      brailleOutputStream);
  assert (o!=0);
  return o;
}

bsrUTF16SmallEndianBrailleGenerator::bsrUTF16SmallEndianBrailleGenerator (
  brailleOptions::bsrByteOrderingKind
                      byteOrderingKind,
  ostream&            brailleOutputStream)
    : bsrBrailleGenerator (
        byteOrderingKind,
        brailleOutputStream)
{}

bsrUTF16SmallEndianBrailleGenerator::~bsrUTF16SmallEndianBrailleGenerator ()
{}

void bsrUTF16SmallEndianBrailleGenerator::generateCodeForBrailleCell (
  bsrCellKind cellKind)
{
}

void bsrUTF16SmallEndianBrailleGenerator::generateCodeForBrailleSign (
  S_bsrBrailleSign brailleSign)
{
}

string bsrUTF16SmallEndianBrailleGenerator::asString () const
{
  stringstream s;

  s <<
    "UTF16SmallEndianBrailleGenerator" << 
    ", byteOrderingKind: " <<
    brailleOptions::byteOrderingKindAsString (
      fByteOrderingKind);

  return s.str ();
}

void bsrUTF16SmallEndianBrailleGenerator::print (ostream& os)
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_bsrUTF16SmallEndianBrailleGenerator& elt)
{
  elt->print (os);
  return os;
}


}

    /*
    // generate a BOM if requested
    switch (gBrailleOptions->fByteOrderingKind) {
      case brailleOptions::kByteOrderingBigEndian:
        fBrailleCodeIOstream <<
          kBOMBigEndian;
        break;
      case brailleOptions::kByteOrderingSmallEndian:
        fBrailleCodeIOstream <<
          kBOMSmallEndian;
        break;
    } // switch

    if (true) { // JMI TEMP
      // generate the table of all Dots 6 cells
      for (wchar_t wch = L'\u2800'; wch <= L'\u280f'; wch++) {
        fBrailleCodeIOstream << wch;
      } // for
      fBrailleCodeIOstream << kBrailleEOL;
      for (wchar_t wch = L'\u2810'; wch <= L'\u281f'; wch++) {
        fBrailleCodeIOstream << wch;
      } // for
      fBrailleCodeIOstream << kBrailleEOL;
      fBrailleCodeIOstream << kBrailleEOP;
      
      for (wchar_t wch = L'\u2820'; wch <= L'\u282f'; wch++) {
        fBrailleCodeIOstream << wch;
      } // for
      fBrailleCodeIOstream << kBrailleEOL;
      for (wchar_t wch = L'\u2830'; wch <= L'\u283f'; wch++) {
        fBrailleCodeIOstream << wch;
      } // for
      fBrailleCodeIOstream << kBrailleEOL;
      fBrailleCodeIOstream << kBrailleEOP;
    }
    */

