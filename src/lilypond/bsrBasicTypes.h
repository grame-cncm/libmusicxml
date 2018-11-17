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
// UTF-16 Braille Dots 6 cells
const wchar_t
  kBOMBigEndian   = L'\ufeff',
  kBOMSmallEndian = L'\ufffe',
  
  kBrailleEOL = L'\u000a',
  kBrailleEOP = L'\u000c',

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

string brailleDots6CellAsShortString (wchar_t wch);

string brailleDots6CellAsString (wchar_t wch);

//______________________________________________________________________________
// lower case letters
const wchar_t
  kBrlA = kDots1,
  kBrlB = kDots12,
  kBrlC = kDots14,
  kBrlD = kDots145,
  kBrlE = kDots15,
  kBrlF = kDots124,
  kBrlG = kDots1245,
  kBrlH = kDots125,
  kBrlI = kDots24,
  kBrlJ = kDots245,
  
  kBrlK = kDots13,
  kBrlL = kDots123,
  kBrlM = kDots134,
  kBrlN = kDots1345,
  kBrlO = kDots135,
  kBrlP = kDots1234,
  kBrlQ = kDots12345,
  kBrlR = kDots1235,
  kBrlS = kDots234,
  kBrlT = kDots2345,

  kBrlU = kDots136,
  kBrlV = kDots1236,
  kBrlW = kDots2456,
  kBrlX = kDots1346,
  kBrlY = kDots13456,
  kBrlZ = kDots1356;

//______________________________________________________________________________
// capitals
const wchar_t
  kBrlCapitalsSign = kDots46;

const wstring
  brailleCapitalsSequenceSign { kBrlCapitalsSign, kBrlCapitalsSign };

//______________________________________________________________________________
// decimal digits
const wchar_t
  kBrlNumberSign = kDots3456,
  kBrl1 = kBrlA,
  kBrl2 = kBrlB,
  kBrl3 = kBrlC,
  kBrl4 = kBrlD,
  kBrl5 = kBrlE,
  kBrl6 = kBrlF,
  kBrl7 = kBrlG,
  kBrl8 = kBrlH,
  kBrl9 = kBrlI,
  kBrl0 = kBrlJ;

//______________________________________________________________________________
// lower decimal digits
const wchar_t
  kBrlLower1 = kDots2,
  kBrlLower2 = kDots23,
  kBrlLower3 = kDots25,
  kBrlLower4 = kDots256,
  kBrlLower5 = kDots26,
  kBrlLower6 = kDots235,
  kBrlLower7 = kDots2356,
  kBrlLower8 = kDots236,
  kBrlLower9 = kDots35,
  kBrlLower0 = kDots356;

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
  kBrlDot              = kDots256,
  kBrlComma            = kDots2,
  kBrlQuestionMark     = kDots26,
  kBrlSemicolon        = kDots23,
  kBrlColon            = kDots25,
  kBrlExclamationMark  = kDots235,
  kBrlLeftParenthesis  = kDots236,
  kBrlRightParenthesis = kDots356,
  kBrlDoubleQuote      = kDots2356,
  kBrlDash             = kDots36,
  kBrlQuote            = kDots3;
  
//______________________________________________________________________________
// parentheses
const wstring
  kBrlLiteraryLeftParenthesis  { kDots5, kDots126 },
  kBrlLiteraryRightParenthesis { kDots5, kDots345 },
  kBrlMusicParentheses         { kDots6, kDots3 },
  kBrlSpecialParentheses       { kDots2356, kDots2356 };

//______________________________________________________________________________
// other symbols
const wchar_t
  kBrlSpace    = kDotsNone,
  kBrlSlash    = kDots34,
  kBrlVerseEnd = kDots345,
  kBrlItalics  = kDots456,
  kBrlAsterisk = kDots35,
  kBrlExponent = kDots4;

const wstring
  kBrlParagraph           { kDots5, kDots1234 },
  kBrlAmpersand           { kDots5, kDots123456 },
  kBrlUpsilon             { kDots45, kDots13456 }, // better name JMI ??? 
  kBrlEuro                { kDots45, kDots15 },
  kBrlDollar              { kDots45, kDots234 },
  kBrlPound               { kDots45, kDots123 },
  kBrlCopyright           { kDots5, kDots14 },
  kBrlRegisteredTradeMark { kDots5, kDots1235 },
  kBrlTradeMark           { kDots5, kDots2345 },
  kBrlPercent             { kDots5, kDots346 },
  kBrlPerthousand         { kDots5, kDots346, kDots346 },
  kBrlPertenthousand      { kDots5, kDots346, kDots346, kDots346 };

//______________________________________________________________________________
// octaves, bottom up
const wchar_t
  kBrlOctave1 = kDots4,
  kBrlOctave2 = kDots45,
  kBrlOctave3 = kDots456,
  kBrlOctave4 = kDots5,
  kBrlOctave5 = kDots46,
  kBrlOctave6 = kDots56,
  kBrlOctave7 = kDots6;

const wstring
  kBrlOctaveBelow1 { kBrlOctave1, kBrlOctave1 },
  kBrlOctaveAbove7 { kBrlOctave7, kBrlOctave7 };

//______________________________________________________________________________
// alterations
const wchar_t
  kBrlFlat    = kDots126,
  kBrlNatural = kDots16,
  kBrlSharp   = kDots136;

//______________________________________________________________________________
// augmentations
const wchar_t
  kBrlAugmentationDot = kDots3;

//______________________________________________________________________________
// notes
const wchar_t
  kBrlCEighth  = kDots145,
  kBrlCQuarter = kDots1456,
  kBrlCHalf    = kDots1345,
  kBrlCWhole   = kDots13456,
  
  kBrlDEighth  = kDots15,
  kBrlDQuarter = kDots156,
  kBrlDHalf    = kDots135,
  kBrlDWhole   = kDots1356,
  
  kBrlEEighth  = kDots125,
  kBrlEQuarter = kDots1256,
  kBrlEHalf    = kDots1234,
  kBrlEWhole   = kDots12346,
  
  kBrlFEighth  = kDots1245,
  kBrlFQuarter = kDots12456,
  kBrlFHalf    = kDots12345,
  kBrlFWhole   = kDots123456,
  
  kBrlGEighth  = kDots125,
  kBrlGQuarter = kDots1256,
  kBrlGHalf    = kDots1235,
  kBrlGWhole   = kDots12356,
  
  kBrlAEighth  = kDots24,
  kBrlAQuarter = kDots246,
  kBrlAHalf    = kDots234,
  kBrlAWhole   = kDots2346,
  
  kBrlBEighth  = kDots245,
  kBrlBQuarter = kDots2456,
  kBrlBHalf    = kDots2345,
  kBrlBWhole   = kDots23456;

//______________________________________________________________________________
// intervals
const wchar_t
  kBrlSecond  = kDots34,
  kBrlThird   = kDots346,
  kBrlFourth  = kDots3456,
  kBrlFifth   = kDots35,
  kBrlSixth   = kDots356,
  kBrlSeventh = kDots25,
  kBrlEighth  = kDots36;

//______________________________________________________________________________
// rests
const wchar_t
  kBrlRest8th     = kDots1346,
  kBrlRest128th   = kDots1346,
  kBrlRestQuarter = kDots1236,
  kBrlRest64th    = kDots1236,
  kBrlRestHalf    = kDots136,
  kBrlRest32th    = kDots136,
  kBrlRestWhole   = kDots135,
  kBrlRest16th    = kDots135;

//______________________________________________________________________________
// triplets
const wchar_t
  kBrlTriplet  = kDots23;

//______________________________________________________________________________
// fermatas
const wstring
  kBrlFermataOnANote      { kDots146, kDots126, kDots123 },
  kBrlFermataBetweenNotes { kDots5,   kDots126, kDots123 },
  kBrlFermataOverABarLine { kDots456, kDots126, kDots123 };

//______________________________________________________________________________
// hyphens
const wchar_t
  kBrlMusicHyphen = kDots5;

//______________________________________________________________________________
// keyboard hands
const wstring
  kBrlRightHand { kDots46,  kDots345 },
  kBrlLeftHand  { kDots456, kDots345 };

//______________________________________________________________________________
// bars
const wstring
  kBrlFinalDoubleBar     { kDots126, kDots13 },
  kBrlSectionalDoubleBar { kDots126, kDots13, kDots3 };

//______________________________________________________________________________
// measure divisions
const wstring
  kBrlMeasureDivisionSign { kDots46, kDots13 };

//______________________________________________________________________________
// words
const wchar_t
  kBrlWordSign       = kDots345,
  kBrlWordApostrophe = kDots6;

//______________________________________________________________________________
// pages
const wstring
  kBrlPagination { kDots5, kDots25 };

//______________________________________________________________________________
// brailling numbers
wstring braille (int n);

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
