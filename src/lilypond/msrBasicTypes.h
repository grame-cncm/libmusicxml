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


namespace MusicXML2 
{

// beat data
//______________________________________________________________________________
class msrBeatData
{
  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);
 
  public:
  
    // fields
    // ------------------------------------------------------

    string                fBeatUnit;
    int                   fDots;
};

// diatonic pitches
//______________________________________________________________________________
enum msrDiatonicPitchKind {
  // starting at C for LilyPond relative octave calculations
  kC, kD, kE, kF, kG, kA, kB,
  k_NoDiatonicPitch };

msrDiatonicPitchKind msrDiatonicPitchKindFromString (
  char diatonicNoteName);
  
string msrDiatonicPitchKindAsString (
  msrDiatonicPitchKind diatonicPitchKind);

// alterations
//______________________________________________________________________________
enum msrAlterationKind {
  k_NoAlteration,
  
  kDoubleFlat, kSesquiFlat, kFlat, kSemiFlat,
  kNatural,
  kSemiSharp, kSharp, kSesquiSharp, kDoubleSharp };

msrAlterationKind msrAlterationKindFromMusicXMLAlter (
  float alter);

string msrAlterationKindAsString (
  msrAlterationKind alterationKind);

// intervals
//______________________________________________________________________________
enum msrIntervalKind {
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

int msrIntervalKindAsSemiTones (
  msrIntervalKind intervalKind);
  
int msrIntervalKindAsQuarterTones (
  msrIntervalKind intervalKind);
  
string msrIntervalKindAsString (
  msrIntervalKind intervaKindl);
      
string msrIntervalKindAsShortString (
  msrIntervalKind intervalKind);

msrIntervalKind invertIntervalKind (
  msrIntervalKind intervalKind);

// harmonies
//______________________________________________________________________________
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

string msrHarmonyKindAsString (
  msrHarmonyKind harmonyKind);

string msrHarmonyKindAsShortString (
  msrHarmonyKind harmonyKind);

// constant
const int K_HARMONY_NO_INVERSION = -1;

// quarter tones pitches
//______________________________________________________________________________
enum msrQuarterTonesPitchKind {
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

void setDiatonicPitchKindAndAlterationKind (
  msrQuarterTonesPitchKind quarterTonesPitchKind,
  msrDiatonicPitchKind&    diatonicPitchKind,
  msrAlterationKind&       alterationKind);

msrQuarterTonesPitchKind quarterTonesPitchKindFromDiatonicPitchAndAlteration (
  int                  inputLineNumber,
  msrDiatonicPitchKind diatonicPitchKind,
  msrAlterationKind    alterationKind);

msrDiatonicPitchKind diatonicPitchKindFromQuarterTonesPitchKind (
  int                      inputLineNumber,
  msrQuarterTonesPitchKind quarterTonesPitchKind);

// quarter tones pitches languages
//______________________________________________________________________________
enum msrQuarterTonesPitchesLanguageKind {
  kNederlands, kCatalan, kDeutsch, kEnglish, kEspanol, kFrancais, 
  kItaliano, kNorsk, kPortugues, kSuomi, kSvenska, kVlaams };
  
string msrQuarterTonesPitchesLanguageKindAsString (
  msrQuarterTonesPitchesLanguageKind languageKind);

string msrDiatonicPitchKindAsString ( // JMI
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrDiatonicPitchKind               diatonicPitchKind);

string msrQuarterTonesPitchKindAsString (
  msrQuarterTonesPitchesLanguageKind languageKind,
  msrQuarterTonesPitchKind           quarterTonesPitchKind);

// semi tones pitches
//______________________________________________________________________________
enum msrSemiTonesPitchKind {
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
  
string msrSemiTonesPitchKindAsString (
  msrSemiTonesPitchKind semiTonesPitchKind);

enum msrAlterationPreferenceKind {
  kPreferSharp, kPreferFlat };
  
msrQuarterTonesPitchKind msrSemiTonesPitchKindAsQuarterTonesPitchKind (
  msrSemiTonesPitchKind       semiTonesPitchKind,
  msrAlterationPreferenceKind alterationPreferenceKind);

// fonts
//______________________________________________________________________________
class msrFontSize : public smartable
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrFontSizeKind {
      k_NoFontSize,
      kXXSmallFontSize, kXSmallFontSize, kSmallFontSize,
      kMediumFontSize,
      kLargeFontSize, kXLargeFontSize, kXXLargeFontSize,
      kNumericFontSize };
    
    static string fontSizeKindAsString (
      msrFontSizeKind fontSizeKind);
  
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrFontSize> create (
      msrFontSizeKind fontSizeKind);

    static SMARTP<msrFontSize> create (
      float fontNumericSize);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrFontSize (
      msrFontSizeKind fontSizeKind);
      
    msrFontSize (
      float numericFontSize);
      
    virtual ~msrFontSize ();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrFontSizeKind       getFontSizeKind () const
                              { return fFontSizeKind; }

    float                 getFontNumericSize ();

    // services
    // ------------------------------------------------------

    string                fontSizeKindAsString () const;

    string                fontSizeAsString () const;

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    msrFontSizeKind       fFontSizeKind;

    float                 fFontNumericSize;
                            // only relevant when
                            // fFontSizeKind == kNumericFontSize
};
typedef SMARTP<msrFontSize> S_msrFontSize;
EXP ostream& operator<< (ostream& os, const S_msrFontSize& elt);

enum msrFontStyleKind {
  k_NoFontStyle,
  kNormalFontStyle, KItalicFontStyle };

string msrFontStyleKindAsString (
  msrFontStyleKind fontStyleKind);

enum msrFontWeightKind {
  k_NoFontWeight,
  kNormalFontWeight, kBoldFontWeight };

string msrFontWeightKindAsString (
  msrFontWeightKind tontWeightKind);

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
enum msrDirectionKind {
  k_NoDirection,
  kUpDirection, kDownDirection };

string msrDirectionKindAsString (
  msrDirectionKind directionKind);

// placement
//______________________________________________________________________________
enum msrPlacementKind {
  k_NoPlacement,
  kAbovePlacement, kBelowPlacement };

string msrPlacementKindAsString (
  msrPlacementKind placementKind);

// durations
//______________________________________________________________________________
enum msrDurationKind {
  // from longest to shortest for the algorithms
  kMaxima, kLong, kBreve, kWhole, kHalf, 
  kQuarter,
  kEighth, k16th, k32nd, k64th, k128th, k256th, k512th, k1024th,
  k_NoDuration };

rational msrDurationKindAsWholeNotes (msrDurationKind durationKind);

msrDurationKind wholeNotesAsDurationKind (rational wholeNotes);

string msrDurationKindAsString (msrDurationKind durationKind);

// whole notes
//______________________________________________________________________________
string wholeNotesAsMsrString (
  int      inputLineNumber,
  rational wholeNotes,
  int&     dotsNumber);

string wholeNotesAsMsrString (
  int      inputLineNumber,
  rational wholeNotes);

string multipleRestWholeNoteAsMsrString (
  int      inputLineNumber, // JMI
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

//______________________________________________________________________________
enum msrLineTypeKind {
  kLineTypeSolid, kLineTypeDashed,
  kLineTypeDotted, kLineTypeWavy };

string msrLineTypeKindAsString (
  msrLineTypeKind lineTypeKind);
            
//______________________________________________________________________________
enum msrTremoloTypeKind {
  k_NoTremoloType,
  kTremoloTypeSingle, kTremoloTypeStart, kTremoloTypeStop };

string msrTremoloTypeKindAsString (
  msrTremoloTypeKind tremoloTypeKind);
            
//______________________________________________________________________________
enum msrTechnicalTypeKind { // JMI ???
  k_NoTechnicalType,
  kTechnicalTypeStart, kTechnicalTypeStop };

string msrTechnicalTypeKindAsString (
  msrTechnicalTypeKind technicalTypeKind);
            
//______________________________________________________________________________
enum msrSpannerTypeKind {
  k_NoSpannerType,
  kSpannerTypeStart, kSpannerTypeContinue, kSpannerTypeStop };

string msrSpannerTypeKindAsString (
  msrSpannerTypeKind spannerTypeKind);
            
//______________________________________________________________________________
class msrChordItem : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrChordItem> create (
      int             inputLineNumber,
      int             chordItemNumber,
      msrIntervalKind chordItemIntervalKind);

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
      int             inputLineNumber,
      int             chordItemNumber,
      msrIntervalKind chordItemIntervalKind);

    virtual ~msrChordItem();
  
  public:

    // set and get
    // ------------------------------------------------------

    int                   getChordItemNumber () const
                              { return fChordItemNumber; }
                              
    msrIntervalKind       getChordItemIntervalKind () const
                              { return fChordItemIntervalKind; }
                              
    // services
    // ------------------------------------------------------

    int                   chordItemAsSemitones () const
                              {
                                return
                                  msrIntervalKindAsSemiTones (
                                    fChordItemIntervalKind);
                              }
    
    string                chordItemIntervalKindAsString () const;
    string                chordItemIntervalKindAsShortString () const;
    
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
    msrIntervalKind       fChordItemIntervalKind;
};
typedef SMARTP<msrChordItem> S_msrChordItem;
EXP ostream& operator<< (ostream& os, const S_msrChordItem& elt);

//______________________________________________________________________________
class msrChordIntervals : public smartable
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
                            S_msrChordItem chordItem);
        
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

//______________________________________________________________________________
// global variable 
extern map<msrHarmonyKind, S_msrChordIntervals>
  gChordIntervalsMap;

// global variables
//______________________________________________________________________________

extern map<string, msrQuarterTonesPitchesLanguageKind>
  gQuarterTonesPitchesLanguageKindsMap;

extern map<msrQuarterTonesPitchKind, string> gNederlandsPitchName;
extern map<msrQuarterTonesPitchKind, string> gCatalanPitchName;
extern map<msrQuarterTonesPitchKind, string> gDeutschPitchName;
extern map<msrQuarterTonesPitchKind, string> gEnglishPitchName;
extern map<msrQuarterTonesPitchKind, string> gEspanolPitchName;
extern map<msrQuarterTonesPitchKind, string> gFrancaisPitchName;
extern map<msrQuarterTonesPitchKind, string> gItalianoPitchName;
extern map<msrQuarterTonesPitchKind, string> gNorskPitchName;
extern map<msrQuarterTonesPitchKind, string> gPortuguesPitchName;
extern map<msrQuarterTonesPitchKind, string> gSuomiPitchName;
extern map<msrQuarterTonesPitchKind, string> gSvenskaPitchName;
extern map<msrQuarterTonesPitchKind, string> gVlaamsPitchName;

string existingQuarterTonesPitchesLanguageKinds ();

//______________________________________________________________________________
// tools
extern void initializeChordIntervalsMap ();

extern void printChordIntervalsMap ();

// initialization
//______________________________________________________________________________
void initializeQuarterTonesPitchesLanguageKinds ();


} // namespace MusicXML2


#endif
