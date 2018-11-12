/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___bsrBasicTypes___
#define ___bsrBasicTypes___

#include <list>
#include <map>

#include "smartpointer.h"

#include "rational.h"

#include "msrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
// UTF-16 cells
const wchar_t
  kBOM         = L'\ufeff',
  kEOL         = L'\u000a',
  kEOP         = L'\u000c',

  kDotsNone    = L'\u2800',
  kDots1       = L'\u2801',
  kDots2       = L'\u2802',
  kDots12      = L'\u2803',
  kDots3       = L'\u2804',
  kDots13      = L'\u2805',
  kDots23      = L'\u2806',
  kDots123     = L'\u2807',
  kDots4       = L'\u2808',
  kDots14      = L'\u2809',
  kDots24      = L'\u280a',
  kDots124     = L'\u280b',
  kDots34      = L'\u280c',
  kDots134     = L'\u280d',
  kDots234     = L'\u280e',
  kDots1234    = L'\u280f',

  kDots5       = L'\u2810',
  kDots15      = L'\u2811',
  kDots25      = L'\u2812',
  kDots125     = L'\u2813',
  kDots35      = L'\u2814',
  kDots135     = L'\u2815',
  kDots235     = L'\u2816',
  kDots1235    = L'\u2817',
  kDots45      = L'\u2818',
  kDots145     = L'\u2819',
  kDots245     = L'\u281a',
  kDots1245    = L'\u281b',
  kDots345     = L'\u281c',
  kDots1345    = L'\u281d',
  kDots2345    = L'\u281e',
  kDots12345   = L'\u281f',

  kDots6       = L'\u2820',
  kDots16      = L'\u2821',
  kDots26      = L'\u2822',
  kDots126     = L'\u2823',
  kDots36      = L'\u2824',
  kDots136     = L'\u2825',
  kDots236     = L'\u2826',
  kDots1236    = L'\u2827',
  kDots46      = L'\u2828',
  kDots146     = L'\u2829',
  kDots246     = L'\u282a',
  kDots1246    = L'\u282b',
  kDots346     = L'\u282c',
  kDots1346    = L'\u282d',
  kDots2346    = L'\u282e',
  kDots12346   = L'\u282f',

  kDots56      = L'\u2830',
  kDots156     = L'\u2831',
  kDots256     = L'\u2832',
  kDots1256    = L'\u2833',
  kDots356     = L'\u2834',
  kDots1356    = L'\u2835',
  kDots2356    = L'\u2836',
  kDots12356   = L'\u2837',
  kDots456     = L'\u2838',
  kDots1456    = L'\u2839',
  kDots2456    = L'\u283a',
  kDots12456   = L'\u283b',
  kDots3456    = L'\u283c',
  kDots13456   = L'\u283d',
  kDots23456   = L'\u283e',
  kDots123456  = L'\u283f';

//______________________________________________________________________________
// lower case letters
const wchar_t
  kbc_a = kDots1,
  kbc_b = kDots12,
  kbc_c = kDots15,
  kbc_d = kDots156,
  kbc_e = kDots16,
  kbc_f = kDots124,
  kbc_g = kDots1256,
  kbc_h = kDots125,
  kbc_i = kDots24,
  kbc_j = kDots24,
  
  kbc_k = kDots13,
  kbc_l = kDots123,
  kbc_m = kDots134,
  kbc_n = kDots1345,
  kbc_o = kDots135,
  kbc_p = kDots1234,
  kbc_q = kDots12345,
  kbc_r = kDots1235,
  kbc_s = kDots234,
  kbc_t = kDots2345,

  kbc_u = kDots136,
  kbc_v = kDots1236,
  kbc_w = kDots2456,
  kbc_x = kDots1346,
  kbc_y = kDots13456,
  kbc_z = kDots1356;

//______________________________________________________________________________
// capitals
const wchar_t
  kbc_capitalsPrefix = kDots46;

const wstring
  brailleCapitalsSequence { kbc_capitalsPrefix + kbc_capitalsPrefix };

//______________________________________________________________________________
// decimal digits
const wchar_t
  kbc_numericalPrefix = kDots3456,
  kbc_1 = kbc_a,
  kbc_2 = kbc_b,
  kbc_3 = kbc_c,
  kbc_4 = kbc_d,
  kbc_5 = kbc_e,
  kbc_6 = kbc_f,
  kbc_7 = kbc_g,
  kbc_8 = kbc_h,
  kbc_9 = kbc_i,
  kbc_0 = kbc_j;

//______________________________________________________________________________
// arithmetic operators
const wchar_t
  kac_plus      = kDots235,
  kac_minus     = kDots36,
  kac_times     = kDots35,
  kac_dividedBy = kDots25,
  kac_equals    = kDots2356;
  
//______________________________________________________________________________
// punctuation
const wchar_t
  kbc_dot              = kDots256,
  kbc_comma            = kDots2,
  kbc_questionMark     = kDots26,
  kbc_semicolon        = kDots23,
  kbc_colon            = kDots25,
  kbc_exclamationMark  = kDots235,
  kbc_leftParenthesis  = kDots236,
  kbc_rightParenthesis = kDots356,
  kbc_doubleQuote      = kDots2356,
  kbc_dash             = kDots36,
  kbc_quote            = kDots3;
  
//______________________________________________________________________________
// other symbols
const wchar_t
  kbc_space    = kDotsNone,
  kbc_slash    = kDots34,
  kbc_verseEnd = kDots345,
  kbc_italics  = kDots456,
  kbc_asterisk = kDots35,
  kbc_exponent = kDots4;

const wstring
  kbc_paragraph           { kDots5, kDots1234 },
  kbc_ampersand           { kDots5, kDots123456 },
  kbc_upsilon             { kDots45, kDots13456 }, // better name JMI ??? 
  kbc_euro                { kDots45, kDots15 },
  kbc_dollar              { kDots45, kDots234 },
  kbc_pound               { kDots45, kDots123 },
  kbc_copyright           { kDots5, kDots14 },
  kbc_registeredTradeMark { kDots5, kDots1235 },
  kbc_tradeMark           { kDots5, kDots2345 },
  kbc_percent             { kDots5, kDots346 },
  kbc_perthousand         { kDots5, kDots346, kDots346 },
  kbc_pertenthousand      { kDots5, kDots346, kDots346, kDots346 };

//______________________________________________________________________________
// octaves, bottom up
const wchar_t
  kbc_octave1 = kDots4,
  kbc_octave2 = kDots45,
  kbc_octave3 = kDots456,
  kbc_octave4 = kDots5,
  kbc_octave5 = kDots46;

//______________________________________________________________________________
// notes
const wchar_t
  kbc_cEighth  = kDots145,
  kbc_cQuarter = kDots1456,
  kbc_cHalf    = kDots1345,
  kbc_cWhole   = kDots13456,
  
  kbc_dEighth  = kDots15,
  kbc_dQuarter = kDots156,
  kbc_dHalf    = kDots135,
  kbc_dWhole   = kDots1356,
  
  kbc_eEighth  = kDots125,
  kbc_eQuarter = kDots1256,
  kbc_eHalf    = kDots1234,
  kbc_eWhole   = kDots12346,
  
  kbc_fEighth  = kDots1245,
  kbc_fQuarter = kDots12456,
  kbc_fHalf    = kDots12345,
  kbc_fWhole   = kDots123456,
  
  kbc_gEighth  = kDots125,
  kbc_gQuarter = kDots1256,
  kbc_gHalf    = kDots1235,
  kbc_gWhole   = kDots12356,
  
  kbc_aEighth  = kDots24,
  kbc_aQuarter = kDots246,
  kbc_aHalf    = kDots234,
  kbc_aWhole   = kDots2346,
  
  kbc_bEighth  = kDots245,
  kbc_bQuarter = kDots2456,
  kbc_bHalf    = kDots2345,
  kbc_bWhole   = kDots23456;

//______________________________________________________________________________
// pages
const wstring
  kbs_pagination { kDots5 + kDots25 };

//______________________________________________________________________________
// brailling characters and strings
wchar_t braille (char ch);

wstring braille (string str);

//______________________________________________________________________________
// writing UTF-16 to ostreams
void write_wchar_t (ostream& os, wchar_t wch);

void write_wchar_t ( wchar_t wch );

ostream& operator<< (ostream& os, const wchar_t wch);

void write_wstring (ostream& os, wstring wstr );

ostream& operator<< (ostream& os, const wstring& wstr);

//______________________________________________________________________________
void initializeBSRBasicTypes ();


} // namespace MusicXML2


#endif
