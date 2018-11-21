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


//using namespace std;

namespace MusicXML2
{

#define TRACE_BSR_BASIC_TYPES 0

//______________________________________________________________________________
string bsrSixDotsKindAsShortString (bsrSixDotsKind sixDotsKind)
{
  string result;

  switch (sixDotsKind) {
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
    case kDots2345    : result = ".2345"; break;
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
  } // switch
  
  return result;
}

//______________________________________________________________________________

string bsrSixDotsKindAsString (bsrSixDotsKind sixDotsKind)
{
  string result;

  switch (sixDotsKind) {
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

  return result;
}

//______________________________________________________________________________
string bsrCellKindAsShortString (bsrCellKind cellKind)
{
  string result;

  switch (cellKind) {
  // non 6dots values
  case kCellEOL: result = "FOO"; break; // L'\u000a'
  case kCellEOP: result = "FOO"; break; // L'\u000c'

  // lower case letters
  case kCellA: result = "kCellA"; break; // kDots1,
  case kCellB: result = "kCellB"; break; // kDots12,
  case kCellC: result = "kCellC"; break; // kDots14,
  case kCellD: result = "kCellD"; break; // kDots145,
  case kCellE: result = "kCellE"; break; // kDots15,
  case kCellF: result = "kCellF"; break; // kDots124,
  case kCellG: result = "kCellG"; break; // kDots1245,
  case kCellH: result = "kCellH"; break; // kDots125,
  case kCellI: result = "kCellI"; break; // kDots24,
  case kCellJ: result = "kCellJ"; break; // kDots245,
  
  case kCellK: result = "kCellK"; break; // kDots13,
  case kCellL: result = "kCellL"; break; // kDots123,
  case kCellM: result = "kCellM"; break; // kDots134,
  case kCellN: result = "kCellN"; break; // kDots1345,
  case kCellO: result = "kCellO"; break; // kDots135,
  case kCellP: result = "kCellP"; break; // kDots1234,
  case kCellQ: result = "kCellQ"; break; // kDots12345,
  case kCellR: result = "kCellR"; break; // kDots1235,
  case kCellS: result = "kCellS"; break; // kDots234,
  case kCellT: result = "kCellT"; break; // kDots2345,

  case kCellU: result = "kCellU"; break; // kDots136,
  case kCellV: result = "kCellV"; break; // kDots1236,
  case kCellW: result = "kCellW"; break; // kDots2456,
  case kCellX: result = "kCellX"; break; // kDots1346,
  case kCellY: result = "kCellY"; break; // kDots13456,
  case kCellZ: result = "kCellZ"; break; // kDots1356;

 // decimal digits
  case kCellNumberSign: result = "kCellNumberSign"; break; // kDots3456,
  case kCell1: result = "kCell1"; break; // kCellA,
  case kCell2: result = "kCell2"; break; // kCellB,
  case kCell3: result = "kCell3"; break; // kCellC,
  case kCell4: result = "kCell4"; break; // kCellD,
  case kCell5: result = "kCell5"; break; // kCellE,
  case kCell6: result = "kCell6"; break; // kCellF,
  case kCell7: result = "kCell7"; break; // kCellG,
  case kCell8: result = "kCell8"; break; // kCellH,
  case kCell9: result = "kCell9"; break; // kCellI,
  case kCell0: result = "kCell0"; break; // kCellJ,

  // lower decimal digits
  case kCellLower1: result = "kCellLower1"; break; // kDots2,
  case kCellLower2: result = "kCellLower2"; break; // kDots23,
  case kCellLower3: result = "kCellLower3"; break; // kDots25,
  case kCellLower4: result = "kCellLower4"; break; // kDots256,
  case kCellLower5: result = "kCellLower5"; break; // kDots26,
  case kCellLower6: result = "kCellLower6"; break; // kDots235,
  case kCellLower7: result = "kCellLower7"; break; // kDots2356,
  case kCellLower8: result = "kCellLower8"; break; // kDots236,
  case kCellLower9: result = "kCellLower9"; break; // kDots35,
  case kCellLower0: result = "kCellLower0"; break; // kDots356;

  // arithmetic operators
  case kCell_ac_plus     : result = "FOO"; break; // kDots235,
  case kCell_ac_minus    : result = "FOO"; break; // kDots36,
  case kCell_ac_times    : result = "FOO"; break; // kDots35,
  case kCell_ac_dividedBy: result = "FOO"; break; // kDots25,
  case kCell_ac_equals   : result = "FOO"; break; // kDots2356;
  
  // punctuation
  case kCellDot             : result = "FOO"; break; // kDots256,
  case kCellComma           : result = "FOO"; break; // kDots2,
  case kCellQuestionMark    : result = "FOO"; break; // kDots26,
  case kCellSemicolon       : result = "FOO"; break; // kDots23,
  case kCellColon           : result = "FOO"; break; // kDots25,
  case kCellExclamationMark : result = "FOO"; break; // kDots235,
  case kCellLeftParenthesis : result = "FOO"; break; // kDots236,
  case kCellRightParenthesis: result = "FOO"; break; // kDots356,
  case kCellDoubleQuote     : result = "FOO"; break; // kDots2356,
  case kCellDash            : result = "FOO"; break; // kDots36,
  case kCellQuote           : result = "FOO"; break; // kDots3;

  // other symbols
  case kCellSpace   : result = "kCellSpace"; break; // kDotsNone,
  case kCellSlash   : result = "FOO"; break; // kDots34,
  case kCellVerseEnd: result = "FOO"; break; // kDots345,
  case kCellItalics : result = "FOO"; break; // kDots456,
  case kCellAsterisk: result = "FOO"; break; // kDots35,
  case kCellExponent: result = "FOO"; break; // kDots4;

  // octaves, bottom up
  case kCellOctaveBelow1 : result = "kCellOctaveBelow1"; break; //{ kCellOctave1, kCellOctave1 },
  case kCellOctave1: result = "kCellOctave1"; break; // kDots4,
  case kCellOctave2: result = "kCellOctave2"; break; // kDots45,
  case kCellOctave3: result = "kCellOctave3"; break; // kDots456,
  case kCellOctave4: result = "kCellOctave4"; break; // kDots5,
  case kCellOctave5: result = "kCellOctave5"; break; // kDots46,
  case kCellOctave6: result = "kCellOctave6"; break; // kDots56,
  case kCellOctave7: result = "kCellOctave7"; break; // kDots6;
  case kCellOctaveAbove7 : result = "kCellOctaveAbove7"; break; //{ kCellOctave7, kCellOctave7 };
  
  // alterations
  case kCellFlat   : result = "kCellFlat"; break; // kDots126,
  case kCellNatural: result = "kCellNatural"; break; // kDots16,
  case kCellSharp  : result = "kCellSharp"; break; // kDots136;

  // augmentations
  case kCellAugmentationDot: result = "kCellAugmentationDot"; break; // kDots3;

  // notes
  case kCellCEighth : result = "kCellCEighth"; break; // kDots145,
  case kCellCQuarter: result = "kCellCQuarter"; break; // kDots1456,
  case kCellCHalf   : result = "kCellCHalf"; break; // kDots1345,
  case kCellCWhole  : result = "kCellCWhole"; break; // kDots13456,
  
  case kCellDEighth : result = "kCellDEighth"; break; // kDots15,
  case kCellDQuarter: result = "kCellDQuarter"; break; // kDots156,
  case kCellDHalf   : result = "kCellDHalf"; break; // kDots135,
  case kCellDWhole  : result = "kCellDWhole"; break; // kDots1356,
  
  case kCellEEighth : result = "kCellEEighth"; break; // kDots125,
  case kCellEQuarter: result = "kCellEQuarter"; break; // kDots1256,
  case kCellEHalf   : result = "kCellEHalf"; break; // kDots1234,
  case kCellEWhole  : result = "kCellEWhole"; break; // kDots12346,
  
  case kCellFEighth : result = "kCellFEighth"; break; // kDots1245,
  case kCellFQuarter: result = "kCellFQuarter"; break; // kDots12456,
  case kCellFHalf   : result = "FOkCellFHalfO"; break; // kDots12345,
  case kCellFWhole  : result = "kCellFWhole"; break; // kDots123456,
  
  case kCellGEighth : result = "kCellGEighth"; break; // kDots125,
  case kCellGQuarter: result = "kCellGQuarter"; break; // kDots1256,
  case kCellGHalf   : result = "kCellGHalf"; break; // kDots1235,
  case kCellGWhole  : result = "kCellGWhole"; break; // kDots12356,
  
  case kCellAEighth : result = "kCellAEighth"; break; // kDots24,
  case kCellAQuarter: result = "kCellAQuarter"; break; // kDots246,
  case kCellAHalf   : result = "kCellAHalf"; break; // kDots234,
  case kCellAWhole  : result = "kCellAWhole"; break; // kDots2346,
  
  case kCellBEighth : result = "kCellBEighth"; break; // kDots245,
  case kCellBQuarter: result = "kCellBQuarter"; break; // kDots2456,
  case kCellBHalf   : result = "kCellBHalf"; break; // kDots2345,
  case kCellBWhole  : result = "kCellBWhole"; break; // kDots23456;

  // intervals
  case kCellSecond : result = "kCellSecond"; break; // kDots34,
  case kCellThird  : result = "kCellThird"; break; // kDots346,
  case kCellFourth : result = "kCellFourth"; break; // kDots3456,
  case kCellFifth  : result = "kCellFifth"; break; // kDots35,
  case kCellSixth  : result = "kCellSixth"; break; // kDots356,
  case kCellSeventh: result = "kCellSeventh"; break; // kDots25,
  case kCellEighth : result = "kCellEighth"; break; // kDots36;

  // rests
  case kCellRest8th    : result = "FOO"; break; // kDots1346,
  case kCellRest128th  : result = "FOO"; break; // kDots1346,
  case kCellRestQuarter: result = "FOO"; break; // kDots1236,
  case kCellRest64th   : result = "FOO"; break; // kDots1236,
  case kCellRestHalf   : result = "FOO"; break; // kDots136,
  case kCellRest32th   : result = "FOO"; break; // kDots136,
  case kCellRestWhole  : result = "FOO"; break; // kDots135,
  case kCellRest16th   : result = "FOO"; break; // kDots135;

  // triplets
  case kCellTriplet : result = "kCellTriplet"; break; // kDots23;

  // hyphens
  case kCellMusicHyphen: result = "kCellMusicHyphen"; break; // kDots5;

  // keyboard hands
  case kCellRightHand: result = "FOO"; break; // { kDots46,  kDots345 },
  case kCellLeftHand: result = "FOO"; break; //  { kDots456, kDots345 };

  // bars
  case kCellFinalDoubleBar    : result = "kCellFinalDoubleBar"; break; //     { kDots126, kDots13 },
  case kCellSectionalDoubleBar: result = "kCellSectionalDoubleBar"; break; // { kDots126, kDots13, kDots3 };

  // measure divisions
  case kCellMeasureDivisionSign: result = "FOO"; break; // { kDots46, kDots13 };

  // words
  case kCellWordSign      : result = "kCellWordSign"; break; // kDots345,
  case kCellWordApostrophe: result = "kCellWordApostrophe"; break; // kDots6;

  // pages
  case kCellPagination: result = "kCellPagination"; break; // { kDots5, kDots25 };

  // capitals
  case kCellCapitalsSign: result = "FOO"; break; //: result = "FOO"; break; // kDots46;
  case kCellCapitalsSequenceSign: result = "FOO"; break; // { kCellCapitalsSign, kCellCapitalsSign };

  // parentheses
  case kCellLiteraryLeftParenthesis  : result = "kCellLiteraryLeftParenthesis"; break; //{ kDots5, kDots126 },
  case kCellLiteraryRightParenthesis : result = "kCellLiteraryRightParenthesis"; break; //{ kDots5, kDots345 },
  case kCellMusicParentheses         : result = "kCellMusicParentheses"; break; //{ kDots6, kDots3 },
  case kCellSpecialParentheses       : result = "kCellSpecialParentheses"; break; //{ kDots2356, kDots2356 };
 
  case kCellParagraph           : result = "FOO"; break; //{ kDots5, kDots1234 },
  case kCellAmpersand           : result = "FOO"; break; //{ kDots5, kDots123456 },
  case kCellUpsilon             : result = "FOO"; break; //{ kDots45, kDots13456 }: result = "FOO"; break; // better name JMI ??? 
  case kCellEuro                : result = "FOO"; break; //{ kDots45, kDots15 },
  case kCellDollar              : result = "FOO"; break; //{ kDots45, kDots234 },
  case kCellPound               : result = "FOO"; break; //{ kDots45, kDots123 },
  case kCellCopyright           : result = "FOO"; break; //{ kDots5, kDots14 },
  case kCellRegisteredTradeMark : result = "FOO"; break; //{ kDots5, kDots1235 },
  case kCellTradeMark           : result = "FOO"; break; //{ kDots5, kDots2345 },
  case kCellPercent             : result = "FOO"; break; //{ kDots5, kDots346 },
  case kCellPerthousand         : result = "FOO"; break; //{ kDots5, kDots346, kDots346 },
  case kCellPertenthousand      : result = "FOO"; break; //{ kDots5, kDots346, kDots346, kDots346 };

  // fermatas
  case kCellFermataOnANote      : result = "FOO"; break; //{ kDots146, kDots126, kDots123 },
  case kCellFermataBetweenNotes : result = "FOO"; break; //{ kDots5,   kDots126, kDots123 },
  case kCellFermataOverABarLine : result = "FOO"; break; //{ kDots456, kDots126, kDots123 };

  } // switch

  return result;
}


string bsrCellKindAsString (bsrCellKind cellKind)
{
  return bsrCellKindAsShortString (cellKind);
}

//______________________________________________________________________________
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


/*
//______________________________________________________________________________
// brailling numbers
wstring braille (int n)
{
  wstringstream ws;
  
  
  if (n < 0) {
    ws << kCellQuestionMark; // JMI which minus?
    n = -n;
  }

  if (n <= 9) {
    switch (n) {
      case 0: ws << kCell0; break;
      case 1: ws << kCell1; break;
      case 2: ws << kCell2; break;
      case 3: ws << kCell3; break;
      case 4: ws << kCell4; break;
      case 5: ws << kCell5; break;
      case 6: ws << kCell6; break;
      case 7: ws << kCell7; break;
      case 8: ws << kCell8; break;
      case 9: ws << kCell9; break;
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
  wchar_t result = kCellSpace;
  
  switch (ch) {
    case 'a':
      result = kCellA;
      break;
    case 'b':
      result = kCellB;
      break;
    case 'c':
      result = kCellC;
      break;
    case 'd':
      result = kCellD;
      break;
    case 'e':
      result = kCellE;
      break;
    case 'f':
      result = kCellF;
      break;
    case 'g':
      result = kCellG;
      break;
    case 'h':
      result = kCellH;
      break;
    case 'i':
      result = kCellI;
      break;
    case 'j':
      result = kCellJ;
      break;
    case 'k':
      result = kCellK;
      break;
    case 'l':
      result = kCellL;
      break;
    case 'm':
      result = kCellM;
      break;
    case 'n':
      result = kCellN;
      break;
    case 'o':
      result = kCellO;
      break;
    case 'p':
      result = kCellP;
      break;
    case 'q':
      result = kCellQ;
      break;
    case 'r':
      result = kCellR;
      break;
    case 's':
      result = kCellS;
      break;
    case 't':
      result = kCellT;
      break;
    case 'u':
      result = kCellU;
      break;
    case 'v':
      result = kCellV;
      break;
    case 'w':
      result = kCellW;
      break;
    case 'x':
      result = kCellX;
      break;
    case 'y':
      result = kCellY;
      break;
    case 'z':
      result = kCellZ;
      break;
    default:
      result = kCellQuestionMark;
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

void write_wchar_t ( wchar_t cell )
{
  union Conversion {
    wchar_t       cellar;
    unsigned char chars [2];
  } conversion;

  conversion.cellar = cell;
     
  // write in reverse order!
  cout << conversion.chars [0] << conversion.chars [1];
}

void write_wstring (ostream& os, wstring wstr )
{
  for (unsigned int i = 0; i < wstr.size (); i++) {
    wchar_t cell = wstr [i];
    write_wchar_t (os, cell);
  } // for
}

ostream& operator<< (ostream& os, const wstring& wstr)
{
  for (unsigned int i = 0; i < wstr.size (); i++) {
    wchar_t cell = wstr [i];
    
    union Conversion {
      wchar_t       cellar;
      unsigned char chars [2];
    } conversion;
  
    conversion.cellar = cell;
  
    // write in reverse order!
    os << conversion.chars [0] << conversion.chars [1];
    } // for

  return os;
}
*/

//______________________________________________________________________________
void initializeBSRBasicTypes ()
{}


}


  /* JMI KEEP JUST IN CASE
  switch (cell) {
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
  
