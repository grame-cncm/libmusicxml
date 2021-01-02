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

#include "bsr2brailleCodeGenerators.h"

#include "messagesHandling.h"

#include "oahOah.h"
#include "generalOah.h"
#include "bsrOah.h"


using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
string bsrUTFKindAsString (
  bsrUTFKind UTFKind)
{
  string result;

  switch (UTFKind) {
    case kUTF8:
      result = "UTF8";
      break;
    case kUTF16:
      result = "UTF16";
      break;
  } // switch

  return result;
}

string bsrByteOrderingKindAsString (
  bsrByteOrderingKind byteOrderingKind)
{
  string result;

  switch (byteOrderingKind) {
    case kByteOrderingNone:
      result = "byteOrderingNone";
      break;
    case kByteOrderingBigEndian:
      result = "byteOrderingBigEndian";
      break;
    case kByteOrderingSmallEndian:
      result = "byteOrderingSmallEndian";
      break;
  } // switch

  return result;
}

//______________________________________________________________________________
// writing UTF-16 to ostreams
void write_wchar_t (ostream& os, wchar_t cell)
{
  union Conversion {
    wchar_t       cellar;
    unsigned char chars [2];
  } conversion;

  conversion.cellar = cell;

  // write in reverse order!
  os << conversion.chars [0] << conversion.chars [1];
}

ostream& operator<< (ostream& os, const bsrCellKind cell)
{
  union Conversion {
    wchar_t       cellar;
    unsigned char chars [2];
  } conversion;

  conversion.cellar = cell;

  // write in reverse order!
  os << conversion.chars [0] << conversion.chars [1];

  return os;
}

//______________________________________________________________________________
/* this class is purely virtual
S_bsrBrailleGenerator bsrBrailleGenerator::create (
  bsrByteOrderingKind byteOrderingKind,
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
  ostream& brailleOutputStream)
  : fBrailleOutputStream (
      brailleOutputStream)
{}

bsrBrailleGenerator::~bsrBrailleGenerator ()
{}

void bsrBrailleGenerator::generateCodeForCellsList (
  S_bsrCellsList cellsList)
{
  const list<bsrCellKind>&
    cellsListElements =
      cellsList->getCellsListElements ();

  if (cellsListElements.size ()) {
    list<bsrCellKind>::const_iterator
      iBegin = cellsListElements.begin (),
      iEnd   = cellsListElements.end (),
      i      = iBegin;
    for ( ; ; ) {
      generateCodeForBrailleCell ((*i));
      if (++i == iEnd) break;
      // JMI s << " ";
    } // for
  }
}

void bsrBrailleGenerator::generateCodeForMusicHeading (
  S_bsrMusicHeading musicHeading)
{
  this->
    generateCodeForCellsList (
      musicHeading->
        fetchCellsList ());
}

void bsrBrailleGenerator::generateCodeForLineContents (
  S_bsrLineContents lineContents)
{
  this->
    generateCodeForCellsList (
      lineContents->
        fetchCellsList ());
}

string bsrBrailleGenerator::asString () const
{
  stringstream s;

  s <<
    "BrailleGenerator";

  return s.str ();
}

void bsrBrailleGenerator::print (ostream& os) const
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
S_bsrAsciiBrailleGenerator bsrAsciiBrailleGenerator::create (
  ostream& brailleOutputStream)
{
  bsrAsciiBrailleGenerator* o =
    new bsrAsciiBrailleGenerator (
      brailleOutputStream);
  assert (o!=0);
  return o;
}

bsrAsciiBrailleGenerator::bsrAsciiBrailleGenerator (
  ostream& brailleOutputStream)
    : bsrBrailleGenerator (
        brailleOutputStream)
{
//  writeTestData (brailleOutputStream);
}

bsrAsciiBrailleGenerator::~bsrAsciiBrailleGenerator ()
{}

void bsrAsciiBrailleGenerator::generateCodeForBrailleCell (
  bsrCellKind cellKind)
{
  string stringForCell;

  switch (cellKind) {
    case kCellUnknown:
      {
        stringstream s;

        s <<
          "cannot generate code for braille cell '" <<
          bsrCellKindAsString (cellKind) <<
          "'";
        msrInternalError (
          gGlobalOahOahGroup->getInputSourceName (),
          -999, // TICINO inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;

    case kCellEOL:    stringForCell = "\x0a"; break;
    case kCellEOP:    stringForCell = "\x0c"; break;

    case kDotsNone:   stringForCell = "\x20"; break;

    case kDots1:      stringForCell = "\x41"; break;
    case kDots2:      stringForCell = "\x31"; break;
    case kDots12:     stringForCell = "\x42"; break;
    case kDots3:      stringForCell = "\x27"; break;
    case kDots13:     stringForCell = "\x4b"; break;
    case kDots23:     stringForCell = "\x32"; break;
    case kDots123:    stringForCell = "\x4c"; break;
    case kDots4:      stringForCell = "\x40"; break;
    case kDots14:     stringForCell = "\x43"; break;
    case kDots24:     stringForCell = "\x49"; break;
    case kDots124:    stringForCell = "\x46"; break;
    case kDots34:     stringForCell = "\x2f"; break;
    case kDots134:    stringForCell = "\x4d"; break;
    case kDots234:    stringForCell = "\x53"; break;
    case kDots1234:   stringForCell = "\x50"; break;

    case kDots5:      stringForCell = "\x22"; break;
    case kDots15:     stringForCell = "\x45"; break;
    case kDots25:     stringForCell = "\x33"; break;
    case kDots125:    stringForCell = "\x48"; break;
    case kDots35:     stringForCell = "\x39"; break;
    case kDots135:    stringForCell = "\x4f"; break;
    case kDots235:    stringForCell = "\x37"; break;
    case kDots1235:   stringForCell = "\x52"; break;
    case kDots45:     stringForCell = "\x5e"; break;
    case kDots145:    stringForCell = "\x44"; break;
    case kDots245:    stringForCell = "\x4a"; break;
    case kDots1245:   stringForCell = "\x47"; break;
    case kDots345:    stringForCell = "\x3e"; break;
    case kDots1345:   stringForCell = "\x4e"; break;
    case kDots2345:   stringForCell = "\x54"; break;
    case kDots12345:  stringForCell = "\x51"; break;

    case kDots6:      stringForCell = "\x2c"; break;
    case kDots16:     stringForCell = "\x2a"; break;
    case kDots26:     stringForCell = "\x35"; break;
    case kDots126:    stringForCell = "\x3c"; break;
    case kDots36:     stringForCell = "\x2d"; break;
    case kDots136:    stringForCell = "\x55"; break;
    case kDots236:    stringForCell = "\x38"; break;
    case kDots1236:   stringForCell = "\x56"; break;
    case kDots46:     stringForCell = "\x2e"; break;
    case kDots146:    stringForCell = "\x25"; break;
    case kDots246:    stringForCell = "\x5b"; break;
    case kDots1246:   stringForCell = "\x24"; break;
    case kDots346:    stringForCell = "\x2b"; break;
    case kDots1346:   stringForCell = "\x58"; break;
    case kDots2346:   stringForCell = "\x21"; break;
    case kDots12346:  stringForCell = "\x26"; break;

    case kDots56:     stringForCell = "\x3b"; break;
    case kDots156:    stringForCell = "\x3a"; break;
    case kDots256:    stringForCell = "\x34"; break;
    case kDots1256:   stringForCell = "\x5c"; break;
    case kDots356:    stringForCell = "\x30"; break;
    case kDots1356:   stringForCell = "\x5a"; break;
    case kDots2356:   stringForCell = "\x37"; break;
    case kDots12356:  stringForCell = "\x28"; break;
    case kDots456:    stringForCell = "\x5f"; break;
    case kDots1456:   stringForCell = "\x3f"; break;
    case kDots2456:   stringForCell = "\x57"; break;
    case kDots12456:  stringForCell = "\x5d"; break;
    case kDots3456:   stringForCell = "\x23"; break;
    case kDots13456:  stringForCell = "\x59"; break;
    case kDots23456:  stringForCell = "\x29"; break;
    case kDots123456: stringForCell = "\x3d"; break;
  } // switch

  fBrailleOutputStream <<
    stringForCell;
}

void bsrAsciiBrailleGenerator::writeTestData (ostream& os)
{
  gLogStream <<
    "bsrAsciiBrailleGenerator::writeTestData()" <<
    endl;

  // generate the table of all Dots 6 cells
/*
  " A1B'K2l@CIF/MSP\"E3H9O6R^DJG>NTQ,*5<-U8V.%[$+X!&;:4\\0Z7(_?W]#Y)="

  produces:

  "⠀⠁⠂⠃⠄⠅⠆⠇⠈⠉⠊⠋⠌⠍⠎⠏⠐⠑⠒⠓⠔⠕⠖⠗⠘⠙⠚⠛⠜⠝⠞⠟⠠⠡⠢⠣⠤⠥⠦⠧⠨⠩⠪⠫⠬⠭⠮⠯⠰⠱⠲⠳⠴⠵⠶⠷⠸⠹⠺⠻⠼⠽⠾⠿"
*/
  os <<
    " A1B'K2l@CIF/MSP\"E3H9O6R^DJG>NTQ,*5<-U8V.%[$+X!&;:4\\0Z7(_?W]#Y)=" <<
 //   "\x0a"; // \n
    endl; // \n
}

string bsrAsciiBrailleGenerator::asString () const
{
  stringstream s;

  s <<
    "AsciiBrailleGenerator";

  return s.str ();
}

void bsrAsciiBrailleGenerator::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_bsrAsciiBrailleGenerator& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_bsrUTF8BrailleGenerator bsrUTF8BrailleGenerator::create (
  bsrByteOrderingKind byteOrderingKind,
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
  bsrByteOrderingKind byteOrderingKind,
  ostream&            brailleOutputStream)
    : bsrBrailleGenerator (
        brailleOutputStream)
{
  fByteOrderingKind = byteOrderingKind;

  // generate a BOM if requested
  switch (fByteOrderingKind) {
    case kByteOrderingNone:
      break;
    case kByteOrderingBigEndian:
      fBrailleOutputStream <<
        kBOM_UTF_8;
      break;
    case kByteOrderingSmallEndian:
      // should not occur JMI
      break;
  } // switch

//  writeTestData (brailleOutputStream);
}

bsrUTF8BrailleGenerator::~bsrUTF8BrailleGenerator ()
{}

void bsrUTF8BrailleGenerator::generateCodeForBrailleCell (
  bsrCellKind cellKind)
{
  string stringForCell;

  switch (cellKind) {
    case kCellUnknown:
      {
        stringstream s;

        s <<
          "cannot generate code for braille cell '" <<
          bsrCellKindAsString (cellKind) <<
          "'";
        msrInternalError (
          gGlobalOahOahGroup->getInputSourceName (),
          -999, // TICINO inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;

    case kCellEOL:    stringForCell = "\x0a"; break; // U+000A
    case kCellEOP:    stringForCell = "\x0c"; break; // U+000C

    case kDotsNone:   stringForCell = "\xe2\xa0\x80"; break; // U+2800 ⠀ e2 a0 80 BLANK

    case kDots1:      stringForCell = "\xe2\xa0\x81"; break; // U+2801 ⠀ e2 a0 81 DOTS-1
    case kDots2:      stringForCell = "\xe2\xa0\x82"; break; // U+2802 ⠀ e2 a0 82 DOTS-2
    case kDots12:     stringForCell = "\xe2\xa0\x83"; break; // U+2803 ⠀ e2 a0 83 DOTS-12
    case kDots3:      stringForCell = "\xe2\xa0\x84"; break; // U+2804  ⠄ e2 a0 84 DOTS-3
    case kDots13:     stringForCell = "\xe2\xa0\x85"; break; // U+2805  ⠅ e2 a0 85 DOTS-13
    case kDots23:     stringForCell = "\xe2\xa0\x86"; break; // U+2806  ⠆ e2 a0 86 DOTS-23
    case kDots123:    stringForCell = "\xe2\xa0\x87"; break; // U+2807  ⠇ e2 a0 87 DOTS-123
    case kDots4:      stringForCell = "\xe2\xa0\x88"; break; // U+2808  ⠈ e2 a0 88 DOTS-4
    case kDots14:     stringForCell = "\xe2\xa0\x89"; break; // U+2809  ⠉ e2 a0 89 DOTS-14
    case kDots24:     stringForCell = "\xe2\xa0\x8a"; break; // U+280A  ⠊ e2 a0 8a DOTS-24
    case kDots124:    stringForCell = "\xe2\xa0\x8b"; break; // U+280B  ⠋ e2 a0 8b DOTS-124
    case kDots34:     stringForCell = "\xe2\xa0\x8c"; break; // U+280C  ⠌ e2 a0 8c DOTS-34
    case kDots134:    stringForCell = "\xe2\xa0\x8d"; break; // U+280D  ⠍ e2 a0 8d DOTS-134
    case kDots234:    stringForCell = "\xe2\xa0\x8e"; break; // U+280E  ⠎ e2 a0 8e DOTS-234
    case kDots1234:   stringForCell = "\xe2\xa0\x8f"; break; // U+280F  ⠏ e2 a0 8f DOTS-1234

    case kDots5:      stringForCell = "\xe2\xa0\x90"; break; // U+2810  ⠐ e2 a0 90 DOTS-5
    case kDots15:     stringForCell = "\xe2\xa0\x91"; break; // U+2811  ⠑ e2 a0 91 DOTS-15
    case kDots25:     stringForCell = "\xe2\xa0\x92"; break; // U+2812  ⠒ e2 a0 92 DOTS-25
    case kDots125:    stringForCell = "\xe2\xa0\x93"; break; // U+2813  ⠓ e2 a0 93 DOTS-125
    case kDots35:     stringForCell = "\xe2\xa0\x94"; break; // U+2814  ⠔ e2 a0 94 DOTS-35
    case kDots135:    stringForCell = "\xe2\xa0\x95"; break; // U+2815  ⠕ e2 a0 95 DOTS-135
    case kDots235:    stringForCell = "\xe2\xa0\x96"; break; // U+2816  ⠖ e2 a0 96 DOTS-235
    case kDots1235:   stringForCell = "\xe2\xa0\x97"; break; // U+2817  ⠗ e2 a0 97 DOTS-1235
    case kDots45:     stringForCell = "\xe2\xa0\x98"; break; // U+2818  ⠘ e2 a0 98 DOTS-45
    case kDots145:    stringForCell = "\xe2\xa0\x99"; break; // U+2819  ⠙ e2 a0 99 DOTS-145
    case kDots245:    stringForCell = "\xe2\xa0\x9a"; break; // U+281A  ⠚ e2 a0 9a DOTS-245
    case kDots1245:   stringForCell = "\xe2\xa0\x9b"; break; // U+281B  ⠛ e2 a0 9b DOTS-1245
    case kDots345:    stringForCell = "\xe2\xa0\x9c"; break; // U+281C  ⠜ e2 a0 9c DOTS-345
    case kDots1345:   stringForCell = "\xe2\xa0\x9d"; break; // U+281D  ⠝ e2 a0 9d DOTS-1345
    case kDots2345:   stringForCell = "\xe2\xa0\x9e"; break; // U+281E  ⠞ e2 a0 9e DOTS-2345
    case kDots12345:  stringForCell = "\xe2\xa0\x9f"; break; // U+281F  ⠟ e2 a0 9f DOTS-12345

    case kDots6:      stringForCell = "\xe2\xa0\xa0"; break; // U+2820  ⠠ e2 a0 a0 DOTS-6
    case kDots16:     stringForCell = "\xe2\xa0\xa1"; break; // U+2821  ⠡ e2 a0 a1 DOTS-16
    case kDots26:     stringForCell = "\xe2\xa0\xa2"; break; // U+2822  ⠢ e2 a0 a2 DOTS-26
    case kDots126:    stringForCell = "\xe2\xa0\xa3"; break; // U+2823  ⠣ e2 a0 a3 DOTS-126
    case kDots36:     stringForCell = "\xe2\xa0\xa4"; break; // U+2824  ⠤ e2 a0 a4 DOTS-36
    case kDots136:    stringForCell = "\xe2\xa0\xa5"; break; // U+2825  ⠥ e2 a0 a5 DOTS-136
    case kDots236:    stringForCell = "\xe2\xa0\xa6"; break; // U+2826  ⠦ e2 a0 a6 DOTS-236
    case kDots1236:   stringForCell = "\xe2\xa0\xa7"; break; // U+2827  ⠧ e2 a0 a7 DOTS-1236
    case kDots46:     stringForCell = "\xe2\xa0\xa8"; break; // U+2828  ⠨ e2 a0 a8 DOTS-46
    case kDots146:    stringForCell = "\xe2\xa0\xa9"; break; // U+2829  ⠩ e2 a0 a9 DOTS-146
    case kDots246:    stringForCell = "\xe2\xa0\xaa"; break; // U+282A  ⠪ e2 a0 aa DOTS-246
    case kDots1246:   stringForCell = "\xe2\xa0\xab"; break; // U+282b  ⠫ e2 a0 ab DOTS-1246
    case kDots346:    stringForCell = "\xe2\xa0\xac"; break; // U+282C  ⠬ e2 a0 ac DOTS-346
    case kDots1346:   stringForCell = "\xe2\xa0\xad"; break; // U+282D  ⠭ e2 a0 ad DOTS-1346
    case kDots2346:   stringForCell = "\xe2\xa0\xae"; break; // U+282E  ⠮ e2 a0 ae DOTS-2346
    case kDots12346:  stringForCell = "\xe2\xa0\xaf"; break; // U+282F  ⠯ e2 a0 af DOTS-12346

    case kDots56:     stringForCell = "\xe2\xa0\xb0"; break; // U+2830  ⠰ e2 a0 b0 DOTS-56
    case kDots156:    stringForCell = "\xe2\xa0\xb1"; break; // U+2831  ⠱ e2 a0 b1 DOTS-156
    case kDots256:    stringForCell = "\xe2\xa0\xb2"; break; // U+2832  ⠲ e2 a0 b2 DOTS-256
    case kDots1256:   stringForCell = "\xe2\xa0\xb3"; break; // U+2833  ⠳ e2 a0 b3 DOTS-1256
    case kDots356:    stringForCell = "\xe2\xa0\xb4"; break; // U+2834  ⠴ e2 a0 b4 DOTS-356
    case kDots1356:   stringForCell = "\xe2\xa0\xb5"; break; // U+2835  ⠵ e2 a0 b5 DOTS-1356
    case kDots2356:   stringForCell = "\xe2\xa0\xb6"; break; // U+2836  ⠶ e2 a0 b6 DOTS-2356
    case kDots12356:  stringForCell = "\xe2\xa0\xb7"; break; // U+2837  ⠷ e2 a0 b7 DOTS-12356
    case kDots456:    stringForCell = "\xe2\xa0\xb8"; break; // U+2838  ⠸ e2 a0 b8 DOTS-456
    case kDots1456:   stringForCell = "\xe2\xa0\xb9"; break; // U+2839  ⠹ e2 a0 b9 DOTS-1456
    case kDots2456:   stringForCell = "\xe2\xa0\xba"; break; // U+283A  ⠺ e2 a0 ba DOTS-2456
    case kDots12456:  stringForCell = "\xe2\xa0\xbb"; break; // U+283B  ⠻ e2 a0 bb DOTS-12456
    case kDots3456:   stringForCell = "\xe2\xa0\xbc"; break; // U+283C  ⠼ e2 a0 bc DOTS-3456
    case kDots13456:  stringForCell = "\xe2\xa0\xbd"; break; // U+283D  ⠽ e2 a0 bd DOTS-13456
    case kDots23456:  stringForCell = "\xe2\xa0\xbe"; break; // U+283E  ⠾ e2 a0 be DOTS-23456
    case kDots123456: stringForCell = "\xe2\xa0\xbf"; break; // U+283F ⠿ e2 a0 bf DOTS-123456
  } // switch

  fBrailleOutputStream <<
    stringForCell;
}

void bsrUTF8BrailleGenerator::writeTestData (ostream& os)
{
  gLogStream <<
    "bsrUTF8BrailleGenerator::writeTestData()" <<
    endl;

  // generate the table of all Dots 6 cells
  for (wchar_t wch = L'\u2800'; wch <= L'\u280f'; ++wch) {
    os << wch;
  } // for
  os << kCellEOL;
  for (wchar_t wch = L'\u2810'; wch <= L'\u281f'; ++wch) {
    os << wch;
  } // for
  os << kCellEOL;
  os << kCellEOP;

  for (wchar_t wch = L'\u2820'; wch <= L'\u282f'; ++wch) {
    os << wch;
  } // for
  os << kCellEOL;
  for (wchar_t wch = L'\u2830'; wch <= L'\u283f'; ++wch) {
    os << wch;
  } // for
  os << kCellEOL;
  os << kCellEOP;
  os << endl;
}

string bsrUTF8BrailleGenerator::asString () const
{
  stringstream s;

  s <<
    "UTF8BrailleGenerator" <<
    ", byteOrderingKind: " <<
    bsrByteOrderingKindAsString (
      fByteOrderingKind);

  return s.str ();
}

void bsrUTF8BrailleGenerator::print (ostream& os) const
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
S_bsrUTF8DebugBrailleGenerator bsrUTF8DebugBrailleGenerator::create (
  bsrByteOrderingKind byteOrderingKind,
  ostream&            brailleOutputStream)
{
  bsrUTF8DebugBrailleGenerator* o =
    new bsrUTF8DebugBrailleGenerator (
      byteOrderingKind,
      brailleOutputStream);
  assert (o!=0);
  return o;
}

bsrUTF8DebugBrailleGenerator::bsrUTF8DebugBrailleGenerator (
  bsrByteOrderingKind byteOrderingKind,
  ostream&            brailleOutputStream)
    : bsrUTF8BrailleGenerator (
        byteOrderingKind,
        brailleOutputStream)
{
  fByteOrderingKind = byteOrderingKind;

  // generate a BOM if requested
  switch (fByteOrderingKind) {
    case kByteOrderingNone:
      break;
    case kByteOrderingBigEndian:
      fBrailleOutputStream <<
        kBOM_UTF_8;
      break;
    case kByteOrderingSmallEndian:
      // should not occur JMI
      break;
  } // switch

//  writeTestData (brailleOutputStream);
}

bsrUTF8DebugBrailleGenerator::~bsrUTF8DebugBrailleGenerator ()
{}

string bsrUTF8DebugBrailleGenerator::asString () const
{
  stringstream s;

  s <<
    "UTF8DebugBrailleGenerator" <<
    ", byteOrderingKind: " <<
    bsrByteOrderingKindAsString (
      fByteOrderingKind);

  return s.str ();
}

void bsrUTF8DebugBrailleGenerator::generateCodeForMusicHeading (
  S_bsrMusicHeading musicHeading)
{
  this->
    generateCodeForCellsList (
      musicHeading->
        fetchCellsList ());

  this->
    generateCodeForBrailleCell (
      kCellEOL);

  fBrailleOutputStream <<
    musicHeading->
      asDebugString ();

  this->
    generateCodeForBrailleCell (
      kCellEOL);
}

void bsrUTF8DebugBrailleGenerator::generateCodeForLineContents (
  S_bsrLineContents lineContents)
{
  this->
    generateCodeForCellsList (
      lineContents->
        fetchCellsList ());

  this->
    generateCodeForBrailleCell (
      kCellEOL);

  fBrailleOutputStream <<
    lineContents->
      asDebugString ();

  this->
    generateCodeForBrailleCell (
      kCellEOL);
}

void bsrUTF8DebugBrailleGenerator::print (ostream& os) const
{
  os <<
    asString () <<
    endl;
}

ostream& operator<< (ostream& os, const S_bsrUTF8DebugBrailleGenerator& elt)
{
  elt->print (os);
  return os;
}

//______________________________________________________________________________
S_bsrUTF16BigEndianBrailleGenerator bsrUTF16BigEndianBrailleGenerator::create (
  bsrByteOrderingKind byteOrderingKind,
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
  bsrByteOrderingKind byteOrderingKind,
  ostream&            brailleOutputStream)
    : bsrBrailleGenerator (
        brailleOutputStream)
{
  fByteOrderingKind = byteOrderingKind;

//  writeTestData (brailleOutputStream);
}

bsrUTF16BigEndianBrailleGenerator::~bsrUTF16BigEndianBrailleGenerator ()
{}

void bsrUTF16BigEndianBrailleGenerator::generateCodeForBrailleCell (
  bsrCellKind cellKind)
{
// to be fixed JMI
/*

HTML Entity (Decimal)	&#10241;
HTML Entity (Hexadecimal)	&#x2801;
URL Escape Code	%E2%A0%81
UTF-8 (hex)	0xE2 0xA0 0x81 (e2a081)
UTF-8 (binary)	11100010:10100000:10000001
UTF-16/UTF-16BE (hex)	0x2801 (2801)
UTF-16LE (hex)	0x0128 (0128)
UTF-32/UTF-32bE (hex)	0x00002801 (00002801)
UTF-32lE (hex)	0x01280000 (01280000)
Octal Escape Sequence	\342\240\201

HTML Entity (Decimal)	&#10242;
HTML Entity (Hexadecimal)	&#x2802;
URL Escape Code	%E2%A0%82
UTF-8 (hex)	0xE2 0xA0 0x82 (e2a082)
UTF-8 (binary)	11100010:10100000:10000010
UTF-16/UTF-16BE (hex)	0x2802 (2802)
UTF-16LE (hex)	0x0228 (0228)
UTF-32/UTF-32bE (hex)	0x00002802 (00002802)
UTF-32lE (hex)	0x02280000 (02280000)
Octal Escape Sequence	\342\240\202

HTML Entity (Decimal)	&#10243;
HTML Entity (Hexadecimal)	&#x2803;
URL Escape Code	%E2%A0%83
UTF-8 (hex)	0xE2 0xA0 0x83 (e2a083)
UTF-8 (binary)	11100010:10100000:10000011
UTF-16/UTF-16BE (hex)	0x2803 (2803)
UTF-16LE (hex)	0x0328 (0328)
UTF-32/UTF-32bE (hex)	0x00002803 (00002803)
UTF-32lE (hex)	0x03280000 (03280000)
Octal Escape Sequence	\342\240\203*/

  string stringForCell;

  switch (cellKind) {
    case kCellUnknown:
      {
        stringstream s;

        s <<
          "cannot generate code for braille cell '" <<
          bsrCellKindAsString (cellKind) <<
          "'";
        msrInternalError (
          gGlobalOahOahGroup->getInputSourceName (),
          -999, // TICINO inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;

    case kCellEOL:    stringForCell = "\x0a"; break; // U+000A
    case kCellEOP:    stringForCell = "\x0c"; break; // U+000C

    case kDotsNone:   stringForCell = "\xe2\xa0\x80"; break; // U+2800 ⠀ e2 a0 80 BLANK

    case kDots1:      stringForCell = "\xe2\xa0\x81"; break; // U+2801 ⠀ e2 a0 81 DOTS-1
    case kDots2:      stringForCell = "\xe2\xa0\x82"; break; // U+2802 ⠀ e2 a0 82 DOTS-2
    case kDots12:     stringForCell = "\xe2\xa0\x83"; break; // U+2803 ⠀ e2 a0 83 DOTS-12
    case kDots3:      stringForCell = "\xe2\xa0\x84"; break; // U+2804  ⠄ e2 a0 84 DOTS-3
    case kDots13:     stringForCell = "\xe2\xa0\x85"; break; // U+2805  ⠅ e2 a0 85 DOTS-13
    case kDots23:     stringForCell = "\xe2\xa0\x86"; break; // U+2806  ⠆ e2 a0 86 DOTS-23
    case kDots123:    stringForCell = "\xe2\xa0\x87"; break; // U+2807  ⠇ e2 a0 87 DOTS-123
    case kDots4:      stringForCell = "\xe2\xa0\x88"; break; // U+2808  ⠈ e2 a0 88 DOTS-4
    case kDots14:     stringForCell = "\xe2\xa0\x89"; break; // U+2809  ⠉ e2 a0 89 DOTS-14
    case kDots24:     stringForCell = "\xe2\xa0\x8a"; break; // U+280A  ⠊ e2 a0 8a DOTS-24
    case kDots124:    stringForCell = "\xe2\xa0\x8b"; break; // U+280B  ⠋ e2 a0 8b DOTS-124
    case kDots34:     stringForCell = "\xe2\xa0\x8c"; break; // U+280C  ⠌ e2 a0 8c DOTS-34
    case kDots134:    stringForCell = "\xe2\xa0\x8d"; break; // U+280D  ⠍ e2 a0 8d DOTS-134
    case kDots234:    stringForCell = "\xe2\xa0\x8e"; break; // U+280E  ⠎ e2 a0 8e DOTS-234
    case kDots1234:   stringForCell = "\xe2\xa0\x8f"; break; // U+280F  ⠏ e2 a0 8f DOTS-1234

    case kDots5:      stringForCell = "\xe2\xa0\x90"; break; // U+2810  ⠐ e2 a0 90 DOTS-5
    case kDots15:     stringForCell = "\xe2\xa0\x91"; break; // U+2811  ⠑ e2 a0 91 DOTS-15
    case kDots25:     stringForCell = "\xe2\xa0\x92"; break; // U+2812  ⠒ e2 a0 92 DOTS-25
    case kDots125:    stringForCell = "\xe2\xa0\x93"; break; // U+2813  ⠓ e2 a0 93 DOTS-125
    case kDots35:     stringForCell = "\xe2\xa0\x94"; break; // U+2814  ⠔ e2 a0 94 DOTS-35
    case kDots135:    stringForCell = "\xe2\xa0\x95"; break; // U+2815  ⠕ e2 a0 95 DOTS-135
    case kDots235:    stringForCell = "\xe2\xa0\x96"; break; // U+2816  ⠖ e2 a0 96 DOTS-235
    case kDots1235:   stringForCell = "\xe2\xa0\x97"; break; // U+2817  ⠗ e2 a0 97 DOTS-1235
    case kDots45:     stringForCell = "\xe2\xa0\x98"; break; // U+2818  ⠘ e2 a0 98 DOTS-45
    case kDots145:    stringForCell = "\xe2\xa0\x99"; break; // U+2819  ⠙ e2 a0 99 DOTS-145
    case kDots245:    stringForCell = "\xe2\xa0\x9a"; break; // U+281A  ⠚ e2 a0 9a DOTS-245
    case kDots1245:   stringForCell = "\xe2\xa0\x9b"; break; // U+281B  ⠛ e2 a0 9b DOTS-1245
    case kDots345:    stringForCell = "\xe2\xa0\x9c"; break; // U+281C  ⠜ e2 a0 9c DOTS-345
    case kDots1345:   stringForCell = "\xe2\xa0\x9d"; break; // U+281D  ⠝ e2 a0 9d DOTS-1345
    case kDots2345:   stringForCell = "\xe2\xa0\x9e"; break; // U+281E  ⠞ e2 a0 9e DOTS-2345
    case kDots12345:  stringForCell = "\xe2\xa0\x9f"; break; // U+281F  ⠟ e2 a0 9f DOTS-12345

    case kDots6:      stringForCell = "\xe2\xa0\xa0"; break; // U+2820  ⠠ e2 a0 a0 DOTS-6
    case kDots16:     stringForCell = "\xe2\xa0\xa1"; break; // U+2821  ⠡ e2 a0 a1 DOTS-16
    case kDots26:     stringForCell = "\xe2\xa0\xa2"; break; // U+2822  ⠢ e2 a0 a2 DOTS-26
    case kDots126:    stringForCell = "\xe2\xa0\xa3"; break; // U+2823  ⠣ e2 a0 a3 DOTS-126
    case kDots36:     stringForCell = "\xe2\xa0\xa4"; break; // U+2824  ⠤ e2 a0 a4 DOTS-36
    case kDots136:    stringForCell = "\xe2\xa0\xa5"; break; // U+2825  ⠥ e2 a0 a5 DOTS-136
    case kDots236:    stringForCell = "\xe2\xa0\xa6"; break; // U+2826  ⠦ e2 a0 a6 DOTS-236
    case kDots1236:   stringForCell = "\xe2\xa0\xa7"; break; // U+2827  ⠧ e2 a0 a7 DOTS-1236
    case kDots46:     stringForCell = "\xe2\xa0\xa8"; break; // U+2828  ⠨ e2 a0 a8 DOTS-46
    case kDots146:    stringForCell = "\xe2\xa0\xa9"; break; // U+2829  ⠩ e2 a0 a9 DOTS-146
    case kDots246:    stringForCell = "\xe2\xa0\xaa"; break; // U+282A  ⠪ e2 a0 aa DOTS-246
    case kDots1246:   stringForCell = "\xe2\xa0\xab"; break; // U+282b  ⠫ e2 a0 ab DOTS-1246
    case kDots346:    stringForCell = "\xe2\xa0\xac"; break; // U+282C  ⠬ e2 a0 ac DOTS-346
    case kDots1346:   stringForCell = "\xe2\xa0\xad"; break; // U+282D  ⠭ e2 a0 ad DOTS-1346
    case kDots2346:   stringForCell = "\xe2\xa0\xae"; break; // U+282E  ⠮ e2 a0 ae DOTS-2346
    case kDots12346:  stringForCell = "\xe2\xa0\xaf"; break; // U+282F  ⠯ e2 a0 af DOTS-12346

    case kDots56:     stringForCell = "\xe2\xa0\xb0"; break; // U+2830  ⠰ e2 a0 b0 DOTS-56
    case kDots156:    stringForCell = "\xe2\xa0\xb1"; break; // U+2831  ⠱ e2 a0 b1 DOTS-156
    case kDots256:    stringForCell = "\xe2\xa0\xb2"; break; // U+2832  ⠲ e2 a0 b2 DOTS-256
    case kDots1256:   stringForCell = "\xe2\xa0\xb3"; break; // U+2833  ⠳ e2 a0 b3 DOTS-1256
    case kDots356:    stringForCell = "\xe2\xa0\xb4"; break; // U+2834  ⠴ e2 a0 b4 DOTS-356
    case kDots1356:   stringForCell = "\xe2\xa0\xb5"; break; // U+2835  ⠵ e2 a0 b5 DOTS-1356
    case kDots2356:   stringForCell = "\xe2\xa0\xb6"; break; // U+2836  ⠶ e2 a0 b6 DOTS-2356
    case kDots12356:  stringForCell = "\xe2\xa0\xb7"; break; // U+2837  ⠷ e2 a0 b7 DOTS-12356
    case kDots456:    stringForCell = "\xe2\xa0\xb8"; break; // U+2838  ⠸ e2 a0 b8 DOTS-456
    case kDots1456:   stringForCell = "\xe2\xa0\xb9"; break; // U+2839  ⠹ e2 a0 b9 DOTS-1456
    case kDots2456:   stringForCell = "\xe2\xa0\xba"; break; // U+283A  ⠺ e2 a0 ba DOTS-2456
    case kDots12456:  stringForCell = "\xe2\xa0\xbb"; break; // U+283B  ⠻ e2 a0 bb DOTS-12456
    case kDots3456:   stringForCell = "\xe2\xa0\xbc"; break; // U+283C  ⠼ e2 a0 bc DOTS-3456
    case kDots13456:  stringForCell = "\xe2\xa0\xbd"; break; // U+283D  ⠽ e2 a0 bd DOTS-13456
    case kDots23456:  stringForCell = "\xe2\xa0\xbe"; break; // U+283E  ⠾ e2 a0 be DOTS-23456
    case kDots123456: stringForCell = "\xe2\xa0\xbf"; break; // U+283F ⠿ e2 a0 bf DOTS-123456
  } // switch

  fBrailleOutputStream <<
    stringForCell;
}

void bsrUTF16BigEndianBrailleGenerator::writeTestData (ostream& os)
{
  gLogStream <<
    "bsrUTF16BigEndianBrailleGenerator::writeTestData()" <<
    endl;

  // generate the table of all Dots 6 cells
  for (wchar_t wch = L'\u2800'; wch <= L'\u280f'; ++wch) {
    os << wch;
  } // for
  os << kCellEOL;
  for (wchar_t wch = L'\u2810'; wch <= L'\u281f'; ++wch) {
    os << wch;
  } // for
  os << kCellEOL;
  os << kCellEOP;

  for (wchar_t wch = L'\u2820'; wch <= L'\u282f'; ++wch) {
    os << wch;
  } // for
  os << kCellEOL;
  for (wchar_t wch = L'\u2830'; wch <= L'\u283f'; ++wch) {
    os << wch;
  } // for
  os << kCellEOL;
  os << kCellEOP;
  os << endl;
}

string bsrUTF16BigEndianBrailleGenerator::asString () const
{
  stringstream s;

  s <<
    "UTF16BigEndianBrailleGenerator" <<
    ", byteOrderingKind: " <<
    bsrByteOrderingKindAsString (
      fByteOrderingKind);

  return s.str ();
}

void bsrUTF16BigEndianBrailleGenerator::print (ostream& os) const
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
  bsrByteOrderingKind byteOrderingKind,
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
  bsrByteOrderingKind byteOrderingKind,
  ostream&            brailleOutputStream)
    : bsrBrailleGenerator (
        brailleOutputStream)
{
  fByteOrderingKind = byteOrderingKind;

//  writeTestData (brailleOutputStream);
}

bsrUTF16SmallEndianBrailleGenerator::~bsrUTF16SmallEndianBrailleGenerator ()
{}

void bsrUTF16SmallEndianBrailleGenerator::generateCodeForBrailleCell (
  bsrCellKind cellKind)
{

// to be fixed JMI
/*

HTML Entity (Decimal)	&#10241;
HTML Entity (Hexadecimal)	&#x2801;
URL Escape Code	%E2%A0%81
UTF-8 (hex)	0xE2 0xA0 0x81 (e2a081)
UTF-8 (binary)	11100010:10100000:10000001
UTF-16/UTF-16BE (hex)	0x2801 (2801)
UTF-16LE (hex)	0x0128 (0128)
UTF-32/UTF-32bE (hex)	0x00002801 (00002801)
UTF-32lE (hex)	0x01280000 (01280000)
Octal Escape Sequence	\342\240\201

HTML Entity (Decimal)	&#10242;
HTML Entity (Hexadecimal)	&#x2802;
URL Escape Code	%E2%A0%82
UTF-8 (hex)	0xE2 0xA0 0x82 (e2a082)
UTF-8 (binary)	11100010:10100000:10000010
UTF-16/UTF-16BE (hex)	0x2802 (2802)
UTF-16LE (hex)	0x0228 (0228)
UTF-32/UTF-32bE (hex)	0x00002802 (00002802)
UTF-32lE (hex)	0x02280000 (02280000)
Octal Escape Sequence	\342\240\202

HTML Entity (Decimal)	&#10243;
HTML Entity (Hexadecimal)	&#x2803;
URL Escape Code	%E2%A0%83
UTF-8 (hex)	0xE2 0xA0 0x83 (e2a083)
UTF-8 (binary)	11100010:10100000:10000011
UTF-16/UTF-16BE (hex)	0x2803 (2803)
UTF-16LE (hex)	0x0328 (0328)
UTF-32/UTF-32bE (hex)	0x00002803 (00002803)
UTF-32lE (hex)	0x03280000 (03280000)
Octal Escape Sequence	\342\240\203
*/

  string stringForCell;

  switch (cellKind) {
    case kCellUnknown:
      {
        stringstream s;

        s <<
          "cannot generate code for braille cell '" <<
          bsrCellKindAsString (cellKind) <<
          "'";
        msrInternalError (
          gGlobalOahOahGroup->getInputSourceName (),
          -999, // TICINO inputLineNumber,
          __FILE__, __LINE__,
          s.str ());
      }
      break;

    case kCellEOL:    stringForCell = "\x0a"; break; // U+000A
    case kCellEOP:    stringForCell = "\x0c"; break; // U+000C

    case kDotsNone:   stringForCell = "\xe2\xa0\x80"; break; // U+2800 ⠀ e2 a0 80 BLANK

    case kDots1:      stringForCell = "\xe2\xa0\x81"; break; // U+2801 ⠀ e2 a0 81 DOTS-1
    case kDots2:      stringForCell = "\xe2\xa0\x82"; break; // U+2802 ⠀ e2 a0 82 DOTS-2
    case kDots12:     stringForCell = "\xe2\xa0\x83"; break; // U+2803 ⠀ e2 a0 83 DOTS-12
    case kDots3:      stringForCell = "\xe2\xa0\x84"; break; // U+2804  ⠄ e2 a0 84 DOTS-3
    case kDots13:     stringForCell = "\xe2\xa0\x85"; break; // U+2805  ⠅ e2 a0 85 DOTS-13
    case kDots23:     stringForCell = "\xe2\xa0\x86"; break; // U+2806  ⠆ e2 a0 86 DOTS-23
    case kDots123:    stringForCell = "\xe2\xa0\x87"; break; // U+2807  ⠇ e2 a0 87 DOTS-123
    case kDots4:      stringForCell = "\xe2\xa0\x88"; break; // U+2808  ⠈ e2 a0 88 DOTS-4
    case kDots14:     stringForCell = "\xe2\xa0\x89"; break; // U+2809  ⠉ e2 a0 89 DOTS-14
    case kDots24:     stringForCell = "\xe2\xa0\x8a"; break; // U+280A  ⠊ e2 a0 8a DOTS-24
    case kDots124:    stringForCell = "\xe2\xa0\x8b"; break; // U+280B  ⠋ e2 a0 8b DOTS-124
    case kDots34:     stringForCell = "\xe2\xa0\x8c"; break; // U+280C  ⠌ e2 a0 8c DOTS-34
    case kDots134:    stringForCell = "\xe2\xa0\x8d"; break; // U+280D  ⠍ e2 a0 8d DOTS-134
    case kDots234:    stringForCell = "\xe2\xa0\x8e"; break; // U+280E  ⠎ e2 a0 8e DOTS-234
    case kDots1234:   stringForCell = "\xe2\xa0\x8f"; break; // U+280F  ⠏ e2 a0 8f DOTS-1234

    case kDots5:      stringForCell = "\xe2\xa0\x90"; break; // U+2810  ⠐ e2 a0 90 DOTS-5
    case kDots15:     stringForCell = "\xe2\xa0\x91"; break; // U+2811  ⠑ e2 a0 91 DOTS-15
    case kDots25:     stringForCell = "\xe2\xa0\x92"; break; // U+2812  ⠒ e2 a0 92 DOTS-25
    case kDots125:    stringForCell = "\xe2\xa0\x93"; break; // U+2813  ⠓ e2 a0 93 DOTS-125
    case kDots35:     stringForCell = "\xe2\xa0\x94"; break; // U+2814  ⠔ e2 a0 94 DOTS-35
    case kDots135:    stringForCell = "\xe2\xa0\x95"; break; // U+2815  ⠕ e2 a0 95 DOTS-135
    case kDots235:    stringForCell = "\xe2\xa0\x96"; break; // U+2816  ⠖ e2 a0 96 DOTS-235
    case kDots1235:   stringForCell = "\xe2\xa0\x97"; break; // U+2817  ⠗ e2 a0 97 DOTS-1235
    case kDots45:     stringForCell = "\xe2\xa0\x98"; break; // U+2818  ⠘ e2 a0 98 DOTS-45
    case kDots145:    stringForCell = "\xe2\xa0\x99"; break; // U+2819  ⠙ e2 a0 99 DOTS-145
    case kDots245:    stringForCell = "\xe2\xa0\x9a"; break; // U+281A  ⠚ e2 a0 9a DOTS-245
    case kDots1245:   stringForCell = "\xe2\xa0\x9b"; break; // U+281B  ⠛ e2 a0 9b DOTS-1245
    case kDots345:    stringForCell = "\xe2\xa0\x9c"; break; // U+281C  ⠜ e2 a0 9c DOTS-345
    case kDots1345:   stringForCell = "\xe2\xa0\x9d"; break; // U+281D  ⠝ e2 a0 9d DOTS-1345
    case kDots2345:   stringForCell = "\xe2\xa0\x9e"; break; // U+281E  ⠞ e2 a0 9e DOTS-2345
    case kDots12345:  stringForCell = "\xe2\xa0\x9f"; break; // U+281F  ⠟ e2 a0 9f DOTS-12345

    case kDots6:      stringForCell = "\xe2\xa0\xa0"; break; // U+2820  ⠠ e2 a0 a0 DOTS-6
    case kDots16:     stringForCell = "\xe2\xa0\xa1"; break; // U+2821  ⠡ e2 a0 a1 DOTS-16
    case kDots26:     stringForCell = "\xe2\xa0\xa2"; break; // U+2822  ⠢ e2 a0 a2 DOTS-26
    case kDots126:    stringForCell = "\xe2\xa0\xa3"; break; // U+2823  ⠣ e2 a0 a3 DOTS-126
    case kDots36:     stringForCell = "\xe2\xa0\xa4"; break; // U+2824  ⠤ e2 a0 a4 DOTS-36
    case kDots136:    stringForCell = "\xe2\xa0\xa5"; break; // U+2825  ⠥ e2 a0 a5 DOTS-136
    case kDots236:    stringForCell = "\xe2\xa0\xa6"; break; // U+2826  ⠦ e2 a0 a6 DOTS-236
    case kDots1236:   stringForCell = "\xe2\xa0\xa7"; break; // U+2827  ⠧ e2 a0 a7 DOTS-1236
    case kDots46:     stringForCell = "\xe2\xa0\xa8"; break; // U+2828  ⠨ e2 a0 a8 DOTS-46
    case kDots146:    stringForCell = "\xe2\xa0\xa9"; break; // U+2829  ⠩ e2 a0 a9 DOTS-146
    case kDots246:    stringForCell = "\xe2\xa0\xaa"; break; // U+282A  ⠪ e2 a0 aa DOTS-246
    case kDots1246:   stringForCell = "\xe2\xa0\xab"; break; // U+282b  ⠫ e2 a0 ab DOTS-1246
    case kDots346:    stringForCell = "\xe2\xa0\xac"; break; // U+282C  ⠬ e2 a0 ac DOTS-346
    case kDots1346:   stringForCell = "\xe2\xa0\xad"; break; // U+282D  ⠭ e2 a0 ad DOTS-1346
    case kDots2346:   stringForCell = "\xe2\xa0\xae"; break; // U+282E  ⠮ e2 a0 ae DOTS-2346
    case kDots12346:  stringForCell = "\xe2\xa0\xaf"; break; // U+282F  ⠯ e2 a0 af DOTS-12346

    case kDots56:     stringForCell = "\xe2\xa0\xb0"; break; // U+2830  ⠰ e2 a0 b0 DOTS-56
    case kDots156:    stringForCell = "\xe2\xa0\xb1"; break; // U+2831  ⠱ e2 a0 b1 DOTS-156
    case kDots256:    stringForCell = "\xe2\xa0\xb2"; break; // U+2832  ⠲ e2 a0 b2 DOTS-256
    case kDots1256:   stringForCell = "\xe2\xa0\xb3"; break; // U+2833  ⠳ e2 a0 b3 DOTS-1256
    case kDots356:    stringForCell = "\xe2\xa0\xb4"; break; // U+2834  ⠴ e2 a0 b4 DOTS-356
    case kDots1356:   stringForCell = "\xe2\xa0\xb5"; break; // U+2835  ⠵ e2 a0 b5 DOTS-1356
    case kDots2356:   stringForCell = "\xe2\xa0\xb6"; break; // U+2836  ⠶ e2 a0 b6 DOTS-2356
    case kDots12356:  stringForCell = "\xe2\xa0\xb7"; break; // U+2837  ⠷ e2 a0 b7 DOTS-12356
    case kDots456:    stringForCell = "\xe2\xa0\xb8"; break; // U+2838  ⠸ e2 a0 b8 DOTS-456
    case kDots1456:   stringForCell = "\xe2\xa0\xb9"; break; // U+2839  ⠹ e2 a0 b9 DOTS-1456
    case kDots2456:   stringForCell = "\xe2\xa0\xba"; break; // U+283A  ⠺ e2 a0 ba DOTS-2456
    case kDots12456:  stringForCell = "\xe2\xa0\xbb"; break; // U+283B  ⠻ e2 a0 bb DOTS-12456
    case kDots3456:   stringForCell = "\xe2\xa0\xbc"; break; // U+283C  ⠼ e2 a0 bc DOTS-3456
    case kDots13456:  stringForCell = "\xe2\xa0\xbd"; break; // U+283D  ⠽ e2 a0 bd DOTS-13456
    case kDots23456:  stringForCell = "\xe2\xa0\xbe"; break; // U+283E  ⠾ e2 a0 be DOTS-23456
    case kDots123456: stringForCell = "\xe2\xa0\xbf"; break; // U+283F ⠿ e2 a0 bf DOTS-123456
  } // switch

  fBrailleOutputStream <<
    stringForCell;
}

void bsrUTF16SmallEndianBrailleGenerator::writeTestData (ostream& os)
{
  gLogStream <<
    "bsrUTF16SmallEndianBrailleGenerator::writeTestData()" <<
    endl;

  // generate the table of all Dots 6 cells
  for (wchar_t wch = L'\u2800'; wch <= L'\u280f'; ++wch) {
    os << wch;
  } // for
  os << kCellEOL;
  for (wchar_t wch = L'\u2810'; wch <= L'\u281f'; ++wch) {
    os << wch;
  } // for
  os << kCellEOL;
  os << kCellEOP;

  for (wchar_t wch = L'\u2820'; wch <= L'\u282f'; ++wch) {
    os << wch;
  } // for
  os << kCellEOL;
  for (wchar_t wch = L'\u2830'; wch <= L'\u283f'; ++wch) {
    os << wch;
  } // for
  os << kCellEOL;
  os << kCellEOP;
  os << endl;
}

string bsrUTF16SmallEndianBrailleGenerator::asString () const
{
  stringstream s;

  s <<
    "UTF16SmallEndianBrailleGenerator" <<
    ", byteOrderingKind: " <<
    bsrByteOrderingKindAsString (
      fByteOrderingKind);

  return s.str ();
}

void bsrUTF16SmallEndianBrailleGenerator::print (ostream& os) const
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
