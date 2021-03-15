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

#include <climits>      // INT_MIN, INT_MAX

#include "utilities.h"

#include "bsrBasicTypes.h"

#include "waeMessagesHandling.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "generalOah.h"


namespace MusicXML2
{

//______________________________________________________________________________
string bsrCellKindAsShortString (bsrCellKind cellKind)
{
  string result;

  switch (cellKind) {
    case bsrCellKind::kCellUnknown : result = "***cellUnknown***"; break;

    // non 6dots values
    case bsrCellKind::kCellEOL     : result = "EOL"; break;
    case bsrCellKind::kCellEOP     : result = "EOP"; break;

    // 6dots values for Braille music
    case bsrCellKind::kDotsNone    : result = ".Space"; break;

    case bsrCellKind::kDots1       : result = ".1"; break;
    case bsrCellKind::kDots2       : result = ".2"; break;
    case bsrCellKind::kDots12      : result = ".12"; break;
    case bsrCellKind::kDots3       : result = ".3"; break;
    case bsrCellKind::kDots13      : result = ".13"; break;
    case bsrCellKind::kDots23      : result = ".23"; break;
    case bsrCellKind::kDots123     : result = ".123"; break;
    case bsrCellKind::kDots4       : result = ".4"; break;
    case bsrCellKind::kDots14      : result = ".14"; break;
    case bsrCellKind::kDots24      : result = ".24"; break;
    case bsrCellKind::kDots124     : result = ".124"; break;
    case bsrCellKind::kDots34      : result = ".34"; break;
    case bsrCellKind::kDots134     : result = ".134"; break;
    case bsrCellKind::kDots234     : result = ".234"; break;
    case bsrCellKind::kDots1234    : result = ".1234"; break;

    case bsrCellKind::kDots5       : result = ".5"; break;
    case bsrCellKind::kDots15      : result = ".15"; break;
    case bsrCellKind::kDots25      : result = ".25"; break;
    case bsrCellKind::kDots125     : result = ".125"; break;
    case bsrCellKind::kDots35      : result = ".35"; break;
    case bsrCellKind::kDots135     : result = ".135"; break;
    case bsrCellKind::kDots235     : result = ".235"; break;
    case bsrCellKind::kDots1235    : result = ".1235"; break;
    case bsrCellKind::kDots45      : result = ".45"; break;
    case bsrCellKind::kDots145     : result = ".145"; break;
    case bsrCellKind::kDots245     : result = ".245"; break;
    case bsrCellKind::kDots1245    : result = ".1245"; break;
    case bsrCellKind::kDots345     : result = ".345"; break;
    case bsrCellKind::kDots1345    : result = ".1345"; break;
    case bsrCellKind::kDots2345    : result = ".2345"; break;
    case bsrCellKind::kDots12345   : result = ".12345"; break;

    case bsrCellKind::kDots6       : result = ".6"; break;
    case bsrCellKind::kDots16      : result = ".16"; break;
    case bsrCellKind::kDots26      : result = ".26"; break;
    case bsrCellKind::kDots126     : result = ".126"; break;
    case bsrCellKind::kDots36      : result = ".36"; break;
    case bsrCellKind::kDots136     : result = ".136"; break;
    case bsrCellKind::kDots236     : result = ".236"; break;
    case bsrCellKind::kDots1236    : result = ".1236"; break;
    case bsrCellKind::kDots46      : result = ".46"; break;
    case bsrCellKind::kDots146     : result = ".146"; break;
    case bsrCellKind::kDots246     : result = ".246"; break;
    case bsrCellKind::kDots1246    : result = ".1246"; break;
    case bsrCellKind::kDots346     : result = ".346"; break;
    case bsrCellKind::kDots1346    : result = ".1346"; break;
    case bsrCellKind::kDots2346    : result = ".2346"; break;
    case bsrCellKind::kDots12346   : result = ".12346"; break;

    case bsrCellKind::kDots56      : result = ".56"; break;
    case bsrCellKind::kDots156     : result = ".156"; break;
    case bsrCellKind::kDots256     : result = ".256"; break;
    case bsrCellKind::kDots1256    : result = ".1256"; break;
    case bsrCellKind::kDots356     : result = ".356"; break;
    case bsrCellKind::kDots1356    : result = ".1356"; break;
    case bsrCellKind::kDots2356    : result = ".2356"; break;
    case bsrCellKind::kDots12356   : result = ".12356"; break;
    case bsrCellKind::kDots456     : result = ".456"; break;
    case bsrCellKind::kDots1456    : result = ".1456"; break;
    case bsrCellKind::kDots2456    : result = ".2456"; break;
    case bsrCellKind::kDots12456   : result = ".12456"; break;
    case bsrCellKind::kDots3456    : result = ".3456"; break;
    case bsrCellKind::kDots13456   : result = ".13456"; break;
    case bsrCellKind::kDots23456   : result = ".23456"; break;
    case bsrCellKind::kDots123456  : result = ".123456"; break;
  } // switch

  return result;
}

//______________________________________________________________________________
string bsrCellKindAsString (bsrCellKind cellKind)
{
  string result;

  switch (cellKind) {
    // non 6dots values
    case bsrCellKind::kCellEOL     : result = "CellEOL"; break;
    case bsrCellKind::kCellEOP     : result = "CellEOP"; break;

    // 6dots values for Braille music
    case bsrCellKind::kDotsNone    : result = "Space"; break;

    case bsrCellKind::kDots1       : result = "Dots1"; break;
    case bsrCellKind::kDots2       : result = "Dots2"; break;
    case bsrCellKind::kDots12      : result = "Dots12"; break;
    case bsrCellKind::kDots3       : result = "Dots3"; break;
    case bsrCellKind::kDots13      : result = "Dots13"; break;
    case bsrCellKind::kDots23      : result = "Dots23"; break;
    case bsrCellKind::kDots123     : result = "Dots123"; break;
    case bsrCellKind::kDots4       : result = "Dots4"; break;
    case bsrCellKind::kDots14      : result = "Dots14"; break;
    case bsrCellKind::kDots24      : result = "Dots24"; break;
    case bsrCellKind::kDots124     : result = "Dots124"; break;
    case bsrCellKind::kDots34      : result = "Dots34"; break;
    case bsrCellKind::kDots134     : result = "Dots134"; break;
    case bsrCellKind::kDots234     : result = "Dots234"; break;
    case bsrCellKind::kDots1234    : result = "Dots1234"; break;

    case bsrCellKind::kDots5       : result = "Dots5"; break;
    case bsrCellKind::kDots15      : result = "Dots15"; break;
    case bsrCellKind::kDots25      : result = "Dots25"; break;
    case bsrCellKind::kDots125     : result = "Dots125"; break;
    case bsrCellKind::kDots35      : result = "Dots35"; break;
    case bsrCellKind::kDots135     : result = "Dots135"; break;
    case bsrCellKind::kDots235     : result = "Dots235"; break;
    case bsrCellKind::kDots1235    : result = "Dots1235"; break;
    case bsrCellKind::kDots45      : result = "Dots45"; break;
    case bsrCellKind::kDots145     : result = "Dots145"; break;
    case bsrCellKind::kDots245     : result = "Dots245"; break;
    case bsrCellKind::kDots1245    : result = "Dots1245"; break;
    case bsrCellKind::kDots345     : result = "Dots345"; break;
    case bsrCellKind::kDots1345    : result = "Dots1345"; break;
    case bsrCellKind::kDots12345   : result = "Dots12345"; break;

    case bsrCellKind::kDots6       : result = "Dots6"; break;
    case bsrCellKind::kDots16      : result = "Dots16"; break;
    case bsrCellKind::kDots26      : result = "Dots26"; break;
    case bsrCellKind::kDots126     : result = "Dots126"; break;
    case bsrCellKind::kDots36      : result = "Dots36"; break;
    case bsrCellKind::kDots136     : result = "Dots136"; break;
    case bsrCellKind::kDots236     : result = "Dots236"; break;
    case bsrCellKind::kDots1236    : result = "Dots1236"; break;
    case bsrCellKind::kDots46      : result = "Dots46"; break;
    case bsrCellKind::kDots146     : result = "Dots146"; break;
    case bsrCellKind::kDots246     : result = "Dots246"; break;
    case bsrCellKind::kDots1246    : result = "Dots1246"; break;
    case bsrCellKind::kDots346     : result = "Dots346"; break;
    case bsrCellKind::kDots1346    : result = "Dots1346"; break;
    case bsrCellKind::kDots2346    : result = "Dots2346"; break;
    case bsrCellKind::kDots12346   : result = "Dots12346"; break;

    case bsrCellKind::kDots56      : result = "Dots56"; break;
    case bsrCellKind::kDots156     : result = "Dots156"; break;
    case bsrCellKind::kDots256     : result = "Dots256"; break;
    case bsrCellKind::kDots1256    : result = "Dots1256"; break;
    case bsrCellKind::kDots356     : result = "Dots356"; break;
    case bsrCellKind::kDots1356    : result = "Dots1356"; break;
    case bsrCellKind::kDots2356    : result = "Dots2356"; break;
    case bsrCellKind::kDots12356   : result = "Dots12356"; break;
    case bsrCellKind::kDots456     : result = "Dots456"; break;
    case bsrCellKind::kDots1456    : result = "Dots1456"; break;
    case bsrCellKind::kDots2456    : result = "Dots2456"; break;
    case bsrCellKind::kDots12456   : result = "Dots12456"; break;
    case bsrCellKind::kDots3456    : result = "Dots3456"; break;
    case bsrCellKind::kDots13456   : result = "Dots13456"; break;
    case bsrCellKind::kDots23456   : result = "Dots23456"; break;
    case bsrCellKind::kDots123456  : result = "Dots123456"; break;

    default: result = "Dot6???";
  } // switch

  return result;
}

// braille output kinds
//______________________________________________________________________________
map<string, bsrBrailleOutputKind>
  gGlobalBsrBrailleOutputKindsMap;

void initializeBsrBrailleOutputKindsMap ()
{
  gGlobalBsrBrailleOutputKindsMap ["ascii"] = bsrBrailleOutputKind::kBrailleOutputAscii; // default
  gGlobalBsrBrailleOutputKindsMap ["utf8"]  = bsrBrailleOutputKind::kBrailleOutputUTF8;
  gGlobalBsrBrailleOutputKindsMap ["utf8d"] = bsrBrailleOutputKind::kBrailleOutputUTF8Debug;
  gGlobalBsrBrailleOutputKindsMap ["utf16"] = bsrBrailleOutputKind::kBrailleOutputUTF16;
}

string bsrBrailleOutputKindAsString (
  bsrBrailleOutputKind brailleOutputKind)
{
  string result;

  // no CamelCase here, these strings are used in the command line options
  switch (brailleOutputKind) {
    case bsrBrailleOutputKind::kBrailleOutputAscii:
      result = "ascii";
      break;
    case bsrBrailleOutputKind::kBrailleOutputUTF8:
      result = "utf8";
      break;
    case bsrBrailleOutputKind::kBrailleOutputUTF8Debug:
      result = "utf8d";
      break;
    case bsrBrailleOutputKind::kBrailleOutputUTF16:
      result = "utf16";
      break;
  } // switch

  return result;
}

string existingBsrBrailleOutputKinds (unsigned int namesListMaxLength)
{
  stringstream s;

  unsigned int brailleOutputKindsMapSize =
    gGlobalBsrBrailleOutputKindsMap.size ();

  if (brailleOutputKindsMapSize) {
    unsigned int nextToLast =
      brailleOutputKindsMapSize - 1;

    unsigned int count = 0;
    unsigned int cumulatedLength = 0;

    for (
      map<string, bsrBrailleOutputKind>::const_iterator i =
        gGlobalBsrBrailleOutputKindsMap.begin ();
      i != gGlobalBsrBrailleOutputKindsMap.end ();
      ++i
    ) {
      string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << "\n";
        cumulatedLength = 0;
      }

      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != brailleOutputKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

// chords languages
//______________________________________________________________________________

map<string, bsrTextsLanguageKind>
  gGlobalBsrTextsLanguageKindsMap;

void initializeBsrTextsLanguageKindsMap ()
{
  gGlobalBsrTextsLanguageKindsMap ["english"] = bsrTextsLanguageKind::kTextsEnglish; // default
  gGlobalBsrTextsLanguageKindsMap ["german"]  = bsrTextsLanguageKind::kTextsGerman;
  gGlobalBsrTextsLanguageKindsMap ["italian"] = bsrTextsLanguageKind::kTextsItalian;
  gGlobalBsrTextsLanguageKindsMap ["french"]  = bsrTextsLanguageKind::kTextsFrench;
}

string bsrTextsLanguageKindAsString (
  bsrTextsLanguageKind languageKind)
{
  string result;

  switch (languageKind) {
    case bsrTextsLanguageKind::kTextsEnglish: // default value
      result = "english";
      break;
    case bsrTextsLanguageKind::kTextsGerman:
      result = "german";
      break;
    case bsrTextsLanguageKind::kTextsItalian:
      result = "italian";
      break;
    case bsrTextsLanguageKind::kTextsFrench:
      result = "french";
      break;
  } // switch

  return result;
}

string existingBsrTextsLanguageKinds (unsigned int namesListMaxLength)
{
  stringstream s;

  unsigned int textsLanguageKindsMapSize =
    gGlobalBsrTextsLanguageKindsMap.size ();

  if (textsLanguageKindsMapSize) {
    unsigned int nextToLast =
      textsLanguageKindsMapSize - 1;

    unsigned int count = 0;
    unsigned int cumulatedLength = 0;

    for (
      map<string, bsrTextsLanguageKind>::const_iterator i =
        gGlobalBsrTextsLanguageKindsMap.begin ();
      i != gGlobalBsrTextsLanguageKindsMap.end ();
      ++i
    ) {
      string theString = (*i).first;

      ++count;

      cumulatedLength += theString.size ();
      if (cumulatedLength >= namesListMaxLength) {
        s << "\n";
        cumulatedLength = 0;
      }

      s << theString;

      if (count == nextToLast) {
        s << " and ";
      }
      else if (count != textsLanguageKindsMapSize) {
        s << ", ";
      }
    } // for
  }

  return s.str ();
}

/*
//______________________________________________________________________________
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
  for (unsigned int i = 0; i < wstr.size (); ++i) {
    wchar_t cell = wstr [i];
    write_wchar_t (os, cell);
  } // for
}

ostream& operator<< (ostream& os, const wstring& wstr)
{
  for (unsigned int i = 0; i < wstr.size (); ++i) {
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
{
  // protect library against multiple initializations
  static bool pThisMethodHasBeenRun = false;

  if (! pThisMethodHasBeenRun) {
#ifdef TRACING_IS_ENABLED
    if (getTraceOah () && ! gGlobalGeneralOahGroup->getQuiet ()) {
      gLogStream <<
        "Initializing BSR basic types handling" <<
        endl;
    }
#endif

    // BSR braille output kinds handling
    // ------------------------------------------------------

    initializeBsrBrailleOutputKindsMap ();

    // BSR texts languages handling
    // ------------------------------------------------------

    initializeBsrTextsLanguageKindsMap ();

    pThisMethodHasBeenRun = true;
  }
}


}


  /* JMI KEEP JUST IN CASE
  switch (cell) {
    case kBOMBigEndian   = L'\ufeff'; break;
    case kBOMSmallEndian = L'\ufeff'; break;

    case kBrailleEOL = L'\u000a'; break;
    case kBrailleEOP = L'\u000c'; break;

    case bsrCellKind::kDotsNone    = L'\u2800'; break;
    case bsrCellKind::kDots1       = L'\u2801'; break;
    case bsrCellKind::kDots2       = L'\u2802'; break;
    case bsrCellKind::kDots12      = L'\u2803'; break;
    case bsrCellKind::kDots3       = L'\u2804'; break;
    case bsrCellKind::kDots13      = L'\u2805'; break;
    case bsrCellKind::kDots23      = L'\u2806'; break;
    case bsrCellKind::kDots123     = L'\u2807'; break;
    case bsrCellKind::kDots4       = L'\u2808'; break;
    case bsrCellKind::kDots14      = L'\u2809'; break;
    case bsrCellKind::kDots24      = L'\u280a'; break;
    case bsrCellKind::kDots124     = L'\u280b'; break;
    case bsrCellKind::kDots34      = L'\u280c'; break;
    case bsrCellKind::kDots134     = L'\u280d'; break;
    case bsrCellKind::kDots234     = L'\u280e'; break;
    case bsrCellKind::kDots1234    = L'\u280f'; break;

    case bsrCellKind::kDots5       = L'\u2810'; break;
    case bsrCellKind::kDots15      = L'\u2811'; break;
    case bsrCellKind::kDots25      = L'\u2812'; break;
    case bsrCellKind::kDots125     = L'\u2813'; break;
    case bsrCellKind::kDots35      = L'\u2814'; break;
    case bsrCellKind::kDots135     = L'\u2815'; break;
    case bsrCellKind::kDots235     = L'\u2816'; break;
    case bsrCellKind::kDots1235    = L'\u2817'; break;
    case bsrCellKind::kDots45      = L'\u2818'; break;
    case bsrCellKind::kDots145     = L'\u2819'; break;
    case bsrCellKind::kDots245     = L'\u281a'; break;
    case bsrCellKind::kDots1245    = L'\u281b'; break;
    case bsrCellKind::kDots345     = L'\u281c'; break;
    case bsrCellKind::kDots1345    = L'\u281d'; break;
    case bsrCellKind::kDots2345    = L'\u281e'; break;
    case bsrCellKind::kDots12345   = L'\u281f'; break;

    case bsrCellKind::kDots6       = L'\u2820'; break;
    case bsrCellKind::kDots16      = L'\u2821'; break;
    case bsrCellKind::kDots26      = L'\u2822'; break;
    case bsrCellKind::kDots126     = L'\u2823'; break;
    case bsrCellKind::kDots36      = L'\u2824'; break;
    case bsrCellKind::kDots136     = L'\u2825'; break;
    case bsrCellKind::kDots236     = L'\u2826'; break;
    case bsrCellKind::kDots1236    = L'\u2827'; break;
    case bsrCellKind::kDots46      = L'\u2828'; break;
    case bsrCellKind::kDots146     = L'\u2829'; break;
    case bsrCellKind::kDots246     = L'\u282a'; break;
    case bsrCellKind::kDots1246    = L'\u282b'; break;
    case bsrCellKind::kDots346     = L'\u282c'; break;
    case bsrCellKind::kDots1346    = L'\u282d'; break;
    case bsrCellKind::kDots2346    = L'\u282e'; break;
    case bsrCellKind::kDots12346   = L'\u282f'; break;

    case bsrCellKind::kDots56      = L'\u2830'; break;
    case bsrCellKind::kDots156     = L'\u2831'; break;
    case bsrCellKind::kDots256     = L'\u2832'; break;
    case bsrCellKind::kDots1256    = L'\u2833'; break;
    case bsrCellKind::kDots356     = L'\u2834'; break;
    case bsrCellKind::kDots1356    = L'\u2835'; break;
    case bsrCellKind::kDots2356    = L'\u2836'; break;
    case bsrCellKind::kDots12356   = L'\u2837'; break;
    case bsrCellKind::kDots456     = L'\u2838'; break;
    case bsrCellKind::kDots1456    = L'\u2839'; break;
    case bsrCellKind::kDots2456    = L'\u283a'; break;
    case bsrCellKind::kDots12456   = L'\u283b'; break;
    case bsrCellKind::kDots3456    = L'\u283c'; break;
    case bsrCellKind::kDots13456   = L'\u283d'; break;
    case bsrCellKind::kDots23456   = L'\u283e'; break;
    case bsrCellKind::kDots123456  = L'\u283f'; break;

    default: result = "Dot6???";
  } // switch
  */

//______________________________________________________________________________
/*
string bsrCellKindAsShortString (bsrCellKind cellKind)
{
  string result;

  switch (cellKind) {

  // lower case letters
  case kCellA: result = "kCellA"; break; // bsrCellKind::kDots1,
  case kCellB: result = "kCellB"; break; // bsrCellKind::kDots12,
  case kCellC: result = "kCellC"; break; // bsrCellKind::kDots14,
  case kCellD: result = "kCellD"; break; // bsrCellKind::kDots145,
  case kCellE: result = "kCellE"; break; // bsrCellKind::kDots15,
  case kCellF: result = "kCellF"; break; // bsrCellKind::kDots124,
  case kCellG: result = "kCellG"; break; // bsrCellKind::kDots1245,
  case kCellH: result = "kCellH"; break; // bsrCellKind::kDots125,
  case kCellI: result = "kCellI"; break; // bsrCellKind::kDots24,
  case kCellJ: result = "kCellJ"; break; // bsrCellKind::kDots245,

  case kCellK: result = "kCellK"; break; // bsrCellKind::kDots13,
  case kCellL: result = "kCellL"; break; // bsrCellKind::kDots123,
  case kCellM: result = "kCellM"; break; // bsrCellKind::kDots134,
  case kCellN: result = "kCellN"; break; // bsrCellKind::kDots1345,
  case kCellO: result = "kCellO"; break; // bsrCellKind::kDots135,
  case kCellP: result = "kCellP"; break; // bsrCellKind::kDots1234,
  case kCellQ: result = "kCellQ"; break; // bsrCellKind::kDots12345,
  case kCellR: result = "kCellR"; break; // bsrCellKind::kDots1235,
  case kCellS: result = "kCellS"; break; // bsrCellKind::kDots234,
  case kCellT: result = "kCellT"; break; // bsrCellKind::kDots2345,

  case kCellU: result = "kCellU"; break; // bsrCellKind::kDots136,
  case kCellV: result = "kCellV"; break; // bsrCellKind::kDots1236,
  case kCellW: result = "kCellW"; break; // bsrCellKind::kDots2456,
  case kCellX: result = "kCellX"; break; // bsrCellKind::kDots1346,
  case kCellY: result = "kCellY"; break; // bsrCellKind::kDots13456,
  case kCellZ: result = "kCellZ"; break; // bsrCellKind::kDots1356;

  // lower decimal digits
  case kCellLower1: result = "kCellLower1"; break; // bsrCellKind::kDots2,
  case kCellLower2: result = "kCellLower2"; break; // bsrCellKind::kDots23,
  case kCellLower3: result = "kCellLower3"; break; // bsrCellKind::kDots25,
  case kCellLower4: result = "kCellLower4"; break; // bsrCellKind::kDots256,
  case kCellLower5: result = "kCellLower5"; break; // bsrCellKind::kDots26,
  case kCellLower6: result = "kCellLower6"; break; // bsrCellKind::kDots235,
  case kCellLower7: result = "kCellLower7"; break; // bsrCellKind::kDots2356,
  case kCellLower8: result = "kCellLower8"; break; // bsrCellKind::kDots236,
  case kCellLower9: result = "kCellLower9"; break; // bsrCellKind::kDots35,
  case kCellLower0: result = "kCellLower0"; break; // bsrCellKind::kDots356;

  // arithmetic operators
  case kCell_ac_plus     : result = "FOO"; break; // bsrCellKind::kDots235,
  case kCell_ac_minus    : result = "FOO"; break; // bsrCellKind::kDots36,
  case kCell_ac_times    : result = "FOO"; break; // bsrCellKind::kDots35,
  case kCell_ac_dividedBy: result = "FOO"; break; // bsrCellKind::kDots25,
  case kCell_ac_equals   : result = "FOO"; break; // bsrCellKind::kDots2356;

  // punctuation
  case kCellDot             : result = "FOO"; break; // bsrCellKind::kDots256,
  case kCellComma           : result = "FOO"; break; // bsrCellKind::kDots2,
  case kCellQuestionMark    : result = "FOO"; break; // bsrCellKind::kDots26,
  case kCellSemicolon       : result = "FOO"; break; // bsrCellKind::kDots23,
  case kCellColon           : result = "FOO"; break; // bsrCellKind::kDots25,
  case kCellExclamationMark : result = "FOO"; break; // bsrCellKind::kDots235,
  case kCellLeftParenthesis : result = "FOO"; break; // bsrCellKind::kDots236,
  case kCellRightParenthesis: result = "FOO"; break; // bsrCellKind::kDots356,
  case kCellDoubleQuote     : result = "FOO"; break; // bsrCellKind::kDots2356,
  case kCellDash            : result = "FOO"; break; // bsrCellKind::kDots36,
  case kCellQuote           : result = "FOO"; break; // bsrCellKind::kDots3;

  // other symbols
  case kCellSlash   : result = "FOO"; break; // bsrCellKind::kDots34,
  case kCellVerseEnd: result = "FOO"; break; // bsrCellKind::kDots345,
  case kCellItalics : result = "FOO"; break; // bsrCellKind::kDots456,
  case kCellAsterisk: result = "FOO"; break; // bsrCellKind::kDots35,
  case kCellExponent: result = "FOO"; break; // bsrCellKind::kDots4;

  // intervals
  case kCellSecond : result = "kCellSecond"; break; // bsrCellKind::kDots34,
  case kCellThird  : result = "kCellThird"; break; // bsrCellKind::kDots346,
  case kCellFourth : result = "kCellFourth"; break; // bsrCellKind::kDots3456,
  case kCellFifth  : result = "kCellFifth"; break; // bsrCellKind::kDots35,
  case kCellSixth  : result = "kCellSixth"; break; // bsrCellKind::kDots356,
  case kCellSeventh: result = "kCellSeventh"; break; // bsrCellKind::kDots25,
  case kCellEighth : result = "kCellEighth"; break; // bsrCellKind::kDots36;

  // triplets
  case kCellTriplet : result = "kCellTriplet"; break; // bsrCellKind::kDots23;

  // hyphens
  case kCellMusicHyphen: result = "kCellMusicHyphen"; break; // bsrCellKind::kDots5;

  // keyboard hands
  case kCellRightHand: result = "FOO"; break; // { bsrCellKind::kDots46,  bsrCellKind::kDots345 },
  case kCellLeftHand: result = "FOO"; break; //  { bsrCellKind::kDots456, bsrCellKind::kDots345 };

  // bars
  case kCellFinalDoubleBar    : result = "kCellFinalDoubleBar"; break; //     { bsrCellKind::kDots126, bsrCellKind::kDots13 },
  case kCellSectionalDoubleBar: result = "kCellSectionalDoubleBar"; break; // { bsrCellKind::kDots126, bsrCellKind::kDots13, bsrCellKind::kDots3 };

  // measure divisions
  case kCellMeasureDivisionSign: result = "FOO"; break; // { bsrCellKind::kDots46, bsrCellKind::kDots13 };

  // words
  case kCellWordSign      : result = "kCellWordSign"; break; // bsrCellKind::kDots345,
  case kCellWordApostrophe: result = "kCellWordApostrophe"; break; // bsrCellKind::kDots6;

  // pages
  case kCellPagination: result = "kCellPagination"; break; // { bsrCellKind::kDots5, bsrCellKind::kDots25 };

  // parentheses
  case kCellLiteraryLeftParenthesis  : result = "kCellLiteraryLeftParenthesis"; break; //{ bsrCellKind::kDots5, bsrCellKind::kDots126 },
  case kCellLiteraryRightParenthesis : result = "kCellLiteraryRightParenthesis"; break; //{ bsrCellKind::kDots5, bsrCellKind::kDots345 },
  case kCellMusicParentheses         : result = "kCellMusicParentheses"; break; //{ bsrCellKind::kDots6, bsrCellKind::kDots3 },
  case kCellSpecialParentheses       : result = "kCellSpecialParentheses"; break; //{ bsrCellKind::kDots2356, bsrCellKind::kDots2356 };

  case kCellParagraph           : result = "FOO"; break; //{ bsrCellKind::kDots5, bsrCellKind::kDots1234 },
  case kCellAmpersand           : result = "FOO"; break; //{ bsrCellKind::kDots5, bsrCellKind::kDots123456 },
  case kCellUpsilon             : result = "FOO"; break; //{ bsrCellKind::kDots45, bsrCellKind::kDots13456 }: result = "FOO"; break; // better name JMI ???
  case kCellEuro                : result = "FOO"; break; //{ bsrCellKind::kDots45, bsrCellKind::kDots15 },
  case kCellDollar              : result = "FOO"; break; //{ bsrCellKind::kDots45, bsrCellKind::kDots234 },
  case kCellPound               : result = "FOO"; break; //{ bsrCellKind::kDots45, bsrCellKind::kDots123 },
  case kCellCopyright           : result = "FOO"; break; //{ bsrCellKind::kDots5, bsrCellKind::kDots14 },
  case kCellRegisteredTradeMark : result = "FOO"; break; //{ bsrCellKind::kDots5, bsrCellKind::kDots1235 },
  case kCellTradeMark           : result = "FOO"; break; //{ bsrCellKind::kDots5, bsrCellKind::kDots2345 },
  case kCellPercent             : result = "FOO"; break; //{ bsrCellKind::kDots5, bsrCellKind::kDots346 },
  case kCellPerthousand         : result = "FOO"; break; //{ bsrCellKind::kDots5, bsrCellKind::kDots346, bsrCellKind::kDots346 },
  case kCellPertenthousand      : result = "FOO"; break; //{ bsrCellKind::kDots5, bsrCellKind::kDots346, bsrCellKind::kDots346, bsrCellKind::kDots346 };

  // fermatas
  case kCellFermataOnANote      : result = "FOO"; break; //{ bsrCellKind::kDots146, bsrCellKind::kDots126, bsrCellKind::kDots123 },
  case kCellFermataBetweenNotes : result = "FOO"; break; //{ bsrCellKind::kDots5,   bsrCellKind::kDots126, bsrCellKind::kDots123 },
  case kCellFermataOverABarline : result = "FOO"; break; //{ bsrCellKind::kDots456, bsrCellKind::kDots126, bsrCellKind::kDots123 };
  } // switch

  return result;
}


string bsrCellKindAsString (bsrCellKind cellKind)
{
  return bsrCellKindAsShortString (cellKind);
}
*/

