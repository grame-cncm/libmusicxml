/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msrBasicTypes__
#define __msrBasicTypes__

#include <vector>
#include <map>

#include "smartpointer.h"

#include "rational.h"


using namespace std;

namespace MusicXML2 
{

/*!
\addtogroup utilities
@{
*/


/*!
\internal
\brief A macro to exit when a condition if false
*/
// a macro to abort with code source line information,
// as assert() itself
//______________________________________________________________________________
#define basicMsrAssert( condition, messageIfFalse ) \
{ \
  if (! condition) { \
    cerr << \
      endl << \
      "#### " << messageIfFalse << \
      endl << endl << \
      flush; \
    assert(condition); \
  } \
}

/*!
\internal
\brief A macro to emit warning messages regarding MusicXML data
*/
//______________________________________________________________________________
#define basicMsrMusicXMLWarning( inputLineNumber, message ) \
{ \
  cerr << endl << endl; \
\
  cerr << \
    "!!! MusicXML WARNING !!!, " << \
    gGeneralOptions->fInputSourceName << \
    ", input line " << inputLineNumber << ":" << \
    endl << \
\
    message << \
    endl << endl; \
}

/*!
\internal
\brief A macro to emit error messages regarding MusicXML data and exit
*/
//______________________________________________________________________________
#define basicMsrMusicXMLError( inputLineNumber, message ) \
{ \
  cerr << endl << endl; \
\
  cerr << \
    "### MusicXML ERROR ###, " << \
    gGeneralOptions->fInputSourceName << \
    ", input line " << inputLineNumber << ":" << \
    endl << \
\
    message << \
    endl << endl << \
    idtr; \
\
  if (! gMusicXMLOptions->fIgnoreMusicXMLErrors) \
    assert(false); \
}

/*!
\internal
\brief A macro to emit error messages regarding MSR and exit
*/
//______________________________________________________________________________
#define basicMsrInternalError( inputLineNumber, message ) \
{ \
  cerr << endl << endl; \
\
  cerr << \
    "[[[ basic INTERNAL ERROR ]]], " << \
 /* JMI   gGeneralOptions->fInputSourceName << */ \
    ", input line " << inputLineNumber << ":" << \
    endl << \
\
    message << \
    endl << endl << \
    idtr; \
\
  assert(false); \
}

/*!
\internal
\brief A macro to emit error messages regarding MSR
*/
//______________________________________________________________________________
#define basicMsrInternalWarning( inputLineNumber, message ) \
{ \
  cerr << endl << endl; \
\
  cerr << \
    "[[[ basic INTERNAL WARNING ]]], " << \
   /* JMI gGeneralOptions->fInputSourceName << */ \
    ", input line " << inputLineNumber << ":" << \
    endl << \
\
    message << \
    endl << endl << \
    idtr; \
}


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
  k_NoDiatonicPitch };

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
  kSemiSharp, kSharp, kSesquiSharp, kDoubleSharp };

msrAlteration msrAlterationFromMusicXMLAlter (
  float alter);

string msrAlterationAsString (
  msrAlteration alteration);

// intervals
//______________________________________________________________________________
enum msrInterval {
  kDiminishedUnisson, kUnisson, kAugmentedUnison,
  
  kDiminishedSecond, kMinorSecond, kMajorSecond, kAugmentedSecond,
  
  kDiminishedThird, kMinorThird, kMajorThird, kAugmentedThird,
  
  kDiminishedFourth, kPerFourth, kAugmentedFourth,
  
  kDiminishedFifth, kPerFifth, kAugmentedFifth,
  
  kDiminishedSixth, kMinorSixth, kMajorSixth, kAugmentedSixth,
  
  kDiminishedSeventh, kMinorSeventh, kMajorSeventh, kAugmentedSeventh,
  
  kDiminishedOctave, kPerOctave, kAugmentedOctave,
  
  kDiminishedNinth, kMinorNinth, kMajorNinth, kAugmentedNinth,
  
  kDiminishedTenth, kMinorTenth, kMajorTenth, kAugmentedTenth,
  
  kDiminishedEleventh, kPerEleventh, kAugmentedEleventh,
  
  kDiminishedTwelfth, kPerTwelfth, kAugmentedTwelfth,
  
  kDiminishedThirteenth, kMinorThirteenth, kMajorThirteenth, kAugmentedThirteenth };

int msrIntervalAsSemiTones (
  msrInterval interval);
  
int msrIntervalAsQuarterTones (
  msrInterval interval);
  
string msrIntervalAsString (
  msrInterval interval);
      
string msrIntervalAsShortString (
  msrInterval interval);

msrInterval invertInterval (
  msrInterval interval);

// harmonies
//______________________________________________________________________________
/*
 kind-value indicates the type of chord. Degree elements can then add, subtract, or alter from these starting points. Values include:
  
Triads:
  major (major third, perfect fifth)
  minor (minor third, perfect fifth)
  augmented (major third, augmented fifth)
  diminished (minor third, diminished fifth)
Sevenths:
  dominant (major triad, minor seventh)
  major-seventh (major triad, major seventh)
  minor-seventh (minor triad, minor seventh)
  diminished-seventh (diminished triad, diminished seventh)
  augmented-seventh (augmented triad, minor seventh)
  half-diminished (diminished triad, minor seventh)
  major-minor (minor triad, major seventh)
Sixths:
  major-sixth (major triad, added sixth)
  minor-sixth (minor triad, added sixth)
Ninths:
  dominant-ninth (dominant-seventh, major ninth)
  major-ninth (major-seventh, major ninth)
  minor-ninth (minor-seventh, major ninth)
11ths (usually as the basis for alteration):
  dominant-11th (dominant-ninth, perfect 11th)
  major-11th (major-ninth, perfect 11th)
  minor-11th (minor-ninth, perfect 11th)
13ths (usually as the basis for alteration):
  dominant-13th (dominant-11th, major 13th)
  major-13th (major-11th, major 13th)
  minor-13th (minor-11th, major 13th)
Suspended:
  suspended-second (major second, perfect fifth)
  suspended-fourth (perfect fourth, perfect fifth)
Functional sixths:
  Neapolitan
  Italian
  French
  German
Other:
  pedal (pedal-point bass)
  power (perfect fifth)
  Tristan
  
The "other" kind is used when the harmony is entirely composed of add elements. The "none" kind is used to explicitly encode absence of chords or functional harmony.
*
*
*
*   Kind indicates the type of chord. Values include:
  
  Triads:
      major (major third, perfect fifth)
      minor (minor third, perfect fifth)
      augmented (major third, augmented fifth)
      diminished (minor third, diminished fifth)
  Sevenths:
      dominant (major triad, minor seventh)
      major-seventh (major triad, major seventh)
      minor-seventh (minor triad, minor seventh)
      diminished-seventh
          (diminished triad, diminished seventh)
      augmented-seventh
          (augmented triad, minor seventh)
      half-diminished
          (diminished triad, minor seventh)
      major-minor
          (minor triad, major seventh)
  Sixths:
      major-sixth (major triad, added sixth)
      minor-sixth (minor triad, added sixth)
  Ninths:
      dominant-ninth (dominant-seventh, major ninth)
      major-ninth (major-seventh, major ninth)
      minor-ninth (minor-seventh, major ninth)
  11ths (usually as the basis for alteration):
      dominant-11th (dominant-ninth, perfect 11th)
      major-11th (major-ninth, perfect 11th)
      minor-11th (minor-ninth, perfect 11th)
  13ths (usually as the basis for alteration):
      dominant-13th (dominant-11th, major 13th)
      major-13th (major-11th, major 13th)
      minor-13th (minor-11th, major 13th)
  Suspended:
      suspended-second (major second, perfect fifth)
      suspended-fourth (perfect fourth, perfect fifth)
  Functional sixths:
      Neapolitan
      Italian
      French
      German
  Other:
      pedal (pedal-point bass)
      power (perfect fifth)
      Tristan
  
  The "other" kind is used when the harmony is entirely
  composed of add elements. The "none" kind is used to
  explicitly encode absence of chords or functional
  harmony.

  For the major-minor kind, only the minor symbol is used when
  use-symbols is yes. The major symbol is set using the symbol
  attribute in the degree-value element. The corresponding
  degree-alter value will usually be 0 in this case.

  The text attribute describes how the kind should be spelled
  in a score. If use-symbols is yes, the value of the text
  attribute follows the symbol. The stack-degrees attribute
  is yes if the degree elements should be stacked above each
  other. The parentheses-degrees attribute is yes if all the
  degrees should be in parentheses. The bracket-degrees 
  attribute is yes if all the degrees should be in a bracket.
  If not specified, these values are implementation-specific.
  The alignment attributes are for the entire harmony-chord
  entity of which this kind element is a part.


<!ELEMENT kind (#PCDATA)>
<!ATTLIST kind
    use-symbols          %yes-no;   #IMPLIED
    text                 CDATA      #IMPLIED
    stack-degrees        %yes-no;   #IMPLIED
    parentheses-degrees  %yes-no;   #IMPLIED
    bracket-degrees      %yes-no;   #IMPLIED
    %print-style;
    %halign;
    %valign;
>

  Inversion is a number indicating which inversion is used:
  0 for root position, 1 for first inversion, etc.
*/

enum msrHarmonyKind {
  k_NoHarmony,
  
  kMajorHarmony, kMinorHarmony, 
  kAugmentedHarmony, kDiminishedHarmony,

  kDominantHarmony,
  kMajorSeventhHarmony, kMinorSeventhHarmony,
  kDiminishedSeventhHarmony, kAugmentedSeventhHarmony,
  kHalfDiminishedHarmony,
  kMajorMinorHarmony,

  kMajorSixthHarmony, kMinorSixthHarmony,

  kDominantNinthHarmony,
  kMajorNinthHarmony, kMinorNinthHarmony,

  kDominantEleventhHarmony, kMajorEleventhHarmony, kMinorEleventhHarmony,

  kDominantThirteenthHarmony, kMajorThirteenthHarmony, kMinorThirteenthHarmony,

  kSuspendedSecondHarmony, kSuspendedFourthHarmony,

  kNeapolitanHarmony, kItalianHarmony, kFrenchHarmony, kGermanHarmony,

  kPedalHarmony, kPowerHarmony, kTristanHarmony,

  kOtherHarmony,
  kNoneHarmony };

string harmonyKindAsString (
  msrHarmonyKind harmonyKind);

string harmonyKindAsShortString (
  msrHarmonyKind harmonyKind);

// constant
#define K_HARMONY_NO_INVERSION -1

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
  k_gSemiSharp, k_gSharp, k_gSesquiSharp, k_gDoubleSharp };

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
  kItaliano, kNorsk, kPortugues, kSuomi, kSvenska, kVlaams };
  
string msrQuarterTonesPitchesLanguageAsString (
  msrQuarterTonesPitchesLanguage language);

string msrDiatonicPitchAsString ( // JMI
  msrQuarterTonesPitchesLanguage language,
  msrDiatonicPitch               diatonicPitch);

string msrQuarterTonesPitchAsString (
  msrQuarterTonesPitchesLanguage language,
  msrQuarterTonesPitch           quarterTonesPitch);

// semi tones pitches
//______________________________________________________________________________
enum msrSemiTonesPitch {
  kC_Natural_STP, kB_Sharp_STP = kC_Natural_STP, kD_DoubleFlat_STP = kC_Natural_STP,
  
  kC_Sharp_STP, kB_DoubleSharp_STP = kC_Sharp_STP, kD_Flat_STP = kC_Sharp_STP,
  
  kD_Natural_STP, kC_DoubleSharp_STP = kD_Natural_STP, kE_DoubleFlat_STP = kD_Natural_STP,
  
  kD_Sharp_STP, kE_Flat_STP = kD_Sharp_STP,
  
  kE_Natural_STP, kD_DoubleSharp_STP = kE_Natural_STP, kF_Flat_STP = kE_Natural_STP,
  
  kF_Natural_STP, kE_Sharp_STP = kF_Natural_STP, kG_DoubleFlat_STP = kF_Natural_STP,
  
  kF_Sharp_STP, kE_DoubleSharp_STP = kF_Sharp_STP, kG_Flat_STP = kF_Sharp_STP,
  
  kG_Natural_STP, kF_DoubleSharp_STP = kG_Natural_STP, kA_DoubleFlat_STP = kG_Natural_STP,
  
  kG_Sharp_STP, kA_Flat_STP = kG_Sharp_STP,
  
  kA_Natural_STP, kG_DoubleSharp_STP = kA_Natural_STP, kB_DoubleFlat_STP = kA_Natural_STP,
  
  kA_Sharp_STP, kB_Flat_STP = kA_Sharp_STP,
  
  kB_Natural_STP, kA_DoubleSharp_STP = kB_Natural_STP, kC_Flat_STP = kB_Natural_STP,
  
  k_NoWelTemperedPitch_STP };
  
string msrSemiTonesPitchAsString (
  msrSemiTonesPitch semiTonesPitch);

enum msrAlterationPreference {
  kPreferSharp, kPreferFlat };
  
msrQuarterTonesPitch msrSemiTonesPitchAsQuarterTonesPitch (
  msrSemiTonesPitch       semiTonesPitch,
  msrAlterationPreference alterationPreference);

// fonts
//______________________________________________________________________________
enum msrFontSize {
  k_NoFontSize,
  kNormalFontSize, kLargeFontSize, kMediumFontSize, kSmallFontSize };

string msrFontSizeAsString (
  msrFontSize fontSize);

enum msrFontStyle {
  k_NoFontStyle,
  kNormalFontStyle, KItalicFontStyle };

string msrFontStyleAsString (
  msrFontStyle fontStyle);

enum msrFontWeight {
  k_NoFontWeight,
  kNormalFontWeight, kBoldFontWeight };

string msrFontWeightAsString (
  msrFontWeight tontWeight);

// alignement
//______________________________________________________________________________
enum msrJustifyKind {
  k_NoJustify,
  kLeftJustify, kCenterJustify, kRightJustify };

string msrJustifyKindAsString (
  msrJustifyKind justifyKind);

enum msrVerticalAlignmentKind {
  k_NoVerticalAlignment,
  kTopVerticalAlignment, kMiddleVerticalAlignment, kBottomVerticalAlignment };

string msrVerticalAlignmentKindAsString (
  msrVerticalAlignmentKind verticalAlignmentKind);

// direction
//______________________________________________________________________________
enum msrDirection{
  k_NoDirection,
  kUpDirection, kDownDirection};

string msrDirectionAsString (
  msrDirection direction);

// placement
//______________________________________________________________________________
enum msrPlacement {
  k_NoPlacement,
  kAbovePlacement, kBelowPlacement };

string msrPlacementAsString (
  msrPlacement placement);

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
void initializeQuarterTonesPitchesLanguages ();

// durations
//______________________________________________________________________________
enum msrDuration {
  // from longest to shortest for the algorithms
  kMaxima, kLong, kBreve, kWhole, kHalf, 
  kQuarter,
  kEighth, k16th, k32nd, k64th, k128th, k256th, k512th, k1024th,
  k_NoDuration };

rational msrDurationAsWholeNotes (msrDuration duration);

string msrDurationAsString (msrDuration duration);

// whole notes
//______________________________________________________________________________
string wholeNotesAsMsrString (
  int      inputLineNumber,
  rational wholeNotes,
  int&     dotsNumber);

string wholeNotesAsMsrString (
  int      inputLineNumber,
  rational wholeNotes);

// measure style
//______________________________________________________________________________

enum msrSlashTypeKind {
  kSlashTypeStart, kSlashTypeStop };

string msrSlashTypeKindAsString (
  msrSlashTypeKind slashTypeKind);

enum msrSlashUseDotsKind {
  kSlashUseDotsYes, kSlashUseDotsNo };

string msrSlashUseDotsKindAsString (
  msrSlashUseDotsKind slashUseDotsKind);

enum msrSlashUseStemsKind {
  kSlashUseStemsYes, kSlashUseStemsNo };

string msrSlashUseStemsKindAsString (
  msrSlashUseStemsKind slashUseStemsKind);

/*!
\brief A msr harmony representation.

  A harmony is represented by a list of syllables,
*/
//______________________________________________________________________________
class EXP msrChordItem : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrChordItem> create (
      int         inputLineNumber,
      int         chordItemNumber,
      msrInterval chordItemInterval);

    /* JMI
    SMARTP<msrChordItem> createChordItemNewbornClone (
      S_msrPart containingPart);

    SMARTP<msrChordItem> createChordItemDeepCopy ( // JMI ???
      S_msrPart containingPart);
      */

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrChordItem (
      int         inputLineNumber,
      int         chordItemNumber,
      msrInterval chordItemInterval);

    virtual ~msrChordItem();
  
  public:

    // set and get
    // ------------------------------------------------------

    int                   getChordItemNumber () const
                              { return fChordItemNumber; }
                              
    msrInterval           getChordItemInterval () const
                              { return fChordItemInterval; }
                              
    // services
    // ------------------------------------------------------

    int                   chordItemAsSemitones () const
                              {
                                return
                                  msrIntervalAsSemiTones (
                                    fChordItemInterval);
                              }
    
    string                chordItemIntervalAsString () const;
    string                chordItemIntervalAsShortString () const;
    
    string                chordItemAsString () const;
   
    // visitors
    // ------------------------------------------------------

/* JMI
    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);
*/

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    int                   fChordItemNumber;
    msrInterval           fChordItemInterval;
};
typedef SMARTP<msrChordItem> S_msrChordItem;
EXP ostream& operator<< (ostream& os, const S_msrChordItem& elt);

/*!
\brief A msr harmony representation.

  A harmony is represented by a list of syllables,
*/
//______________________________________________________________________________
class EXP msrChordIntervals : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrChordIntervals> create (
      int            inputLineNumber,
      msrHarmonyKind chordIntervalsHarmonyKind);

    /* JMI
    SMARTP<msrChordIntervals> createChordIntervalsNewbornClone (
      S_msrPart containingPart);

    SMARTP<msrChordIntervals> createChordIntervalsDeepCopy ( // JMI ???
      S_msrPart containingPart);
      */

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrChordIntervals (
      int            inputLineNumber,
      msrHarmonyKind chordIntervalsHarmonyKind);

    virtual ~msrChordIntervals();
  
  private:

    // initialization
    // ------------------------------------------------------

    void                  initializeChordIntervals ();

  public:

    // set and get
    // ------------------------------------------------------

    msrHarmonyKind        getChordIntervalsHarmonyKind () const
                              { return fChordIntervalsHarmonyKind; }
                              
    const vector <S_msrChordItem>&
                          getChordIntervalsItems () const
                              { return fChordIntervalsItems; }
                              
    // services
    // ------------------------------------------------------

    void                  appendChordItemToChordIntervals (
                            S_msrChordItem chordItem)
                              {
                                fChordIntervalsItems.push_back (
                                  chordItem);
                              }
        
    string                chordIntervalsAsString () const;
    string                chordIntervalsAsShortString () const;
   
    // visitors
    // ------------------------------------------------------
/* JMI
    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);
*/

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    msrHarmonyKind        fChordIntervalsHarmonyKind;
    vector <S_msrChordItem>
                          fChordIntervalsItems;
};
typedef SMARTP<msrChordIntervals> S_msrChordIntervals;
EXP ostream& operator<< (ostream& os, const S_msrChordIntervals& elt);

// global variable 
extern map<msrHarmonyKind, S_msrChordIntervals>
  gChordIntervalsMap;

// tools
extern void initializeChordIntervalsMap ();

extern void printChordIntervalsMap ();


} // namespace MusicXML2


#endif
