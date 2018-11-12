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

#include "xml2lyOptionsHandling.h"

#include "bsrBasicTypes.h"


using namespace std;

namespace MusicXML2 {

#define TRACE_BSR_BASIC_TYPES 0

//______________________________________________________________________________
// brailling characters and strings
wchar_t braille (char ch)
{
  wchar_t result = kbc_space;
  
  switch (ch) {
    case 'a':
      result = kbc_a;
      break;
    case 'b':
      result = kbc_b;
      break;
    case 'c':
      result = kbc_c;
      break;
    case 'd':
      result = kbc_d;
      break;
    case 'e':
      result = kbc_e;
      break;
    case 'f':
      result = kbc_f;
      break;
    case 'g':
      result = kbc_g;
      break;
    case 'h':
      result = kbc_h;
      break;
    case 'i':
      result = kbc_i;
      break;
    case 'j':
      result = kbc_j;
      break;
    case 'k':
      result = kbc_k;
      break;
    case 'l':
      result = kbc_l;
      break;
    case 'm':
      result = kbc_m;
      break;
    case 'n':
      result = kbc_n;
      break;
    case 'o':
      result = kbc_o;
      break;
    case 'p':
      result = kbc_p;
      break;
    case 'q':
      result = kbc_q;
      break;
    case 'r':
      result = kbc_r;
      break;
    case 's':
      result = kbc_s;
      break;
    case 't':
      result = kbc_t;
      break;
    case 'u':
      result = kbc_u;
      break;
    case 'v':
      result = kbc_v;
      break;
    case 'w':
      result = kbc_w;
      break;
    case 'x':
      result = kbc_x;
      break;
    case 'y':
      result = kbc_y;
      break;
    case 'z':
      result = kbc_z;
      break;
    default:
      result = kbc_questionMark;
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
