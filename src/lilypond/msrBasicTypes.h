/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msrMsrBasicTypes__
#define __msrMsrBasicTypes__

#include <string>
#include <map>


using namespace std;

namespace MusicXML2 
{

// notes names and chords languages
// ------------------------------------------------------

/*
  The alter element represents chromatic alteration
  in number of semitones (e.g., -1 for flat, 1 for sharp).
  Decimal values like 0.5 (quarter tone sharp) are used for microtones.

  The following table lists note names for quarter-tone accidentals
  in various languages; here the pre- fixes semi- and sesqui-
  respectively mean ‘half’ and ‘one and a half’.
  
  Languages that do not appear in this table do not provide special note names yet.
  
  Language    semi-sharp semi-flat sesqui-sharp sesqui-flat
  --------    ---------- --------- ------------ -----------
                 +0.5      -0.5        +1.5       -1.5
  nederlands     -ih       -eh        -isih       -eseh

  We use dutch pitches names for the enumeration below.
  The following is a series of Cs with increasing pitches:
    \relative c'' { ceseh ces ceh c cih cis cisih }

  Given the duration of a note and the divisions attribute, a program can usually infer the symbolic note type (e.g. quarter note, dotted-eighth note). However, it is much easier for notation programs if this is represented explicitly, rather than making the program infer the correct symbolic value. In some cases, the intended note duration does not match what is written, be it some of Bach’s dotted notations, notes inégales, or jazz swing rhythms.
  
  The type element is used to indicate the symbolic note type, such as quarter, eighth, or 16th. MusicXML symbolic note types range from 256th notes to long notes: 256th, 128th, 64th, 32nd, 16th, eighth, quarter, half, whole, breve, and long. The type element may be followed by one or more empty dot elements to indicate dotted notes.
*/

// diatonic pitches
//______________________________________________________________________________
enum msrDiatonicPitch {
  // starting at C for LilyPond relative octave calculations
  kC, kD, kE, kF, kG, kA, kB,
  k_NoDiatonicPitch};

msrDiatonicPitch msrDiatonicPitchFromString (
  char diatonicNoteName);
  
string msrDiatonicPitchAsString (
  msrDiatonicPitch diatonicPitch);

// alterations
//______________________________________________________________________________
enum msrAlteration {
  k_NoAlteration,
  
  kDoubleFlat, kSesquiFlat, kFlat, kSemiFlat,
  kNatural,
  kSemiSharp, kSharp, kSesquiSharp, kDoubleSharp};

msrAlteration msrAlterationFromMusicXMLAlter (
  float alter);

string msrAlterationAsString (
  msrAlteration alteration);

// quarter tones pitches
//______________________________________________________________________________
enum msrQuarterTonesPitch {
  k_NoQuarterTonesPitch,

  k_Rest,
  
  k_aDoubleFlat, k_aSesquiFlat, k_aFlat, k_aSemiFlat,
  k_aNatural,
  k_aSemiSharp, k_aSharp, k_aSesquiSharp, k_aDoubleSharp,
  
  k_bDoubleFlat, k_bSesquiFlat, k_bFlat, k_bSemiFlat,
  k_bNatural,
  k_bSemiSharp, k_bSharp, k_bSesquiSharp, k_bDoubleSharp,
  
  k_cDoubleFlat, k_cSesquiFlat, k_cFlat, k_cSemiFlat,
  k_cNatural,
  k_cSemiSharp, k_cSharp, k_cSesquiSharp, k_cDoubleSharp,
  
  k_dDoubleFlat, k_dSesquiFlat, k_dFlat, k_dSemiFlat,
  k_dNatural,
  k_dSemiSharp, k_dSharp, k_dSesquiSharp, k_dDoubleSharp,
  
  k_eDoubleFlat, k_eSesquiFlat, k_eFlat, k_eSemiFlat,
  k_eNatural,
  k_eSemiSharp, k_eSharp, k_eSesquiSharp, k_eDoubleSharp,
  
  k_fDoubleFlat, k_fSesquiFlat, k_fFlat, k_fSemiFlat,
  k_fNatural,
  k_fSemiSharp, k_fSharp, k_fSesquiSharp, k_fDoubleSharp,
  
  k_gDoubleFlat, k_gSesquiFlat, k_gFlat, k_gSemiFlat,
  k_gNatural,
  k_gSemiSharp, k_gSharp, k_gSesquiSharp, k_gDoubleSharp};

void setDiatonicPitchAndAlteration (
  msrQuarterTonesPitch quarterTonesPitch,
  msrDiatonicPitch&    diatonicPitch,
  msrAlteration&       alteration);

msrQuarterTonesPitch quarterTonesPitchFromDiatonicPitchAndAlteration (
  int              inputLineNumber,
  msrDiatonicPitch diatonicPitch,
  msrAlteration    alteration);

msrDiatonicPitch msrDiatonicPitchFromQuarterTonesPitch (
  int                  inputLineNumber,
  msrQuarterTonesPitch quarterTonesPitch);

// quarter tones pitches languages
//______________________________________________________________________________
enum msrQuarterTonesPitchesLanguage {
  kNederlands, kCatalan, kDeutsch, kEnglish, kEspanol, kFrancais, 
  kItaliano, kNorsk, kPortugues, kSuomi, kSvenska, kVlaams};
  
string msrQuarterTonesPitchesLanguageAsString (
  msrQuarterTonesPitchesLanguage language);

string msrDiatonicPitchAsString ( // JMI
  msrQuarterTonesPitchesLanguage language,
  msrDiatonicPitch              diatonicPitch);

string msrQuarterTonesPitchAsString (
  msrQuarterTonesPitchesLanguage language,
  msrQuarterTonesPitch          quarterTonesPitch);

// global variables
//______________________________________________________________________________

extern map<string, msrQuarterTonesPitchesLanguage>
  gQuarterTonesPitchesLanguagesMap;

extern map<msrQuarterTonesPitch, string> gNederlandsPitchName;
extern map<msrQuarterTonesPitch, string> gCatalanPitchName;
extern map<msrQuarterTonesPitch, string> gDeutschPitchName;
extern map<msrQuarterTonesPitch, string> gEnglishPitchName;
extern map<msrQuarterTonesPitch, string> gEspanolPitchName;
extern map<msrQuarterTonesPitch, string> gFrancaisPitchName;
extern map<msrQuarterTonesPitch, string> gItalianoPitchName;
extern map<msrQuarterTonesPitch, string> gNorskPitchName;
extern map<msrQuarterTonesPitch, string> gPortuguesPitchName;
extern map<msrQuarterTonesPitch, string> gSuomiPitchName;
extern map<msrQuarterTonesPitch, string> gSvenskaPitchName;
extern map<msrQuarterTonesPitch, string> gVlaamsPitchName;

string existingQuarterTonesPitchesLanguages ();

// initialization
//______________________________________________________________________________
void initializePitchesLanguages ();

// durations
//______________________________________________________________________________
enum msrDuration {
  // from longest to shortest for the algorithms
  kMaxima, kLong, kBreve, kWhole, kHalf, 
  kQuarter,
  kEighth, k16th, k32nd, k64th, k128th, k256th, k512th, k1024th,
  k_NoDuration};

string msrDurationAsString (msrDuration duration);


/*! @} */

}


#endif
