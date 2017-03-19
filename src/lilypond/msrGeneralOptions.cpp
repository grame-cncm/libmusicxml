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
#include <list>
#include <algorithm>
#include <iomanip>      // setw, set::precision, ...

#include "msr.h"

using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
// global variables


//_______________________________________________________________________________
/*
void initializeStringToMsrNoteNamesLanguage ()
{
  gMsrNoteNamesLanguageMap ["dutch"]     = kNederlands;
  gMsrNoteNamesLanguageMap ["catalan"]   = kCatalan;
  gMsrNoteNamesLanguageMap ["deutsch"]   = kDeutsch;
  gMsrNoteNamesLanguageMap ["english"]   = kEnglish;
  gMsrNoteNamesLanguageMap ["espanol"]   = kEspanol;
  gMsrNoteNamesLanguageMap ["italiano"]  = kItaliano;
  gMsrNoteNamesLanguageMap ["francais"]  = kFrancais;
  gMsrNoteNamesLanguageMap ["norsk"]     = kNorsk;
  gMsrNoteNamesLanguageMap ["portugues"] = kPortugues;
  gMsrNoteNamesLanguageMap ["suomi"]     = kSuomi;
  gMsrNoteNamesLanguageMap ["svenska"]   = kSvenska;
  gMsrNoteNamesLanguageMap ["vlaams"]    = kVlaams;
}

msrNoteNamesLanguage getMsrNoteNamesLanguage (string lang)
{
  return gMsrNoteNamesLanguageMap [lang];
}
*/

string msrNoteData::diatonicPitchAsString (
  msrDiatonicPitch diatonicPitch)
{
  string result;
  
  switch (diatonicPitch) {
    case msrNoteData::kA: result = "a"; break;
    case msrNoteData::kB: result = "b"; break;
    case msrNoteData::kC: result = "c"; break;
    case msrNoteData::kD: result = "d"; break;
    case msrNoteData::kE: result = "e"; break;
    case msrNoteData::kF: result = "f"; break;
    case msrNoteData::kG: result = "g"; break;
    default:              result = "?";
  } // switch

  return result;
}

msrNoteData::msrAlterationKind msrNoteData::alterationFromAlter (
  int   inputLineNumber,
  float alter)
{

/*
  The alter element represents chromatic alteration
  in number of semitones (e.g., -1 for flat, 1 for sharp).
  Decimal values like 0.5 (quarter tone sharp) are used for microtones:

  semi-sharp semi-flat sesqui-sharp sesqui-flat double-sharp double-flat
      +0.5      -0.5        +1.5       -1.5         +2.0        -2.0
*/

  msrNoteData::msrAlterationKind result;
  
  if      (alter == 0 ) {
    result = msrNoteData::kNatural;
  }
  
  else if (alter == -1 ) {
    result = msrNoteData::kFlat;
  }
  
  else if (alter == 1 ) {
    result = msrNoteData::kSharp;
  }
  
  else if (alter == -0.5 ) {
    result = msrNoteData::kSemiFlat;
  }
  
  else if (alter == +0.5 ) {
    result = msrNoteData::kSemiSharp;
  }
  
  else if (alter == -1.5 ) {
    result = msrNoteData::kSesquiFlat;
  }
  
  else if (alter == +1.5 ) {
    result = msrNoteData::kSesquiSharp;
  }
  
  else if (alter == -2 ) {
    result = msrNoteData::kDoubleFlat;
  }
  
  else if (alter == +2 ) {
    result = msrNoteData::kDoubleSharp;
  }
  
  else {
    result = msrNoteData::k_NoAlteration;
    // error message will be issued by caller
/* JMI
    stringstream s;
    
    s <<
      " alter '" << alter <<
      "' should be -2, -1.5, -1, -0.5, 0, +0.5, +1, +1.5 or +2";
      
    msrMusicXMLError (
      inputLineNumber,
      s.str());
      */
  }

  return result;
}

string msrNoteData::alterationKindAsString (
  msrAlterationKind alterationKind)
{
  /*
    The alter element represents chromatic alteration
    in number of semitones (e.g., -1 for flat, 1 for sharp).
    Decimal values like 0.5 (quarter tone sharp) are used for microtones.
  
    We use dutch pitches names for the enumeration below.

    The following is a series of Cs with increasing pitches:
      \relative c'' { ceseh ces ceh c cih cis cisih }

    The following table lists note names for quarter-tone accidentals
    in various languages; here the pre- fixes semi- and sesqui-
    respectively mean ‘half’ and ‘one and a half’.
    
    Languages that do not appear in this table do not provide
    special note names yet.
    
  semi-sharp semi-flat sesqui-sharp sesqui-flat double-sharp double-flat
      +0.5      -0.5        +1.5       -1.5         +2.0        -2.0

nederlands
      -ih        -eh        -isih      -eseh       -isih        -eses
    
  */

  string result;
  
  switch (alterationKind) {

    case msrNoteData::kDoubleFlat:
      result = "eses";
      break;
      
    case msrNoteData::kSesquiFlat:
      result = "eseh";
      break;
      
    case msrNoteData::kFlat:
      result = "es";
      break;
      
    case msrNoteData::kSemiFlat:
      result = "eh";
      break;
      
    case msrNoteData::kNatural:
      break;
      
    case msrNoteData::kSemiSharp:
      result = "ih";
      break;
      
    case msrNoteData::kSharp:
      result = "is";
      break;
      
    case msrNoteData::kSesquiSharp:
      result = "isih";
      break;
      
    case msrNoteData::kDoubleSharp:
      result = "isis";
      break;      
      
    case msrNoteData::k_NoAlteration:
      result = "???";
      break;      
  } // switch  

  return result;
}

//_______________________________________________________________________________
S_msrGeneralOptions msrGeneralOptions::create ()
{
  msrGeneralOptions* o = new msrGeneralOptions();
  assert(o!=0);
  return o;
}

msrGeneralOptions::msrGeneralOptions ()
{
  // interactive mode
  fInteractive = false;
  
  // trace
  fTrace = true;
  
  // debug
  fDebug = false;
  fDebugDebug = false;

  // forcing debug information at specific places in the code
  fForceDebug = false;
  
  // measure number-selective debug
  fSaveDebug = false;
  fSaveDebugDebug = false;
}

msrGeneralOptions::~msrGeneralOptions () {}

S_msrGeneralOptions gGeneralOptions;

}
