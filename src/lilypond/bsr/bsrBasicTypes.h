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

// cell kinds
//______________________________________________________________________________
enum bsrCellKind {
  kCellUnknown,

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

// lower case letters
//______________________________________________________________________________
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

// capitals
//______________________________________________________________________________
const bsrCellKind
  kCellCapitalsSign = kDots46;
//  kCellCapitalsSequenceSign, // { kCellCapitalsSign, kCellCapitalsSign };

// decimal digits
//______________________________________________________________________________
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

// lower decimal digits
//______________________________________________________________________________
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

// alterations
//______________________________________________________________________________
const bsrCellKind
  kCellFlat    = kDots126,
  kCellNatural = kDots16,
  kCellSharp   = kDots146;

// augmentation dots
//______________________________________________________________________________
const bsrCellKind
  kCellAugmentationDot = kDots3;

// arithmetic operators
//______________________________________________________________________________
const bsrCellKind
  kCell_ac_plus      = kDots235,
  kCell_ac_minus     = kDots36,
  kCell_ac_times     = kDots35,
  kCell_ac_dividedBy = kDots25,
  kCell_ac_equals    = kDots2356;

// words
//______________________________________________________________________________
const bsrCellKind
  kCellWordSign        = kDots345,

  kCellWordApostrophe  = kDots6,

  kCellParenthesis     = kDots2356,
  kCellQuestionMark    = kDots26;

// braille cells
//______________________________________________________________________________
void brailleCellKindAsUTF8 (bsrCellKind cellKind, ostream& os);
void brailleCellKindAsUTF16 (bsrCellKind cellKind, ostream& os);

void brailleCellKind (bsrCellKind cellKind, ostream& os);

// braille output kinds
//______________________________________________________________________________
enum bsrBrailleOutputKind {
    kBrailleOutputAscii, // default value
    kBrailleOutputUTF8, kBrailleOutputUTF16 };

string bsrBrailleOutputKindAsString (
  bsrBrailleOutputKind brailleOutputKind);

extern map<string, bsrBrailleOutputKind>
  gBsrBrailleOutputKindsMap;

string existingBsrBrailleOutputKinds (int namesListMaxLength);

void initializeBsrBrailleOutputKindsMap ();

// chords languages
//______________________________________________________________________________
enum bsrTextsLanguageKind {
  kTextsEnglish, // BANA's default
  kTextsGerman, kTextsItalian, kTextsFrench };

string bsrTextsLanguageKindAsString (
  bsrTextsLanguageKind languageKind);

extern map<string, bsrTextsLanguageKind>
  gBsrTextsLanguageKindsMap;

string existingBsrTextsLanguageKinds (int namesListMaxLength);

void initializeBsrTextsLanguageKindsMap ();


/*
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

EXP ostream& operator<< (ostream& os, const bsrDot6Cell cell);

void write_wstring (ostream& os, wstring wstr );

EXP ostream& operator<< (ostream& os, const wstring& wstr);
*/

// warnings and errors
//______________________________________________________________________________
EXP void bsrMusicXMLWarning (
  std::string inputSourceName,
  int         inputLineNumber,
  std::string message);

EXP void bsrInternalError (
  std::string inputSourceName,
  int    inputLineNumber,
  std::string sourceCodeFileName,
  int    sourceCodeLineNumber,
  std::string message);

// initialization
//______________________________________________________________________________
void initializeBSRBasicTypes ();


} // namespace MusicXML2


#endif


/*
from https://brltty.app/pipermail/brltty/2012-October/009556.html :

[BRLTTY] Braille Code

Dave Mielke dave at mielke.cc
Wed Oct 10 14:15:08 EDT 2012
Previous message: [BRLTTY] Braille Code
Next message: [BRLTTY] Braille Code
Messages sorted by: [ date ] [ thread ] [ subject ] [ author ]
[quoted lines by Dave Mielke on 2012/10/10 at 14:13 -0400]

>Each language has its own mapping. That being said, the NABCC (North American
>Braille Computer Code) is probably as good a place to start as any. You can
>find a table for that mapping within the file Tables/en-nabcc.ttb in brltty's
>source tree. I've attached it to this message, as well.

I forgot to attach the table. Here it is.

--
Dave Mielke           | 2213 Fox Crescent | The Bible is the very Word of God.
Phone: 1-613-726-0014 | Ottawa, Ontario   | 2011 May 21 is the End of Salvation.
EMail: dave at mielke.cc | Canada  K2A 1H7   | http://Mielke.cc/now.html
http://FamilyRadio.com/                   | http://Mielke.cc/bible/
-------------- next part --------------
###############################################################################
# BRLTTY - A background process providing access to the console screen (when in
#          text mode) for a blind person using a refreshable braille display.
#
# Copyright (C) 1995-2012 by The BRLTTY Developers.
#
# BRLTTY comes with ABSOLUTELY NO WARRANTY.
#
# This is free software, placed under the terms of the
# GNU Lesser General Public License, as published by the Free Software
# Foundation; either version 2.1 of the License, or (at your option) any
# later version. Please see the file LICENSE-LGPL for details.
#
# Web Page: http://mielke.cc/brltty/
#
# This software is maintained by Dave Mielke <dave at mielke.cc>.
###############################################################################

# BRLTTY Text Table - English (North American Braille Computer Code)

# This is a description of the default text table used by BRLTTY.
# It's based on the North American Braille Computer Code, but defines the full
# Latin1 (ISO-8859-1) character set.

# The 95 printable characters of the standard 7-bit US-ASCII character set
# (32-126) are identical to their representations within the North American
# Braille Computer Code (these are the only characters which the NABCC actually
# defines). Characters from literary braille, symbols from The Nemeth Braille
# Code for Mathematics and Science Notation, and a bit of human imagination
# have all been combined to create an easy-to-remember, one-to-one mapping
# between each character and its braille counterpart. All possible combinations
# involving only the original 6 braille dots are used, but that only allows for
# 64 out of the required 95 character representations. The presence or absence
# of dot 7 is used to differentiate between pairs of characters which either
# are very closely related in meaning or, in a few cases where a more intuitive
# reason couldn't be found, have a very close logical relationship within the
# US-ASCII code. Dot 8 isn't used at all.

# The space and the 26 lowercase letters (a-z) are the same as in literary
# braille:

     #Hex    Dots       Dec Char Description
char \X20 (        )  #  32      space
char \X61 (1       )  #  97   a  latin small letter a
char \X62 (12      )  #  98   b  latin small letter b
char \X63 (1  4    )  #  99   c  latin small letter c
char \X64 (1  45   )  # 100   d  latin small letter d
char \X65 (1   5   )  # 101   e  latin small letter e
char \X66 (12 4    )  # 102   f  latin small letter f
char \X67 (12 45   )  # 103   g  latin small letter g
char \X68 (12  5   )  # 104   h  latin small letter h
char \X69 ( 2 4    )  # 105   i  latin small letter i
char \X6A ( 2 45   )  # 106   j  latin small letter j
char \X6B (1 3     )  # 107   k  latin small letter k
char \X6C (123     )  # 108   l  latin small letter l
char \X6D (1 34    )  # 109   m  latin small letter m
char \X6E (1 345   )  # 110   n  latin small letter n
char \X6F (1 3 5   )  # 111   o  latin small letter o
char \X70 (1234    )  # 112   p  latin small letter p
char \X71 (12345   )  # 113   q  latin small letter q
char \X72 (123 5   )  # 114   r  latin small letter r
char \X73 ( 234    )  # 115   s  latin small letter s
char \X74 ( 2345   )  # 116   t  latin small letter t
char \X75 (1 3  6  )  # 117   u  latin small letter u
char \X76 (123  6  )  # 118   v  latin small letter v
char \X77 ( 2 456  )  # 119   w  latin small letter w
char \X78 (1 34 6  )  # 120   x  latin small letter x
char \X79 (1 3456  )  # 121   y  latin small letter y
char \X7A (1 3 56  )  # 122   z  latin small letter z

# The 26 uppercase letters (A-Z) are the same as their lowercase counterparts
# except that dot 7 is added:

     #Hex    Dots       Dec Char Description
char \X41 (1     7 )  #  65   A  latin capital letter a
char \X42 (12    7 )  #  66   B  latin capital letter b
char \X43 (1  4  7 )  #  67   C  latin capital letter c
char \X44 (1  45 7 )  #  68   D  latin capital letter d
char \X45 (1   5 7 )  #  69   E  latin capital letter e
char \X46 (12 4  7 )  #  70   F  latin capital letter f
char \X47 (12 45 7 )  #  71   G  latin capital letter g
char \X48 (12  5 7 )  #  72   H  latin capital letter h
char \X49 ( 2 4  7 )  #  73   I  latin capital letter i
char \X4A ( 2 45 7 )  #  74   J  latin capital letter j
char \X4B (1 3   7 )  #  75   K  latin capital letter k
char \X4C (123   7 )  #  76   L  latin capital letter l
char \X4D (1 34  7 )  #  77   M  latin capital letter m
char \X4E (1 345 7 )  #  78   N  latin capital letter n
char \X4F (1 3 5 7 )  #  79   O  latin capital letter o
char \X50 (1234  7 )  #  80   P  latin capital letter p
char \X51 (12345 7 )  #  81   Q  latin capital letter q
char \X52 (123 5 7 )  #  82   R  latin capital letter r
char \X53 ( 234  7 )  #  83   S  latin capital letter s
char \X54 ( 2345 7 )  #  84   T  latin capital letter t
char \X55 (1 3  67 )  #  85   U  latin capital letter u
char \X56 (123  67 )  #  86   V  latin capital letter v
char \X57 ( 2 4567 )  #  87   W  latin capital letter w
char \X58 (1 34 67 )  #  88   X  latin capital letter x
char \X59 (1 34567 )  #  89   Y  latin capital letter y
char \X5A (1 3 567 )  #  90   Z  latin capital letter z

# The 10 decimal digits (0-9) are the same as in the Nemeth Code:

     #Hex    Dots       Dec Char Description
char \X30 (  3 56  )  #  48   0  digit zero
char \X31 ( 2      )  #  49   1  digit one
char \X32 ( 23     )  #  50   2  digit two
char \X33 ( 2  5   )  #  51   3  digit three
char \X34 ( 2  56  )  #  52   4  digit four
char \X35 ( 2   6  )  #  53   5  digit five
char \X36 ( 23 5   )  #  54   6  digit six
char \X37 ( 23 56  )  #  55   7  digit seven
char \X38 ( 23  6  )  #  56   8  digit eight
char \X39 (  3 5   )  #  57   9  digit nine

# Common symbols used within mathematical expressions by popular computer
# programming languages are the same as in the Nemeth Code:

     #Hex    Dots       Dec Char Description
char \X2E (   4 6  )  #  46   .  full stop
char \X2B (  34 6  )  #  43   +  plus sign
char \X2D (  3  6  )  #  45   -  hyphen-minus
char \X2A (1    6  )  #  42   *  asterisk
char \X2F (  34    )  #  47   /  solidus
char \X28 (123 56  )  #  40   (  left parenthesis
char \X29 ( 23456  )  #  41   )  right parenthesis

# With all of these major considerations having been taken into account,
# convenient representations were still available, and are used, for some of
# the remaining characters:

     #Hex    Dots       Dec Char Description
char \X26 (1234 6  )  #  38   &  ampersand
char \X23 (  3456  )  #  35   #  number sign

# The remaining characters are what they are. Dot 7 isn't used either within
# the number block (32-63) or, with the exception of the DEL control character
# (127), within the lowercase block (96-127). With the exception of the
# underscore (95), dot 7 is used for every character within the uppercase block
# (64-95). Adding dot 7 to any character within the lowercase block (96-127)
# yields its corresponding character within the uppercase block (64-95) except
# that removing dot 7 from the DEL control character yields the underscore.

     #Hex    Dots       Dec Char Description
char \X2C (     6  )  #  44   ,  comma
char \X3B (    56  )  #  59   ;  semicolon
char \X3A (1   56  )  #  58   :  colon
char \X21 ( 234 6  )  #  33   !  exclamation mark
char \X3F (1  456  )  #  63   ?  question mark
char \X22 (    5   )  #  34   "  quotation mark
char \X27 (  3     )  #  39   '  apostrophe
char \X60 (   4    )  #  96   `  grave accent
char \X5E (   45 7 )  #  94   ^  circumflex accent
char \X7E (   45   )  # 126   ~  tilde
char \X5B ( 2 4 67 )  #  91   [  left square bracket
char \X5D (12 4567 )  #  93   ]  right square bracket
char \X7B ( 2 4 6  )  # 123   {  left curly bracket
char \X7D (12 456  )  # 125   }  right curly bracket
char \X3D (123456  )  #  61   =  equals sign
char \X3C (12   6  )  #  60   <  less-than sign
char \X3E (  345   )  #  62   >  greater-than sign
char \X24 (12 4 6  )  #  36   $  dollar sign
char \X25 (1  4 6  )  #  37   %  percent sign
char \X40 (   4  7 )  #  64   @  commercial at
char \X7C (12  56  )  # 124   |  vertical line
char \X5C (12  567 )  #  92   \  reverse solidus
char \X5F (   456  )  #  95   _  low line

# Each of the characters within the basic control character block (0-31) is the
# same as its corresponding character within both the uppercase block (64-95)
# and the lowercase block (96-127) except that dots 7 and 8 are both used.

     #Hex    Dots       Dec Char Description
char \X00 (   4  78)  #   0  ^@  null
char \X01 (1     78)  #   1  ^A  start of heading
char \X02 (12    78)  #   2  ^B  start of text
char \X03 (1  4  78)  #   3  ^C  end of text
char \X04 (1  45 78)  #   4  ^D  end of transmission
char \X05 (1   5 78)  #   5  ^E  enquiry
char \X06 (12 4  78)  #   6  ^F  acknowledge
char \X07 (12 45 78)  #   7  ^G  bell
char \X08 (12  5 78)  #   8  ^H  backspace
char \X09 ( 2 4  78)  #   9  ^I  horizontal tabulation
char \X0A ( 2 45 78)  #  10  ^J  line feed
char \X0B (1 3   78)  #  11  ^K  vertical tabulation
char \X0C (123   78)  #  12  ^L  form feed
char \X0D (1 34  78)  #  13  ^M  carriage return
char \X0E (1 345 78)  #  14  ^N  shift out
char \X0F (1 3 5 78)  #  15  ^O  shift in
char \X10 (1234  78)  #  16  ^P  data link escape
char \X11 (12345 78)  #  17  ^Q  device control one
char \X12 (123 5 78)  #  18  ^R  device control two
char \X13 ( 234  78)  #  19  ^S  device control three
char \X14 ( 2345 78)  #  20  ^T  device control four
char \X15 (1 3  678)  #  21  ^U  negative acknowledge
char \X16 (123  678)  #  22  ^V  synchronous idle
char \X17 ( 2 45678)  #  23  ^W  end of transmission block
char \X18 (1 34 678)  #  24  ^X  cancel
char \X19 (1 345678)  #  25  ^Y  end of medium
char \X1A (1 3 5678)  #  26  ^Z  substitute
char \X1B ( 2 4 678)  #  27  ^[  escape
char \X1C (12  5678)  #  28  ^\  file separator
char \X1D (12 45678)  #  29  ^]  group separator
char \X1E (   45 78)  #  30  ^^  record separator
char \X1F (   45678)  #  31  ^_  unit separator

# Each of the characters within the extended control character block (128-159)
# is the same as its corresponding character within the basic control character
# block (0-31) except that only dot 8 is used.

     #Hex    Dots       Dec Char Description
char \X80 (   4   8)  # 128  ~@  <control>
char \X81 (1      8)  # 129  ~A  <control>
char \X82 (12     8)  # 130  ~B  break permitted here
char \X83 (1  4   8)  # 131  ~C  no break here
char \X84 (1  45  8)  # 132  ~D  <control>
char \X85 (1   5  8)  # 133  ~E  next line
char \X86 (12 4   8)  # 134  ~F  start of selected area
char \X87 (12 45  8)  # 135  ~G  end of selected area
char \X88 (12  5  8)  # 136  ~H  character tabulation set
char \X89 ( 2 4   8)  # 137  ~I  character tabulation with justification
char \X8A ( 2 45  8)  # 138  ~J  line tabulation set
char \X8B (1 3    8)  # 139  ~K  partial line down
char \X8C (123    8)  # 140  ~L  partial line up
char \X8D (1 34   8)  # 141  ~M  reverse line feed
char \X8E (1 345  8)  # 142  ~N  single shift two
char \X8F (1 3 5  8)  # 143  ~O  single shift three
char \X90 (1234   8)  # 144  ~P  device control string
char \X91 (12345  8)  # 145  ~Q  private use one
char \X92 (123 5  8)  # 146  ~R  private use two
char \X93 ( 234   8)  # 147  ~S  set transmit state
char \X94 ( 2345  8)  # 148  ~T  cancel character
char \X95 (1 3  6 8)  # 149  ~U  message waiting
char \X96 (123  6 8)  # 150  ~V  start of guarded area
char \X97 ( 2 456 8)  # 151  ~W  end of guarded area
char \X98 (1 34 6 8)  # 152  ~X  start of string
char \X99 (1 3456 8)  # 153  ~Y  <control>
char \X9A (1 3 56 8)  # 154  ~Z  single character introducer
char \X9B ( 2 4 6 8)  # 155  ~[  control sequence introducer
char \X9C (12  56 8)  # 156  ~\  string terminator
char \X9D (12 456 8)  # 157  ~]  operating system command
char \X9E (   45  8)  # 158  ~^  privacy message
char \X9F (   456 8)  # 159  ~_  application program command

# Representations for the uppercase accented letters are drawn from the
# remaining combinations which use both dots 7 and 8. The representation for a
# lowercase accented letter is the same as its uppercase counterpart except
# that dot 7 isn't used. This scheme retains the use of dot 7 as the modifier
# for a capitalized letter. The only exception to these rules is that, due to
# the nature of the Latin1 character set, the German lowercase double-s is
# treated as though it were an uppercase y-dieresis (neither has an uppercase
# definition). These representations have been gathered, as much as possible,
# into logical groupings.

# The 5 letters with a circumflex accent (^) use the [1-5] dot combinations:

     #Hex    Dots       Dec Char Description
char \XC2 ( 2    78)  # 194   ?  latin capital letter a with circumflex
char \XCA ( 23   78)  # 202   ?  latin capital letter e with circumflex
char \XCE ( 2  5 78)  # 206   ?  latin capital letter i with circumflex
char \XD4 ( 2  5678)  # 212   ?  latin capital letter o with circumflex
char \XDB ( 2   678)  # 219   ?  latin capital letter u with circumflex
char \XE2 ( 2     8)  # 226   ?  latin small letter a with circumflex
char \XEA ( 23    8)  # 234   ?  latin small letter e with circumflex
char \XEE ( 2  5  8)  # 238   ?  latin small letter i with circumflex
char \XF4 ( 2  56 8)  # 244   ?  latin small letter o with circumflex
char \XFB ( 2   6 8)  # 251   ?  latin small letter u with circumflex

# The 5 letters with a grave accent (`) use the [6-0] dot combinations:

     #Hex    Dots       Dec Char Description
char \XC0 ( 23 5 78)  # 192   ?  latin capital letter a with grave
char \XC8 ( 23 5678)  # 200   ?  latin capital letter e with grave
char \XCC ( 23  678)  # 204   ?  latin capital letter i with grave
char \XD2 (  3 5 78)  # 210   ?  latin capital letter o with grave
char \XD9 (  3 5678)  # 217   ?  latin capital letter u with grave
char \XE0 ( 23 5  8)  # 224   ?  latin small letter a with grave
char \XE8 ( 23 56 8)  # 232   ?  latin small letter e with grave
char \XEC ( 23  6 8)  # 236   ?  latin small letter i with grave
char \XF2 (  3 5  8)  # 242   ?  latin small letter o with grave
char \XF9 (  3 56 8)  # 249   ?  latin small letter u with grave

# The 6 letters with an acute accent (') use the [a-f] dot combinations with
# dots 3 and 6 added:

     #Hex    Dots       Dec Char Description
char \XC1 (1    678)  # 193   ?  latin capital letter a with acute
char \XC9 (12   678)  # 201   ?  latin capital letter e with acute
char \XCD (1  4 678)  # 205   ?  latin capital letter i with acute
char \XD3 (1  45678)  # 211   ?  latin capital letter o with acute
char \XDA (1   5678)  # 218   ?  latin capital letter u with acute
char \XDD (12 4 678)  # 221   ?  latin capital letter y with acute
char \XE1 (1    6 8)  # 225   ?  latin small letter a with acute
char \XE9 (12   6 8)  # 233   ?  latin small letter e with acute
char \XED (1  4 6 8)  # 237   ?  latin small letter i with acute
char \XF3 (1  456 8)  # 243   ?  latin small letter o with acute
char \XFA (1   56 8)  # 250   ?  latin small letter u with acute
char \XFD (12 4 6 8)  # 253   ?  latin small letter y with acute

# The 6 letters with a dieresis accent (") use the [f-j] dot combinations with
# dots 3 and 6 added, and the number sign (because it fits the sequence
# reasonably well):

     #Hex    Dots       Dec Char Description
char \XC4 (1234 678)  # 196   ?  latin capital letter a with diaeresis
char \XCB (12345678)  # 203   ?  latin capital letter e with diaeresis
char \XCF (123 5678)  # 207   ?  latin capital letter i with diaeresis
char \XD6 ( 234 678)  # 214   ?  latin capital letter o with diaeresis
char \XDC ( 2345678)  # 220   ?  latin capital letter u with diaeresis
char \XE4 (1234 6 8)  # 228   ?  latin small letter a with diaeresis
char \XEB (123456 8)  # 235   ?  latin small letter e with diaeresis
char \XEF (123 56 8)  # 239   ?  latin small letter i with diaeresis
char \XF6 ( 234 6 8)  # 246   ?  latin small letter o with diaeresis
char \XFC ( 23456 8)  # 252   ?  latin small letter u with diaeresis
char \XFF (  3456 8)  # 255   ?  latin small letter y with diaeresis

# There is no uppercase y-dieresis in the Latin1 character set. The German
# lowercase double-s, which also doesn't have an uppercase counterpart in the
# Latin1 character set, uses its representation:

     #Hex    Dots       Dec Char Description
char \XDF (  345678)  # 223   ?  latin small letter sharp s

# The remaining accented letters are:

     #Hex    Dots       Dec Char Description
char \XC3 (    5 78)  # 195   ?  latin capital letter a with tilde
char \XD1 (   4 678)  # 209   ?  latin capital letter n with tilde
char \XD5 (    5678)  # 213   ?  latin capital letter o with tilde
char \XC5 (  345 78)  # 197   ?  latin capital letter a with ring above
char \XC7 (  34 678)  # 199   ?  latin capital letter c with cedilla
char \XD8 (  34  78)  # 216   ?  latin capital letter o with stroke
char \XC6 (  3   78)  # 198   ?  latin capital letter ae
char \XD0 (     678)  # 208   ?  latin capital letter eth
char \XDE (  3  678)  # 222   ?  latin capital letter thorn
char \XE3 (    5  8)  # 227   ?  latin small letter a with tilde
char \XF1 (   4 6 8)  # 241   ?  latin small letter n with tilde
char \XF5 (    56 8)  # 245   ?  latin small letter o with tilde
char \XE5 (  345  8)  # 229   ?  latin small letter a with ring above
char \XE7 (  34 6 8)  # 231   ?  latin small letter c with cedilla
char \XF8 (  34   8)  # 248   ?  latin small letter o with stroke
char \XE6 (  3    8)  # 230   ?  latin small letter ae
char \XF0 (     6 8)  # 240   ?  latin small letter eth
char \XFE (  3  6 8)  # 254   ?  latin small letter thorn

# Some characters are the same as other characters which they resemble but with
# dot 7 added:

     #Hex    Dots       Dec Char Description
char \XAD (  3  67 )  # 173   ?  soft hyphen
char \XAB (12   67 )  # 171   ?  left-pointing double angle quotation mark
char \XBB (  345 7 )  # 187   ?  right-pointing double angle quotation mark
char \XA6 (1   567 )  # 166   ?  broken bar
char \XB9 ( 2    7 )  # 185   ?  superscript one
char \XB2 ( 23   7 )  # 178   ?  superscript two
char \XB3 ( 2  5 7 )  # 179   ?  superscript three
char \XB1 (  34 67 )  # 177   ?  plus-minus sign
char \XD7 (1    67 )  # 215   ?  multiplication sign
char \XF7 (  34  7 )  # 247   ?  division sign
char \XB7 (   4 67 )  # 183   ?  middle dot
char \XA1 ( 234 67 )  # 161   ?  inverted exclamation mark
char \XBF (1  4567 )  # 191   ?  inverted question mark
char \XA2 (12 4 67 )  # 162   ?  cent sign
char \XA3 (  34567 )  # 163   ?  pound sign

# A few more characters follow this same convention but their relationships
# to their base characters is a bit obscure:

     #Hex    Dots       Dec Char Description
char \XA4 (1  4 67 )  # 164   ?  currency sign
char \XA5 (1234 67 )  # 165   ?  yen sign

# Some characters are represented by the first letters of their names lowered
# by one row of dots:

     #Hex    Dots       Dec Char Description
char \XAC ( 2  567 )  # 172   ?  not sign
char \XB6 ( 23 5 7 )  # 182   ?  pilcrow sign
char \XA9 ( 23 567 )  # 169   ?  copyright sign
char \XAE ( 23  67 )  # 174   ?  registered sign
char \XA7 (  3 5 7 )  # 167   ?  section sign
char \XB0 (  3 567 )  # 176   ?  degree sign

# The three fraction characters use combinations of dots 1 and 4 (which
# progress from left to right as the value of the fraction increases) together
# with dots 2,3,5,6,7:

     #Hex    Dots       Dec Char Description
char \XBC (123 567 )  # 188   ?  vulgar fraction one quarter
char \XBD (1234567 )  # 189   ?  vulgar fraction one half
char \XBE ( 234567 )  # 190   ?  vulgar fraction three quarters

# Each of the three extended accent characters is the same as its conventional
# compose character but with dot 7 added:

     #Hex    Dots       Dec Char Description
char \XB4 (  3   7 )  # 180   ?  acute accent
char \XB8 (     67 )  # 184   ?  cedilla
char \XA8 (    5 7 )  # 168   ?  diaeresis

# The two gender symbols are:

     #Hex    Dots       Dec Char Description
char \XBA (      7 )  # 186   ?  masculine ordinal indicator
char \XAA (       8)  # 170   ?  feminine ordinal indicator

# The three remaining characters are:

     #Hex    Dots       Dec Char Description
char \XAF ( 2   67 )  # 175   ?  macron
char \XB5 (    567 )  # 181   ?  micro sign
char \XA0 (      78)  # 160  ~   no-break space

# The nonbreaking space is dots 7 and 8 because this presents a sequence of
# nonbreaking spaces as a smooth low line segment.

     #Hex    Dots       Dec Char Description
char \X7F (   4567 )  # 127  ^?  delete

*/


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
  kCellFermataOverABarline , //{ kDots456, kDots126, kDots123 };
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
