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

#include "msrBasicTypes.h"


namespace MusicXML2 
{

//______________________________________________________________________________
enum bsrCellKind {
  // non 6dots values
  kCellEOL     , // L'\u000a'
  kCellEOP     , // L'\u000c'

  // 6dots values for Braille music
  kDotsNone    , // L'\u2800'
  kDots1       , // L'\u2801'
  kDots2       , // L'\u2802'
  kDots12      , // L'\u2803'
  kDots3       , // L'\u2804'
  kDots13      , // L'\u2805'
  kDots23      , // L'\u2806'
  kDots123     , // L'\u2807'
  kDots4       , // L'\u2808'
  kDots14      , // L'\u2809'
  kDots24      , // L'\u280a'
  kDots124     , // L'\u280b'
  kDots34      , // L'\u280c'
  kDots134     , // L'\u280d'
  kDots234     , // L'\u280e'
  kDots1234    , // L'\u280f'

  kDots5       , // L'\u2810'
  kDots15      , // L'\u2811'
  kDots25      , // L'\u2812'
  kDots125     , // L'\u2813'
  kDots35      , // L'\u2814'
  kDots135     , // L'\u2815'
  kDots235     , // L'\u2816'
  kDots1235    , // L'\u2817'
  kDots45      , // L'\u2818'
  kDots145     , // L'\u2819'
  kDots245     , // L'\u281a'
  kDots1245    , // L'\u281b'
  kDots345     , // L'\u281c'
  kDots1345    , // L'\u281d'
  kDots2345    , // L'\u281e'
  kDots12345   , // L'\u281f'

  kDots6       , // L'\u2820'
  kDots16      , // L'\u2821'
  kDots26      , // L'\u2822'
  kDots126     , // L'\u2823'
  kDots36      , // L'\u2824'
  kDots136     , // L'\u2825'
  kDots236     , // L'\u2826'
  kDots1236    , // L'\u2827'
  kDots46      , // L'\u2828'
  kDots146     , // L'\u2829'
  kDots246     , // L'\u282a'
  kDots1246    , // L'\u282b'
  kDots346     , // L'\u282c'
  kDots1346    , // L'\u282d'
  kDots2346    , // L'\u282e'
  kDots12346   , // L'\u282f'

  kDots56      , // L'\u2830'
  kDots156     , // L'\u2831'
  kDots256     , // L'\u2832'
  kDots1256    , // L'\u2833'
  kDots356     , // L'\u2834'
  kDots1356    , // L'\u2835'
  kDots2356    , // L'\u2836'
  kDots12356   , // L'\u2837'
  kDots456     , // L'\u2838'
  kDots1456    , // L'\u2839'
  kDots2456    , // L'\u283a'
  kDots12456   , // L'\u283b'
  kDots3456    , // L'\u283c'
  kDots13456   , // L'\u283d'
  kDots23456   , // L'\u283e'
  kDots123456    // L'\u283f'
};

//______________________________________________________________________________
string bsrCellKindAsShortString (bsrCellKind cellKind);

string bsrCellKindAsString (bsrCellKind cellKind);

//______________________________________________________________________________
// lower case letters
const bsrCellKind
  kCellA = kDots1,
  kCellB = kDots12,
  kCellC = kDots14,
  kCellD = kDots145,
  kCellE = kDots15,
  kCellF = kDots124,
  kCellG = kDots1245,
  kCellH = kDots125,
  kCellI = kDots24,
  kCellJ = kDots245,
  
  kCellK = kDots13,
  kCellL = kDots123,
  kCellM = kDots134,
  kCellN = kDots1345,
  kCellO = kDots135,
  kCellP = kDots1234,
  kCellQ = kDots12345,
  kCellR = kDots1235,
  kCellS = kDots234,
  kCellT = kDots2345,

  kCellU = kDots136,
  kCellV = kDots1236,
  kCellW = kDots2456,
  kCellX = kDots1346,
  kCellY = kDots13456,
  kCellZ = kDots1356;

//______________________________________________________________________________
// capitals
const bsrCellKind
  kCellCapitalsSign = kDots46;
//  kCellCapitalsSequenceSign, // { kCellCapitalsSign, kCellCapitalsSign };

//______________________________________________________________________________
// decimal digits
const bsrCellKind
  kCellNumberSign = kDots3456,
  kCell1 = kCellA,
  kCell2 = kCellB,
  kCell3 = kCellC,
  kCell4 = kCellD,
  kCell5 = kCellE,
  kCell6 = kCellF,
  kCell7 = kCellG,
  kCell8 = kCellH,
  kCell9 = kCellI,
  kCell0 = kCellJ;

//______________________________________________________________________________
// lower decimal digits
const bsrCellKind
  kCellLower1 = kDots2,
  kCellLower2 = kDots23,
  kCellLower3 = kDots25,
  kCellLower4 = kDots256,
  kCellLower5 = kDots26,
  kCellLower6 = kDots235,
  kCellLower7 = kDots2356,
  kCellLower8 = kDots236,
  kCellLower9 = kDots35,
  kCellLower0 = kDots356;

//______________________________________________________________________________
// alterations
const bsrCellKind
  kCellFlat    = kDots126,
  kCellNatural = kDots16,
  kCellSharp   = kDots136;

// augmentation dots
const bsrCellKind
  kCellAugmentationDot = kDots3;

//______________________________________________________________________________
// arithmetic operators
const bsrCellKind
  kCell_ac_plus      = kDots235,
  kCell_ac_minus     = kDots36,
  kCell_ac_times     = kDots35,
  kCell_ac_dividedBy = kDots25,
  kCell_ac_equals    = kDots2356;

//______________________________________________________________________________
// words
const bsrCellKind
  kCellWordSign       = kDots345,
  kCellWordApostrophe = kDots6;

//______________________________________________________________________________
// braille cells
void brailleCellKindAsUTF8 (bsrCellKind cellKind, ostream& os);
void brailleCellKindAsUTF16 (bsrCellKind cellKind, ostream& os);

void brailleCellKind (bsrCellKind cellKind, ostream& os);


//______________________________________________________________________________
/*
enum bsrCellKind2 {
  // lower decimal digits
  kCellLower1, // kDots2,
  kCellLower2, // kDots23,
  kCellLower3, // kDots25,
  kCellLower4, // kDots256,
  kCellLower5, // kDots26,
  kCellLower6, // kDots235,
  kCellLower7, // kDots2356,
  kCellLower8, // kDots236,
  kCellLower9, // kDots35,
  kCellLower0, // kDots356;

  
  // punctuation
  kCellDot             , // kDots256,
  kCellComma           , // kDots2,
  kCellQuestionMark    , // kDots26,
  kCellSemicolon       , // kDots23,
  kCellColon           , // kDots25,
  kCellExclamationMark , // kDots235,
  kCellLeftParenthesis , // kDots236,
  kCellRightParenthesis, // kDots356,
  kCellDoubleQuote     , // kDots2356,
  kCellDash            , // kDots36,
  kCellQuote           , // kDots3;

  // other symbols
  kCellSlash   , // kDots34,
  kCellVerseEnd, // kDots345,
  kCellItalics , // kDots456,
  kCellAsterisk, // kDots35,
  kCellExponent, // kDots4;

  // intervals
  kCellSecond , // kDots34,
  kCellThird  , // kDots346,
  kCellFourth , // kDots3456,
  kCellFifth  , // kDots35,
  kCellSixth  , // kDots356,
  kCellSeventh, // kDots25,
  kCellEighth , // kDots36;

  // triplets
  kCellTriplet , // kDots23;

  // hyphens
  kCellMusicHyphen, // kDots5;

  // keyboard hands
  kCellRightHand, // { kDots46,  kDots345 },
  kCellLeftHand, //  { kDots456, kDots345 };

  // bars
  kCellFinalDoubleBar    , //     { kDots126, kDots13 },
  kCellSectionalDoubleBar, // { kDots126, kDots13, kDots3 };

  // measure divisions
  kCellMeasureDivisionSign, // { kDots46, kDots13 };

  // words
  kCellWordSign      , // kDots345,
  kCellWordApostrophe, // kDots6;

  // capitals
  kCellCapitalsSign, //, // kDots46;
  kCellCapitalsSequenceSign, // { kCellCapitalsSign, kCellCapitalsSign };

  // parentheses
  kCellLiteraryLeftParenthesis  , //{ kDots5, kDots126 },
  kCellLiteraryRightParenthesis , //{ kDots5, kDots345 },
  kCellMusicParentheses         , //{ kDots6, kDots3 },
  kCellSpecialParentheses       , //{ kDots2356, kDots2356 };

  // other symbols
  kCellParagraph           , //{ kDots5, kDots1234 },
  kCellAmpersand           , //{ kDots5, kDots123456 },
  kCellUpsilon             , //{ kDots45, kDots13456 }, // better name JMI ??? 
  kCellEuro                , //{ kDots45, kDots15 },
  kCellDollar              , //{ kDots45, kDots234 },
  kCellPound               , //{ kDots45, kDots123 },
  kCellCopyright           , //{ kDots5, kDots14 },
  kCellRegisteredTradeMark , //{ kDots5, kDots1235 },
  kCellTradeMark           , //{ kDots5, kDots2345 },
  kCellPercent             , //{ kDots5, kDots346 },
  kCellPerthousand         , //{ kDots5, kDots346, kDots346 },
  kCellPertenthousand      , //{ kDots5, kDots346, kDots346, kDots346 };

  // fermatas
  kCellFermataOnANote      , //{ kDots146, kDots126, kDots123 },
  kCellFermataBetweenNotes , //{ kDots5,   kDots126, kDots123 },
  kCellFermataOverABarLine , //{ kDots456, kDots126, kDots123 };
};
*/

/*
U+2800  ⠀ e2 a0 80  BRAILLE PATTERN BLANK
U+2801  ⠁ e2 a0 81  BRAILLE PATTERN DOTS-1
U+2802  ⠂ e2 a0 82  BRAILLE PATTERN DOTS-2
U+2803  ⠃ e2 a0 83  BRAILLE PATTERN DOTS-12
U+2804  ⠄ e2 a0 84  BRAILLE PATTERN DOTS-3
U+2805  ⠅ e2 a0 85  BRAILLE PATTERN DOTS-13
U+2806  ⠆ e2 a0 86  BRAILLE PATTERN DOTS-23
U+2807  ⠇ e2 a0 87  BRAILLE PATTERN DOTS-123
U+2808  ⠈ e2 a0 88  BRAILLE PATTERN DOTS-4
U+2809  ⠉ e2 a0 89  BRAILLE PATTERN DOTS-14
U+280A  ⠊ e2 a0 8a  BRAILLE PATTERN DOTS-24
U+280B  ⠋ e2 a0 8b  BRAILLE PATTERN DOTS-124
U+280C  ⠌ e2 a0 8c  BRAILLE PATTERN DOTS-34
U+280D  ⠍ e2 a0 8d  BRAILLE PATTERN DOTS-134
U+280E  ⠎ e2 a0 8e  BRAILLE PATTERN DOTS-234
U+280F  ⠏ e2 a0 8f  BRAILLE PATTERN DOTS-1234
U+2810  ⠐ e2 a0 90  BRAILLE PATTERN DOTS-5
U+2811  ⠑ e2 a0 91  BRAILLE PATTERN DOTS-15
U+2812  ⠒ e2 a0 92  BRAILLE PATTERN DOTS-25
U+2813  ⠓ e2 a0 93  BRAILLE PATTERN DOTS-125
U+2814  ⠔ e2 a0 94  BRAILLE PATTERN DOTS-35
U+2815  ⠕ e2 a0 95  BRAILLE PATTERN DOTS-135
U+2816  ⠖ e2 a0 96  BRAILLE PATTERN DOTS-235
U+2817  ⠗ e2 a0 97  BRAILLE PATTERN DOTS-1235
U+2818  ⠘ e2 a0 98  BRAILLE PATTERN DOTS-45
U+2819  ⠙ e2 a0 99  BRAILLE PATTERN DOTS-145
U+281A  ⠚ e2 a0 9a  BRAILLE PATTERN DOTS-245
U+281B  ⠛ e2 a0 9b  BRAILLE PATTERN DOTS-1245
U+281C  ⠜ e2 a0 9c  BRAILLE PATTERN DOTS-345
U+281D  ⠝ e2 a0 9d  BRAILLE PATTERN DOTS-1345
U+281E  ⠞ e2 a0 9e  BRAILLE PATTERN DOTS-2345
U+281F  ⠟ e2 a0 9f  BRAILLE PATTERN DOTS-12345
U+2820  ⠠ e2 a0 a0  BRAILLE PATTERN DOTS-6
U+2821  ⠡ e2 a0 a1  BRAILLE PATTERN DOTS-16
U+2822  ⠢ e2 a0 a2  BRAILLE PATTERN DOTS-26
U+2823  ⠣ e2 a0 a3  BRAILLE PATTERN DOTS-126
U+2824  ⠤ e2 a0 a4  BRAILLE PATTERN DOTS-36
U+2825  ⠥ e2 a0 a5  BRAILLE PATTERN DOTS-136
U+2826  ⠦ e2 a0 a6  BRAILLE PATTERN DOTS-236
U+2827  ⠧ e2 a0 a7  BRAILLE PATTERN DOTS-1236
U+2828  ⠨ e2 a0 a8  BRAILLE PATTERN DOTS-46
U+2829  ⠩ e2 a0 a9  BRAILLE PATTERN DOTS-146
U+282A  ⠪ e2 a0 aa  BRAILLE PATTERN DOTS-246
U+282B  ⠫ e2 a0 ab  BRAILLE PATTERN DOTS-1246
U+282C  ⠬ e2 a0 ac  BRAILLE PATTERN DOTS-346
U+282D  ⠭ e2 a0 ad  BRAILLE PATTERN DOTS-1346
U+282E  ⠮ e2 a0 ae  BRAILLE PATTERN DOTS-2346
U+282F  ⠯ e2 a0 af  BRAILLE PATTERN DOTS-12346
U+2830  ⠰ e2 a0 b0  BRAILLE PATTERN DOTS-56
U+2831  ⠱ e2 a0 b1  BRAILLE PATTERN DOTS-156
U+2832  ⠲ e2 a0 b2  BRAILLE PATTERN DOTS-256
U+2833  ⠳ e2 a0 b3  BRAILLE PATTERN DOTS-1256
U+2834  ⠴ e2 a0 b4  BRAILLE PATTERN DOTS-356
U+2835  ⠵ e2 a0 b5  BRAILLE PATTERN DOTS-1356
U+2836  ⠶ e2 a0 b6  BRAILLE PATTERN DOTS-2356
U+2837  ⠷ e2 a0 b7  BRAILLE PATTERN DOTS-12356
U+2838  ⠸ e2 a0 b8  BRAILLE PATTERN DOTS-456
U+2839  ⠹ e2 a0 b9  BRAILLE PATTERN DOTS-1456
U+283A  ⠺ e2 a0 ba  BRAILLE PATTERN DOTS-2456
U+283B  ⠻ e2 a0 bb  BRAILLE PATTERN DOTS-12456
U+283C  ⠼ e2 a0 bc  BRAILLE PATTERN DOTS-3456
U+283D  ⠽ e2 a0 bd  BRAILLE PATTERN DOTS-13456
U+283E  ⠾ e2 a0 be  BRAILLE PATTERN DOTS-23456
U+283F  ⠿ e2 a0 bf  BRAILLE PATTERN DOTS-123456


FROM http://unicode.org/faq/utf_bom.html#BOM :


Q: How do I write a UTF converter?

A: The freely available open source project International Components for Unicode (ICU) has UTF conversion built into it. The latest version may be downloaded from the ICU Project web site.
* 
http://site.icu-project.org

 
Bytes Encoding Form
00 00 FE FF UTF-32, big-endian
FF FE 00 00 UTF-32, little-endian
FE FF       UTF-16, big-endian
FF FE       UTF-16, little-endian
EF BB BF    UTF-8


Q: Is there a standard method to package a Unicode character so it fits an 8-Bit ASCII stream?

A: There are three or four options for making Unicode fit into an 8-bit format.

a) Use UTF-8. This preserves ASCII, but not Latin-1, because the characters >127 are different from Latin-1. UTF-8 uses the bytes in the ASCII only for ASCII characters. Therefore, it works well in any environment where ASCII characters have a significance as syntax characters, e.g. file name syntaxes, markup languages, etc., but where the all other characters may use arbitrary bytes. 
Example: “Latin Small Letter s with Acute” (015B) would be encoded as two bytes: C5 9B.

b) Use Java or C style escapes, of the form \uXXXXX or \xXXXXX. This format is not standard for text files, but well defined in the framework of the languages in question, primarily for source files.
Example: The Polish word “wyjście” with character “Latin Small Letter s with Acute” (015B) in the middle (ś is one character) would look like: “wyj\u015Bcie".

c) Use the &#xXXXX; or &#DDDDD; numeric character escapes as in HTML or XML. Again, these are not standard for plain text files, but well defined within the framework of these markup languages.
Example: “wyjście” would look like “wyj&#x015B;cie"

d) Use SCSU. This format compresses Unicode into 8-bit format, preserving most of ASCII, but using some of the control codes as commands for the decoder. However, while ASCII text will look like ASCII text after being encoded in SCSU, other characters may occasionally be encoded with the same byte values, making SCSU unsuitable for 8-bit channels that blindly interpret any of the bytes as ASCII characters.
Example: “<SC2> wyjÛcie” where <SC2> indicates the byte 0x12 and “Û” corresponds to byte 0xDB. [AF]


A: The following table summarizes some of the properties of each of the UTFs. 

Name  UTF-8 UTF-16  UTF-16BE  UTF-16LE  UTF-32  UTF-32BE  UTF-32LE
Smallest code point 0000  0000  0000  0000  0000  0000  0000
Largest code point  10FFFF  10FFFF  10FFFF  10FFFF  10FFFF  10FFFF  10FFFF
Code unit size  8 bits  16 bits 16 bits 16 bits 32 bits 32 bits 32 bits
Byte order  N/A <BOM> big-endian  little-endian <BOM> big-endian  little-endian
Fewest bytes per character  1 2 2 2 4 4 4
Most bytes per character  4 4 4 4 4 4 4



Q: What’s the algorithm to convert from UTF-16 to character codes?

A: The Unicode Standard used to contain a short algorithm, now there is just a bit distribution table. Here are three short code snippets that translate the information from the bit distribution table into C code that will convert to and from UTF-16.

Using the following type definitions

typedef unsigned int16 UTF16;
typedef unsigned int32 UTF32;
the first snippet calculates the high (or leading) surrogate from a character code C.

const UTF16 HI_SURROGATE_START, // 0xD800
UTF16 X, // (UTF16) C;
UTF32 U, // (C >> 16) & ((1 << 5) - 1);
UTF16 W, // (UTF16) U - 1;
UTF16 HiSurrogate, // HI_SURROGATE_START | (W << 6) | X >> 10;
where X, U and W correspond to the labels used in Table 3-5 UTF-16 Bit Distribution. The next snippet does the same for the low surrogate.

const UTF16 LO_SURROGATE_START, // 0xDC00
UTF16 X, // (UTF16) C;
UTF16 LoSurrogate, // (UTF16) (LO_SURROGATE_START | X & ((1 << 10) - 1));
Finally, the reverse, where hi and lo are the high and low surrogate, and C the resulting character

UTF32 X, // (hi & ((1 << 6) -1)) << 10 | lo & ((1 << 10) -1);
UTF32 W, // (hi >> 6) & ((1 << 5) - 1);
UTF32 U, // W + 1;
UTF32 C, // U << 16 | X;
A caller would need to ensure that C, hi, and lo are in the appropriate ranges. [AF]

Q: Isn’t there a simpler way to do this?

A: There is a much simpler computation that does not try to follow the bit distribution table.

// constants
const UTF32 LEAD_OFFSET, // 0xD800 - (0x10000 >> 10);
const UTF32 SURROGATE_OFFSET, // 0x10000 - (0xD800 << 10) - 0xDC00;

// computations
UTF16 lead, // LEAD_OFFSET + (codepoint >> 10);
UTF16 trail, // 0xDC00 + (codepoint & 0x3FF);

UTF32 codepoint, // (lead << 10) + trail + SURROGATE_OFFSET;


*/

/*
//______________________________________________________________________________

  


//______________________________________________________________________________
// brailling numbers
wstring braille (int n);

//______________________________________________________________________________
// brailling characters and strings
bsrDot6Cell braille (char ch);

wstring braille (string str);

//______________________________________________________________________________
// writing UTF-16 to ostreams
void write_bsrDot6Cell (ostream& os, bsrDot6Cell cell);

void write_bsrDot6Cell ( bsrDot6Cell cell );

ostream& operator<< (ostream& os, const bsrDot6Cell cell);

void write_wstring (ostream& os, wstring wstr );

ostream& operator<< (ostream& os, const wstring& wstr);
*/

//______________________________________________________________________________
void initializeBSRBasicTypes ();


} // namespace MusicXML2


#endif
