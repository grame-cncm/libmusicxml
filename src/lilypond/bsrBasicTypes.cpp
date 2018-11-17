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

#include <climits>      // INT_MIN, INT_MAX

#include "utilities.h"

#include "messagesHandling.h"
#include "generalOptions.h"

#include "xml2brlOptionsHandling.h"

#include "bsrBasicTypes.h"


using namespace std;

namespace MusicXML2 {

#define TRACE_BSR_BASIC_TYPES 0

//______________________________________________________________________________
// UTF-16 Braille Dots 6 cells

string brailleDots6CellAsShortString (wchar_t wch)
{
  string result;

  switch (wch) {
    case kBOMBigEndian   : result = ".BOMBE"; break;
    case kBOMSmallEndian : result = ".BOMSE"; break;
  
    case kBrailleEOL : result = ".BEOL"; break;
    case kBrailleEOP : result = ".BEOP"; break;

    case kDotsNone    : result = ".None"; break;
    case kDots1       : result = ".1"; break;
    case kDots2       : result = ".2"; break;
    case kDots12      : result = ".12"; break;
    case kDots3       : result = ".3"; break;
    case kDots13      : result = ".13"; break;
    case kDots23      : result = ".23"; break;
    case kDots123     : result = ".123"; break;
    case kDots4       : result = ".4"; break;
    case kDots14      : result = ".14"; break;
    case kDots24      : result = ".24"; break;
    case kDots124     : result = ".124"; break;
    case kDots34      : result = ".34"; break;
    case kDots134     : result = ".134"; break;
    case kDots234     : result = ".234"; break;
    case kDots1234    : result = ".1234"; break;

    case kDots5       : result = ".5"; break;
    case kDots15      : result = ".15"; break;
    case kDots25      : result = ".25"; break;
    case kDots125     : result = ".125"; break;
    case kDots35      : result = ".35"; break;
    case kDots135     : result = ".135"; break;
    case kDots235     : result = ".235"; break;
    case kDots1235    : result = ".1235"; break;
    case kDots45      : result = ".45"; break;
    case kDots145     : result = ".145"; break;
    case kDots245     : result = ".245"; break;
    case kDots1245    : result = ".1245"; break;
    case kDots345     : result = ".345"; break;
    case kDots1345    : result = ".1345"; break;
    case kDots12345   : result = ".12345"; break;

    case kDots6       : result = ".6"; break;
    case kDots16      : result = ".16"; break;
    case kDots26      : result = ".26"; break;
    case kDots126     : result = ".126"; break;
    case kDots36      : result = ".36"; break;
    case kDots136     : result = ".136"; break;
    case kDots236     : result = ".236"; break;
    case kDots1236    : result = ".1236"; break;
    case kDots46      : result = ".46"; break;
    case kDots146     : result = ".146"; break;
    case kDots246     : result = ".246"; break;
    case kDots1246    : result = ".1246"; break;
    case kDots346     : result = ".346"; break;
    case kDots1346    : result = ".1346"; break;
    case kDots2346    : result = ".2346"; break;
    case kDots12346   : result = ".12346"; break;

    case kDots56      : result = ".56"; break;
    case kDots156     : result = ".156"; break;
    case kDots256     : result = ".256"; break;
    case kDots1256    : result = ".1256"; break;
    case kDots356     : result = ".356"; break;
    case kDots1356    : result = ".1356"; break;
    case kDots2356    : result = ".2356"; break;
    case kDots12356   : result = ".12356"; break;
    case kDots456     : result = ".456"; break;
    case kDots1456    : result = ".1456"; break;
    case kDots2456    : result = ".2456"; break;
    case kDots12456   : result = ".12456"; break;
    case kDots3456    : result = ".3456"; break;
    case kDots13456   : result = ".13456"; break;
    case kDots23456   : result = ".23456"; break;
    case kDots123456  : result = ".123456"; break;

    default: result = ".Dot6???";
  } // switch
  
  return result;
}

string brailleDots6CellAsString (wchar_t wch)
{
  string result;

  switch (wch) {
    case kBOMBigEndian   : result = "BOMBigEndian"; break;
    case kBOMSmallEndian : result = "BOMSmallEndian"; break;
  
    case kBrailleEOL : result = "BrailleEOL"; break;
    case kBrailleEOP : result = "BrailleEOP"; break;

    case kDotsNone    : result = "DotsNone"; break;
    case kDots1       : result = "Dots1"; break;
    case kDots2       : result = "Dots2"; break;
    case kDots12      : result = "Dots12"; break;
    case kDots3       : result = "Dots3"; break;
    case kDots13      : result = "Dots13"; break;
    case kDots23      : result = "Dots23"; break;
    case kDots123     : result = "Dots123"; break;
    case kDots4       : result = "Dots4"; break;
    case kDots14      : result = "Dots14"; break;
    case kDots24      : result = "Dots24"; break;
    case kDots124     : result = "Dots124"; break;
    case kDots34      : result = "Dots34"; break;
    case kDots134     : result = "Dots134"; break;
    case kDots234     : result = "Dots234"; break;
    case kDots1234    : result = "Dots1234"; break;

    case kDots5       : result = "Dots5"; break;
    case kDots15      : result = "Dots15"; break;
    case kDots25      : result = "Dots25"; break;
    case kDots125     : result = "Dots125"; break;
    case kDots35      : result = "Dots35"; break;
    case kDots135     : result = "Dots135"; break;
    case kDots235     : result = "Dots235"; break;
    case kDots1235    : result = "Dots1235"; break;
    case kDots45      : result = "Dots45"; break;
    case kDots145     : result = "Dots145"; break;
    case kDots245     : result = "Dots245"; break;
    case kDots1245    : result = "Dots1245"; break;
    case kDots345     : result = "Dots345"; break;
    case kDots1345    : result = "Dots1345"; break;
    case kDots12345   : result = "Dots12345"; break;

    case kDots6       : result = "Dots6"; break;
    case kDots16      : result = "Dots16"; break;
    case kDots26      : result = "Dots26"; break;
    case kDots126     : result = "Dots126"; break;
    case kDots36      : result = "Dots36"; break;
    case kDots136     : result = "Dots136"; break;
    case kDots236     : result = "Dots236"; break;
    case kDots1236    : result = "Dots1236"; break;
    case kDots46      : result = "Dots46"; break;
    case kDots146     : result = "Dots146"; break;
    case kDots246     : result = "Dots246"; break;
    case kDots1246    : result = "Dots1246"; break;
    case kDots346     : result = "Dots346"; break;
    case kDots1346    : result = "Dots1346"; break;
    case kDots2346    : result = "Dots2346"; break;
    case kDots12346   : result = "Dots12346"; break;

    case kDots56      : result = "Dots56"; break;
    case kDots156     : result = "Dots156"; break;
    case kDots256     : result = "Dots256"; break;
    case kDots1256    : result = "Dots1256"; break;
    case kDots356     : result = "Dots356"; break;
    case kDots1356    : result = "Dots1356"; break;
    case kDots2356    : result = "Dots2356"; break;
    case kDots12356   : result = "Dots12356"; break;
    case kDots456     : result = "Dots456"; break;
    case kDots1456    : result = "Dots1456"; break;
    case kDots2456    : result = "Dots2456"; break;
    case kDots12456   : result = "Dots12456"; break;
    case kDots3456    : result = "Dots3456"; break;
    case kDots13456   : result = "Dots13456"; break;
    case kDots23456   : result = "Dots23456"; break;
    case kDots123456  : result = "Dots123456"; break;

    default: result = "Dot6???";
  } // switch

  /* JMI KEEP JUST IN CASE
  switch (wch) {
    case kBOMBigEndian   = L'\ufeff'; break;
    case kBOMSmallEndian = L'\ufeff'; break;
  
    case kBrailleEOL = L'\u000a'; break;
    case kBrailleEOP = L'\u000c'; break;

    case kDotsNone    = L'\u2800'; break;
    case kDots1       = L'\u2801'; break;
    case kDots2       = L'\u2802'; break;
    case kDots12      = L'\u2803'; break;
    case kDots3       = L'\u2804'; break;
    case kDots13      = L'\u2805'; break;
    case kDots23      = L'\u2806'; break;
    case kDots123     = L'\u2807'; break;
    case kDots4       = L'\u2808'; break;
    case kDots14      = L'\u2809'; break;
    case kDots24      = L'\u280a'; break;
    case kDots124     = L'\u280b'; break;
    case kDots34      = L'\u280c'; break;
    case kDots134     = L'\u280d'; break;
    case kDots234     = L'\u280e'; break;
    case kDots1234    = L'\u280f'; break;

    case kDots5       = L'\u2810'; break;
    case kDots15      = L'\u2811'; break;
    case kDots25      = L'\u2812'; break;
    case kDots125     = L'\u2813'; break;
    case kDots35      = L'\u2814'; break;
    case kDots135     = L'\u2815'; break;
    case kDots235     = L'\u2816'; break;
    case kDots1235    = L'\u2817'; break;
    case kDots45      = L'\u2818'; break;
    case kDots145     = L'\u2819'; break;
    case kDots245     = L'\u281a'; break;
    case kDots1245    = L'\u281b'; break;
    case kDots345     = L'\u281c'; break;
    case kDots1345    = L'\u281d'; break;
    case kDots2345    = L'\u281e'; break;
    case kDots12345   = L'\u281f'; break;

    case kDots6       = L'\u2820'; break;
    case kDots16      = L'\u2821'; break;
    case kDots26      = L'\u2822'; break;
    case kDots126     = L'\u2823'; break;
    case kDots36      = L'\u2824'; break;
    case kDots136     = L'\u2825'; break;
    case kDots236     = L'\u2826'; break;
    case kDots1236    = L'\u2827'; break;
    case kDots46      = L'\u2828'; break;
    case kDots146     = L'\u2829'; break;
    case kDots246     = L'\u282a'; break;
    case kDots1246    = L'\u282b'; break;
    case kDots346     = L'\u282c'; break;
    case kDots1346    = L'\u282d'; break;
    case kDots2346    = L'\u282e'; break;
    case kDots12346   = L'\u282f'; break;

    case kDots56      = L'\u2830'; break;
    case kDots156     = L'\u2831'; break;
    case kDots256     = L'\u2832'; break;
    case kDots1256    = L'\u2833'; break;
    case kDots356     = L'\u2834'; break;
    case kDots1356    = L'\u2835'; break;
    case kDots2356    = L'\u2836'; break;
    case kDots12356   = L'\u2837'; break;
    case kDots456     = L'\u2838'; break;
    case kDots1456    = L'\u2839'; break;
    case kDots2456    = L'\u283a'; break;
    case kDots12456   = L'\u283b'; break;
    case kDots3456    = L'\u283c'; break;
    case kDots13456   = L'\u283d'; break;
    case kDots23456   = L'\u283e'; break;
    case kDots123456  = L'\u283f'; break;

    default: result = "Dot6???";
  } // switch
  */
  
  return result;
}

//______________________________________________________________________________
// brailling numbers
wstring braille (int n)
{
  wstringstream ws;
  
  
  if (n < 0) {
    ws << kBrlQuestionMark; // JMI which minus?
    n = -n;
  }

  if (n <= 9) {
    switch (n) {
      case 0: ws << kBrl0; break;
      case 1: ws << kBrl1; break;
      case 2: ws << kBrl2; break;
      case 3: ws << kBrl3; break;
      case 4: ws << kBrl4; break;
      case 5: ws << kBrl5; break;
      case 6: ws << kBrl6; break;
      case 7: ws << kBrl7; break;
      case 8: ws << kBrl8; break;
      case 9: ws << kBrl9; break;
    } // switch
  }
  
  else {
    ws <<
      braille (n / 10) <<
      braille (n % 10);
  }

  return ws.str ();
}

//______________________________________________________________________________
// brailling characters and strings
wchar_t braille (char ch)
{
  wchar_t result = kBrlSpace;
  
  switch (ch) {
    case 'a':
      result = kBrlA;
      break;
    case 'b':
      result = kBrlB;
      break;
    case 'c':
      result = kBrlC;
      break;
    case 'd':
      result = kBrlD;
      break;
    case 'e':
      result = kBrlE;
      break;
    case 'f':
      result = kBrlF;
      break;
    case 'g':
      result = kBrlG;
      break;
    case 'h':
      result = kBrlH;
      break;
    case 'i':
      result = kBrlI;
      break;
    case 'j':
      result = kBrlJ;
      break;
    case 'k':
      result = kBrlK;
      break;
    case 'l':
      result = kBrlL;
      break;
    case 'm':
      result = kBrlM;
      break;
    case 'n':
      result = kBrlN;
      break;
    case 'o':
      result = kBrlO;
      break;
    case 'p':
      result = kBrlP;
      break;
    case 'q':
      result = kBrlQ;
      break;
    case 'r':
      result = kBrlR;
      break;
    case 's':
      result = kBrlS;
      break;
    case 't':
      result = kBrlT;
      break;
    case 'u':
      result = kBrlU;
      break;
    case 'v':
      result = kBrlV;
      break;
    case 'w':
      result = kBrlW;
      break;
    case 'x':
      result = kBrlX;
      break;
    case 'y':
      result = kBrlY;
      break;
    case 'z':
      result = kBrlZ;
      break;
    default:
      result = kBrlQuestionMark;
  } // switch

  return result;
}

wstring braille (string str)
{
  wstringstream ws;
  
  if (str.size ()) {  
    string::const_iterator
      iBegin = str.begin (),
      iEnd   = str.end (),
      i      = iBegin;
    for ( ; ; ) {
      char ch = (*i);

      ws << braille (ch);
      if (++i == iEnd) break;
    } // for
  }

  return ws.str ();
}

//______________________________________________________________________________
// writing UTF-16 to ostreams
void write_wchar_t (ostream& os, wchar_t wch)
{
  union Conversion {
    wchar_t       wchar;
    unsigned char chars [2];
  } conversion;

  conversion.wchar = wch;

  // write in reverse order!
  os << conversion.chars [0] << conversion.chars [1];
}

void write_wchar_t ( wchar_t wch )
{
  union Conversion {
    wchar_t       wchar;
    unsigned char chars [2];
  } conversion;

  conversion.wchar = wch;
     
  // write in reverse order!
  cout << conversion.chars [0] << conversion.chars [1];
}

ostream& operator<< (ostream& os, const wchar_t wch)
{
  union Conversion {
    wchar_t       wchar;
    unsigned char chars [2];
  } conversion;

  conversion.wchar = wch;

  // write in reverse order!
  os << conversion.chars [0] << conversion.chars [1];

  return os;
}

void write_wstring (ostream& os, wstring wstr )
{
  for (unsigned int i = 0; i < wstr.size (); i++) {
    wchar_t wch = wstr [i];
    write_wchar_t (os, wch);
  } // for
}

ostream& operator<< (ostream& os, const wstring& wstr)
{
  for (unsigned int i = 0; i < wstr.size (); i++) {
    wchar_t wch = wstr [i];
    
    union Conversion {
      wchar_t       wchar;
      unsigned char chars [2];
    } conversion;
  
    conversion.wchar = wch;
  
    // write in reverse order!
    os << conversion.chars [0] << conversion.chars [1];
    } // for

  return os;
}



//______________________________________________________________________________
void initializeBSRBasicTypes ()
{}


}
