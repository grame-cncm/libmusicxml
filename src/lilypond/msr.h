/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msr__
#define __msr__

#include <sstream>
#include <vector>
#include <string>
#include <list>
#include <map>

#include "smartpointer.h"
#include "rational.h"
#include "exports.h"
#include "typedefs.h"

#include "tree_browser.h"

#include "msrUtilities.h"
#include "msrGeneralOptions.h"

//JMI#include "musicxml2msr.h"

namespace MusicXML2 
{

/*!
\addtogroup msr
@{
*/

/*
  The classes in this file implement the
  
              MSR (Music Score Representation)
*/

//______________________________________________________________________________
// PRE-declarations for class dependencies

class msrBarline;
typedef SMARTP<msrBarline> S_msrBarline;

class msrBarCheck;
typedef SMARTP<msrBarCheck> S_msrBarCheck;

class msrBarNumberCheck;
typedef SMARTP<msrBarNumberCheck> S_msrBarNumberCheck;

class msrBreak;
typedef SMARTP<msrBreak> S_msrBreak;

class msrNote;
typedef SMARTP<msrNote> S_msrNote;

class msrMeasureRepeat;
typedef SMARTP<msrMeasureRepeat> S_msrMeasureRepeat;

class msrMultipleRest;
typedef SMARTP<msrMultipleRest> S_msrMultipleRest;

class msrGraceNotes;
typedef SMARTP<msrGraceNotes> S_msrGraceNotes;

class msrAfterGraceNotes;
typedef SMARTP<msrAfterGraceNotes> S_msrAfterGraceNotes;

class msrChord;
typedef SMARTP<msrChord> S_msrChord;

class msrTuplet;
typedef SMARTP<msrTuplet> S_msrTuplet;

class msrStanza;
typedef SMARTP<msrStanza> S_msrStanza;

class msrVoice;
typedef SMARTP<msrVoice> S_msrVoice;

class msrHarmony;
typedef SMARTP<msrHarmony> S_msrHarmony;

class msrSegment;
typedef SMARTP<msrSegment> S_msrSegment;

class msrMeasure;
typedef SMARTP<msrMeasure> S_msrMeasure;

class msrTranspose;
typedef SMARTP<msrTranspose> S_msrTranspose;

class msrStaffDetails;
typedef SMARTP<msrStaffDetails> S_msrStaffDetails;

class msrTempo;
typedef SMARTP<msrTempo> S_msrTempo;

class msrSegno;
typedef SMARTP<msrSegno> S_msrSegno;

class msrCoda;
typedef SMARTP<msrCoda> S_msrCoda;

class msrEyeGlasses;
typedef SMARTP<msrEyeGlasses> S_msrEyeGlasses;

class msrPedal;
typedef SMARTP<msrPedal> S_msrPedal;

class msrStaff;
typedef SMARTP<msrStaff> S_msrStaff;

class msrPart;
typedef SMARTP<msrPart> S_msrPart;

class msrPartGroup;
typedef SMARTP<msrPartGroup> S_msrPartGroup;

class msrScore;
typedef SMARTP<msrScore> S_msrScore;

class msrRepeat;
typedef SMARTP<msrRepeat> S_msrRepeat;


/*!
\internal
\brief A macro to simplify indentation
*/
//______________________________________________________________________________
#define idtr indenter::gIndenter
#define tab  indenter::gIndenter.getSpacer ()

/*!
\internal
\brief A macro to exit when a condition if false
*/
// a macro to abort with code source line information,
// as assert() itself
//______________________________________________________________________________
#define msrAssert( condition, messageIfFalse ) \
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
#define msrMusicXMLWarning( inputLineNumber, message ) \
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
#define msrMusicXMLError( inputLineNumber, message ) \
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
#define msrInternalError( inputLineNumber, message ) \
{ \
  cerr << endl << endl; \
\
  cerr << \
    "[[[ MSR INTERNAL ERROR ]]], " << \
    gGeneralOptions->fInputSourceName << \
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
#define msrInternalWarning( inputLineNumber, message ) \
{ \
  cerr << endl << endl; \
\
  cerr << \
    "[[[ MSR INTERNAL WARNING ]]], " << \
    gGeneralOptions->fInputSourceName << \
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

//______________________________________________________________________________
/*!
  \brief The MSR code generation options.

  A class is used to avoid passing arguments one by one
  to the various methods that need them.
*/

// trace and display

#define _TRACE_MSR_LONG_NAME_  "traceMsr"
#define _TRACE_MSR_SHORT_NAME_ "tmsr"

#define _TRACE_MSR_VISITORS_LONG_NAME_  "traceMsrVisitors"
#define _TRACE_MSR_VISITORS_SHORT_NAME_ "tmvisits"

#define _DISPLAY_MSR_LONG_NAME_  "displayMsr"
#define _DISPLAY_MSR_SHORT_NAME_ "msr"

#define _DISPLAY_MSR_SUMMARY_LONG_NAME_  "displayMsrSummary"
#define _DISPLAY_MSR_SUMMARY_SHORT_NAME_ "sum"

// languages

#define _MSR_PITCHES_LANGUAGE_LONG_NAME_  "msrPitchesLanguage"
#define _MSR_PITCHES_LANGUAGE_SHORT_NAME_ "mpl"

// parts

#define _PART_NAME_LONG_NAME_  "partName"
#define _PART_NAME_SHORT_NAME_ "part"

// voices

#define _STAFF_RELATIVE_VOICE_NUMBERS_LONG_NAME_  "staffRelativeVoiceNumbers"
#define _STAFF_RELATIVE_VOICE_NUMBERS_SHORT_NAME_ "srvn"

#define _SHOW_SILENT_VOICES_LONG_NAME_  "showSilentVoices"
#define _SHOW_SILENT_VOICES_SHORT_NAME_ "ssv"

#define _KEEP_SILENT_VOICES_LONG_NAME_  "keepSilentVoices"
#define _KEEP_SILENT_VOICES_SHORT_NAME_ "ksv"

// notes

#define _DELAY_RESTS_DYNAMICS_LONG_NAME_  "delayRestsDynamics"
#define _DELAY_RESTS_DYNAMICS_SHORT_NAME_ "drdyns"

#define _DELAY_RESTS_WORDS_LONG_NAME_  "delayRestsWords"
#define _DELAY_RESTS_WORDS_SHORT_NAME_ "drwords"

#define _DELAY_RESTS_SLURS_LONG_NAME_  "delayRestsSlurs"
#define _DELAY_RESTS_SLURS_SHORT_NAME_ "drslurs"

#define _DELAY_RESTS_LIGATURES_LONG_NAME_  "delayRestsLigatures"
#define _DELAY_RESTS_LIGATURES_SHORT_NAME_ "drligs"

#define _DELAY_RESTS_WEDGES_LONG_NAME_  "delayRestsWedges"
#define _DELAY_RESTS_WEDGES_SHORT_NAME_ "drwedges"

// lyrics

#define _SHOW_MSR_STANZAS_LONG_NAME_  "showMsrStanzas"
#define _SHOW_MSR_STANZAS_SHORT_NAME_ "sms"

#define _KEEP_MUTE_STANZAS_LONG_NAME_  "keepMuteStanzas"
#define _KEEP_MUTE_STANZAS_SHORT_NAME_ "kms"

// harmonies

#define _SHOW_HARMONY_VOICES_LONG_NAME_  "showHarmonyVoices"
#define _SHOW_HARMONY_VOICES_SHORT_NAME_ "shv"

#define _KEEP_EMPTY_HARMONIES_VOICE_LONG_NAME_  "keepEmptyHarmoniesVoice"
#define _KEEP_EMPTY_HARMONIES_VOICE_SHORT_NAME_ "kehv"


class EXP msrOptions : public smartable
{
  public:

    static SMARTP<msrOptions> create ();
      
    SMARTP<msrOptions>        createCloneWithDetailedTrace ();

  public:

    // initialisation
    // ------------------------------------------------------

    void                  initializeMsrOptions (
                            bool boolOptionsInitialValue);
    
    void                  printMsrOptionsHelp ();

    void                  printMsrOptionsValues (int fieldWidth);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptions();
  
    virtual ~msrOptions();
 
    // set and get
    // ------------------------------------------------------

    bool                  setMsrQuarterTonesPitchesLanguage (
                            string language);    
 
  public:

    // trace and display
    // --------------------------------------
    
    bool                  fTraceMsr;
    
    bool                  fTraceMsrVisitors;
    
    bool                  fDisplayMsr;
    
    bool                  fDisplayMsrSummary;
    
    // languages
    // --------------------------------------
    
    msrQuarterTonesPitchesLanguage
                          fMsrQuarterTonesPitchesLanguage;
    
    // parts
    // --------------------------------------
    
    map<string, string>   fPartsRenaming;

    // voices
    // --------------------------------------
    
    bool                  fCreateStaffRelativeVoiceNumbers;
    
    bool                  fShowSilentVoices;
    bool                  fKeepSilentVoices;

    // notes
    // --------------------------------------
    
    bool                  fDelayRestsDynamics;
    bool                  fDelayRestsWords; // JMI
    bool                  fDelayRestsSlurs; // JMI
    bool                  fDelayRestsLigatures; // JMI
    bool                  fDelayRestsWedges; // JMI

    // lyrics
    // --------------------------------------
    
    bool                  fShowMsrStanzas;
    bool                  fKeepMuteStanzas;
    
    // harmonies
    // --------------------------------------
    
    bool                  fShowHarmonyVoices;
    bool                  fKeepEmptyHarmonyVoices;
};
typedef SMARTP<msrOptions> S_msrOptions;

extern S_msrOptions gMsrOptions;
extern S_msrOptions gMsrOptionsUserChoices;
extern S_msrOptions gMsrOptionsWithDetailedTrace;

/*!
\brief A generic msr element representation.

  An element is represented by its name and the
  list of its enclosed elements plus optional parameters.
*/
//______________________________________________________________________________
class EXP msrElement : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

  protected:

     msrElement (
      int           inputLineNumber);

    virtual ~msrElement();

  public:

    // set and get
    // ------------------------------------------------------

    int getInputLineNumber ()
      { return fInputLineNumber; }

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v) = 0;

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

    virtual void          printStructure (ostream& os) {}
    
  protected:
     
    int            fInputLineNumber;
};
typedef SMARTP<msrElement> S_msrElement;
EXP ostream& operator<< (ostream& os, const S_msrElement& elt);

/*!
\brief A generic msr element representation.

  An element is represented by its name and the
  list of its enclosed elements plus optional parameters.
*/
//______________________________________________________________________________
template <typename T> class EXP msrBrowser : public browser<T> 
{
  public:
    
    msrBrowser (basevisitor* v) : fVisitor (v) {}
    
    virtual ~msrBrowser() {}

  public:

    virtual void set (basevisitor* v) { fVisitor = v; }
    
    virtual void browse (T& t) {
      enter (t);

      t.browseData (fVisitor);
      
      leave (t);
    }

  protected:
  
    basevisitor*  fVisitor;

    virtual void enter (T& t) { t.acceptIn  (fVisitor); }
    virtual void leave (T& t) { t.acceptOut (fVisitor); }
};

/*!
\brief A beat description for MusicXML.
*/
//______________________________________________________________________________
class msrBeatData // JMI ???
{
  public:
  
    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);
 
  public:
  
    string fBeatUnit;
    int    fDots;
};

/*!
\brief A msr stem representation.

  A beam is represented by a msrBeamKind value
*/

//______________________________________________________________________________

class EXP msrOctaveShift : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrOctaveShiftKind {
      kOctaveShiftUp, kOctaveShiftDown,
      kOctaveShiftStop};

    static string octaveShiftKindAsString (
      msrOctaveShiftKind octaveShiftKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrOctaveShift> create (
      int                inputLineNumber,
      msrOctaveShiftKind octaveShiftKind,
      int                octaveShiftSize);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOctaveShift (
      int                inputLineNumber,
      msrOctaveShiftKind octaveShiftKind,
      int                octaveShiftSize);
      
    virtual ~msrOctaveShift();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrOctaveShiftKind    getOctaveShiftKind () const
                              { return fOctaveShiftKind; }

    int                   getOctaveShiftSize () const
                              { return fOctaveShiftSize; }

    // services
    // ------------------------------------------------------

    string                octaveShiftKindAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    msrOctaveShiftKind fOctaveShiftKind;

    int                fOctaveShiftSize;
};
typedef SMARTP<msrOctaveShift> S_msrOctaveShift;
EXP ostream& operator<< (ostream& os, const S_msrOctaveShift& elt);

//______________________________________________________________________________

class EXP msrStem : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrStemKind {
      k_NoStem,
      kStemUp, kStemDown, kStemNone, kStemDouble};

    static string stemKindAsString (
      msrStemKind stemKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStem> create (
      int           inputLineNumber,
      msrStemKind   stemKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrStem (
      int           inputLineNumber,
      msrStemKind   stemKind);
      
    virtual ~msrStem();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrStemKind getStemKind () const { return fStemKind; }

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    msrStemKind fStemKind;
};
typedef SMARTP<msrStem> S_msrStem;
EXP ostream& operator<< (ostream& os, const S_msrStem& elt);

//______________________________________________________________________________
class EXP msrBeam : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrBeamKind {
      k_NoBeam,
      kBeginBeam, kContinueBeam, kEndBeam,
      kForwardHookBeam, kBackwardHookBeam};
    
    static string beamKindAsString (
      msrBeamKind beamKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBeam> create (
      int           inputLineNumber,
      int           number,
      msrBeamKind   beamKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrBeam (
      int           inputLineNumber,
      int           number,
      msrBeamKind   beamKind);
      
    virtual ~msrBeam();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrBeamKind getBeamKind   () const { return fBeamKind; }
    int         getBeamNumber () const { return fBeamNumber; }

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    int         fBeamNumber;
    msrBeamKind fBeamKind;
};
typedef SMARTP<msrBeam> S_msrBeam;
EXP ostream& operator<< (ostream& os, const S_msrBeam& elt);

/*!
\brief A msr articulation representation.

  An articulation is represented by the numerator and denominator
*/
//______________________________________________________________________________
class EXP msrArticulation : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrArticulationKind {
        kAccent, kBreathMark, kCaesura, 
        kSpiccato,
        kStaccato, kStaccatissimo,
        kStress, kUnstress,
        kDetachedLegato,
        kStrongAccent, kTenuto,
        kFermata, // barline ???
        kArpeggiato, // ???
        kDoit, kFalloff, kPlop, kScoop};

    static string articulationKindAsString (
      msrArticulationKind articulationKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrArticulation> create (
      int                 inputLineNumber,
      msrArticulationKind articulationKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrArticulation (
      int                 inputLineNumber,
      msrArticulationKind articulationKind);
      
    virtual ~msrArticulation();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrArticulationKind   getArticulationKind () const
                              { return fArticulationKind; }
        
    // services
    // ------------------------------------------------------

    virtual string                articulationKindAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

//  private:
  protected:

    msrArticulationKind fArticulationKind;
};
typedef SMARTP<msrArticulation> S_msrArticulation;
EXP ostream& operator<< (ostream& os, const S_msrArticulation& elt);

/*!
\brief A msr articulation representation.

  An articulation is represented by the numerator and denominator
*/
//______________________________________________________________________________
class EXP msrFermata : public msrArticulation
{
  public:
          
    // data types
    // ------------------------------------------------------

    enum msrFermataKind {
        kNormalFermataKind, kAngledFermataKind, kSquareFermataKind};

    static string fermataKindAsString (
      msrFermataKind fermataKind);

    enum msrFermataType {
        kUprightFermataType, kInvertedFermataType};

    static string fermataTypeAsString (
      msrFermataType fermataType);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrFermata> create (
      int            inputLineNumber,
      msrFermataKind fermataKind,
      msrFermataType fermataType);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrFermata (
      int            inputLineNumber,
      msrFermataKind fermataKind,
      msrFermataType fermataType);
      
    virtual ~msrFermata();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrFermataKind        getFermataKind () const
                              { return fFermataKind; }
        
    msrFermataType        getFermataType () const
                              { return fFermataType; }
        
    // services
    // ------------------------------------------------------

    string                fermataAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    msrFermataKind        fFermataKind;
    msrFermataType        fFermataType;
};
typedef SMARTP<msrFermata> S_msrFermata;
EXP ostream& operator<< (ostream& os, const S_msrFermata& elt);

/*!
\brief A msr technical representation.

  An technical is represented by the numerator and denominator
*/
//______________________________________________________________________________
class EXP msrTechnical : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrTechnicalKind {
        kArrow, // rich JMI
        kDoubleTongue,
        kDownBow,
        kFingernails,
        kHarmonic,
        kHeel,
        kHole, // rich JMI
        kOpenString,
        kSnapPizzicato,
        kStopped,
        kTap,
        kThumbPosition,
        kToe,
        kTripleTongue,
        kUpBow};

    static string technicalKindAsString (
      msrTechnicalKind technicalKind);
      
    enum msrTechnicalPlacementKind {
      k_NoTechnicalPlacement,
      kTechnicalPlacementAbove, kTechnicalPlacementBelow};

    static string technicalPlacementKindAsString (
      msrTechnicalPlacementKind technicalPlacementKind);
      
/* JMI
    enum msrTechnicalAccidentalMarkKind {
      kNatural, kSharp, kFlat};

    static string technicalAccidentalMarkKindAsString (
      msrTechnicalAccidentalMarkKind technicalAccidentalMarkKind);
  */
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTechnical> create (
      int                       inputLineNumber,
      msrTechnicalKind          technicalKind,
      msrTechnicalPlacementKind technicalPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTechnical (
      int                       inputLineNumber,
      msrTechnicalKind          technicalKind,
      msrTechnicalPlacementKind technicalPlacementKind);
      
    virtual ~msrTechnical();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrTechnicalKind      getTechnicalKind () const
                              { return fTechnicalKind; }
        
    void                  setTechnicalPlacementKind (
                            msrTechnicalPlacementKind technicalPlacementKind)
                              {
                                fTechnicalPlacementKind =
                                  technicalPlacementKind;
                              }
        
    msrTechnicalPlacementKind
                          getTechnicalPlacementKind () const
                              { return fTechnicalPlacementKind; }

        /* JMI
    void                  setTechnicalAccidentalMarkKind (
                            msrTechnicalAccidentalMarkKind
                              technicalAccidentalMarkKind)
                              {
                                fTechnicalAccidentalMarkKind =
                                  technicalAccidentalMarkKind;
                              }
        
    msrTechnicalAccidentalMarkKind
                          getTechnicalAccidentalMarkKind () const
                              { return fTechnicalAccidentalMarkKind; }
*/

    void                  setTechnicalNoteUplink (S_msrNote note)
                              { fTechnicalNoteUplink = note; }

    S_msrNote             getTechnicalNoteUplink () const
                              { return fTechnicalNoteUplink; }
        
    // services
    // ------------------------------------------------------

    string                technicalKindAsString () const;

    string                technicalPlacementKindAsString () const;

    string                technicalAccidentalMarkKindAsString () const;
    
    string                technicalAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    msrTechnicalKind                fTechnicalKind;

    msrTechnicalPlacementKind       fTechnicalPlacementKind;
    
// JMI    msrTechnicalAccidentalMarkKind  fTechnicalAccidentalMarkKind;

    S_msrNote                       fTechnicalNoteUplink;
};
typedef SMARTP<msrTechnical> S_msrTechnical;
EXP ostream& operator<< (ostream& os, const S_msrTechnical& elt);

/*!
\brief A msr technicalWithInteger representation.

  An technicalWithInteger is represented by the numerator and denominator
*/
//______________________________________________________________________________
class EXP msrTechnicalWithInteger : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrTechnicalWithIntegerKind {
        kBend,
        kFingering,
        kFret,
        kString};

    static string technicalWithIntegerKindAsString (
      msrTechnicalWithIntegerKind technicalWithIntegerKind);
      
    enum msrTechnicalWithIntegerPlacementKind {
      k_NoTechnicalWithIntegerPlacement,
      kTechnicalWithIntegerPlacementAbove, kTechnicalWithIntegerPlacementBelow};

    static string technicalWithIntegerPlacementKindAsString (
      msrTechnicalWithIntegerPlacementKind technicalWithIntegerPlacementKind);
            
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTechnicalWithInteger> create (
      int                         inputLineNumber,
      msrTechnicalWithIntegerKind technicalWithIntegerKind,
      int                         technicalWithIntegerValue,
      msrTechnicalWithIntegerPlacementKind
                                  technicalWithIntegerPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTechnicalWithInteger (
      int                         inputLineNumber,
      msrTechnicalWithIntegerKind technicalWithIntegerKind,
      int                         technicalWithIntegerValue,
      msrTechnicalWithIntegerPlacementKind
                                  technicalWithIntegerPlacementKind);
      
    virtual ~msrTechnicalWithInteger();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrTechnicalWithIntegerKind
                          getTechnicalWithIntegerKind () const
                              { return fTechnicalWithIntegerKind; }
        
    int                   getTechnicalWithIntegerValue () const
                              { return fTechnicalWithIntegerValue; }
        
    void                  setTechnicalWithIntegerPlacementKind (
                            msrTechnicalWithIntegerPlacementKind
                              technicalWithIntegerPlacementKind)
                              {
                                fTechnicalWithIntegerPlacementKind =
                                  technicalWithIntegerPlacementKind;
                              }
        
    msrTechnicalWithIntegerPlacementKind
                          getTechnicalWithIntegerPlacementKind () const
                              { return fTechnicalWithIntegerPlacementKind; }

    void                  setTechnicalWithIntegerNoteUplink (S_msrNote note)
                              { fTechnicalWithIntegerNoteUplink = note; }

    S_msrNote             getTechnicalWithIntegerNoteUplink () const
                              { return fTechnicalWithIntegerNoteUplink; }
        
    // services
    // ------------------------------------------------------

    string                technicalWithIntegerKindAsString () const;

    string                technicalWithIntegerPlacementKindAsString () const;

    string                technicalWithIntegerAccidentalMarkKindAsString () const;
    
    string                technicalWithIntegerAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    msrTechnicalWithIntegerKind     fTechnicalWithIntegerKind;

    int                             fTechnicalWithIntegerValue;

    msrTechnicalWithIntegerPlacementKind
                                    fTechnicalWithIntegerPlacementKind;
    
    S_msrNote                       fTechnicalWithIntegerNoteUplink;
};
typedef SMARTP<msrTechnicalWithInteger> S_msrTechnicalWithInteger;
EXP ostream& operator<< (ostream& os, const S_msrTechnicalWithInteger& elt);

/*!
\brief A msr technicalWithString representation.

  An technicalWithString is represented by the numerator and denominator
*/
//______________________________________________________________________________
class EXP msrTechnicalWithString : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrTechnicalWithStringKind {
        kHammerOn,
        kHandbell,
        kOtherTechnical,
        kPluck,
        kPullOff};

    static string technicalWithStringKindAsString (
      msrTechnicalWithStringKind technicalWithStringKind);
      
    enum msrTechnicalWithStringPlacementKind {
      k_NoTechnicalWithStringPlacement,
      kTechnicalWithStringPlacementAbove, kTechnicalWithStringPlacementBelow};

    static string technicalWithStringPlacementKindAsString (
      msrTechnicalWithStringPlacementKind technicalWithStringPlacementKind);
            
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTechnicalWithString> create (
      int                         inputLineNumber,
      msrTechnicalWithStringKind technicalWithStringKind,
      string                      technicalWithStringValue,
      msrTechnicalWithStringPlacementKind
                                  technicalWithStringPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTechnicalWithString (
      int                         inputLineNumber,
      msrTechnicalWithStringKind technicalWithStringKind,
      string                      technicalWithStringValue,
      msrTechnicalWithStringPlacementKind
                                  technicalWithStringPlacementKind);
      
    virtual ~msrTechnicalWithString();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrTechnicalWithStringKind
                          getTechnicalWithStringKind () const
                              { return fTechnicalWithStringKind; }

    string                getTechnicalWithStringValue () const
                              { return fTechnicalWithStringValue; }
        
    void                  setTechnicalWithStringPlacementKind (
                            msrTechnicalWithStringPlacementKind
                              technicalWithStringPlacementKind)
                              {
                                fTechnicalWithStringPlacementKind =
                                  technicalWithStringPlacementKind;
                              }
        
    msrTechnicalWithStringPlacementKind
                          getTechnicalWithStringPlacementKind () const
                              { return fTechnicalWithStringPlacementKind; }

    void                  setTechnicalWithStringNoteUplink (S_msrNote note)
                              { fTechnicalWithStringNoteUplink = note; }

    S_msrNote             getTechnicalWithStringNoteUplink () const
                              { return fTechnicalWithStringNoteUplink; }
        
    // services
    // ------------------------------------------------------

    string                technicalWithStringKindAsString () const;

    string                technicalWithStringPlacementKindAsString () const;

    string                technicalWithStringAccidentalMarkKindAsString () const;
    
    string                technicalWithStringAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    msrTechnicalWithStringKind      fTechnicalWithStringKind;

    string                          fTechnicalWithStringValue;

    msrTechnicalWithStringPlacementKind
                                    fTechnicalWithStringPlacementKind;
    
    S_msrNote                       fTechnicalWithStringNoteUplink;
};
typedef SMARTP<msrTechnicalWithString> S_msrTechnicalWithString;
EXP ostream& operator<< (ostream& os, const S_msrTechnicalWithString& elt);

/*!
\brief A msr ornament representation.

  An ornament is represented by the numerator and denominator
*/
//______________________________________________________________________________
class EXP msrOrnament : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrOrnamentKind {
        kTrillMark, kWavyLine,
        kTurn, kInvertedTurn, kDelayedTurn,
        kDelayedInvertedTurn, kVerticalTurn,
        kMordent, kInvertedMordent,
        kSchleifer, kShake};

    static string ornamentKindAsString (
      msrOrnamentKind ornamentKind);
      
    enum msrOrnamentPlacementKind {
      k_NoOrnamentPlacement,
      kOrnamentPlacementAbove, kOrnamentPlacementBelow};

    static string ornamentPlacementKindAsString (
      msrOrnamentPlacementKind ornamentPlacementKind);
      
    enum msrOrnamentAccidentalMarkKind { // JMI msrAlteration ?
      kDoubleFlat, kSesquiFlat, kFlat, kSemiFlat,
      kNatural,
      kSemiSharp, kSharp, kSesquiSharp, kDoubleSharp};

    static string ornamentAccidentalMarkKindAsString (
      msrOrnamentAccidentalMarkKind ornamentAccidentalMarkKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrOrnament> create (
      int             inputLineNumber,
      msrOrnamentKind ornamentKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOrnament (
      int             inputLineNumber,
      msrOrnamentKind ornamentKind);
      
    virtual ~msrOrnament();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrOrnamentKind       getOrnamentKind () const
                              { return fOrnamentKind; }
        
    void                  setOrnamentPlacementKind (
                            msrOrnamentPlacementKind ornamentPlacementKind)
                              {
                                fOrnamentPlacementKind =
                                  ornamentPlacementKind;
                              }
        
    msrOrnamentPlacementKind
                          getOrnamentPlacementKind () const
                              { return fOrnamentPlacementKind; }
        
    void                  setOrnamentAccidentalMarkKind (
                            msrOrnamentAccidentalMarkKind
                              ornamentAccidentalMarkKind)
                              {
                                fOrnamentAccidentalMarkKind =
                                  ornamentAccidentalMarkKind;
                              }
        
    msrOrnamentAccidentalMarkKind
                          getOrnamentAccidentalMarkKind () const
                              { return fOrnamentAccidentalMarkKind; }

    void                  setOrnamentNoteUplink (S_msrNote note)
                              { fOrnamentNoteUplink = note; }

    S_msrNote             getOrnamentNoteUplink () const
                              { return fOrnamentNoteUplink; }
        
    // services
    // ------------------------------------------------------

    string                ornamentKindAsString () const;

    string                ornamentPlacementKindAsString () const;

    string                ornamentAccidentalMarkKindAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    msrOrnamentKind               fOrnamentKind;

    msrOrnamentPlacementKind      fOrnamentPlacementKind;
    
    msrOrnamentAccidentalMarkKind fOrnamentAccidentalMarkKind;

    S_msrNote                     fOrnamentNoteUplink;
};
typedef SMARTP<msrOrnament> S_msrOrnament;
EXP ostream& operator<< (ostream& os, const S_msrOrnament& elt);

/*!
\brief A msr singleTremolo representation.

  An singleTremolo is represented by the numerator and denominator
*/
//______________________________________________________________________________
class EXP msrSingleTremolo : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrSingleTremoloPlacementKind {
      k_NoSingleTremoloPlacement,
      kSingleTremoloPlacementAbove, kSingleTremoloPlacementBelow};

    static string singleTremoloPlacementKindAsString (
      msrSingleTremoloPlacementKind singleTremoloPlacementKind);
            
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSingleTremolo> create (
      int                     inputLineNumber,
      int                     singleTremoloMarksNumber,
      msrSingleTremoloPlacementKind
                              singleTremoloPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSingleTremolo (
      int                     inputLineNumber,
      int                     singleTremoloMarksNumber,
      msrSingleTremoloPlacementKind
                              singleTremoloPlacementKind);
      
    virtual ~msrSingleTremolo();
  
  public:

    // set and get
    // ------------------------------------------------------

    void                  setSingleTremoloPlacementKind (
                            msrSingleTremoloPlacementKind
                              SingleTremoloPlacementKind)
                              {
                                fSingleTremoloPlacementKind =
                                  SingleTremoloPlacementKind;
                              }
        
    int                   getSingleTremoloMarksNumber () const
                              { return fSingleTremoloMarksNumber; }
                
    msrSingleTremoloPlacementKind
                          getSingleTremoloPlacementKind () const
                              { return fSingleTremoloPlacementKind; }
        
    void                  setSingleTremoloNoteUplink (S_msrNote note)
                              { fSingleTremoloNoteUplink = note; }

    S_msrNote             getSingleTremoloNoteUplink () const
                              { return fSingleTremoloNoteUplink; }
        
    // services
    // ------------------------------------------------------

    string                singleTremoloPlacementKindAsString () const;
    
    string                singleTremoloAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    int                           fSingleTremoloMarksNumber;

    msrSingleTremoloPlacementKind fSingleTremoloPlacementKind;
    
    S_msrNote                     fSingleTremoloNoteUplink;
};
typedef SMARTP<msrSingleTremolo> S_msrSingleTremolo;
EXP ostream& operator<< (ostream& os, const S_msrSingleTremolo& elt);

//______________________________________________________________________________
class EXP msrDoubleTremolo : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrDoubleTremoloKind {
      kNotesDoubleTremolo, kChordsDoubleTremolo};

    static string msrDoubleTremoloKindAsString (
      msrDoubleTremoloKind doubleTremolotKind);
            
    // creation from MusicXML
    enum msrDoubleTremoloPlacementKind {
      k_NoDoubleTremoloPlacement,
      kDoubleTremoloPlacementAbove, kDoubleTremoloPlacementBelow};

    static string doubleTremoloPlacementKindAsString (
      msrDoubleTremoloPlacementKind doubleTremoloPlacementKind);
            
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrDoubleTremolo> create (
      int                  inputLineNumber,
      msrDoubleTremoloKind doubleTremoloKind,
      int                  doubleTremoloMarksNumber,
      msrDoubleTremoloPlacementKind
                           doubleTremoloPlacementKind,
      S_msrVoice           voiceUplink);


    SMARTP<msrDoubleTremolo> createDoubleTremoloShallowClone (
      S_msrVoice voiceClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrDoubleTremolo (
      int                  inputLineNumber,
      msrDoubleTremoloKind doubleTremoloKind,
      int                  doubleTremoloMarksNumber,
      msrDoubleTremoloPlacementKind
                           doubleTremoloPlacementKind,
      S_msrVoice           voiceUplink);
      
    virtual ~msrDoubleTremolo();
  
  public:

    // set and get
    // ------------------------------------------------------

    void                  setDoubleTremoloKind (
                            msrDoubleTremoloKind doubleTremoloKind)
                              { fDoubleTremoloKind = doubleTremoloKind; }

    msrDoubleTremoloKind  getDoubleTremoloKind () const
                              { return fDoubleTremoloKind; }
        
    void                  setDoubleTremoloPlacementKind (
                            msrDoubleTremoloPlacementKind
                              DoubleTremoloPlacementKind)
                              {
                                fDoubleTremoloPlacementKind =
                                  DoubleTremoloPlacementKind;
                              }
        
    int                   getDoubleTremoloMarksNumber () const
                              { return fDoubleTremoloMarksNumber; }
                
    msrDoubleTremoloPlacementKind
                          getDoubleTremoloPlacementKind () const
                              { return fDoubleTremoloPlacementKind; }
        
    void                  setDoubleTremoloNoteFirstElement (S_msrNote note);
    void                  setDoubleTremoloChordFirstElement (S_msrChord chord);

    S_msrElement          getDoubleTremoloFirstElement () const
                              { return fDoubleTremoloFirstElement; }
        
    void                  setDoubleTremoloNoteSecondElement (S_msrNote note);
    void                  setDoubleTremoloChordSecondElement (S_msrChord chord);

    S_msrElement          getDoubleTremoloSecondElement () const
                              { return fDoubleTremoloSecondElement; }
        
    S_msrVoice            getDoubleTremoloVoiceUplink () const
                            { return fDoubleTremoloVoiceUplink; }

    // harmony
    void                  setDoubleTremoloHarmony (S_msrHarmony harmony)
                              { fDoubleTremoloHarmony = harmony; }
                      
    const S_msrHarmony&   getDoubleTremoloHarmony () const
                              { return fDoubleTremoloHarmony; };
                      
     // divisions
    void                  setDoubleTremoloSoundingDivisions (int divisions)
                              { fDoubleTremoloSoundingDivisions = divisions; }
            
    int                   getDoubleTremoloSoundingDivisions () const
                              { return fDoubleTremoloSoundingDivisions; }
            
    // measure uplink
    void                  setDoubleTremoloMeasureUplink (
                            const S_msrMeasure& measure)
                              { fDoubleTremoloMeasureUplink = measure; }
                      
    S_msrMeasure          getDoubleTremoloMeasureUplink () const
                            { return fDoubleTremoloMeasureUplink; }

    // measure number
    void                  setDoubleTremoloMeasureNumber (
                            string measureNumber)
                              { fDoubleTremoloMeasureNumber = measureNumber; }
    
    string                getDoubleTremoloMeasureNumber () const
                              { return fDoubleTremoloMeasureNumber; }
 
    // position in measure
    void                  setDoubleTremoloPositionInMeasure (int position)
                              { fDoubleTremoloPositionInMeasure = position; }

    const int             getDoubleTremoloPositionInMeasure () const
                              { return fDoubleTremoloPositionInMeasure; }

    // services
    // ------------------------------------------------------

    string                doubleTremoloPlacementKindAsString () const;
    
    string                doubleTremoloAsShortString () const;
    
    string                doubleTremoloAsString () const;

    void                  setDoubleTremoloFirstNotePositionInMeasure (
                            int position);
                    
    void                  setDoubleTremoloFirstNoteMeasureNumber (
                            string measureNumber);
                    
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // divisions handling is done at the part level
    S_msrPart                     fDoubleTremoloDirectPartUplink;

    // sounding divisions
    // the same as the displayed divisions of both members
    int                           fDoubleTremoloSoundingDivisions;

    msrDoubleTremoloKind          fDoubleTremoloKind; // JMI ???
    
    int                           fDoubleTremoloMarksNumber;

    msrDoubleTremoloPlacementKind fDoubleTremoloPlacementKind;

    // the two elements of a double tremole are notes or chords
    S_msrElement                  fDoubleTremoloFirstElement;
    S_msrElement                  fDoubleTremoloSecondElement;

    S_msrMeasure                  fDoubleTremoloMeasureUplink;

    string                        fDoubleTremoloMeasureNumber;
    int                           fDoubleTremoloPositionInMeasure;
    
    S_msrHarmony                  fDoubleTremoloHarmony;

    S_msrVoice                    fDoubleTremoloVoiceUplink;

};
typedef SMARTP<msrDoubleTremolo> S_msrDoubleTremolo;
EXP ostream& operator<< (ostream& os, const S_msrDoubleTremolo& elt);

/*!
\brief A msr rehearsal representation.

  An rehearsal is represented by the numerator and denominator
*/
//______________________________________________________________________________
class EXP msrRehearsal : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrRehearsalKind {
        kNone,
        kRectangle, kOval, kCircle, kBracket, kTriangle, kDiamond };

    static string rehearsalKindAsString (
      msrRehearsalKind rehearsalKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRehearsal> create (
      int              inputLineNumber,
      msrRehearsalKind rehearsalKind,
      string           rehearsalText);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRehearsal (
      int              inputLineNumber,
      msrRehearsalKind rehearsalKind,
      string           rehearsalText);
      
    virtual ~msrRehearsal();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrRehearsalKind      getRehearsalKind () const
                              { return fRehearsalKind; }
        
    // services
    string                getRehearsalText () const
                              { return fRehearsalText; }
        
    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    msrRehearsalKind fRehearsalKind;

    string           fRehearsalText;
};
typedef SMARTP<msrRehearsal> S_msrRehearsal;
EXP ostream& operator<< (ostream& os, const S_msrRehearsal& elt);

/*!
\brief A msr tie representation.

  A tie is represented by a msrTieKind value (hairpins in LilyPond)
*/
//______________________________________________________________________________
class EXP msrTie : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrTieKind {
      k_NoTie,
      kStartTie, kContinueTie, kStopTie};        
    
    static string tieKindAsString (
      msrTieKind tieKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTie> create (
      int           inputLineNumber,
      msrTieKind    tieKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTie (
      int           inputLineNumber,
      msrTieKind    tieKind);
      
    virtual ~msrTie();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrTieKind            getTieKind () const
                              { return fTieKind; }

    // services
    // ------------------------------------------------------

    string                tieKindAsString () const // JMI
                              { return tieKindAsString (fTieKind); }

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    msrTieKind fTieKind;
};
typedef SMARTP<msrTie> S_msrTie;
EXP ostream& operator<< (ostream& os, const S_msrTie& elt);

/*!
\brief A msr slur representation.

  A slur is represented by a SlurKind value (hairpins in LilyPond)
*/
//______________________________________________________________________________
class EXP msrSlur : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrSlurKind {
      k_NoSlur,
      kStartSlur, kContinueSlur, kStopSlur};
    
    static string slurKindAsString (
      msrSlurKind slurKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSlur> create (
      int           inputLineNumber,
      int           slurNumber,
      msrSlurKind   slurKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSlur (
      int           inputLineNumber,
      int           slurNumber,
      msrSlurKind   slurKind);
      
    virtual ~msrSlur();
  
  public:

    // set and get
    // ------------------------------------------------------

    int                   getSlurNumber () const { return fSlurNumber; }
    
    msrSlurKind           getSlurKind () const { return fSlurKind; }

    // services
    // ------------------------------------------------------

    string                slurKindAsString ();

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    int         fSlurNumber;

    msrSlurKind fSlurKind;
};
typedef SMARTP<msrSlur> S_msrSlur;
EXP ostream& operator<< (ostream& os, const S_msrSlur& elt);

/*!
\brief A msr ligature representation.

  A ligature is represented by a SlurKind value (hairpins in LilyPond)
*/
//______________________________________________________________________________
class EXP msrLigature : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrLigatureKind {
      k_NoLigature,
      kStartLigature, kContinueLigature, kStopLigature};
    
    static string ligatureKindAsString (
      msrLigatureKind ligatureKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrLigature> create (
      int             inputLineNumber,
      int             ligatureNumber,
      msrLigatureKind ligatureKind);
      
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrLigature (
      int             inputLineNumber,
      int             ligatureNumber,
      msrLigatureKind ligatureKind);
      
    virtual ~msrLigature();
  
  public:

    // set and get
    // ------------------------------------------------------

    int                   getLigatureNumber () const
                              { return fLigatureNumber; }
    
    msrLigatureKind       getLigatureKind () const
                              { return fLigatureKind; }

    // services
    // ------------------------------------------------------

    string                ligatureKindAsString ();

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    int               fLigatureNumber;

    msrLigatureKind   fLigatureKind;
};
typedef SMARTP<msrLigature> S_msrLigature;
EXP ostream& operator<< (ostream& os, const S_msrLigature& elt);

/*!
\brief A msr dynamics representation.

  A dynamics is represented by a msrDynamicsKind value
*/
//______________________________________________________________________________
class EXP msrDynamics : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrDynamicsKind {
          kF, kFF, kFFF, kFFFF, kFFFFF, kFFFFFF,
          kP, kPP, kPPP, kPPPP, kPPPPP, kPPPPPP,
          kMF, kMP, kFP, kFZ, kRF, kSF, kRFZ, kSFZ, kSFP, kSFPP, kSFFZ,
          k_NoDynamics};
    
    static string dynamicsKindAsString (
      msrDynamicsKind dynamicsKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrDynamics> create (
      int             inputLineNumber,
      msrDynamicsKind dynamicsKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrDynamics (
      int             inputLineNumber,
      msrDynamicsKind dynamicsKind);
      
    virtual ~msrDynamics();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrDynamicsKind       getDynamicsKind () const
                              { return fDynamicsKind; }

    // services
    // ------------------------------------------------------

    string                dynamicsKindAsString ();

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    msrDynamicsKind fDynamicsKind;
};
typedef SMARTP<msrDynamics> S_msrDynamics;
EXP ostream& operator<< (ostream& os, const S_msrDynamics& elt);

/*!
\brief A msr dynamics representation.

  A dynamics is represented by a msrDynamicsKind value
*/
//______________________________________________________________________________
class EXP msrOtherDynamics : public msrElement
{
  public:
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrOtherDynamics> create (
      int    inputLineNumber,
      string otherDynamicsString);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOtherDynamics (
      int    inputLineNumber,
      string otherDynamicsString);
      
    virtual ~msrOtherDynamics();
  
  public:

    // set and get
    // ------------------------------------------------------

    string                getOtherDynamicsString () const
                              { return fOtherDynamicsString; }

    // services
    // ------------------------------------------------------

    string                otherDynamicsAsString ();

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    string                fOtherDynamicsString;
};
typedef SMARTP<msrOtherDynamics> S_msrOtherDynamics;
EXP ostream& operator<< (ostream& os, const S_msrOtherDynamics& elt);

/*!
\brief A msr wedge representation.

  A wedge is represented by a msrWedgeKind value (hairpins in LilyPond)
*/
//______________________________________________________________________________
class EXP msrWedge : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrWedgeKind
      { kCrescendoWedge, kDecrescendoWedge, kStopWedge };
    
    static string wedgeKindAsString (
      msrWedgeKind wedgeKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrWedge> create (
      int           inputLineNumber,
      msrWedgeKind  wedgeKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrWedge (
      int           inputLineNumber,
      msrWedgeKind  wedgeKind);
      
    virtual ~msrWedge();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrWedgeKind          getWedgeKind () const { return fWedgeKind; }

    string                wedgeKindAsString ();

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    msrWedgeKind fWedgeKind;
};
typedef SMARTP<msrWedge> S_msrWedge;
EXP ostream& operator<< (ostream& os, const S_msrWedge& elt);

/*!
\brief A msr clef representation.

  A clef is represented by its name
*/
//______________________________________________________________________________
class EXP msrClef : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrClefKind {
      k_NoClef,
      kTrebleClef,
      kSopranoClef, kMezzoSopranoClef, kAltoClef, kTenorClef, kBaritoneClef,
      kBassClef,
      kTrebleLine1Clef,
      kTrebleMinus15Clef, kTrebleMinus8Clef,
      kTreblePlus8Clef, kTreblePlus15Clef, 
      kBassMinus15Clef, kBassMinus8Clef,
      kBassPlus8Clef, kBassPlus15Clef,
      kVarbaritoneClef,
      kTablature4Clef, kTablature5Clef, kTablature6Clef, kTablature7Clef,
      kPercussionClef};
      
    static string clefKindAsString (
      msrClefKind clefKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrClef> create (
      int          inputLineNumber,
      msrClefKind  clefKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrClef (
      int          inputLineNumber,
      msrClefKind  clefKind);
      
    virtual ~msrClef();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrClefKind           getClefKind () const
                              { return fClefKind; }
                
    // services
    // ------------------------------------------------------

    string                clefAsString () const;

    bool                  clefIsATablatureClef () const;
    
    bool                  clefIsAPercussionClef () const;

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    msrClefKind  fClefKind;
};
typedef SMARTP<msrClef> S_msrClef;
EXP ostream& operator<< (ostream& os, const S_msrClef& elt);

/*!
\brief A msr key representation.

  A key is represented by the tonic and the mode
*/
//______________________________________________________________________________
class EXP msrHumdrumScotKeyItem : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrHumdrumScotKeyItemKind {
        kTraditionalKind, kHumdrumScotKind };
        
    static string HumdrumScotKeyItemKindAsString (
      msrHumdrumScotKeyItemKind HumdrumScotKeyItemKind);

    enum msrHumdrumScotKeyItemModeKind {
        kMajorMode, kMinorMode,
        kIonianMode, kDorianMode, kPhrygianMode, kLydianMode,
        kMixolydianMode, kAeolianMode, kLocrianMode };

    static string HumdrumScotKeyItemModeKindAsString (
      msrHumdrumScotKeyItemModeKind HumdrumScotKeyItemModeKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrHumdrumScotKeyItem> create (
      int inputLineNumber);
      
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrHumdrumScotKeyItem (
      int inputLineNumber);
            
    virtual ~msrHumdrumScotKeyItem();

  public:

    // set and get
    // ------------------------------------------------------

    void                  setKeyItemDiatonicPitch (
                            msrDiatonicPitch diatonicPitch);
                              
    msrDiatonicPitch      getKeyItemDiatonicPitch () const
                              { return fKeyDiatonicPitch; }

    void                  setKeyItemAlteration (
                            msrAlteration alteration);
                              
    msrAlteration         getKeyItemAlteration () const
                              { return fKeyAlteration; }

    void                  setKeyItemOctave (int keyOctave);
                              
    int                   getKeyItemOctave () const
                              { return fKeyOctave; }

    // services
    // ------------------------------------------------------

    string                humdrumScotKeyItemAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);
  
  private:
  
    msrDiatonicPitch      fKeyDiatonicPitch;
    msrAlteration         fKeyAlteration;
    int                   fKeyOctave;
};
typedef SMARTP<msrHumdrumScotKeyItem> S_msrHumdrumScotKeyItem;
EXP ostream& operator<< (ostream& os, const S_msrHumdrumScotKeyItem& elt);

/*!
\brief A msr key representation.

  A key is represented by the tonic and the mode
*/
//______________________________________________________________________________
class EXP msrKey : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrKeyKind {
        kTraditionalKind, kHumdrumScotKind };
        
    static string keyKindAsString (
      msrKeyKind keyKind);

    enum msrKeyModeKind {
        kMajorMode, kMinorMode,
        kIonianMode, kDorianMode, kPhrygianMode, kLydianMode,
        kMixolydianMode, kAeolianMode, kLocrianMode };

    static string keyModeKindAsString (
      msrKeyModeKind keyModeKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrKey> createTraditional (
      int                  inputLineNumber,
      msrQuarterTonesPitch keyTonicPitch,
      msrKeyModeKind       keyModeKind,
      int                  keyCancel);
      
    static SMARTP<msrKey> createHumdrumScot (
      int                  inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrKey ( // for traditional keys
      int                  inputLineNumber,
      msrQuarterTonesPitch keyTonicPitch,
      msrKeyModeKind       keyModeKind,
      int                  keyCancel);
      
    msrKey ( // for Humdrum/Scot keys
      int                  inputLineNumber);
      
    virtual ~msrKey();

  public:

    // set and get
    // ------------------------------------------------------

    msrKeyKind            getKeyKind () const
                              { return fKeyKind; }

    // traditional keys
    
    msrQuarterTonesPitch  getKeyTonicQuarterTonesPitch () const
                              { return fKeyTonicQuarterTonesPitch; }
                              
    msrKeyModeKind        getKeyModeKind () const
                              { return fKeyModeKind; }
                            
    int                   getKeyCancel () const
                              { return fKeyCancel; }

    // Humdrum/Scot keys

 //   void                  setKeyItemsOctavesAreSpecified ()
 //                           { fKeyItemsOctavesAreSpecified = true; }
                              
    bool                  getKeyItemsOctavesAreSpecified () const
                              { return fKeyItemsOctavesAreSpecified; }

    const vector<S_msrHumdrumScotKeyItem>&
                          getHumdrumScotKeyItemsVector ()
                              { return fHumdrumScotKeyItemsVector; }

    
    // services
    // ------------------------------------------------------

    void                  appendHumdrumScotKeyItem (
                            S_msrHumdrumScotKeyItem item);                                

    string                keyAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);
  
  private:

    msrKeyKind            fKeyKind;
    
    // traditional keys

    msrQuarterTonesPitch  fKeyTonicQuarterTonesPitch;
    msrKeyModeKind        fKeyModeKind;
    int                   fKeyCancel;

    // Humdrum/Scot keys
    vector<S_msrHumdrumScotKeyItem>
                          fHumdrumScotKeyItemsVector;
    bool                  fKeyItemsOctavesAreSpecified;
};
typedef SMARTP<msrKey> S_msrKey;
EXP ostream& operator<< (ostream& os, const S_msrKey& elt);

/*!
\brief A msr key representation.

  A key is represented by the tonic and the mode
*/
//______________________________________________________________________________
class EXP msrTimeItem : public msrElement
{
  public:
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTimeItem> create (
      int inputLineNumber);
      
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTimeItem (
      int inputLineNumber);
            
    virtual ~msrTimeItem();

  public:

    // set and get
    // ------------------------------------------------------

    const vector<int>&    getTimeBeatsNumbersVector ()
                              { return fTimeBeatsNumbersVector; }

    void                  setTimeBeatValue (int timeBeatValue)
                              { fTimeBeatValue = timeBeatValue; }
                              
    int                   getTimeBeatValue () const
                              { return fTimeBeatValue; }

    // services
    // ------------------------------------------------------

    void                  appendBeatsNumber (int beatsNumber);

    int                   getTimeBeatsNumber () const;
                              
    string                timeItemAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);
  
  private:
  
    vector<int>           fTimeBeatsNumbersVector; // 5+3+1 is possible
    int                   fTimeBeatValue;
};
typedef SMARTP<msrTimeItem> S_msrTimeItem;
EXP ostream& operator<< (ostream& os, const S_msrTimeItem& elt);

/*!
\brief A msr time representation.

  A time is represented by the numerator and denominator
*/
//______________________________________________________________________________
class EXP msrTime : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrTimeSymbolKind {
        k_NoTimeSymbol,
        kTimeSymbolCommon,
        kTimeSymbolCut,
        kTimeSymbolNote,
        kTimeSymbolDottedNote,
        kTimeSymbolSingleNumber,
        kTimeSymbolSenzaMisura };

    static string timeSymbolKindAsString (
      msrTimeSymbolKind timeSymbolKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTime> create (
      int               inputLineNumber,
      msrTimeSymbolKind timeSymbolKind);

    static SMARTP<msrTime> createFourQuartersTime (
      int inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTime (
      int               inputLineNumber,
      msrTimeSymbolKind timeSymbolKind);
      
    virtual ~msrTime();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrTimeSymbolKind     getTimeSymbolKind () const
                              { return fTimeSymbolKind; }
                  
    bool                  getTimeIsCompound () const
                              { return fTimeIsCompound; }
                  
    const vector<S_msrTimeItem>&
                          getTimeItemsVector ()
                              { return fTimeItemsVector; }

    // services
    // ------------------------------------------------------
                  
    void                  appendTimeItem (
                            S_msrTimeItem timeItem);

    rational              wholeNotesPerMeasure () const;

    string                timeAsShortString () const;
    
    string                timeAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    msrTimeSymbolKind     fTimeSymbolKind;

    vector<S_msrTimeItem> fTimeItemsVector;

    // a time is compound if it contains several items
    // or if the only one has several beats numbers
    // i.e. 3/4 is not, (3+4)/8 is, and 2/4+3/4 is too
    bool                  fTimeIsCompound;
};
typedef SMARTP<msrTime> S_msrTime;
EXP ostream& operator<< (ostream& os, const S_msrTime& elt);

/*!
\brief A msr repeat representation.

  A repeat is represented by:
    - a sequence of elements for the common segment
    - a vector of sequences of elements for the alternate endings
*/
//______________________________________________________________________________
class EXP msrMeasure : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrMeasureKind {
        kUnknownMeasureKind,
        kFullMeasureKind,
        kUpbeatMeasureKind,
        kUnderfullMeasureKind,
        kOverfullMeasureKind,
        kSenzaMisuraMeasureKind,
        kEmptyMeasureKind}; // for <measure ... /> without nested contents
    
    static string measureKindAsString (
      msrMeasureKind measureKind);

    enum msrMeasureImplicitKind {
        kMeasureImplicitYes,
        kMeasureImplicitNo };
      
    static string measureImplicitKindAsString (
      msrMeasureImplicitKind measureImplicitKind);

    enum msrMeasureFirstInSegmentKind {
        kMeasureFirstInSegmentYes,
        kMeasureFirstInSegmentNo };
      
    static string measureFirstInSegmentKindAsString (
      msrMeasureFirstInSegmentKind measureFirstInSegmentKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasure> create (
      int           inputLineNumber,
      S_msrPart     measureDirectPartUplink,
      string        measureNumber,
      S_msrSegment  measureSegmentUplink);
    
    SMARTP<msrMeasure> createMeasureShallowClone (
      S_msrSegment segmentClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrMeasure (
      int           inputLineNumber,
      S_msrPart     measureDirectPartUplink,
      string        measureNumber,
      S_msrSegment  measureSegmentUplink);
      
    virtual ~msrMeasure();
  
  private:

    // initialization
    // ------------------------------------------------------

    void                  initializeMeasure ();

  public:

    // set and get
    // ------------------------------------------------------

    // measure number
    
    void                  setMeasureNumber (string measureNumber)
                              { fMeasureNumber = measureNumber; }

    string                getMeasureNumber () const
                              { return fMeasureNumber; }

    // divisions
    
    int                   getMeasureDivisionsPerFullMeasure () const
                              { return fMeasureDivisionsPerFullMeasure; }

    void                  setMeasurePosition (
                            int inputLineNumber,
                            int measurePosition);

    int                   getMeasurePosition () const
                              { return fMeasurePosition; }

    // measure kind
    
    void                  setMeasureKind (msrMeasureKind measureKind)
                              { fMeasureKind = measureKind; }

    msrMeasureKind        getMeasureKind () const
                              { return fMeasureKind; }

    // measure 'first in segment' kind
    
    void                  setMeasureFirstInSegmentKind (
                            msrMeasureFirstInSegmentKind
                              measureFirstInSegmentKind)
                              {
                                fMeasureFirstInSegmentKind =
                                  measureFirstInSegmentKind;
                              }

    msrMeasureFirstInSegmentKind
                          getMeasureFirstInSegmentKind () const
                              { return fMeasureFirstInSegmentKind; }

    // chords handling
    
    S_msrNote              getMeasureLastHandledNote () const
                              { return fMeasureLastHandledNote; }

    // uplinks
    
    S_msrSegment          getMeasureSegmentUplink () const
                              { return fMeasureSegmentUplink; }
                      
    S_msrVoice            getMeasureVoiceDirectUplink () const
                              { return fMeasureVoiceDirectUplink; }

    S_msrPart             getMeasureDirectPartUplink () const
                             { return fMeasureDirectPartUplink; }

    // elements list
    
    const list<S_msrElement>&
                          getMeasureElementsList () const
                              { return fMeasureElementsList; }

    // services
    // ------------------------------------------------------

    // measure kind
    
    string                getMeasureKindAsString () const;

    // divisions

    void                  bringMeasureToMeasurePosition (
                            int inputLineNumber,
                            int measurePosition);

    // measure length
    
    int                   getMeasureLength () const
                            // divisions, positions start at 1
                              { return fMeasurePosition - 1; }
                      
    string                getMeasureLengthAsString () const;

    // clefs
    
    void                  appendClefToMeasure (S_msrClef clef);

    // keys
    
    void                  appendKeyToMeasure (S_msrKey key);

    // times

    void                  appendTimeToMeasure (S_msrTime time);

    void                  setMeasureDivisionsPerFullMeasureFromTime (
                            S_msrTime time);
  
    // transpose

    void                  appendTransposeToMeasure (
                            S_msrTranspose transpose);

    // staff details

    void                  appendStaffDetailsToMeasure (
                            S_msrStaffDetails staffDetails);

    // bar number checks
    
    void                  appendBarNumberCheckToMeasure (
                            S_msrBarNumberCheck barNumberCheck);

    // breaks

    void                  appendBreakToMeasure (S_msrBreak break_);

    // tempo
    
    void                  appendTempoToMeasure (S_msrTempo tempo);

    // rehearsals
    
    void                  appendRehearsalToMeasure (
                            S_msrRehearsal rehearsal);

    // octave shifts
    
    void                  appendOctaveShiftToMeasure (
                            S_msrOctaveShift octaveShift);

    // bar lines
    
    void                  appendBarlineToMeasure (S_msrBarline barline);
    void                  prependBarlineToMeasure (S_msrBarline barline);

    // bar checks
    
    void                  appendBarCheckToMeasure (S_msrBarCheck barCheck);

    // notes
    
    void                  appendNoteToMeasure      (S_msrNote note);
    void                  appendNoteToMeasureClone (S_msrNote note);

    // tremolos
    
    void                  appendDoubleTremoloToMeasure (
                            S_msrDoubleTremolo doubleTremolo);

    // repeats
    
    void                  appendMeasureRepeatToMeasure (
                            S_msrMeasureRepeat measureRepeat);

    void                  appendMultipleRestToMeasure (
                            S_msrMultipleRest multipleRest);

    // chords
    
    void                  appendChordToMeasure (S_msrChord chord);

    // tuplets
    
    void                  appendTupletToMeasure (S_msrTuplet tuplet);

    // harmonies
    
    void                  appendHarmonyToMeasure (S_msrHarmony harmony);
    
    void                  appendHarmonyToMeasureClone (S_msrHarmony harmony);

    // grace notes
    
    void                  appendGraceNotesToMeasure (
                            S_msrGraceNotes graceNotes);

    void                  prependGraceNotesToMeasure (
                            S_msrGraceNotes graceNotes);
    
    void                  appendAfterGraceNotesToMeasure (
                            S_msrAfterGraceNotes afterGraceNotes);

    void                  prependAfterGraceNotesToMeasure (
                            S_msrAfterGraceNotes afterGraceNotes);
                            
    // segno
    
    void                  appendSegnoToMeasure (S_msrSegno segno);

    // coda
    
    void                  appendCodaToMeasure (S_msrCoda coda);

    // eyeglasses
    
    void                  appendEyeGlassesToMeasure (
                            S_msrEyeGlasses eyeGlasses);

    // pedal
    
    void                  appendPedalToMeasure (S_msrPedal pedal);

    // other elements
    
    void                  prependOtherElementToMeasure (S_msrElement elem);
                      
    void                  appendOtherElementToMeasure (S_msrElement elem);

    // last element of measure
    
    S_msrElement          getLastElementOfMeasure () const
                              { return fMeasureElementsList.back (); }

     /* JMI                 
    S_msrElement          removeLastElementFromMeasure (
                            int inputLineNumber);
*/

    // removing elements from measure

    void                  removeNoteFromMeasure (
                            int       inputLineNumber,
                            S_msrNote note);

    void                  removeElementFromMeasure (S_msrElement elem); // JMI

/* JMI
    bool                  checkForIncompleteMeasure (
                            int                        inputLineNumber,
                            msrMeasure::msrMeasureKind measureKind);

    bool                  checkForOverfullMeasure (
                            int inputLineNumber);
*/

    // finalization

    void                  finalizeMeasure (
                            int inputLineNumber);

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // divisions
    
    int                   fMeasureDivisionsPerFullMeasure;
    int                   fMeasurePosition; // in divisions

    // measure number
    
    string                fMeasureNumber;

    // implicit kind
    
    msrMeasureImplicitKind
                          fMeasureImplicitKind; // JMI

    // measure kind

    msrMeasureKind        fMeasureKind;
    
    // measure 'first in segment' kind

    msrMeasureFirstInSegmentKind
                          fMeasureFirstInSegmentKind;
                        
    // chords handling
    
    S_msrNote             fMeasureLastHandledNote;
    
    // uplinks
    
    S_msrSegment          fMeasureSegmentUplink;

    S_msrVoice            fMeasureVoiceDirectUplink; // to accelerate things

    S_msrPart             fMeasureDirectPartUplink; // to accelerate things

    // elements list

    list<S_msrElement>    fMeasureElementsList;
};
typedef SMARTP<msrMeasure> S_msrMeasure;
EXP ostream& operator<< (ostream& os, const S_msrMeasure& elt);

/*!
\brief The msr sequential music element
*/
//______________________________________________________________________________
class EXP msrSegment : public msrElement
{
  public:
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSegment> create (
      int        inputLineNumber,
      S_msrPart  segmentDirectPartUplink,
      S_msrVoice segmentVoicekUplink);

    SMARTP<msrSegment> createSegmentShallowClone (
      S_msrVoice voiceClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSegment (
      int          inputLineNumber,
      S_msrPart    segmentDirectPartUplink,
      S_msrVoice   segmentVoicekUplink);
      
    virtual ~msrSegment();
    
  private:

    // initialization
    // ------------------------------------------------------

    void                  initializeSegment ();

    void                  createSegmentInitialMeasure ();

  public:

    // set and get
    // ------------------------------------------------------
                            
    S_msrPart             getSegmentDirectPartUplink () const
                             { return fSegmentDirectPartUplink; }

    int                   getSegmentAbsoluteNumber () const
                              { return fSegmentAbsoluteNumber; }
                      
    S_msrVoice            getSegmentVoiceUplink () const
                              { return fSegmentVoiceUplink; }
                      
    const list<S_msrMeasure>&
                          getSegmentMeasuresList () const
                              { return fSegmentMeasuresList; }
                                            
    list<S_msrMeasure>&   getSegmentMeasuresListToModify ()
                              { return fSegmentMeasuresList; }
                                            
    // measures
    
    void                  createMeasureAndAppendItToSegment (
                            int    inputLineNumber,
                            string measureNumber);
                      
    const string          getSegmentMeasureNumber () const
                              { return fSegmentMeasureNumber; }

    // services
    // ------------------------------------------------------

    string                segmentAsString ();

    // divisions
    
    void                  bringSegmentToMeasurePosition (
                            int inputLineNumber,
                            int measurePosition);
  
    // measures

    void                  appendMeasureToSegment (S_msrMeasure measure);

    void                  appendMeasureToSegmentIfNeeded (  // JMI
                            int    inputLineNumber,
                            string measureNumber);

    // clef, key, time
    
    void                  appendClefToSegment (S_msrClef clef);
    void                  appendKeyToSegment (S_msrKey key);
    void                  appendTimeToSegment (S_msrTime time);

    // transpose

    void                  appendTransposeToSegment (
                            S_msrTranspose transpose);

    // staff details

    void                  appendStaffDetailsToSegment (
                            S_msrStaffDetails staffDetails);

    // tempo
    
    void                  appendTempoToSegment (S_msrTempo tempo);

    // rehearsals
    
    void                  appendRehearsalToSegment (
                            S_msrRehearsal rehearsal);

    // octave shifts
    
    void                  appendOctaveShiftToSegment (
                            S_msrOctaveShift octaveShift);

    // harmonies
    
    void                  appendHarmonyToSegment (S_msrHarmony harmony);
    
    void                  appendHarmonyToSegmentClone (S_msrHarmony harmony);

    // notes
    
    void                  appendNoteToSegment      (S_msrNote note);
    void                  appendNoteToSegmentClone (S_msrNote note);

    // tremolos
    
    void                  appendDoubleTremoloToSegment (
                            S_msrDoubleTremolo doubleTremolo);

    void                  appendMeasureRepeatToSegment (
                            S_msrMeasureRepeat measureRepeat);

    // repeats
    
    void                  appendMultipleRestToSegment (
                            S_msrMultipleRest multipleRest);

    // chords
    
    void                  appendChordToSegment (S_msrChord chord);

    // tuplets
    
    void                  appendTupletToSegment (S_msrTuplet tuplet);

    // bar lines
    
    void                  appendBarlineToSegment (
                            S_msrBarline barline);

    void                  prependBarlineToSegment (
                            S_msrBarline barline);

    // bar checks
    
    void                  appendBarCheckToSegment (
                            S_msrBarCheck barCheck);

    // bar number checks
    
    void                  appendBarNumberCheckToSegment (
                            S_msrBarNumberCheck barNumberCheck);

    // breaks

    void                  appendBreakToSegment (S_msrBreak break_);

    // segno
    
    void                  appendSegnoToSegment (S_msrSegno segno);

    // coda
    
    void                  appendCodaToSegment (S_msrCoda coda);

    // eyeglasses
    
    void                  appendEyeGlassesToSegment (
                            S_msrEyeGlasses eyeGlasses);

    // pedal
    
    void                  appendPedalToSegment (S_msrPedal pedal);

    // grace notes
    
    void                  appendGraceNotesToSegment (
                            S_msrGraceNotes graceNotes);

    void                  prependGraceNotesToSegment (
                            S_msrGraceNotes graceNotes);
    
    void                  appendAfterGraceNotesToSegment (
                            S_msrAfterGraceNotes afterGraceNotes);

    void                  prependAfterGraceNotesToSegment (
                            S_msrAfterGraceNotes afterGraceNotes);

    // other elements
    
    void                  appendOtherElementToSegment (S_msrElement elem);

    // removing elements
    
    void                  removeNoteFromSegment (
                            int       inputLineNumber,
                            S_msrNote note);

    S_msrMeasure          removeLastMeasureFromSegment (
                            int inputLineNumber);

    // finalization

    void                  finalizeCurrentMeasureInSegment (
                            int inputLineNumber);

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    S_msrPart             fSegmentDirectPartUplink;
    
    static int            gSegmentsCounter;

    int                   fSegmentAbsoluteNumber;
        
    // the measures in the segment contain the mmusic
    list<S_msrMeasure>    fSegmentMeasuresList;

    string                fSegmentMeasureNumber;

    bool                  fMeasureNumberHasBeenSetInSegment; // JMI

    S_msrVoice            fSegmentVoiceUplink;
};
typedef SMARTP<msrSegment> S_msrSegment;
EXP ostream& operator<< (ostream& os, const S_msrSegment& elt);

/*!
\brief A msr repeat representation.

  A repeat is represented by:
    - a sequence of elements for the common segment
    - a vector of sequences of elements for the alternate endings
*/
//______________________________________________________________________________
class EXP msrGraceNotes : public msrElement
{
  public:
        
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrGraceNotes> create (
      int        inputLineNumber,
      S_msrPart  graceNotesDirectPartUplink,
      bool       slashed,
      S_msrVoice graceNotesVoiceUplink);
    
    SMARTP<msrGraceNotes> createGraceNotesShallowClone (
      S_msrVoice voiceClone);

    SMARTP<msrGraceNotes> createSkipGraceNotesClone (
      S_msrVoice voiceClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrGraceNotes (
      int        inputLineNumber,
      S_msrPart  graceNotesDirectPartUplink,
      bool       gracenoteIsSlashed,
      S_msrVoice graceNotesVoiceUplink);
      
    virtual ~msrGraceNotes();
  
  public:

    // set and get
    // ------------------------------------------------------
                              
    S_msrPart             getGraceNotesDirectPartUplink () const
                              { return fGraceNotesDirectPartUplink; }

    list<S_msrNote>&      getGraceNotesNotesList ()
                              { return fGraceNotesNotesList; }

    bool                  getGraceNotesIsSlashed () const
                              { return fGraceNotesIsSlashed; }

    // services
    // ------------------------------------------------------

    void                  appendNoteToGraceNotes (S_msrNote note);

    string                graceNotesAsShortString () const;
    
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    S_msrPart             fGraceNotesDirectPartUplink;

    list<S_msrNote>       fGraceNotesNotesList;

    bool                  fGraceNotesIsSlashed;

    S_msrVoice            fGraceNotesVoiceUplink;
};
typedef SMARTP<msrGraceNotes> S_msrGraceNotes;
EXP ostream& operator<< (ostream& os, const S_msrGraceNotes& elt);

/*!
\brief A msr repeat representation.

  A repeat is represented by:
    - a sequence of elements for the common segment
    - a vector of sequences of elements for the alternate endings
*/
//______________________________________________________________________________
class EXP msrAfterGraceNotes : public msrElement
{
  public:
        
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrAfterGraceNotes> create (
      int        inputLineNumber,
      S_msrPart  afterGraceNotesDirectPartUplink,
      S_msrNote  afterGraceNotesNote,
      bool       aftergracenoteIsSlashed,
      S_msrVoice afterGraceNotesVoiceUplink);
    
    SMARTP<msrAfterGraceNotes> createAfterGraceNotesShallowClone (
      S_msrNote  noteClone,
      S_msrVoice voiceClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrAfterGraceNotes (
      int        inputLineNumber,
      S_msrPart  afterGraceNotesDirectPartUplink,
      S_msrNote  afterGraceNotesNote,
      bool       aftergracenoteIsSlashed,
      S_msrVoice afterGraceNotesVoiceUplink);
      
    virtual ~msrAfterGraceNotes();
  
  public:

    // set and get
    // ------------------------------------------------------
                              
    S_msrPart             getAfterGraceNotesDirectPartUplink () const
                              { return fAfterGraceNotesDirectPartUplink; }

    S_msrNote             getAfterGraceNotesNote ()
                              { return fAfterGraceNotesNote; }

    list<S_msrNote>&      getAfterGraceNotesNotesList ()
                              { return fAfterGraceNotesNotesList; }

    void                  setAfterGraceNotesIsSlashed ()
                              { fAfterGraceNotesIsSlashed = true; }
                              
    bool                  getAfterGraceNotesIsSlashed () const
                              { return fAfterGraceNotesIsSlashed; }

    // services
    // ------------------------------------------------------

    void                  appendNoteToAfterGraceNotes (S_msrNote note);

    string                afterGraceNotesAsShortString () const;
    
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    S_msrPart             fAfterGraceNotesDirectPartUplink;

    S_msrNote             fAfterGraceNotesNote;

    list<S_msrNote>       fAfterGraceNotesNotesList;

    bool                  fAfterGraceNotesIsSlashed;
    
    S_msrVoice            fAfterGraceNotesVoiceUplink;
};
typedef SMARTP<msrAfterGraceNotes> S_msrAfterGraceNotes;
EXP ostream& operator<< (ostream& os, const S_msrAfterGraceNotes& elt);

/*!
\brief A words representation.

  A words is represented by the stanza to use
*/
//______________________________________________________________________________
class EXP msrWords : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrWordsPlacementKind {
      kWordsPlacementAbove, kWordsPlacementBelow};

    static string wordsPlacementKindAsString (
      msrWordsPlacementKind wordsPlacementKind);
      
    enum msrWordsFontStyleKind {
      kNormalStyle, KItalicStyle};

    static string wordsFontStyleKindAsString (
      msrWordsFontStyleKind wordsFontStyleKind);
      
    enum msrWordsFontWeightKind {
      kNormalWeight, kBoldWeight};

    static string msrWordsFontWeightKindAsString (
      msrWordsFontWeightKind wordsFontWeightKind);
      
    enum msrWordsXMLLangKind {
      kItLang, kEnLang, kDeLang, kFrLang, kJaLang};

    static string msrWordsXMLLangKindAsString (
      msrWordsXMLLangKind wordsXMLLangKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrWords> create (
      int                    inputLineNumber,
      msrWordsPlacementKind  wordsPlacementKind,
      string                 wordsContents,
      msrWordsFontStyleKind  wordsFontStyleKind,
      string                 wordsFontSize,
      msrWordsFontWeightKind wordsFontWeightKind,
      msrWordsXMLLangKind    wordsXMLLangKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrWords (
      int                    inputLineNumber,
      msrWordsPlacementKind  wordsPlacementKind,
      string                 wordsContents,
      msrWordsFontStyleKind  wordsFontStyleKind,
      string                 wordsFontSize,
      msrWordsFontWeightKind wordsFontWeightKind,
      msrWordsXMLLangKind    wordsXMLLangKind);
      
    virtual ~msrWords();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrWordsPlacementKind   getWordsPlacementKind () const
                                { return fWordsPlacementKind; }

    string                  getWordsContents () const
                                { return fWordsContents; }

    msrWordsFontStyleKind   getWordsFontStyleKind () const
                                { return fWordsFontStyleKind; }

    string                  getWordsFontSize () const
                                { return fWordsFontSize; }

    msrWordsFontWeightKind  getWordsFontWeightKind () const
                                { return fWordsFontWeightKind; }

    msrWordsXMLLangKind     getWordsXMLLangKind () const
                                { return fWordsXMLLangKind; }

    // services
    // ------------------------------------------------------

    string    wordsAsString () const;
    
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:
                        
  msrWordsPlacementKind  fWordsPlacementKind;
  
  string                 fWordsContents;

  msrWordsFontStyleKind  fWordsFontStyleKind;
  string                 fWordsFontSize;
  msrWordsFontWeightKind fWordsFontWeightKind;
  
  msrWordsXMLLangKind    fWordsXMLLangKind;
};
typedef SMARTP<msrWords> S_msrWords;
EXP ostream& operator<< (ostream& os, const S_msrWords& elt);

/*!
\brief A msr syllable representation.

  A syllable is represented by a type and and a string.
  In the case of "single", the list contains only one string
*/
//______________________________________________________________________________
class EXP msrSyllable : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    // we want to end the line in the LilyPond code at a break
    enum msrSyllableKind {
      k_NoSyllable,
      kSingleSyllable, kBeginSyllable, kMiddleSyllable, kEndSyllable,
      kRestSyllable, kSkipSyllable,
      kTiedSyllable,
      kSlurSyllable, kSlurBeyondEndSyllable,
      kLigatureSyllable, kLigatureBeyondEndSyllable,
      kBarcheckSyllable, kBarNumberCheckSyllable,
      kBreakSyllable};

    static string syllableKindAsString (
      msrSyllableKind syllableKind);
      
    enum msrSyllableExtendKind {
      k_NoSyllableExtend,
      kStandaloneSyllableExtend,
      kStartSyllableExtend, kContinueSyllableExtend, kStopSyllableExtend};

    static string syllableExtendKindAsString (
      msrSyllableExtendKind syllableExtendKind);


    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSyllable> create (
      int                   inputLineNumber,
      S_msrPart             syllableDirectPartUplink,
      msrSyllableKind       syllableKind,
      string                syllableText,
      msrSyllableExtendKind syllableExtendKind,
      int                   divisions,
      S_msrStanza           syllableStanzaUplink);

    SMARTP<msrSyllable> createSyllableShallowClone (S_msrPart partClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSyllable (
      int                   inputLineNumber,
      S_msrPart             syllableDirectPartUplink,
      msrSyllableKind       syllableKind,
      string                syllableText,
      msrSyllableExtendKind syllableExtendKind,
      int                   divisions,
      S_msrStanza           syllableStanzaUplink);
        
    virtual ~msrSyllable();

  public:

    // set and get
    // ------------------------------------------------------
                              
    S_msrPart             getSyllableDirectPartUplink () const
                             { return fSyllableDirectPartUplink; }

    msrSyllableKind       getSyllableKind () const
                              { return fSyllableKind; }

    void                  setSyllableNoteUplink (S_msrNote note);

    S_msrNote             getSyllableNoteUplink () const
                              { return fSyllableNoteUplink; }

    string                getSyllableText () const
                              { return fSyllableText; }

    msrSyllableExtendKind getSyllableExtendKind () const
                              { return fSyllableExtendKind; }

    int                   getSyllableDivisions () const
                              { return fSyllableDivisions; }

    // services
    // ------------------------------------------------------
  
    string                syllableKindAsString () const;

    string                syllableDivisionsAsString () const;

    string                syllableNoteUplinkAsString () const;

    string                syllableAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:
  
    S_msrPart             fSyllableDirectPartUplink;

    msrSyllableKind       fSyllableKind;
    string                fSyllableText;
    msrSyllableExtendKind fSyllableExtendKind;
    
    int                   fSyllableDivisions;
    
    S_msrStanza           fSyllableStanzaUplink;

    S_msrNote             fSyllableNoteUplink;
};
typedef SMARTP<msrSyllable> S_msrSyllable;
EXP ostream& operator<< (ostream& os, const S_msrSyllable& elt);

/*!
\brief A msr harmony representation.

  A harmony is represented by a list of syllables,
*/
//______________________________________________________________________________
class EXP msrHarmony : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrHarmonyKind {
      k_NoHarmony,
      kMajor, kMinor, kDominant,
      kAugmented, kDiminished,
      kSuspendedFourth,
      kMajorSeventh,kMinorSeventh,
      kMajorNinth, kMinorNinth};

    static string harmonyKindAsString (
      msrHarmonyKind harmonyKind);
      
    enum msrHarmonyDegreeTypeKind {
      kAdd, kAlter, kSubstract};

    static string harmonyDegreeTypeKindAsString (
      msrHarmonyDegreeTypeKind harmonyDegreeTypeKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrHarmony> create (
      int                  inputLineNumber,
      S_msrPart            harmonyDirectPartUplink,
      msrQuarterTonesPitch harmonyRootQuarterTonesPitch,
      msrHarmonyKind       harmonyKind,
      string               harmonyKindText,
      msrQuarterTonesPitch harmonyBassQuarterTonesPitch,
      int                  harmonySoundingDivisions);
    
    SMARTP<msrHarmony> createHarmonyShallowClone (
      S_msrPart partClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrHarmony (
      int                  inputLineNumber,
      S_msrPart            harmonyDirectPartUplink,
      msrQuarterTonesPitch harmonyRootQuarterTonesPitch,
      msrHarmonyKind       harmonyKind,
      string               harmonyKindText,
      msrQuarterTonesPitch harmonyBassQuarterTonesPitch,
      int                  harmonySoundingDivisions);

    virtual ~msrHarmony();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrPart             getHarmonyDirectPartUplink () const
                             { return fHarmonyDirectPartUplink; }

 // JMI   void                  setHarmonySoundingDivisions (int divisions)
       //                       { fHarmonySoundingDivisions = divisions; }

    int                   getHarmonySoundingDivisions () const
                              { return fHarmonySoundingDivisions; }

    msrQuarterTonesPitch  getHarmonyRootQuarterTonesPitch () const
                              { return fHarmonyRootQuarterTonesPitch; }
                                
    msrHarmonyKind        getHarmonyKind () const
                              { return fHarmonyKind; }
                
    string                getHarmonyKindText () const
                              { return fHarmonyKindText; }
                
    msrQuarterTonesPitch  getHarmonyBassQuarterTonesPitch () const
                              { return fHarmonyBassQuarterTonesPitch; }                                              

    // services
    // ------------------------------------------------------

    string                harmonyKindAsString () const;
    string                harmonyKindAsShortString () const;
    
    string                harmonyAsString () const;
   
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    S_msrPart             fHarmonyDirectPartUplink;

    int                   fHarmonySoundingDivisions;
    
    msrQuarterTonesPitch  fHarmonyRootQuarterTonesPitch;

    msrHarmonyKind        fHarmonyKind;
    string                fHarmonyKindText;

    msrQuarterTonesPitch  fHarmonyBassQuarterTonesPitch;
};
typedef SMARTP<msrHarmony> S_msrHarmony;
EXP ostream& operator<< (ostream& os, const S_msrHarmony& elt);

/*!
\brief A msr note representation.

  A note is represented by its name, optional accidentals,
    duration (in the form of numerator/denominator) and optional dots.
*/
//______________________________________________________________________________
class EXP msrNote : public msrElement
{
  public:

    // constants
    // ------------------------------------------------------

    #define K_NO_OCTAVE -1

    // data types
    // ------------------------------------------------------

    enum msrNoteKind {
      k_NoNoteKind,
      kRestNote, kSkipNote,
      kStandaloneNote,
      kDoubleTremoloMemberNote,
      kGraceNote,
      kChordMemberNote,
      kTupletMemberNote};
      
    static string noteKindAsString (
      msrNoteKind noteKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrNote> create (
      int                  inputLineNumber,
      S_msrPart            noteDirectPartUplink,
      
      msrNoteKind          noteKind,
    
      msrQuarterTonesPitch noteQuarterTonesPitch,
      int                  noteSoundingDivisions,
      int                  noteDisplayedDivisions,
      int                  noteDotsNumber,
      msrDuration          noteGraphicDuration,
      int                  noteOctave,
      
      msrQuarterTonesPitch noteQuarterTonesDisplayPitch,
      int                  noteDisplayOctave,
      
      bool                 noteIsARest,
      bool                 noteIsUnpitched,
    
      bool                 noteIsAGraceNote);
    
    SMARTP<msrNote> createNoteShallowClone (S_msrPart partClone);
    
    // creation from xml2Msr
    // ------------------------------------------------------

    static SMARTP<msrNote> createSkipNote (
      int       inputLineNumber,
      S_msrPart noteDirectPartUplink,
      int       divisions,
      int       dotsNumber,
      int       staffNumber,
      int       externalVoiceNumber);
    
  protected:
 
    // constructors/destructor
    // ------------------------------------------------------

    msrNote (
      int                  inputLineNumber,
      S_msrPart            noteDirectPartUplink,

      msrNoteKind          noteKind,
    
      msrQuarterTonesPitch noteQuarterTonesPitch,
      int                  noteSoundingDivisions,
      int                  noteDisplayedDivisions,
      int                  noteDotsNumber,
      msrDuration          noteGraphicDuration,
      int                  noteOctave,

      msrQuarterTonesPitch noteQuarterTonesDisplayPitch,
      int                  noteDisplayOctave,
      
      bool                 noteIsARest,
      bool                 noteIsUnpitched,
    
      bool                 noteIsAGraceNote);
        
    virtual ~msrNote();
    
  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeNote ();
  
  public:

    // set and get
    // ------------------------------------------------------

    // basic note description
    // -------------------------------

    // note kind
    
    void                  setNoteKind (msrNoteKind noteKind)
                              { fNoteKind = noteKind; }

    msrNoteKind           getNoteKind () const
                              { return fNoteKind; }

    // note pitch

    msrQuarterTonesPitch  getNoteQuarterTonesPitch () const
                              { return fNoteQuarterTonesPitch; }
                              
    int                   getNoteSoundingDivisions () const
                              { return fNoteSoundingDivisions; }

    // note display
    
    msrQuarterTonesPitch  getNoteQuarterTonesDisplayPitch () const
                              { return fNoteQuarterTonesDisplayPitch; }
                              
    void                  setNoteDisplayedDivisions (int divisions)
                              { fNoteDisplayedDivisions = divisions; }

    int                   getNoteDisplayedDivisions () const
                              { return fNoteDisplayedDivisions; }

    // dots
    
    int                   getNoteDotsNumber () const
                              { return fNoteDotsNumber; }

    msrDuration           getNoteGraphicDuration () const
                              { return fNoteGraphicDuration; }

    int                   getNoteOctave () const
                              { return fNoteOctave; }

    int                   getNoteDisplayOctave () const
                              { return fNoteDisplayOctave; }

    bool                  getNoteIsARest () const
                            // useful for rest tuplet members
                            { return fNoteIsARest; }
                      
    bool                  getNoteIsUnpitched () const
                              { return fNoteIsUnpitched; }
                      
    // octave shifts
    void                  setNoteOctaveShift (
                             S_msrOctaveShift octaveShift)
                                { fNoteOctaveShift = octaveShift; }

    S_msrOctaveShift      getNoteOctaveShift () const
                              { return fNoteOctaveShift; }

    // note context
    // -------------------------------

    // direct part uplink
    S_msrPart             getNoteDirectPartUplink () const
                             { return fNoteDirectPartUplink; }

    // chord members
    void                  setNoteBelongsToAChord ();

    bool                  getNoteBelongsToAChord () const
                              { return fNoteBelongsToAChord; }

    // tuplet uplink
    void                  setNoteTupletUplink (
                            const S_msrTuplet& tuplet)
                              { fNoteTupletUplink = tuplet; }
                      
    S_msrTuplet           getNoteTupletUplink () const
                              { return fNoteTupletUplink; }

    // harmony
    void                  setNoteHarmony (S_msrHarmony harmony);
                              
    const S_msrHarmony&   getNoteHarmony () const
                              { return fNoteHarmony; };

    // note lyrics
    // -------------------------------

    list<S_msrSyllable>   getNoteSyllables () const
                              { return fNoteSyllables; }              

    void                  setNoteSyllableExtendKind (
                            msrSyllable::msrSyllableExtendKind
                              syllableExtendKind)
                              {
                                fNoteSyllableExtendKind =
                                  syllableExtendKind;
                              }

    msrSyllable::msrSyllableExtendKind
                          getNoteSyllableExtendKind () const
                              { return fNoteSyllableExtendKind; }              

    // elements attached to the note
    // -------------------------------

    // stems
    void                  setNoteStem (S_msrStem stem);

    S_msrStem             getNoteStem () const
                              { return fNoteStem; }

    // beams
    const list<S_msrBeam>&
                          getNoteBeams () const
                              { return fNoteBeams; }

    // articulations
    const list<S_msrArticulation>&
                          getNoteArticulations () const
                              { return fNoteArticulations; }
                      
    list<S_msrArticulation>&
                          getNoteArticulationsToModify ()
                              { return fNoteArticulations; }

    // technicals
    const list<S_msrTechnical>&
                          getNoteTechnicals () const
                              { return fNoteTechnicals; }
                              
    const list<S_msrTechnicalWithInteger>&
                          getNoteTechnicalWithIntegers () const
                              { return fNoteTechnicalWithIntegers; }
                              
    const list<S_msrTechnicalWithString>&
                          getNoteTechnicalWithStrings () const
                              { return fNoteTechnicalWithStrings; }
                              
    // ornaments
    const list<S_msrOrnament>&
                          getNoteOrnaments () const
                              { return fNoteOrnaments; }
      /* JMI                
    list<S_msrOrnament>&  getNoteOrnamentsToModify ()
                              { return fNoteOrnaments; }
        */
        
    // singleTremolo
    S_msrSingleTremolo    getNoteSingleTremolo () const
                              { return fNoteSingleTremolo; }

    // ties
    void                  setNoteTie (S_msrTie tie)
                              { fNoteTie = tie; }
    S_msrTie              getNoteTie () const
                              { return fNoteTie; }

    // dynamics
    const list<S_msrDynamics>&
                          getNoteDynamics () const
                              { return fNoteDynamics; };
    const list<S_msrOtherDynamics>&
                          getNoteOtherDynamics () const
                              { return fNoteOtherDynamics; };

    list<S_msrDynamics>&  getNoteDynamicsToModify () // JMI
                              { return fNoteDynamics; };
        
    // words
    const list<S_msrWords>&
                          getNoteWords () const
                              { return fNoteWords; };
                      
    list<S_msrWords>&     getNoteWordsToModify ()
                              { return fNoteWords; };
                      
    // wedges
    const list<S_msrWedge>&
                          getNoteWedges () const
                              { return fNoteWedges; };

    list<S_msrWedge>&
                          getNoteWedgesToModify ()
                              { return fNoteWedges; };

    // slurs
    const list<S_msrSlur>&
                          getNoteSlurs () const
                              { return fNoteSlurs; }

    // ligatures
    const list<S_msrLigature>&
                          getNoteLigatures () const
                              { return fNoteLigatures; }

    // note measure information
    // -------------------------------

    // measure number
    void                  setNoteMeasureNumber (
                            string measureNumber)
                              { fNoteMeasureNumber = measureNumber; }

    string                getNoteMeasureNumber () const
                              { return fNoteMeasureNumber; }
 
    // position in measure
    void                  setNotePositionInMeasure (
                            int positionInMeasure)
                              {
                                fNotePositionInMeasure =
                                  positionInMeasure;
                              }
                      
    int                   getNotePositionInMeasure () const
                              { return fNotePositionInMeasure; }

    void                  setNoteOccupiesAFullMeasure ()
                              { fNoteOccupiesAFullMeasure = true; }
                      
    bool                  getNoteOccupiesAFullMeasure () const
                              { return fNoteOccupiesAFullMeasure; }

    // note redundant information (for speed)
    bool                  getNoteIsStemless () const
                              { return fNoteIsStemless; }

    void                  setNoteIsFirstNoteInADoubleTremolo ()
                              { fNoteIsFirstNoteInADoubleTremolo = true; }

    bool                  getNoteIsFirstNoteInADoubleTremolo () const
                              { return fNoteIsFirstNoteInADoubleTremolo; }
                  
    void                  setNoteIsSecondNoteInADoubleTremolo ()
                              { fNoteIsSecondNoteInADoubleTremolo = true; }

    bool                  getNoteIsSecondNoteInADoubleTremolo () const
                              { return fNoteIsSecondNoteInADoubleTremolo; }
                  
    bool                  getNoteHasATrill () const
                              { return fNoteHasATrill; }
                  
    void                  setNoteIsFollowedByGraceNotes ()
                              { fNoteIsFollowedByGraceNotes = true; }

    bool                  getNoteIsFollowedByGraceNotes () const
                              { return fNoteIsFollowedByGraceNotes; }
                  
    bool                  getNoteHasADelayedOrnament () const
                              { return fNoteHasADelayedOrnament; }
                  
    // measure uplink
    void                  setNoteMeasureUplink (
                            const S_msrMeasure& measure)
                              { fNoteMeasureUplink = measure; }
                      
    S_msrMeasure          getNoteMeasureUplink () const
                              { return fNoteMeasureUplink; }

    // services
    // ------------------------------------------------------

    string                notePitchAsString () const;
    string                noteDisplayPitchAsString () const;

    string                noteDisplayOctaveAsString () const;
    
    bool                  noteIsAPitchedRest () const;
    
    string                noteAsShortStringWithRawDivisions () const;
    string                noteAsShortString () const;
    string                noteAsString () const;
      
    // diatonic pitch
    msrDiatonicPitch      noteDiatonicPitch (
                            int inputLineNumber) const;
                            
    string                noteDiatonicPitchAsString (
                            int inputLineNumber) const;

    // divisions
    string                noteSoundingDivisionsAsMsrString () const;
    string                noteDisplayedDivisionsAsMsrString () const;
    
    string                skipOrRestDivisionsAsMsrString () const;
    
    // graphic duration
    string                noteGraphicDurationAsMsrString () const;
    string                tupletNoteGraphicDurationAsMsrString (
                            int actualNotes, int normalNotes) const;

    // tuplet members
    void                  applyTupletMemberDisplayFactorToNote (
                            int actualNotes, int normalNotes);
                    
    // lyrics
    void                  appendSyllableToNote (S_msrSyllable syllable);

    // beams
    void                  addBeamToNote (S_msrBeam beam);
    
    // articulations
    void                  addArticulationToNote (S_msrArticulation art);
    
    // technicals
    void                  addTechnicalToNote (S_msrTechnical tech);
    
    void                  addTechnicalWithIntegerToNote (
                            S_msrTechnicalWithInteger tech);
    
    void                  addTechnicalWithStringToNote (
                            S_msrTechnicalWithString tech);
    
    // ornaments
    void                  addOrnamentToNote (S_msrOrnament orn);
    
    // singleTremolo
    void                  addSingleTremoloToNote (S_msrSingleTremolo trem);
    
    // dynamics
    void                  addDynamicsToNote (S_msrDynamics dynamics);
    void                  addOtherDynamicsToNote (S_msrOtherDynamics otherDynamics);

    S_msrDynamics         removeFirstDynamics (); // ???

    // words
    void                  addWordsToNote (S_msrWords words);
    
    // slurs
    void                  addSlurToNote (S_msrSlur slur);
    
    // ligatures
    void                  addLigatureToNote (S_msrLigature ligature);
    
    //  wedges
    void                  addWedgeToNote (S_msrWedge wedge);

    S_msrWedge            removeFirstWedge (); // JMI

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // divisions handling is done at the part level
    S_msrPart                 fNoteDirectPartUplink;
    
    // basic note description
    // ------------------------------------------------------

    msrNoteKind           fNoteKind;

    msrQuarterTonesPitch  fNoteQuarterTonesPitch;
    int                   fNoteSoundingDivisions;
    int                   fNoteDisplayedDivisions;
    int                   fNoteDotsNumber;
    msrDuration           fNoteGraphicDuration;
    
    int                   fNoteOctave;
    
    msrQuarterTonesPitch  fNoteQuarterTonesDisplayPitch;
    int                   fNoteDisplayOctave;
                                // for unpitched notes
                                // and pitched rests
    
    bool                  fNoteIsARest;
    bool                  fNoteIsUnpitched;
  
    bool                  fNoteIsAGraceNote;
    
    S_msrOctaveShift      fNoteOctaveShift; // JMI ???

    // staff and voice context
    // ------------------------------------------------------

    int                   fNoteStaffNumber;
    int                   fNoteVoiceNumber;

    // chord member?
    // ------------------------------------------------------

    bool                  fNoteBelongsToAChord;
    
    // tuplet member?
    // ------------------------------------------------------

    bool                  fNoteBelongsToATuplet;
    S_msrTuplet           fNoteTupletUplink;

    // multiple rest member?
    // ------------------------------------------------------

    bool                  fNoteBelongsToAMultipleRest; // JMI
    int                   fNoteMultipleRestSequenceNumber; // JMI

    // lyrics
    // ------------------------------------------------------

   list<S_msrSyllable>    fNoteSyllables;
    msrSyllable::msrSyllableExtendKind
                      fNoteSyllableExtendKind; // MEGA JMI
    
    // stem
    // ------------------------------------------------------

    S_msrStem             fNoteStem;

    // beams
    // ------------------------------------------------------

    list<S_msrBeam>       fNoteBeams;
                                      
    // articulations
    // ------------------------------------------------------

    list<S_msrArticulation>
                          fNoteArticulations;

    // technicals
    // ------------------------------------------------------

    list<S_msrTechnical>  fNoteTechnicals;
    list<S_msrTechnicalWithInteger>
                      fNoteTechnicalWithIntegers;
    list<S_msrTechnicalWithString>
                      fNoteTechnicalWithStrings;

    // ornaments
    // ------------------------------------------------------

    list<S_msrOrnament>   fNoteOrnaments;
    
    // single tremolo
    // ------------------------------------------------------

    S_msrSingleTremolo    fNoteSingleTremolo;

    // tie
    // ------------------------------------------------------

    S_msrTie              fNoteTie;
    
    // dynamics
    // ------------------------------------------------------

    list<S_msrDynamics>   fNoteDynamics;
    list<S_msrOtherDynamics>
                          fNoteOtherDynamics;
    list<S_msrWedge>      fNoteWedges;

    // words
    // ------------------------------------------------------

    list<S_msrWords>      fNoteWords;

    // slurs
    // ------------------------------------------------------

    list<S_msrSlur>       fNoteSlurs;

    // ligatures
    // ------------------------------------------------------

    list<S_msrLigature>   fNoteLigatures;

    // harmony
    // ------------------------------------------------------

    S_msrHarmony          fNoteHarmony;

    // note measure information
    // ------------------------------------------------------

    string                fNoteMeasureNumber;
    int                   fNotePositionInMeasure;
    bool                  fNoteOccupiesAFullMeasure;
    S_msrMeasure          fNoteMeasureUplink;

    // note redundant information (for speed)
    // ------------------------------------------------------

    // this is needed to handle stemless notes,
    // because the <stem> is visited after 'visitorStart ( S_msrNote )' 
    bool                  fNoteIsStemless;

    // this is needed to handle double tremolos
    bool                  fNoteIsFirstNoteInADoubleTremolo;
    bool                  fNoteIsSecondNoteInADoubleTremolo;

    // this is useful to produce a nice \aftergrace in LilyPond 
    bool                  fNoteHasATrill;
    bool                  fNoteIsFollowedByGraceNotes;

    // this is needed to produce a delayed turn/inverted-turn in LilyPond 
    bool                  fNoteHasADelayedOrnament;
};
typedef SMARTP<msrNote> S_msrNote;
EXP ostream& operator<< (ostream& os, const S_msrNote& elt);

/*!
\brief The msr chord element
*/
//______________________________________________________________________________
class EXP msrChord : public msrElement
{
  public:

    static SMARTP<msrChord> create (
      int         inputLineNumber,
      S_msrPart   chordDirectPartUplink,
      int         chordSoundingDivisions,
      int         chordDisplayedDivisions,
      msrDuration chordGraphicDuration);

    // creation from MusicXML
    // ------------------------------------------------------

    SMARTP<msrChord>      createChordShallowClone (
                            S_msrPart partClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrChord (
      int         inputLineNumber,
      S_msrPart   chordDirectPartUplink,
      int         chordSoundingDivisions,
      int         chordDisplayedDivisions,
      msrDuration chordGraphicDuration);
      
    virtual ~msrChord();
  
  public:

    // set and get
    // ------------------------------------------------------
                              
     // divisions
    void                  setChordSoundingDivisions (int divisions);
            
    int                   getChordSoundingDivisions () const
                              { return fChordSoundingDivisions; }
                        
    void                  setChordDisplayedDivisions (int divisions);
            
    int                   getChordDisplayedDivisions () const
                              { return fChordDisplayedDivisions; }
                        
    string                chordSoundingDivisionsAsMsrString () const;
    string                chordDisplayedDivisionsAsMsrString () const;

    // direct part uplink
    S_msrPart             getChordDirectPartUplink () const
                             { return fChordDirectPartUplink; }

    // graphic duration
    msrDuration           getChordGraphicDuration () const
                              { return fChordGraphicDuration; }
            
    // chord notes
    const vector<S_msrNote>&
                          getChordNotes () const
                              { return fChordNotes; }

    // articulations
    const list<S_msrArticulation>&
                          getChordArticulations () const
                              { return fChordArticulations; }

    // technicals
    const list<S_msrTechnical>&
                          getChordTechnicals () const
                              { return fChordTechnicals; }
                              
    const list<S_msrTechnicalWithInteger>&
                          getChordTechnicalWithIntegers () const
                              { return fChordTechnicalWithIntegers; }
    
    const list<S_msrTechnicalWithString>&
                          getChordTechnicalWithStrings () const
                              { return fChordTechnicalWithStrings; }
    
    // ornaments
    const list<S_msrOrnament>&
                          getChordOrnaments () const
                              { return fChordOrnaments; }
    
    // singleTremolo
    S_msrSingleTremolo    getChordSingleTremolo () const
                              { return fChordSingleTremolo; }

    // dynamics
    const list<S_msrDynamics>&
                          getChordDynamics () const
                              { return fChordDynamics; }
    const list<S_msrOtherDynamics>&
                          getChordOtherDynamics () const
                              { return fChordOtherDynamics; }
                      
    const list<S_msrWedge>&
                          getChordWedges () const
                              { return fChordWedges; }

    // words
    const list<S_msrWords>&
                          getChordWords () const
                              { return fChordWords; }
                      
    // slurs
    const list<S_msrSlur>&
                          getChordSlurs () const
                              { return fChordSlurs; }
                      
    // ligatures
    const list<S_msrLigature>&
                          getChordLigatures () const
                              { return fChordLigatures; }
                      
    // double tremolo
    void                  setChordIsFirstChordInADoubleTremolo ()
                              { fChordIsFirstChordInADoubleTremolo = true; }

    bool                  getChordIsFirstChordInADoubleTremolo () const
                              { return fChordIsFirstChordInADoubleTremolo; }
                  
    void                  setChordIsSecondChordInADoubleTremolo ()
                              { fChordIsSecondChordInADoubleTremolo = true; }

    bool                  getChordIsSecondChordInADoubleTremolo () const
                              { return fChordIsSecondChordInADoubleTremolo; }
                  
    // tie
    void                  setChordTie (
                            const S_msrTie tie)
                              { fChordTie = tie; }

    S_msrTie              getChordTie () const
                              { return fChordTie; }

    // harmony
    void                  setChordHarmony (S_msrHarmony harmony)
                              { fChordHarmony = harmony; }
                      
    const S_msrHarmony&   getChordHarmony () const
                              { return fChordHarmony; };
                      
    // measure uplink
    void                  setChordMeasureUplink (
                            const S_msrMeasure& measure)
                              { fChordMeasureUplink = measure; }
                      
    S_msrMeasure          getChordMeasureUplink () const
                            { return fChordMeasureUplink; }

    // measure number
    void                  setChordMeasureNumber (
                            string measureNumber)
                              { fChordMeasureNumber = measureNumber; }
    
    string                getChordMeasureNumber () const
                              { return fChordMeasureNumber; }
 
    // position in measure
    void                  setChordPositionInMeasure (int position)
                              { fChordPositionInMeasure = position; }

    const int             getChordPositionInMeasure () const
                              { return fChordPositionInMeasure; }
                         
    // services
    // ------------------------------------------------------
    
    string                chordGraphicDurationAsMsrString () const;

    void                  addFirstNoteToChord (S_msrNote note);
    void                  addAnotherNoteToChord (S_msrNote note);

 // JMI   S_msrNote     chordLastNote () const
       //               { return fChordNotes.back (); }

    // articulations
    void                  addArticulationToChord (S_msrArticulation art);
     
    // technicals
    void                  addTechnicalToChord (S_msrTechnical tech);
    
    void                  addTechnicalWithIntegerToChord (
                            S_msrTechnicalWithInteger tech);
    
    void                  addTechnicalWithStringToChord (
                            S_msrTechnicalWithString tech);
    
    // ornaments
    void                  addOrnamentToChord (S_msrOrnament orn);
     
    // singleTremolo
    void                  addSingleTremoloToChord (S_msrSingleTremolo trem);
    
    // dynamics
    void                  addDynamicsToChord (S_msrDynamics dynamic)
                              { fChordDynamics.push_back (dynamic); }
    void                  addOtherDynamicsToChord (S_msrOtherDynamics otherDynamic)
                              { fChordOtherDynamics.push_back (otherDynamic); }
    void                  addWedgeToChord (S_msrWedge wedge)
                              { fChordWedges.push_back (wedge); }
                    
    // words
    void                  addWordsToChord (S_msrWords dynamic)
                              { fChordWords.push_back (dynamic); }
                    
    // slurs
    void                  addSlurToChord (S_msrSlur slur)
                              { fChordSlurs.push_back (slur); }
                      
    // ligatures
    void                  addLigatureToChord (S_msrLigature ligature)
                              { fChordLigatures.push_back (ligature); }
                      
    // tuplet members
    /* JMI
    void                  applyTupletMemberDisplayedFactorToChordMembers (
                            int actualNotes, int normalNotes);
*/

    // position in measure
    void                  setChordFirstNotePositionInMeasure (
                            int position);
                    
    void                  setChordFirstNoteMeasureNumber (
                            string measureNumber);
                    
    // harmony
    string                chordAsStringwithRawDivisions () const;
    string                chordAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // divisions handling is done at the part level
    S_msrPart                 fChordDirectPartUplink;

    // sounding divisions
    int                       fChordSoundingDivisions;
    // displayed divisions
    int                       fChordDisplayedDivisions;
                                  
    // graphic duration is needed for grace notes,
    // since they don't have any note (sounding) duration
    msrDuration               fChordGraphicDuration;
    
    vector<S_msrNote>         fChordNotes;

    // position in measure
    S_msrMeasure              fChordMeasureUplink;

    string                    fChordMeasureNumber;
    int                       fChordPositionInMeasure;

    // articulations
    list<S_msrArticulation>   fChordArticulations;

    // single tremolo
    S_msrSingleTremolo        fChordSingleTremolo;
    
    // double tremolo
    bool                      fChordIsFirstChordInADoubleTremolo;
    bool                      fChordIsSecondChordInADoubleTremolo;

    // technicals
    list<S_msrTechnical>      fChordTechnicals;
    
    list<S_msrTechnicalWithInteger>
                              fChordTechnicalWithIntegers;
    
    list<S_msrTechnicalWithString>
                              fChordTechnicalWithStrings;
    
    // ornaments
    list<S_msrOrnament>       fChordOrnaments;

    // single tremolo
    S_msrSingleTremolo        fNoteSingleTremolo;
    
    // ties
    S_msrTie                  fChordTie;
    
    // dynamics
    list<S_msrDynamics>       fChordDynamics;
    list<S_msrOtherDynamics>  fChordOtherDynamics;

    list<S_msrWedge>          fChordWedges;
    
    // words
    list<S_msrWords>          fChordWords;
    
    // slurs
    list<S_msrSlur>           fChordSlurs;

    // ligatures
    list<S_msrLigature>       fChordLigatures;

    // harmony
    S_msrHarmony              fChordHarmony;
};
typedef SMARTP<msrChord> S_msrChord;
EXP ostream& operator<< (ostream& os, const S_msrChord& elt);

/*!
\brief A msr LilyPond variable/value association representation.
*/
//______________________________________________________________________________
class EXP msrVarValAssoc : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrVarValAssoc> create (
      int           inputLineNumber,
      string        variableName,
      string        value);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrVarValAssoc (
      int           inputLineNumber,
      string        variableName,
      string        value);
      
    virtual ~msrVarValAssoc();
  
  public:

    // set and get
    // ------------------------------------------------------

    void          changeAssocValue (string value)
                      { fVariableValue = value; }

    string        getVariableName  () const
                      { return fVariableName; };
    string        getVariableValue () const
                      { return fVariableValue; };

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    string             fVariableName;
    string             fVariableValue;
};
typedef SMARTP<msrVarValAssoc> S_msrVarValAssoc;
EXP ostream& operator<< (ostream& os, const S_msrVarValAssoc& elt);

/*!
\brief A MSR identification representation.

  A header is represented by variable/value pairs
*/
//______________________________________________________________________________
class EXP msrIdentification : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrIdentification> create (
      int inputLineNumber);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrIdentification (
      int inputLineNumber);
      
    virtual ~msrIdentification();
  
  public:

    // set and get
    // ------------------------------------------------------

    void                  setWorkNumber (
                            int    inputLineNumber,
                            string val);

    void                  setWorkTitle (
                            int    inputLineNumber,
                            string val);

    void                  setMovementNumber (
                            int    inputLineNumber,
                            string val);
          
    void                  setMovementTitle (
                            int    inputLineNumber,
                            string val);
          
    void                  addComposer (
                            int    inputLineNumber,
                            string type,
                            string val);

    void                  addArranger (
                            int    inputLineNumber,
                            string type,
                            string val);

    void                  addPoet (
                            int    inputLineNumber,
                            string type,
                            string val);

    void                  addLyricist (
                            int    inputLineNumber,
                            string type,
                            string val);

    void                  setRights (
                            int    inputLineNumber,
                            string val);

    void                  addSoftware (
                            int    inputLineNumber,
                            string val);

    void                  setEncodingDate (
                            int    inputLineNumber,
                            string val);

    void                  setScoreInstrumentAssoc (
                            int    inputLineNumber,
                            string val);

    S_msrVarValAssoc      getWorkNumber () const
                              { return fWorkNumber; }
    
    S_msrVarValAssoc      getWorkTitle () const
                              { return fWorkTitle; }
    
    S_msrVarValAssoc      getMovementNumber () const
                              { return fMovementNumber; }
    
    S_msrVarValAssoc      getMovementTitle () const
                              { return fMovementTitle; }
    
    const vector<S_msrVarValAssoc>&
                          getComposers () const
                              { return fComposers; };
                    
    const vector<S_msrVarValAssoc>&
                          getArrangers () const
                              { return fArrangers; };
                    
    const vector<S_msrVarValAssoc>&
                          getPoets () const
                              { return fPoets; };
    
    const vector<S_msrVarValAssoc>&
                          getLyricists () const
                              { return fLyricists; };
    
    S_msrVarValAssoc      getRights () const
                              { return fRights; }
    
    const vector<S_msrVarValAssoc>&
                          getSoftwares () const
                              { return fSoftwares; };
    
    S_msrVarValAssoc      getEncodingDate () const
                              { return fEncodingDate; }
    
    S_msrVarValAssoc      getScoreInstrumentAssoc () const
                              { return fScoreInstrumentAssoc; }

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    S_msrVarValAssoc         fWorkNumber;
    S_msrVarValAssoc         fWorkTitle;
    S_msrVarValAssoc         fMovementNumber;
    S_msrVarValAssoc         fMovementTitle;
    vector<S_msrVarValAssoc> fComposers;
    vector<S_msrVarValAssoc> fArrangers;
    vector<S_msrVarValAssoc> fPoets;
    vector<S_msrVarValAssoc> fLyricists;
    S_msrVarValAssoc         fRights;
    vector<S_msrVarValAssoc> fSoftwares;
    S_msrVarValAssoc         fEncodingDate;
    
    S_msrVarValAssoc         fScoreInstrumentAssoc;
};
typedef SMARTP<msrIdentification> S_msrIdentification;
EXP ostream& operator<< (ostream& os, const S_msrIdentification& elt);

/*!
\brief A MSR page geometry representation.

  A page geometry is represented by variable/value pairs
*/
//______________________________________________________________________________
class EXP msrPageGeometry : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrPageGeometry> create (
      int           inputLineNumber);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrPageGeometry (
      int           inputLineNumber);
      
    virtual ~msrPageGeometry();
  
  public:

    // set and get
    // ------------------------------------------------------

    void    setMillimeters        (float val) { fMillimeters = val; }
    void    setTenths             (int val)   { fTenths = val; }

    float   getMillimeters        () const    { return fMillimeters; }
    float   getTenths             () const    { return fTenths; }

    void    setPaperWidth         (float val) { fPaperWidth = val; }
    void    setPaperHeight        (float val) { fPaperHeight = val; }
    void    setTopMargin          (float val) { fTopMargin = val; }
    void    setBottomMargin       (float val) { fBottomMargin = val; }
    void    setLeftMargin         (float val) { fLeftMargin = val; }
    void    setRightMargin        (float val) { fRightMargin = val; }
    void    setBetweenSystemSpace (float val) { fBetweenSystemSpace = val; }
    void    setPageTopSpace       (float val) { fPageTopSpace = val; }
    

    float   getPaperWidth         () const    { return fPaperWidth; }
    float   getPaperHeight        () const    { return fPaperHeight; }
    float   getTopMargin          () const    { return fTopMargin; }
    float   getBottomMargin       () const    { return fBottomMargin; }
    float   getLeftMargin         () const    { return fLeftMargin; }
    float   getRightMargin        () const    { return fRightMargin; }
    float   getBetweenSystemSpace () const    { return fBetweenSystemSpace; }
    float   getPageTopSpace       () const    { return fPageTopSpace; }
    
    // services
    // ------------------------------------------------------

    float   globalStaffSize       () const;

    // visitors
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);


  private:

    // page height, margins and the like in centimeters are in centimeters
    float             fPaperWidth;
    float             fPaperHeight;
    float             fTopMargin;
    float             fBottomMargin;
    float             fLeftMargin;
    float             fRightMargin;

    float             fMillimeters;
    int               fTenths;
    
    float             fBetweenSystemSpace;
    float             fPageTopSpace; 
};
typedef SMARTP<msrPageGeometry> S_msrPageGeometry;
EXP ostream& operator<< (ostream& os, const S_msrPageGeometry& elt);

/*!
\brief A msr midi representation.

  A midi is represented by variable/value pairs
*/
//______________________________________________________________________________
class EXP msrCreditWords : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrCreditWords> create (
      int    inputLineNumber,
      string creditWordsContents,
      string creditWordsFontFamily,
      int    creditWordsFontSize,
      string creditWordsFontWeight,
      string creditWordsFontJustify,
      string creditWordsFontHAlign,
      string creditWordsFontVAlign,
      string creditWordsFontXMLLanguage);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrCreditWords (
      int    inputLineNumber,
      string creditWordsContents,
      string creditWordsFontFamily,
      int    creditWordsFontSize,
      string creditWordsFontWeight,
      string creditWordsFontJustify,
      string creditWordsFontHAlign,
      string creditWordsFontVAlign,
      string creditWordsFontXMLLanguage);
      
    virtual ~msrCreditWords();
  
  public:

    // set and get
    // ------------------------------------------------------
    string                getCreditWordsContents () const
                              { return fCreditWordsContents; }

    int                   getCreditWordsFontSize () const
                              { return fCreditWordsFontSize; }

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

/*
    <credit-words default-x="607" default-y="1443" font-family="ＭＳ ゴシック" font-size="24" font-weight="bold" justify="center" valign="top" xml:lang="ja">越後獅子</credit-words>
*/

    string                  fCreditWordsContents;

    string                  fCreditWordsFontFamily;
    int                     fCreditWordsFontSize;
    string                  fCreditWordsFontWeight;
    string                  fCreditWordsFontJustify;
    string                  fCreditWordsFontHAlign;
    string                  fCreditWordsFontVAlign;
    string                  fCreditWordsFontXMLLanguage;
};
typedef SMARTP<msrCreditWords> S_msrCreditWords;
EXP ostream& operator<< (ostream& os, const S_msrCreditWords& elt);

/*!
\brief A msr midi representation.

  A midi is represented by variable/value pairs
*/
//______________________________________________________________________________
class EXP msrCredit : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrCredit> create (
      int inputLineNumber,
      int creditPageNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrCredit (
      int inputLineNumber,
      int creditPageNumber);
      
    virtual ~msrCredit();
  
  public:

    // set and get
    // ------------------------------------------------------

    int                   getCreditPageNumber () const
                              { return fCreditPageNumber; }

    const vector<S_msrCreditWords>&
                          getCreditWordsList () const
                              { return fCreditWordsList; }

    // services
    // ------------------------------------------------------

    void                  appendCreditWordsToCredit (
                            S_msrCreditWords creditWords);

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    int                       fCreditPageNumber;
    
    vector<S_msrCreditWords>  fCreditWordsList;
};
typedef SMARTP<msrCredit> S_msrCredit;
EXP ostream& operator<< (ostream& os, const S_msrCredit& elt);

/*!
\brief A msr layout representation.

  A layout is represented by variable/value pairs
*/
//______________________________________________________________________________
class EXP msrLayout : public msrElement // JMI
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrLayout> create (
      int           inputLineNumber);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrLayout (
      int           inputLineNumber);
      
    virtual ~msrLayout();
  
  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    void                  addMsrVarValAssoc (S_msrVarValAssoc assoc)
                              { fVarValAssocs.push_back(assoc); } // JMI ???
      
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:
  
    vector<S_msrVarValAssoc> fVarValAssocs;
};
typedef SMARTP<msrLayout> S_msrLayout;
EXP ostream& operator<< (ostream& os, const S_msrLayout& elt);

/*!
\brief A msr comment representation.

  A comment is represented by its contents
*/
//______________________________________________________________________________

class EXP msrComment : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrCommentGapKind {
      kGapAfterwards, kNoGapAfterwards};

    static string commentGapKindAsString (
      msrCommentGapKind commentGapKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrComment> create (
      int               inputLineNumber,
      string            contents,
      msrCommentGapKind commentGapKind = kNoGapAfterwards);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrComment (
      int               inputLineNumber,
      string            contents,
      msrCommentGapKind commentGapKind = kNoGapAfterwards);
      
    virtual ~msrComment();
  
  public:

    // set and get
    // ------------------------------------------------------

    string                getContents () const
                              { return fContents; }
                    
    msrCommentGapKind     getCommentGapKind  () const
                              { return fCommentGapKind; }
    
    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    string            fContents;
    msrCommentGapKind fCommentGapKind;
};
typedef SMARTP<msrComment> S_msrComment;
EXP ostream& operator<< (ostream& os, const S_msrComment& elt);

/*!
\brief A msr break representation.

  A break is represented by the number of the next bar
*/
//______________________________________________________________________________
class EXP msrBreak : public msrElement
{
  public:
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBreak> create (
      int    inputLineNumber,
      string nextBarNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrBreak (
      int    inputLineNumber,
      string nextBarNumber);
      
    virtual ~msrBreak();
  
  public:

    // set and get
    // ------------------------------------------------------

    string                getNextBarNumber () const
                              { return fNextBarNumber; }

    // services
    // ------------------------------------------------------

    string                breakAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    string                fNextBarNumber;
};
typedef SMARTP<msrBreak> S_msrBreak;
EXP ostream& operator<< (ostream& os, const S_msrBreak& elt);

/*!
\brief A msr barNumberCheck representation.

  A barNumberCheck is represented by the number of the next bar
*/
//______________________________________________________________________________
class EXP msrBarCheck : public msrElement
{
  public:
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBarCheck> create (
      int           inputLineNumber,
      int           nextBarNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrBarCheck (
      int           inputLineNumber,
      int           nextBarNumber);
      
    virtual ~msrBarCheck();
  
  public:

    // set and get
    // ------------------------------------------------------

    int         getNextBarNumber () const
                    { return fNextBarNumber; }

    // services
    // ------------------------------------------------------

    string      barCheckAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    int fNextBarNumber;
};
typedef SMARTP<msrBarCheck> S_msrBarCheck;
EXP ostream& operator<< (ostream& os, const S_msrBarCheck& elt);

/*!
\brief A msr barNumberCheck representation.

  A barNumberCheck is represented by the number of the next bar
*/
//______________________________________________________________________________
class EXP msrBarNumberCheck : public msrElement
{
  public:
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBarNumberCheck> create (
      int    inputLineNumber,
      string nextBarNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrBarNumberCheck (
      int    inputLineNumber,
      string nextBarNumber);
      
    virtual ~msrBarNumberCheck();
  
  public:

    // set and get
    // ------------------------------------------------------

    string                getNextBarNumber () const
                              { return fNextBarNumber; }

    // services
    // ------------------------------------------------------

    string                barNumberCheckAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    string                fNextBarNumber;
};
typedef SMARTP<msrBarNumberCheck> S_msrBarNumberCheck;
EXP ostream& operator<< (ostream& os, const S_msrBarNumberCheck& elt);

/*!
\brief A msr tuplet representation.

  A tuplet is represented by the number of actual notes and
  normal notes, i.e. a triplet is a tuplet with 3 actual notes
  played for the duration of 2 actual notes
*/
//______________________________________________________________________________
class EXP msrTuplet : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrTupletKind {
      kStartTuplet, kContinueTuplet, kStopTuplet, 
      k_NoTuplet };

    static string tupletKindAsString (
      msrTupletKind tupletKind);
      
    enum msrTupletShowNumberKind {
      kTupletShowNumberYes, kTupletShowNumberNo };

    static string tupletShowNumberKindAsString (
      msrTupletShowNumberKind tupletShowNumberKind);
      
    enum msrTupletShowTypeKind {
      kTupletShowTypeYes, kTupletShowTypeNo };

    static string tupletShowNumberKindAsString (
      msrTupletShowTypeKind tupletShowTypeKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTuplet> create (
      int inputLineNumber,
      int number,
      int actualNotes,
      int normalNotes,
      int notePositionInMeasure);

    SMARTP<msrTuplet> createTupletShallowClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTuplet (
      int inputLineNumber,
      int number,
      int actualNotes,
      int normalNotes,
      int notePositionInMeasure);
      
    virtual ~msrTuplet();
  
  public:

    // set and get
    // ------------------------------------------------------
    
    int                   getTupletNumber () const
                              { return fTupletNumber; }

    int                   getTupletActualNotes () const
                              { return fTupletActualNotes; }
    int                   getTupletNormalNotes () const
                              { return fTupletNormalNotes; }
    
    const list<S_msrElement>&
                          getTupletElements () const
                              { return fTupletElements; }

    int                   getTupletSoundingDivisions () const
                              { return fTupletSoundingDivisions; }
    int                   getTupletDisplayedDivisions () const
                              { return fTupletDisplayedDivisions; }
            
    // measure uplink
    void                  setTupletMeasureUplink (
                            const S_msrMeasure& measure)
                              { fTupletMeasureUplink = measure; }
                      
    S_msrMeasure          getTupletMeasureUplink () const
                              { return fTupletMeasureUplink; }

    // measure number
    void                  setTupletMeasureNumber (string measureNumber);

    string                   getTupletMeasureNumber () const
                              { return fTupletMeasureNumber; }
 
    // position in measure
    int                   setTupletPositionInMeasure (int position);
                            // returns the position after the tuplet

    const int             getTupletPositionInMeasure () const
                              { return fTupletPositionInMeasure; }
                      
    // services
    // ------------------------------------------------------

    void                  addNoteToTuplet (S_msrNote note);
    void                  addChordToTuplet (S_msrChord chord);
    void                  addTupletToTuplet (S_msrTuplet tuplet);
    
    void                  addTupletToTupletClone (S_msrTuplet tuplet);
    
    void                  removeFirstNoteFromTuplet (
                            int       inputLineNumber,
                            S_msrNote note);

 // JMI   void                  applyDisplayFactorToTupletMembers ();
    
    void                  unapplyDisplayFactorToTupletMembers (
                            int containingTupletActualNotes,
                            int containingTupletNormalNotes);

    string                tupletAsShortString () const;
    string                tupletAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    int                   fTupletNumber;
    
    int                   fTupletActualNotes;
    int                   fTupletNormalNotes;

    int                   fTupletSoundingDivisions;
    int                   fTupletDisplayedDivisions;

    S_msrMeasure          fTupletMeasureUplink;

    string                fTupletMeasureNumber;
    int                   fTupletPositionInMeasure;
    
    list<S_msrElement>    fTupletElements;
};
typedef SMARTP<msrTuplet> S_msrTuplet;
EXP ostream& operator<< (ostream& os, const S_msrTuplet& elt);

/*!
\brief A msr time representation.

  A time is represented by the numerator and denominator
*/
//______________________________________________________________________________
class EXP msrTranspose : public msrElement
{
  public:
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTranspose> create (
      int           inputLineNumber,
      int           transposeDiatonic,
      int           transposeChromatic);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTranspose (
      int           inputLineNumber,
      int           transposeDiatonic,
      int           transposeChromatic);
      
    virtual ~msrTranspose();
  
  public:

    // set and get
    // ------------------------------------------------------

    int                   getTransposeDiatonic () const
                              { return fTransposeDiatonic; }
                  
    int                   getTransposeChromatic () const
                              { return fTransposeChromatic; }
                  
    // services
    // ------------------------------------------------------

    string                transposeAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    int      fTransposeDiatonic;
    int      fTransposeChromatic;
};
typedef SMARTP<msrTranspose> S_msrTranspose;
EXP ostream& operator<< (ostream& os, const S_msrTranspose& elt);

/*!
\brief A tempo representation.

  A tempo is represented by the stanza to use
*/
//______________________________________________________________________________
class EXP msrTempo : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTempo> create (
      int           inputLineNumber,
      int           tempoUnit,
      int           perMinute);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTempo (
      int           inputLineNumber,
      int           tempoUnit,
      int           perMinute);
      
    virtual ~msrTempo();
  
  public:

    // set and get
    // ------------------------------------------------------

    string                getTempoIndication () const
                              { return fTempoIndication; }

    void                  setTempoIndication (string indication);

    int                   getTempoUnit () const
                              { return fTempoUnit; }

    int                   getPerMinute () const
                              { return fPerMinute; }

    // services
    // ------------------------------------------------------

    string                tempoAsString () const;
    
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    string fTempoIndication;
    
    int    fTempoUnit; // 0 to indicate there's only an indication
    int    fPerMinute;
};
typedef SMARTP<msrTempo> S_msrTempo;
EXP ostream& operator<< (ostream& os, const S_msrTempo& elt);

/*!
\brief A msr stanza representation.

  A stanza is represented by a list of syllables,
*/
//______________________________________________________________________________
class EXP msrStanza : public msrElement
{
  public:

    // constants
    // ------------------------------------------------------

    #define K_MUTE_STANZA_NUMBER -99
    
    // data types
    // ------------------------------------------------------

    enum msrStanzaKind {
       kRegularStanza, kMuteStanza };

    static string stanzaKindAsString (
      msrStanzaKind stanzaKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStanza> create (
      int           inputLineNumber,
      S_msrPart     stanzaDirectPartUplink,
      int           stanzaNumber,
      msrStanzaKind stanzaKind,
      S_msrVoice    stanzaVoiceUplink);
    
    SMARTP<msrStanza> createStanzaShallowClone (
      S_msrVoice voiceClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrStanza (
      int           inputLineNumber,
      S_msrPart     stanzaDirectPartUplink,
      int           stanzaNumber,
      msrStanzaKind stanzaKind,
      S_msrVoice    stanzaVoiceUplink);

  public:
  
    virtual ~msrStanza();
  
  private:

    // initialization
    // ------------------------------------------------------

    void                  initializeStanza ();

  public:

    // set and get
    // ------------------------------------------------------
                              
    S_msrPart             getStanzaDirectPartUplink () const
                              { return fStanzaDirectPartUplink; }

    int                   getStanzaNumber () const
                              { return fStanzaNumber; }
                
    string                getStanzaName () const;
                
    S_msrVoice            getStanzaVoiceUplink () const
                              { return fStanzaVoiceUplink; }
                
    msrStanzaKind         getStanzaKind () const
                              { return fStanzaKind; }
                
    const vector<S_msrSyllable>&
                          getSyllables () const
                              { return fSyllables; }

    void                  setStanzaTextPresent ()
                              { fStanzaTextPresent = true; }

    bool                  getStanzaTextPresent () const
                              { return fStanzaTextPresent; }


    // services
    // ------------------------------------------------------

    void                  appendSyllableToStanza (
                            S_msrSyllable syllable);
      
    S_msrSyllable         appendRestSyllableToStanza (
                            int inputLineNumber,
                            int divisions);

    S_msrSyllable         appendSkipSyllableToStanza (
                            int inputLineNumber,
                            int divisions);

    S_msrSyllable         appendTiedSyllableToStanza ( // JMI
                            int inputLineNumber,
                            int divisions);

    S_msrSyllable         appendSlurSyllableToStanza ( // JMI
                            int inputLineNumber,
                            int divisions);

    S_msrSyllable         appendSlurBeyondEndSyllableToStanza ( // JMI
                            int inputLineNumber,
                            int divisions);

    S_msrSyllable         appendLigatureSyllableToStanza ( // JMI
                            int inputLineNumber,
                            int divisions);

    S_msrSyllable         appendLigatureBeyondEndSyllableToStanza ( // JMI
                            int inputLineNumber,
                            int divisions);

    S_msrSyllable         appendBarNumberCheckSyllableToStanza (
                            int    inputLineNumber,
                            string nextMeasureNumber);

    S_msrSyllable         appendBarcheckSyllableToStanza (
                            int inputLineNumber,
                            int divisions);

    S_msrSyllable         appendBreakSyllableToStanza (
                            int    inputLineNumber,
                            string nextMeasureNumber);
                
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    S_msrPart             fStanzaDirectPartUplink;

    int                   fStanzaNumber;
    msrStanzaKind         fStanzaKind;

    string                fStanzaName;

    vector<S_msrSyllable> fSyllables;

    bool                  fStanzaTextPresent;

    S_msrVoice            fStanzaVoiceUplink;

};
typedef SMARTP<msrStanza> S_msrStanza;
EXP ostream& operator<< (ostream& os, const S_msrStanza& elt);

/*!
\brief A msr barline representation.

  A barline is represented by the number of the next bar
*/
//______________________________________________________________________________
class EXP msrSegno : public msrElement
{
  public:
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSegno> create (
      int                       inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSegno (
      int                       inputLineNumber);
      
    virtual ~msrSegno();
  
  public:

    // set and get
    // ------------------------------------------------------
    
    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:
};
typedef SMARTP<msrSegno> S_msrSegno;
EXP ostream& operator<< (ostream& os, const S_msrSegno& elt);

/*!
\brief A msr barline representation.

  A barline is represented by the number of the next bar
*/
//______________________________________________________________________________
class EXP msrCoda : public msrElement
{
  public:
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrCoda> create (
      int                       inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrCoda (
      int                       inputLineNumber);
      
    virtual ~msrCoda();
  
  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:
};
typedef SMARTP<msrCoda> S_msrCoda;
EXP ostream& operator<< (ostream& os, const S_msrCoda& elt);

/*!
\brief A msr barline representation.

  A barline is represented by the number of the next bar
*/
//______________________________________________________________________________
class EXP msrEyeGlasses : public msrElement
{
  public:
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrEyeGlasses> create (
      int                       inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrEyeGlasses (
      int                       inputLineNumber);
      
    virtual ~msrEyeGlasses();
  
  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:
};
typedef SMARTP<msrEyeGlasses> S_msrEyeGlasses;
EXP ostream& operator<< (ostream& os, const S_msrEyeGlasses& elt);

/*!
\brief A msr barline representation.

  A barline is represented by the number of the next bar
*/
//______________________________________________________________________________
class EXP msrPedal : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrPedalType {
      kPedalStart, kPedalContinue, kPedalChange, kPedalStop };
      
    static string pedalTypeAsString (
      msrPedalType pedalType);
      
    enum msrPedalLine {
      kPedalLineYes, kPedalLineNo};
      
    static string pedalLineAsString (
      msrPedalLine pedalLine);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrPedal> create (
      int              inputLineNumber,
      msrPedalType pedaTypeKind,
      msrPedalLine pedalLine);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrPedal (
      int              inputLineNumber,
      msrPedalType pedaTypeKind,
      msrPedalLine pedalLine);
      
    virtual ~msrPedal();
  
  public:

    // set and get
    // ------------------------------------------------------
    
    msrPedalType          getPedalType () const
                              { return fPedalType; }
                    
    msrPedalLine          getPedalLine () const
                              { return fPedalLine; }
                    
    // services
    // ------------------------------------------------------

    string                 pedalTypeAsString ();
    string                 pedalLineAsString ();
    
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    msrPedalType        fPedalType;
    msrPedalLine        fPedalLine;
};
typedef SMARTP<msrPedal> S_msrPedal;
EXP ostream& operator<< (ostream& os, const S_msrPedal& elt);

/*!
\brief A msr barline representation.

  A barline is represented by the number of the next bar
*/
//______________________________________________________________________________
class EXP msrBarline : public msrElement
{
  public:

/*
   // http://usermanuals.musicxml.com/MusicXML/Content/EL-MusicXML-bar-style.htm

      <barline location="right">
        <bar-style>light-heavy</bar-style>
        <ending type="stop" number="1"/>
        <repeat direction="backward"/>
      </barline>

<!--
    Repeat marks. The start of the repeat has a forward direction
    while the end of the repeat has a backward direction. Backward
    repeats that are not part of an ending can use the times
    attribute to indicate the number of times the repeated section
    is played. The winged attribute indicates whether the repeat
    has winged extensions that appear above and below the barline.
    The straight and curved values represent single wings, while
    the double-straight and double-curved values represent double
    wings. The none value indicates no wings and is the default.
-->
<!ELEMENT repeat EMPTY>
<!ATTLIST repeat
    direction (backward | forward) #REQUIRED
    times CDATA #IMPLIED
    winged (none | straight | curved |
        double-straight | double-curved) #IMPLIED
>
*/

    // data types
    // ------------------------------------------------------

    enum msrBarlineLocation {
      k_NoLocation,
      kLeftLocation, kMiddleLocation, kRightLocation }; // kRightLocation by default
        
    static string barlineLocationAsString (
      msrBarlineLocation barlineLocation);
      
    enum msrBarlineStyle {
      k_NoStyle,
      kRegularStyle,  // by default
      kDottedStyle, kDashedStyle, kHeavyStyle,
      kLightLightStyle, kLightHeavyStyle, kHeavyLightStyle, kHeavyHeavyStyle,
      kTickStyle, kShortStyle,
      kNoneStyle};

    static string barlineStyleAsString (
      msrBarlineStyle barlineStyle);
      
    enum msrBarlineEndingType {
      k_NoEndingType,
      kStartEndingType, kStopEndingType, kDiscontinueEndingType};

    static string barlineEndingTypeAsString (
      msrBarlineEndingType barlineEndingType);
      
    enum msrBarlineRepeatDirection {
      k_NoRepeatDirection,
      kForwardRepeatDirection, kBackwardRepeatDirection};

    static string barlineRepeatDirectionAsString (
      msrBarlineRepeatDirection barlineRepeatDirection);
      
    enum msrBarlineRepeatWinged {
      k_NoRepeatWinged,
      kNoneRepeatWinged,
      kStraightRepeatWinged, kCurvedRepeatWinged,
      kDoubleStraightRepeatWinged, kDoubleCurvedRepeatWinged };

    static string barlineRepeatWingedAsString (
      msrBarlineRepeatWinged barlineRepeatWinged);
      
    enum msrBarlineCategory {
      kStandaloneBarline,
      kRepeatStartBarline, kRepeatEndBarline,
      kHookedEndingStartBarline, kHookedEndingEndBarline,
      kHooklessEndingStartBarline, kHooklessEndingEndBarline};
      
    static string barlineCategoryAsString (
      msrBarlineCategory barlineCategory);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBarline> create (
      int                       inputLineNumber,
      bool                      barlineHasSegno,
      bool                      barlineHasCoda,
      msrBarlineLocation        location,
      msrBarlineStyle           style,
      msrBarlineEndingType      endingType,
      string                    endingNumber,
      msrBarlineRepeatDirection repeatDirection,
      msrBarlineRepeatWinged    repeatWinged);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrBarline (
      int                       inputLineNumber,
      bool                      barlineHasSegno,
      bool                      barlineHasCoda,
      msrBarlineLocation        location,
      msrBarlineStyle           style,
      msrBarlineEndingType      endingType,
      string                    endingNumber,
      msrBarlineRepeatDirection repeatDirection,
      msrBarlineRepeatWinged    repeatWinged);
      
    virtual ~msrBarline();
  
  public:

    // set and get
    // ------------------------------------------------------

    bool                  getBarlineHasSegno () const
                              { return fBarlineHasSegno; }
    bool                  getBarlineHasCoda () const
                              { return fBarlineHasCoda; }

    msrBarlineLocation    getLocation () const
                              { return fLocation; }
                    
    msrBarlineStyle       getStyle () const
                              { return fStyle; }
                    
    msrBarlineEndingType  getEndingType () const
                              { return fEndingType; }
                    
    string                getEndingNumber () const
                              { return fEndingNumber; }
                    
    msrBarlineRepeatDirection
                          getRepeatDirection () const
                              { return fRepeatDirection; }
                    
    msrBarlineRepeatWinged
                          getRepeatWinged () const
                              { return fRepeatWinged; }
                    
    const list<int>&      getEndingNumbersList () const
                              { return fEndingNumbersList; }
                        
    msrBarlineCategory    getBarlineCategory () const
                              { return fBarlineCategory; }

    void                  setBarlineCategory (
                            msrBarlineCategory barlineCategory)
                              { fBarlineCategory = barlineCategory; }
    
    // measure number
    void                  setBarlineMeasureNumber (
                            int positionInMeasure) // JMI virer???
                              { fBarlineMeasureNumber= positionInMeasure; }
                      
    int                   getBarlineMeasureNumber() const
                              { return fBarlineMeasureNumber; }

    // position in measure
    void                  setBarlinePositionInMeasure (
                            int positionInMeasure)
                              { fBarlinePositionInMeasure = positionInMeasure; }
                      
    int                   getBarlinePositionInMeasure () const
                              { return fBarlinePositionInMeasure; }

    // services
    // ------------------------------------------------------

    string                barlineAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    bool                        fBarlineHasSegno;
    bool                        fBarlineHasCoda;

    msrBarlineLocation          fLocation;
    msrBarlineStyle             fStyle;
    msrBarlineEndingType        fEndingType;
    string                      fEndingNumber; // may be "1, 2"
    msrBarlineRepeatDirection   fRepeatDirection;
    msrBarlineRepeatWinged      fRepeatWinged;

    msrBarlineCategory          fBarlineCategory;

    int                         fBarlineMeasureNumber;
    int                         fBarlinePositionInMeasure;

    // the numbers extracted from fEndingNumber
    list<int>                   fEndingNumbersList;
};
typedef SMARTP<msrBarline> S_msrBarline;
EXP ostream& operator<< (ostream& os, const S_msrBarline& elt);

/*!
\brief A msr repeat representation.

  A repeat is represented by:
    - a sequence of elements for the common segment
    - a vector of sequences of elements for the alternate endings
*/
//______________________________________________________________________________
class EXP msrRepeatEnding : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrRepeatEndingKind {
      kHookedEnding,
      kHooklessEnding};

    static string repeatEndingKindAsString (
      msrRepeatEndingKind repeatEndingKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeatEnding> create (
      int                 inputLineNumber,
      string              repeatEndingNumber, // may be "1, 2"
      msrRepeatEndingKind repeatEndingKind,
      S_msrSegment        segment,
      S_msrRepeat         repeatUplink);
    
    SMARTP<msrRepeatEnding> createRepeatEndingShallowClone (
      S_msrRepeat repeatClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRepeatEnding (
      int                 inputLineNumber,
      string              repeatEndingNumber, // may be "1, 2"
      msrRepeatEndingKind repeatEndingKind,
      S_msrSegment        segment,
      S_msrRepeat         repeatUplink);
      
    virtual ~msrRepeatEnding();
  
  public:

    // set and get
    // ------------------------------------------------------

    string                getRepeatEndingNumber () const
                              { return fRepeatEndingNumber; }
                
    void                  setRepeatEndingNumber (int repeatEndingNumber)
                              { fRepeatEndingNumber = repeatEndingNumber; }

    void                  setRepeatEndingInternalNumber (
                            int repeatEndingInternalNumber)
                              {
                                fRepeatEndingInternalNumber =
                                  repeatEndingInternalNumber;
                              }
                      
    int                   getRepeatEndingInternalNumber () const
                              { return fRepeatEndingInternalNumber; }
                
    msrRepeatEndingKind   getRepeatEndingKind () const
                              { return fRepeatEndingKind; }
                
    S_msrSegment          getRepeatEndingSegment () const
                              { return fRepeatEndingSegment; }
                
    S_msrRepeat           getRepeatEndingRepeatUplink () const
                              { return fRepeatEndingRepeatUplink; }

    // services
    // ------------------------------------------------------
  
    string                repeatEndingAsString () const;

 //  JMI void                  appendElementToRepeatEnding (S_msrElement elem);
                    
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:
  
    string              fRepeatEndingNumber; // may be "1, 2"
    int                 fRepeatEndingInternalNumber; // internally assigned
    
    msrRepeatEndingKind fRepeatEndingKind;
    
    S_msrSegment        fRepeatEndingSegment;

    S_msrRepeat         fRepeatEndingRepeatUplink;
};
typedef SMARTP<msrRepeatEnding> S_msrRepeatEnding;
EXP ostream& operator<< (ostream& os, const S_msrRepeatEnding& elt);

/*!
\brief A msr repeat representation.

  A repeat is represented by:
    - a sequence of elements for the common segment
    - a vector of sequences of elements for the alternate endings
*/
//______________________________________________________________________________
class EXP msrRepeat : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeat> create (
      int          inputLineNumber,
      S_msrVoice   voiceUplink);
    
    SMARTP<msrRepeat> createRepeatShallowClone (
      S_msrVoice voiceClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRepeat (
      int          inputLineNumber,
      S_msrVoice   voiceUplink);
      
    virtual ~msrRepeat();
  
  public:

    // set and get
    // ------------------------------------------------------

    void                  setRepeatCommonSegment (
                            S_msrSegment repeatCommonSegment);
                  
    S_msrSegment          getRepeatCommonSegment () const
                              { return fRepeatCommonSegment; }

    const vector<S_msrRepeatEnding>&
                          getRepeatEndings () const
                            { return fRepeatEndings; }

    S_msrVoice            getRepeatVoiceUplink () const
                            { return fRepeatVoiceUplink; }

    // services
    // ------------------------------------------------------

    void                  addRepeatEnding (S_msrRepeatEnding repeatEnding);

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    S_msrSegment              fRepeatCommonSegment;
    
    vector<S_msrRepeatEnding> fRepeatEndings;
    int                       fRepeatEndingsInternalCounter;
    
    S_msrVoice                fRepeatVoiceUplink;
};
typedef SMARTP<msrRepeat> S_msrRepeat;
EXP ostream& operator<< (ostream& os, const S_msrRepeat& elt);

/*!
\brief A msr measure repeat representation.

  A repeat is represented by:
    - a sequence of elements for the common segment
    - a vector of sequences of elements for the alternate endings
*/
//______________________________________________________________________________
class EXP msrMeasureRepeatPattern : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasureRepeatPattern> create (
      int        inputLineNumber,
      S_msrVoice voiceUplink);
    
    SMARTP<msrMeasureRepeatPattern> createMeasureRepeatPatternShallowClone (
        S_msrVoice voiceClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrMeasureRepeatPattern (
      int        inputLineNumber,
      S_msrVoice voiceUplink);
      
    virtual ~msrMeasureRepeatPattern();
  
  public:

    // set and get
    // ------------------------------------------------------

    void                  setMeasureRepeatPatternSegment (
                            S_msrSegment measureRepeatPatternSegment);
                              
    S_msrSegment          getMeasureRepeatPatternSegment () const
                              { return fMeasureRepeatPatternSegment; }

    S_msrVoice            getMeasureRepeatPatternVoiceUplink () const
                            { return fMeasureRepeatPatternVoiceUplink; }

    // services
    // ------------------------------------------------------

    int                   measureRepeatPatternMeasuresNumber () const;
        
    string                measureRepeatPatternAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:
  
    S_msrSegment          fMeasureRepeatPatternSegment;
    
    S_msrVoice            fMeasureRepeatPatternVoiceUplink;
};
typedef SMARTP<msrMeasureRepeatPattern> S_msrMeasureRepeatPattern;
EXP ostream& operator<< (ostream& os, const S_msrMeasureRepeatPattern& elt);

//______________________________________________________________________________
class EXP msrMeasureRepeatReplicas : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasureRepeatReplicas> create (
      int        inputLineNumber,
      S_msrVoice voiceUplink);
    
    SMARTP<msrMeasureRepeatReplicas> createMeasureRepeatReplicasShallowClone (
        S_msrVoice voiceClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrMeasureRepeatReplicas (
      int        inputLineNumber,
      S_msrVoice voiceUplink);
      
    virtual ~msrMeasureRepeatReplicas();
  
  public:

    // set and get
    // ------------------------------------------------------

    void                  setMeasureRepeatReplicasSegment (
                            S_msrSegment measureRepeatReplicasSegment);

    S_msrSegment          getMeasureRepeatReplicasSegment () const
                              { return fMeasureRepeatReplicasSegment; }

    S_msrVoice            getMeasureRepeatReplicasVoiceUplink () const
                            { return fMeasureRepeatReplicasVoiceUplink; }

    // services
    // ------------------------------------------------------

    int                   measureRepeatReplicasMeasuresNumber () const;
    
    int                   measureRepeatReplicasNumber () const;

    string                measureRepeatReplicasAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:
      
    S_msrSegment          fMeasureRepeatReplicasSegment;

    S_msrVoice            fMeasureRepeatReplicasVoiceUplink;
};
typedef SMARTP<msrMeasureRepeatReplicas> S_msrMeasureRepeatReplicas;
EXP ostream& operator<< (ostream& os, const S_msrMeasureRepeatReplicas& elt);

//______________________________________________________________________________
class EXP msrMeasureRepeat : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrMeasureRepeatKind {
      kStartMeasureRepeat, kStopMeasureRepeat, 
      k_NoMeasureRepeat };

    static string measureRepeatKindAsString (
      msrMeasureRepeatKind measureRepeatKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasureRepeat> create (
      int          inputLineNumber,
      int          measureRepeatMeasuresNumber,
      int          measureRepeatSlashesNumber,
      S_msrVoice   voiceUplink);
    
    SMARTP<msrMeasureRepeat> createMeasureRepeatShallowClone (
      S_msrVoice   voiceClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrMeasureRepeat (
      int          inputLineNumber,
      int          measureRepeatMeasuresNumber,
      int          measureRepeatSlashesNumber,
      S_msrVoice   voiceUplink);
      
    virtual ~msrMeasureRepeat();
  
  public:

    // set and get
    // ------------------------------------------------------

    int                   getMeasureRepeatMeasuresNumber () const
                              { return fMeasureRepeatMeasuresNumber; }
                              
    int                   getMeasureRepeatSlashesNumber () const
                              { return fMeasureRepeatSlashesNumber; }
                              
    void                  setMeasureRepeatPattern (
                            S_msrMeasureRepeatPattern
                              measureRepeatPattern);
                  
    S_msrMeasureRepeatPattern
                          getMeasureRepeatPattern () const
                              { return fMeasureRepeatPattern; }

    void                  setMeasureRepeatReplicas (
                            S_msrMeasureRepeatReplicas
                              measureRepeatReplicas);
                  
    S_msrMeasureRepeatReplicas
                          getMeasureRepeatReplicas () const
                              { return fMeasureRepeatReplicas; }

    S_msrVoice            getMeasureRepeatVoiceUplink () const
                            { return fMeasureRepeatVoiceUplink; }

    // services
    // ------------------------------------------------------

    int                   measureRepeatPatternMeasuresNumber () const
                            {
                              return
                                fMeasureRepeatPattern->
                                  measureRepeatPatternMeasuresNumber ();
                            }
    
    int                   measureRepeatReplicasMeasuresNumber () const
                            {
                              return
                                fMeasureRepeatReplicas->
                                  measureRepeatReplicasMeasuresNumber ();
                            }
    
    int                   measureRepeatReplicasNumber () const;

    string                measureRepeatAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:
    int                   fMeasureRepeatMeasuresNumber;
    int                   fMeasureRepeatSlashesNumber;

    S_msrMeasureRepeatPattern
                          fMeasureRepeatPattern;
    
    S_msrMeasureRepeatReplicas
                          fMeasureRepeatReplicas;

    S_msrVoice            fMeasureRepeatVoiceUplink;
};
typedef SMARTP<msrMeasureRepeat> S_msrMeasureRepeat;
EXP ostream& operator<< (ostream& os, const S_msrMeasureRepeat& elt);

//______________________________________________________________________________
class EXP msrMultipleRestContents : public msrElement
{
  public:
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMultipleRestContents> create (
      int        inputLineNumber,
      S_msrVoice voiceUplink);
    
    SMARTP<msrMultipleRestContents> createMultipleRestContentsShallowClone (
      S_msrVoice voiceClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrMultipleRestContents (
      int        inputLineNumber,
      S_msrVoice voiceUplink);
      
    virtual ~msrMultipleRestContents();
  
  public:

    // set and get
    // ------------------------------------------------------

    void                  setMultipleRestContentsSegment (
                            S_msrSegment multipleRestContentsSegment);

    S_msrSegment          getMultipleRestContentsSegment () const
                              { return fMultipleRestContentsSegment; }

    S_msrVoice            getMultipleRestContentsVoiceUplink () const
                            { return fMultipleRestContentsVoiceUplink; }

    // services
    // ------------------------------------------------------

    int                   multipleRestContentsMeasuresNumber () const;

    string                multipleRestContentsAsString () const;
    
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:
    
    S_msrSegment          fMultipleRestContentsSegment;

    S_msrVoice            fMultipleRestContentsVoiceUplink;
};
typedef SMARTP<msrMultipleRestContents> S_msrMultipleRestContents;
EXP ostream& operator<< (ostream& os, const S_msrMultipleRestContents& elt);

//______________________________________________________________________________
class EXP msrMultipleRest : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrMultipleRestKind {
      kStartMultipleRest, kStopMultipleRest, 
      k_NoMultipleRest };

    static string multipleRestKindAsString (
      msrMultipleRestKind multipleRestKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMultipleRest> create (
      int        inputLineNumber,
      int        multipleRestMeasuresNumber,
      S_msrVoice voiceUplink);
    
    SMARTP<msrMultipleRest> createMultipleRestShallowClone (
      S_msrVoice voiceClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrMultipleRest (
      int        inputLineNumber,
      int        multipleRestMeasuresNumber,
      S_msrVoice voiceUplink);
      
    virtual ~msrMultipleRest();
  
  public:

    // set and get
    // ------------------------------------------------------

    int                   getMultipleRestMeasuresNumber () const
                              { return fMultipleRestMeasuresNumber; }
                              
    void                  setMultipleRestContents (
                            S_msrMultipleRestContents multipleRestContents);
                  
    S_msrMultipleRestContents
                          getMultipleRestContents () const
                              { return fMultipleRestContents; }

    S_msrVoice            getMultipleRestVoiceUplink () const
                            { return fMultipleRestVoiceUplink; }

    // services
    // ------------------------------------------------------

    int                   multipleRestContentsMeasuresNumber () const
                            {
                              return
                                fMultipleRestContents->
                                  multipleRestContentsMeasuresNumber ();
                            }

    string                multipleRestAsString () const;
    
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:
    int                   fMultipleRestMeasuresNumber;
    
    S_msrMultipleRestContents
                          fMultipleRestContents;

    S_msrVoice            fMultipleRestVoiceUplink;
};
typedef SMARTP<msrMultipleRest> S_msrMultipleRest;
EXP ostream& operator<< (ostream& os, const S_msrMultipleRest& elt);

/*!
\brief A msr voice representation.

  A vpoce is represented by a its string contents
*/
//______________________________________________________________________________
class EXP msrVoice : public msrElement
{
  public:

    // constants
    // ------------------------------------------------------

    #define K_SILENT_VOICE_NUMBER -111
    
    // data types
    // ------------------------------------------------------

    enum msrVoiceKind {
        kRegularVoice,
        kHarmonyVoice,  // for MusicXML <harmony/>, LilyPond ChordNames
        kSilentVoice }; // for voices that don't start at the very beginning
          
    static string voiceKindAsString (
      msrVoiceKind voiceKind);
      
    enum msrVoiceFinalizationStatus {
        kKeepVoice,
        kEraseVoice };
          
    static string voiceFinalizationStatusAsString (
      msrVoiceFinalizationStatus voiceFinalizationStatus);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrVoice> create (
      int          inputLineNumber,
      S_msrPart    voiceDirectPartUplink,
      msrVoiceKind voiceKind,
      int          externalVoiceNumber,
      S_msrStaff   voiceStaffUplink);
    
    SMARTP<msrVoice> createVoiceShallowClone (
      S_msrStaff staffClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    // for regular voices
    msrVoice (
      int          inputLineNumber,
      S_msrPart    voiceDirectPartUplink,
      msrVoiceKind voiceKind,
      int          externalVoiceNumber,
      S_msrStaff   voiceStaffUplink);

    virtual ~msrVoice();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeVoice ();

  public:

    // set and get
    // ------------------------------------------------------

    // voice kind
    
    msrVoiceKind          getVoiceKind () const
                              { return fVoiceKind; }

    // voice numbers
    
    void                  setExternalVoiceNumber (int externalVoiceNumber)
                              { fExternalVoiceNumber = externalVoiceNumber; }

    int                   getExternalVoiceNumber () const
                              { return fExternalVoiceNumber; }
                
    int                   getStaffRelativeVoiceNumber () const
                              { return fStaffRelativeVoiceNumber; }

    int                   getVoiceAbsoluteNumber () const
                            { return fVoiceAbsoluteNumber; }
                            
    // voice name
    
    string                getVoiceName () const;

    // stanzas
    
    S_msrStanza           getVoiceMuteStanza () const
                              { return fVoiceMuteStanza; }
               
    const map<int, S_msrStanza>&
                          getVoiceStanzasMap () const
                              { return fVoiceStanzasMap; }

    // voice last segment
    
    void                  setVoiceCloneLastSegment (
                            S_msrSegment segment)
                              { fVoiceLastSegment = segment; }

    S_msrSegment          getVoiceLastSegment () const
                              { return fVoiceLastSegment; }

    // counters
    
    int                   getVoiceActualNotesCounter () const
                              { return fVoiceActualNotesCounter; }

    int                   getVoiceActualHarmoniesCounter () const
                              { return fVoiceActualHarmoniesCounter; }

     // measures
     
    void                  createMeasureAndAppendItToVoice (
                            int    inputLineNumber,
                            string measureNumbe);

    void                  appendAFirstMeasureToVoiceIfNeeded (
                             int inputLineNumber);
                        
    const string          getVoiceMeasureNumber () const
                              { return fVoiceMeasureNumber; }

    // has music been inserted in the voice?
    
    void                  setMusicHasBeenInsertedInVoice ()
                              { fMusicHasBeenInsertedInVoice = true; }

    bool                  getMusicHasBeenInsertedInVoice () const
                              { return fMusicHasBeenInsertedInVoice; }

    // multiple rests
    
    void                  setVoiceContainsMultipleRests (
                            int inputLineNumber);

    bool                  getVoiceContainsMultipleRests () const
                              { return fVoiceContainsMultipleRests; }

    // uplinks
    
    S_msrStaff            getVoiceStaffUplink () const
                              { return fVoiceStaffUplink; }

    S_msrPart             getVoiceDirectPartUplink () const
                             { return fVoiceDirectPartUplink; }

    // services
    // ------------------------------------------------------

    // voice kind

    string                voiceKindAsString () const;

    // divisions

    void                  bringVoiceToMeasurePosition (
                            int inputLineNumber,
                            int measurePosition);
  
    // clef, key, time
    
    void                  appendClefToVoice (S_msrClef clef);
    void                  appendKeyToVoice  (S_msrKey  key);
    void                  appendTimeToVoice (S_msrTime time);
    
    // transpose

    void                  appendTransposeToVoice (
                            S_msrTranspose transpose);

    // staff details

    void                  appendStaffDetailsToVoice (
                            S_msrStaffDetails staffDetails);

// JMI    void          appendWordsToVoice  (S_msrWords words);

    // tempo
    
    void                  appendTempoToVoice (S_msrTempo tempo);

    // rehearsals
    
    void                  appendRehearsalToVoice (
                            S_msrRehearsal rehearsal);

    // octave shifts
    
    void                  appendOctaveShiftToVoice (
                            S_msrOctaveShift octaveShift);

    // notes
    
    void                  appendNoteToVoice      (S_msrNote note);
    void                  appendNoteToVoiceClone (S_msrNote note);

    // tremolos
    
    void                  appendDoubleTremoloToVoice (
                            S_msrDoubleTremolo doubleTremolo);

    // chords
    
    void                  appendChordToVoice  (S_msrChord chord);

    // tuplets
    
    void                  appendTupletToVoice (S_msrTuplet tuplet);

    // harmonies

    void                  appendHarmonyToVoice (S_msrHarmony harmony);
    
    void                  appendHarmonyToVoiceClone (S_msrHarmony harmony);

    // grace notes
    
    void                  appendGraceNotesToVoice (
                            S_msrGraceNotes graceNotes);

    void                  prependGraceNotesToVoice (
                            S_msrGraceNotes graceNotes);

    void                  appendAfterGraceNotesToVoice (
                            S_msrAfterGraceNotes afterGraceNotes);

    void                  prependAfterGraceNotesToVoice (
                            S_msrAfterGraceNotes afterGraceNotes);

    // lyrics
    
    void                  appendSyllableToVoice (
                            int           inputLineNumber,
                            int           stanzaNumber,
                            S_msrSyllable syllable);

    // bar checks
    
    void                  appendBarCheckToVoice (
                            S_msrBarCheck barCheck);

    // bar number checks
    
    void                  appendBarNumberCheckToVoice (
                            S_msrBarNumberCheck barNumberCheck);

    // breaks
    
    void                  appendBreakToVoice (S_msrBreak break_);

    // bar lines
    
    void                  prependBarlineToVoice (S_msrBarline barline);
    void                  appendBarlineToVoice (S_msrBarline barline);

    // segno
    
    void                  appendSegnoToVoice (S_msrSegno segno);

    // coda
    
    void                  appendCodaToVoice (S_msrCoda coda);

    // eyeglasses
    
    void                  appendEyeGlassesToVoice (
                            S_msrEyeGlasses eyeGlasses);

    // pedal
    
    void                  appendPedalToVoice (S_msrPedal pedal);

    // other elements
    
    void                  appendOtherElementToVoice (S_msrElement elem);
                            // for other types of elements not known
                            // in this header file, such as LPSR elements

    // removing elements from voice
    
    void                  removeNoteFromVoice (
                            int       inputLineNumber,
                            S_msrNote note);

    S_msrMeasure          removeLastMeasureFromVoice (
                            int inputLineNumber);

    // repeats
    
    void                  createRepeatAndAppendItToVoice (
                            int inputLineNumber);
    
    void                  createMeasureRepeatFromItsFirstMeasureInVoice (
                            int inputLineNumber,
                            int measureRepeatMeasuresNumber,
                            int measureRepeatSlashes);

    void                  appendPendingMeasureRepeatToVoice (
                            int inputLineNumber);
                            
    void                  createMultipleRestInVoice (
                            int inputLineNumber,
                            int multipleRestMeasuresNumber);

    void                  appendPendingMultipleRestToVoice (
                            int inputLineNumber);
                            
    void                  appendMultipleRestCloneToVoice (
                            int               inputLineNumber,
                            S_msrMultipleRest multipleRest);
  
    void                  appendRepeatCloneToVoice (
                            int         inputLineNumber,
                            S_msrRepeat repeatCLone);
    
    void                  appendRepeatEndingToVoice (
                            int       inputLineNumber,
                            string    repeatEndingNumber, // may be "1, 2"
                            msrRepeatEnding::msrRepeatEndingKind
                                      repeatEndingKind);

    void                  appendRepeatEndingCloneToVoice (
                            S_msrRepeatEnding repeatEndingClone);

    // segments
    
    void                  createNewLastSegmentForVoice (
                            int inputLineNumber);
    
    void                  createNewLastSegmentWithFirstMeasureForVoice (
                            int          inputLineNumber,
                            S_msrMeasure firstMeasure);

    // stanzas

    S_msrStanza           addStanzaToVoiceByItsNumber (
                            int inputLineNumber,
                            int stanzaNumber);

    void                  addStanzaToVoiceWithoutCatchUp (
                            S_msrStanza stanza);

    void                  catchUpWithVoiceMuteStanza (
                            S_msrStanza stanza);
                    
    void                  addStanzaToVoiceWithCatchUp (
                            S_msrStanza stanza);

    S_msrStanza           createStanzaInVoiceIfNeeded (
                            int inputLineNumber,
                            int stanzaNumber);
    
/* JMI
    bool                  checkForIncompleteVoiceLastMeasure (
                            int inputLineNumber);
*/

    // finalization
    
    void                  finalizeCurrentMeasureInVoice (
                            int inputLineNumber);

    msrVoiceFinalizationStatus
                          finalizeVoice (
                            int inputLineNumber);
    
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // voice numbers

    int                   fVoiceAbsoluteNumber;
    // voice numbers in MusicXML may be greater than 4
    // while there can only be 4 in a staff
    // we thus have to cope with that
    int                   fExternalVoiceNumber;
    int                   fStaffRelativeVoiceNumber;

    // voice kind

    msrVoiceKind          fVoiceKind;

    // voice name

    string                fVoiceName;

    // counters
    
    static int            gVoicesCounter;

    int                   fVoiceActualNotesCounter;
    int                   fVoiceActualHarmoniesCounter;

    // measures
    
    string                fVoiceMeasureNumber;
    
    // checking for musically empty voices
    
    bool                  fMusicHasBeenInsertedInVoice;

    // voice internal handling
    
    // fVoiceLastSegment contains the music
    // not yet stored in fVoiceInitialRepeatsAndSegments,
    // it is thus logically the end of the latter,
    // and is created implicitly for every voice.
    // Is is needed 'outside' of the 'list<S_msrElement>'
    // because it is not a mere S_msrElement, but a S_msrSegment
    list<S_msrElement>    fVoiceInitialRepeatsAndSegments;
    S_msrSegment          fVoiceLastSegment;

    // fVoiceFirstSegment is used to work around LilyPond issue 34
    S_msrSegment          fVoiceFirstSegment;

    // fVoiceCurrentRepeat is null or
    // the last msrRepeat in fVoiceInitialRepeatsAndSegments
    S_msrRepeat           fVoiceCurrentRepeat;

    // fVoicePendingMeasureRepeat is null
    // or the last msrMeasureRepeat created with its repeated measure,
    // but not yet appended to the voice
    S_msrMeasureRepeat    fVoicePendingMeasureRepeat;

    // fVoicePendingMeasureRepeat is null
    // or the last msrMeasureRepeat created with its repeated measure,
    // but not yet appended to the voice
    S_msrMultipleRest     fVoicePendingMultipleRest;
    bool                  fVoiceContainsMultipleRests;

    // stanzas
    
    // the mute stanza, collecting skips along the way,
    // to be used as a 'prelude' by actual stanzas
    // that start at later points
    S_msrStanza           fVoiceMuteStanza;
                            // K_MUTE_STANZA_NUMBER
    
    map<int, S_msrStanza> fVoiceStanzasMap;

    // uplinks
    
    S_msrStaff            fVoiceStaffUplink;

    S_msrPart             fVoiceDirectPartUplink;


};
EXP ostream& operator<< (ostream& os, const S_msrVoice& elt);

/*!
\brief A msr staff representation.

  A staff is represented by a its string contents
*/

/* JMI
Staff assignment is only needed for music notated on multiple staves. Used by both notes and directions. Staff values are numbers, with 1 referring to the top-most staff in a part.
*/

/*!
\brief A msr comment representation.

  A comment is represented by its contents
*/
//______________________________________________________________________________

class EXP msrStaffLinesNumber : public msrElement
{
  public:
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStaffLinesNumber> create (
      int inputLineNumber,
      int linesNumber);

    SMARTP<msrStaffLinesNumber> createStaffLinesNumberShallowClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrStaffLinesNumber (
      int inputLineNumber,
      int linesNumber);
      
    virtual ~msrStaffLinesNumber();
  
  public:

    // set and get
    // ------------------------------------------------------

    int                   getLinesNumber () const
                              { return fLinesNumber; }
                        
    // services
    // ------------------------------------------------------

    string                staffLinesNumberAsString () const;
         
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    int                   fLinesNumber;
};
typedef SMARTP<msrStaffLinesNumber> S_msrStaffLinesNumber;
EXP ostream& operator<< (ostream& os, const S_msrStaffLinesNumber& elt);

//______________________________________________________________________________
class EXP msrStaffTuning : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStaffTuning> create (
      int                  inputLineNumber,
      int                  staffTuningLineNumber,
      msrQuarterTonesPitch quarterTonesPitch,
      int                  staffTuningOctave);
    
    SMARTP<msrStaffTuning> createStaffTuningShallowClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrStaffTuning (
      int                  inputLineNumber,
      int                  staffTuningLineNumber,
      msrQuarterTonesPitch quarterTonesPitch,
      int                  staffTuningOctave);
         
    ~ msrStaffTuning ();
  
  public:

    // set and get
    // ------------------------------------------------------

    int                   getStaffTuningLineNumber () const
                              { return fStaffTuningLineNumber; }

    msrQuarterTonesPitch  getStaffTuningQuarterTonesPitch () const
                              { return fStaffTuningQuarterTonesPitch; }

    int                   getStaffTuningOctave () const
                              { return fStaffTuningOctave; }

    // services
    // ------------------------------------------------------

    string                staffTuningAsString () const;
         
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);
    
  public:
  
    // data
    // ------------------------------------------------------
    
    int                   fStaffTuningLineNumber;
    
    msrQuarterTonesPitch  fStaffTuningQuarterTonesPitch;
    int                   fStaffTuningOctave;
};
typedef SMARTP<msrStaffTuning> S_msrStaffTuning;
EXP ostream& operator<< (ostream& os, const S_msrStaffTuning& elt);

/*!
\brief A msr comment representation.

  A comment is represented by its contents
*/
//______________________________________________________________________________

class EXP msrStaffDetails : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrStaffTypeKind {
      kRegularStaffType,
      kOssiaStaffType, kCueStaffType, kEditorialStaffType,
      kAlternateStaffType};

    static string staffTypeKindAsString (
      msrStaffTypeKind staffTypeKind);
      
    enum msrShowFretsKind {
      kShowFretsNumbers, kShowFretsLetters};

    static string showFretsKindAsString (
      msrShowFretsKind showFretsKind);
      
    enum msrPrintObjectKind {
      kPrintObjectYes, kPrintObjectNo};

    static string printObjectKindKindAsString (
      msrPrintObjectKind printObjectKind);
      
    enum msrPrintSpacingKind {
      kPrintSpacingYes, kPrintSpacingNo};

    static string printSpacingKindKindAsString (
      msrPrintSpacingKind printSpacingKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStaffDetails> create (
      int                   inputLineNumber,
      msrStaffTypeKind      staffTypeKind,
      S_msrStaffLinesNumber staffLinesNumber,
      S_msrStaffTuning      staffTuning,
      msrShowFretsKind      showFretsKind,
      msrPrintObjectKind    printObjectKind,
      msrPrintSpacingKind   printSpacingKind);

    SMARTP<msrStaffDetails> createStaffDetailsShallowClone (
      S_msrStaffLinesNumber staffLinesNumberClone,
      S_msrStaffTuning      staffTuningClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrStaffDetails (
      int                   inputLineNumber,
      msrStaffTypeKind      staffTypeKind,
      S_msrStaffLinesNumber staffLinesNumber,
      S_msrStaffTuning      staffTuning,
      msrShowFretsKind      showFretsKind,
      msrPrintObjectKind    printObjectKind,
      msrPrintSpacingKind   printSpacingKind);
      
    virtual ~msrStaffDetails();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrStaffTypeKind      getStaffTypeKind () const
                              { return fStaffTypeKind; }
                        
    S_msrStaffLinesNumber getStaffLinesNumber () const
                              { return fStaffLinesNumber; }
                        
    S_msrStaffTuning      getStaffTuning () const
                              { return fStaffTuning; }
                        
    msrShowFretsKind      getShowFretsKind () const
                              { return fShowFretsKind; }
                        
    msrPrintObjectKind    getPrintObjectKind () const
                              { return fPrintObjectKind; }
                        
    msrPrintSpacingKind   getPrintSpacingKind () const
                              { return fPrintSpacingKind; }
                        
    // services
    // ------------------------------------------------------

    string                staffDetailsAsShortString ();
    
    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    msrStaffTypeKind      fStaffTypeKind;

    S_msrStaffLinesNumber fStaffLinesNumber;
    S_msrStaffTuning      fStaffTuning;

    msrShowFretsKind      fShowFretsKind;
    
    msrPrintObjectKind    fPrintObjectKind;
    msrPrintSpacingKind   fPrintSpacingKind;
};
typedef SMARTP<msrStaffDetails> S_msrStaffDetails;
EXP ostream& operator<< (ostream& os, const S_msrStaffDetails& elt);

/*!
\brief A msr staff representation.

  A staff is represented by a its string contents
*/

/*
Staff assignment is only needed for music notated on multiple staves. Used by both notes and directions. Staff values are numbers, with 1 referring to the top-most staff in a part.
*/
//______________________________________________________________________________
class EXP msrStaff : public msrElement
{
  public:

    enum msrStaffKind {
      kRegularStaff,
      kTablatureStaff,
      kPercussionStaff,
      kHarmonyStaff};

    static string staffKindAsString (
      msrStaffKind staffKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStaff> create (
      int          inputLineNumber,
      S_msrPart    staffDirectPartUplink,
      msrStaffKind staffKind,
      int          staffNumber);
    
    SMARTP<msrStaff> createStaffShallowClone (
      S_msrPart partClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrStaff (
      int          inputLineNumber,
      S_msrPart    staffDirectPartUplink,
      msrStaffKind staffKind,
      int          staffNumber);
      
    virtual ~msrStaff();
  
  private:

    // initialization
    // ------------------------------------------------------

    void                  initializeStaff ();

  public:

    // set and get
    // ------------------------------------------------------

    // staff kind
    
    msrStaffKind          getStaffKind () const
                              { return fStaffKind; }

    // staff number and anme
    
    int                   getStaffNumber () const
                              { return fStaffNumber; }
                
    string                getStaffName () const;

// JMI    string          getStaffInstrumentName () const
      //                  { return fStaffInstrumentName; }

    // clef, key, time
    
    S_msrClef             getStaffCurrentClef () const
                              { return fStaffCurrentClef; };
                              
    S_msrKey              getStaffCurrentKey  () const
                              { return fStaffCurrentKey; };
                              
    S_msrTime             getStaffCurrentTime () const;

    // transpose
    
    S_msrTranspose        getStaffTranspose () const
                              { return fStaffTranspose; };
    
    // staff details

    S_msrStaffDetails     getCurrentStaffStaffDetails () const
                            { return fCurrentStaffStaffDetails; }

    // staff voices
    
    const map<int, S_msrVoice>&
                          getStaffVoiceRelativeNumberToVoiceMap () const
                              {
                                return
                                  fStaffVoiceRelativeNumberToVoiceMap;
                              }

    const map<int, S_msrVoice>&
                          getStaffAllVoicesMap () const
                              { return fStaffAllVoicesMap; }

    // staff silent voice
    
    const S_msrVoice      getStaffSilentVoice () const
                              { return fStaffSilentVoice; }

    // measures
    
    void                  createMeasureAndAppendItToStaff (
                            int    inputLineNumber,
                            string measureNumber);
                      
    const string          getStaffMeasureNumber () const
                              { return fStaffMeasureNumber; }

    // uplinks

    S_msrPart             getStaffDirectPartUplink () const
                              { return fStaffDirectPartUplink; }

    // services
    // ------------------------------------------------------

    // staff details
    
    void                  appendStaffDetailsToStaff (
                            S_msrStaffDetails staffDetails);

    // clef, key, time

    void                  appendClefToStaff (S_msrClef clef);
    void                  appendKeyToStaff (S_msrKey  key);
    void                  appendTimeToStaff (S_msrTime time);
    
    // transpose

    void                  appendTransposeToStaff (
                            S_msrTranspose transpose);

    // staff voices
    
    S_msrVoice            addVoiceToStaffByItsRelativeNumber (
                            int                    inputLineNumber,
                            msrVoice::msrVoiceKind voiceKind,
                            int                    voiceRelativeNumber);
  
    S_msrVoice            addVoiceToStaffByItsExternalNumber (
                            int inputLineNumber,
                            int externalVoiceNumber);
        
    string                staffKindAsString () const;
    
    S_msrVoice            createVoiceInStaffByItsExternalNumber (
                            int inputLineNumber,
                            int externalVoiceNumber);

    void                  registerVoiceInStaff (
                            int inputLineNumber,
                            S_msrVoice voice);

    S_msrVoice            fetchVoiceFromStaffByItsExternalNumber (
                            int inputLineNumber,
                            int externalVoiceNumber);

    void                  createStaffSilentVoice (
                            int inputLineNumber);
  
    const int             getStaffNumberOfMusicVoices () const;

    // repeats
    
    void                  createRepeatAndAppendItToStaff (
                            int inputLineNumber);
    
    void                  createMeasureRepeatFromItsFirstMeasureInStaff (
                            int inputLineNumber,
                            int measureRepeatMeasuresNumber,
                            int measureRepeatSlashes);
    
    void                  appendPendingMeasureRepeatToStaff (
                            int inputLineNumber);
                            
    void                  createMultipleRestInStaff (
                            int inputLineNumber,
                            int multipleRestMeasuresNumber);
    
    void                  appendPendingMultipleRestToStaff (
                            int inputLineNumber);
                            
    void                  appendMultipleRestCloneToStaff (
                            int               inputLineNumber,
                            S_msrMultipleRest multipleRest);
  
    void                  appendRepeatCloneToStaff (
                            int         inputLineNumber,
                            S_msrRepeat repeatCLone);

    void                  appendRepeatEndingToStaff (
                            int       inputLineNumber,
                            string    repeatEndingNumber, // may be "1, 2"
                            msrRepeatEnding::msrRepeatEndingKind
                                      repeatEndingKind);

    void                  appendRepeatEndingCloneToStaff (
                            S_msrRepeatEnding repeatEndingClone);

/* JMI
    void                  createRepeatAndAppendItToAllStaffVoices (
                            int inputLineNumber);
    */
    
    // barlines
    
    void                  appendBarlineToStaff (S_msrBarline barline);
    
 //   void            appendHarmonyToStaff (S_msrHarmony harmony); // JMI

    // transpose
    
    void                  appendTransposeToAllStaffVoices ( // JMI
                            S_msrTranspose transpose);
  
    // finalization

    void                  finalizeCurrentMeasureInStaff (
                            int inputLineNumber);

    void                  finalizeStaff (
                            int inputLineNumber);

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

    virtual void          printStructure (ostream& os);

  private:

    // staff kind

    string                fStaffName;
    
    // staff kind

    msrStaffKind          fStaffKind;

    // staff number and anme

    int                   fStaffNumber;

    // staff voices
    
    static int            gMaxStaffVoices;

    map<int, S_msrVoice>  fStaffVoiceRelativeNumberToVoiceMap;
                            //numbered 1 to gMaxStaffVoices
                              
    map<int, S_msrVoice>  fStaffAllVoicesMap;
                            // [K_SILENT_VOICE_NUMBER] is used
                            // for the staff silent voice

    // voice that to not start at the very beginning of the staff
    // are initialized as deep clones of the staff silent voice,
    // in order to 'fill the gap'
    S_msrVoice            fStaffSilentVoice;

 // JMI   string                fStaffInstrumentName;

    // measures

    string                fStaffMeasureNumber;

    // clef, key, time
    
    S_msrClef             fStaffCurrentClef;
    
    S_msrKey              fStaffCurrentKey;
    
    S_msrTime             fStaffCurrentTime;

    // transpose
    
    S_msrTranspose        fStaffTranspose;

    // staff details

    S_msrStaffDetails     fCurrentStaffStaffDetails;
    
    // counters
    
    int                   fRegisteredVoicesCounter;

/* JMI
    // staff tunings
    
    list<S_msrStaffTuning>
                          fStaffTuningsList;
*/

    // uplinks

    // divisions handling is done at the part level
    S_msrPart             fStaffDirectPartUplink;
};
typedef SMARTP<msrStaff> S_msrStaff;
EXP ostream& operator<< (ostream& os, const S_msrStaff& elt);

/*!
\brief A msr part representation.

  A part is represented by a its string contents
*/
//______________________________________________________________________________
class EXP msrPart : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrPart> create (
      int            inputLineNumber,
      string         partID,
      S_msrPartGroup partPartGroupUplink);
                
    SMARTP<msrPart> createPartShallowClone (
      S_msrPartGroup partGroupClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrPart (
      int            inputLineNumber,
      string         partID,
      S_msrPartGroup partPartGroupUplink);
      
    virtual ~msrPart();
  
  private:

    // initialization
    // ------------------------------------------------------

    void                  initializePart ();

  public:

    // set and get
    // ------------------------------------------------------


    // ID and name
    
    void                  setPartID (string partID)
                              { fPartID = partID; }
    
    string                getPartID () const
                              { return fPartID; }

    void                  setPartMsrName (string partMsrName);
    
    string                getPartMsrName () const
                            { return fPartMsrName; }

    void                  setPartName (string partName)
                              { fPartName = partName; }
    
    string                getPartName () const
                              { return fPartName; }

    void                  setPartAbbreviation (
                            string partAbbreviation)
                              { fPartAbbreviation = partAbbreviation; }
                
    string                getPartAbbreviation () const
                              { return fPartAbbreviation; }

    string                getPartCombinedName () const;

    // divisions
    
    void                  setPartMeasurePositionHighTide (
                            int inputLineNumber,
                            int measurePosition);
                      
    void                  updatePartMeasurePositionHighTide (
                            int inputLineNumber,
                            int measurePosition);
                    
    const int             getPartMeasurePositionHighTide () const
                              {
                                return
                                  fPartMeasurePositionHighTide;
                              }

    // measures

    void                  setPartCurrentMeasureNumber (
                            string measureNumber)
                              {
                                fPartCurrentMeasureNumber =
                                  measureNumber;
                              }

    const string          getPartCurrentMeasureNumber () const
                              { return fPartCurrentMeasureNumber; }

    // staff details

    S_msrStaffDetails     getCurrentPartStaffDetails () const
                            { return fCurrentPartStaffDetails; }

    // instrument anme

    void                  setPartInstrumentName (
                            string partInstrumentName)
                              { fPartInstrumentName = partInstrumentName; }
                              
    string                getPartInstrumentName () const
                              { return fPartInstrumentName; }
                
    void                  setPartInstrumentAbbreviation (
                            string partInstrumentAbbreviation)
                              {
                                fPartInstrumentAbbreviation =
                                  partInstrumentAbbreviation;
                              }
                              
    string                getPartInstrumentAbbreviation() const
                              { return fPartInstrumentAbbreviation; }

    // harmony staff and voice
    
    S_msrStaff            getPartHarmonyStaff () const
                              { return fPartHarmonyStaff; }
                  
    S_msrVoice            getPartHarmonyVoice () const
                              { return fPartHarmonyVoice; }
                  
    void                  setPartHarmoniesSupplierVoice (
                            int        inputLineNumber,
                            S_msrVoice partHarmoniesSupplierVoice);

    S_msrVoice            getPartHarmoniesSupplierVoice () const
                              { return fPartHarmoniesSupplierVoice; }
                  
    // divisions per quarter note
    
    void                  setPartDivisionsPerQuarterNote (
                            int divisionsPerQuarterNote);
                      
    const int             getPartDivisionsPerQuarterNote () const
                              { return fPartDivisionsPerQuarterNote; }

    // clef, key, time
    
    S_msrClef             getPartCurrentClef () const
                              { return fPartCurrentClef; };

    S_msrKey              getPartCurrentKey  () const
                              { return fPartCurrentKey; };

    void                  setPartCurrentTime (S_msrTime time)
                              { fPartCurrentTime = time; };

    S_msrTime             getPartCurrentTime () const
                              { return fPartCurrentTime; };

    // transpose

    S_msrTranspose        getPartTranspose () const
                              { return fPartTranspose; };
    
    // uplinks
    
    S_msrPartGroup        getPartPartGroupUplink () const
                              { return fPartPartGroupUplink; }
              
    const map<int, S_msrStaff>&
                          getPartStavesMap ()
                              { return fPartStavesMap; }

    // services
    // ------------------------------------------------------

    // divisions

    void                  setupDurationsDivisions (
                            int divisionPerQuarterNote);
    
    int                   durationAsDivisions (
                            int         inputLineNumber,
                            msrDuration duration);

    void                  printDurationsDivisions (ostream& os);
    
    string                divisionsAsMsrString (
                            int  inputLineNumber,
                            int  divisions,
                            int& numberOfDotsNeeded);

    string                divisionsAsMsrString (
                            int  inputLineNumber,
                            int  divisions);

    string                tupletDivisionsAsMsrString (
                            int  inputLineNumber,
                            int  divisions,
                            int actualNotes,
                            int normalNotes);

    void                  testDivisionsAndDurations (); // JMI
    void                  testTupletSoundingDivisionsAndDurations ();

    // clef, key, time
    
    void                  appendClefToPart (S_msrClef clef);
    void                  appendKeyToPart (S_msrKey  key);
    void                  appendTimeToPart (S_msrTime time);

    // transpose

    void                  appendTransposeToPart (
                            S_msrTranspose transpose);

    // harmony staff and voice
    
    void                  createPartHarmonyStaffAndVoice (
                            int inputLineNumber);
        
    // measures
    
    void                  createMeasureAndAppendItToPart (
                            int    inputLineNumber,
                            string measureNumber);

    // staff details

    void                  appendStaffDetailsToPart (
                            S_msrStaffDetails staffDetails);

    // tuplets JMI ???
    
    // barlines
    
    void                  appendBarlineToPart (S_msrBarline barline);
              
    // repeats
    
    void                  createRepeatAndAppendItToPart (
                            int inputLineNumber);
    
    void                  appendRepeatCloneToPart (
                            int         inputLineNumber,
                            S_msrRepeat repeatCLone);
    
    void                  appendRepeatEndingToPart (
                            int       inputLineNumber,
                            string    repeatEndingNumber, // may be "1, 2"
                            msrRepeatEnding::msrRepeatEndingKind
                                      repeatEndingKind);
    
    void                  appendRepeatEndingCloneToPart (
                            S_msrRepeatEnding repeatEndingCLone);

//    void            appendRepeatEndingToPart (int inputLineNumber);

    void                  createMeasureRepeatFromItsFirstMeasureInPart (
                            int inputLineNumber,
                            int measureRepeatMeasuresNumber,
                            int measureRepeatSlashes);

    void                  appendPendingMeasureRepeatToPart (
                            int inputLineNumber);
                            
    void                  appendMeasureRepeatCloneToPart (
                            int               inputLineNumber,
                            S_msrMultipleRest multipleRest);
  
    void                  createMultipleRestInPart (
                            int inputLineNumber,
                            int multipleRestMeasuresNumber);

    void                  appendPendingMultipleRestToPart (
                            int inputLineNumber);
                            
    void                  appendMultipleRestCloneToPart (
                            int               inputLineNumber,
                            S_msrMultipleRest multipleRest);

    // staves
    
    S_msrStaff            addStaffToPartByItsNumber (
                            int                    inputLineNumber,
                            msrStaff::msrStaffKind staffKind,
                            int                    staffNumber);

    void                  addStaffToPartCloneByItsNumber (
                            S_msrStaff staff);

    S_msrStaff            fetchStaffFromPart (int staffNumber);

    // voices

    void                  removePartEmptyVoices ();
    
    // harmonies
    
    void                  appendHarmonyToPart (
                            S_msrVoice   harmoniesSupplierVoice,
                            S_msrHarmony harmony);

    void                  appendHarmonyToPartClone (
                            S_msrVoice   harmoniesSupplierVoice,
                            S_msrHarmony harmony);

    // backup
    
    void                  handleBackup (int divisions);

    // finalization

    void                  finalizeCurrentMeasureInPart (
                            int inputLineNumber);

    void                  finalizePart (
                            int inputLineNumber);

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

    virtual void          printStructure (ostream& os);

  private:

    // divisions

    int                   fPartDivisionsPerQuarterNote;
    list<pair<msrDuration, int> >
                          fPartDurationsToDivisions;

    // measures

    int                   fPartMeasurePositionHighTide;
    string                fPartCurrentMeasureNumber;

    // part ID and name
    
    string                fPartID; // native
    
    string                fPartMsrName;
                            // may be different than fPartID
                            // if renamed,
                            // coined in constructor

    string                fPartName; // from '<part-name/>'
    string                fPartAbbreviation;

    // instrument name
    
    string                fPartInstrumentName;
    string                fPartInstrumentAbbreviation;

    // clef, key, time
    
    S_msrClef             fPartCurrentClef;
    
    S_msrKey              fPartCurrentKey;
    
    S_msrTime             fPartCurrentTime;

    // transpose
    
    S_msrTranspose        fPartTranspose;

    // staff details

    S_msrStaffDetails     fCurrentPartStaffDetails;
    
    // harmonies

    S_msrStaff            fPartHarmonyStaff;
    S_msrVoice            fPartHarmonyVoice;
    S_msrVoice            fPartHarmoniesSupplierVoice;

    // uplinks
    
    S_msrPartGroup        fPartPartGroupUplink;

    map<int, S_msrStaff>  fPartStavesMap;
};
typedef SMARTP<msrPart> S_msrPart;
EXP ostream& operator<< (ostream& os, const S_msrPart& elt);

/*!
\brief A msr part group representation.

  A part group is represented by a its string contents
*/
//______________________________________________________________________________
class EXP msrPartGroup : public msrElement
{
  public:

    /*
      There is no hierarchy implied in part-group elements.
      All that matters is the sequence of part-group elements relative to score-part elements.
      The sequencing of two consecutive part-group elements does not matter.
      It is the default-x attribute that indicates the left-to-right ordering of the group symbols.

      <part-group number="1" type="start">
      <group-name>Trombones</group-name>
      <group-abbreviation>Trb.</group-abbreviation>
      <group-symbol default-x="-12">brace</group-symbol>
      <group-barline>yes</group-barline>
      </part-group>
    */

    // data types
    // ------------------------------------------------------

    enum msrPartGroupTypeKind {
        k_NoPartGroupType,
        kStartPartGroupType, kStopPartGroupType};
          
    static string partGroupTypeKindAsString (
      msrPartGroupTypeKind partGroupTypeKind);
      
    enum msrPartGroupSymbolKind {
        k_NoPartGroupSymbol,
        kBracePartGroupSymbol, kBracketPartGroupSymbol,
        kLinePartGroupSymbol, kSquarePartGroupSymbol};
          
    static string partGroupSymbolKindAsString (
      msrPartGroupSymbolKind partGroupSymbolKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrPartGroup> create (
      int                    inputLineNumber,
      int                    partGroupNumber,
      string                 partGroupName,
      string                 partGroupDisplayText,
      string                 partGroupAccidentalText,
      string                 partGroupAbbreviation,
      msrPartGroupSymbolKind partGroupSymbolKind,
      int                    partGroupSymbolDefaultX,
      bool                   partGroupBarline,
      S_msrPartGroup         partGroupPartGroupUplink,
      S_msrScore             partGroupScoreUplink);

    SMARTP<msrPartGroup> createPartGroupShallowClone (
      S_msrPartGroup partGroupClone, // the uplink for embeddeed part groups
      S_msrScore     scoreClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrPartGroup (
      int                    inputLineNumber,
      int                    partGroupNumber,
      string                 partGroupName,
      string                 partGroupDisplayText,
      string                 partGroupAccidentalText,
      string                 partGroupAbbreviation,
      msrPartGroupSymbolKind partGroupSymbolKind,
      int                    partGroupSymbolDefaultX,
      bool                   partGroupBarline,
      S_msrPartGroup         partGroupPartGroupUplink,
      S_msrScore             partGroupScoreUplink);
            
    virtual ~msrPartGroup();
  
  public:

    // set and get
    // ------------------------------------------------------

    int                   getPartGroupAbsoluteNumber () const
                              { return fPartGroupAbsoluteNumber; }
    
    int                   getPartGroupNumber () const
                              { return fPartGroupNumber; }
    
    string                getPartGroupDisplayText () const
                              { return fPartGroupDisplayText; }

    string                getPartGroupAccidentalText () const
                              { return fPartGroupAccidentalText; }

    string                getPartGroupName () const
                              { return fPartGroupName; }

    string                getPartGroupAbbreviation () const
                              { return fPartGroupAbbreviation; }

    msrPartGroupSymbolKind
                          getPartGroupSymbolKind () const
                              { return fPartGroupSymbolKind; }

    string                partGroupSymbolKindAsString () const
                              {
                                return
                                  partGroupSymbolKindAsString (
                                    fPartGroupSymbolKind);
                              }

    int                   getPartGroupSymbolDefaultX () const
                              { return fPartGroupSymbolDefaultX; }

    bool                  getPartGroupBarline () const
                              { return fPartGroupBarline; }
    
    string                getPartGroupCombinedName () const;

    void                  setPartGroupInstrumentName (string name)
                              { fPartGroupInstrumentName = name; }
                
    string                getPartGroupInstrumentName () const
                              { return fPartGroupInstrumentName; }
                
    const list<S_msrElement>&
                          getPartGroupElements () const
                              { return fPartGroupElements; }

    S_msrPartGroup        getPartGroupPartGroupUplink () const
                              { return fPartGroupPartGroupUplink; }

    S_msrScore            getPartGroupScoreUplink () const
                              { return fPartGroupScoreUplink; }

    // services
    // ------------------------------------------------------

    S_msrPart             addPartToPartGroupByItsID (
                            int    inputLineNumber,
                            string partID);
    
    void                  addPartToPartGroup (S_msrPart part);
                
    void                  prependSubPartGroupToPartGroup (
                            S_msrPartGroup partGroup);

    void                  appendSubPartGroupToPartGroup (
                            S_msrPartGroup partGroup);

    S_msrPart             fetchPartFromPartGroup (string partID);

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

    virtual void          printStructure (ostream& os);

  private:

    // counters
    
    static int              gPartGroupsCounter;

    // numbers
    
    int                     fPartGroupAbsoluteNumber;
    
    int                     fPartGroupNumber;

    // name
    
    string                  fPartGroupName;

    string                  fPartGroupDisplayText;
    string                  fPartGroupAccidentalText;
    
    string                  fPartGroupAbbreviation;

    // symbol kind
    
    msrPartGroupSymbolKind  fPartGroupSymbolKind;

    // default X
    
    int                     fPartGroupSymbolDefaultX;

    // bar line
    
    bool                    fPartGroupBarline;

    // instrument name

    string                  fPartGroupInstrumentName;

    // accessing parts by name
    map<string, S_msrPart>  fPartGroupPartsMap;

    // allowing for both parts and (sub-)part groups as elements
    list<S_msrElement>      fPartGroupElements;
    
    S_msrPartGroup          fPartGroupPartGroupUplink;

    S_msrScore              fPartGroupScoreUplink;
};
typedef SMARTP<msrPartGroup> S_msrPartGroup;
EXP ostream& operator<< (ostream& os, const S_msrPartGroup& elt);

/*!
\brief A msr score representation.

  A score is represented by a its string contents
*/
//______________________________________________________________________________
class EXP msrScore : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrScore> create (
      int inputLineNumber);

    SMARTP<msrScore> createScoreShallowClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrScore (
      int inputLineNumber);
      
    virtual ~msrScore();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrIdentification   getIdentification () const
                              { return fIdentification; }

    S_msrPageGeometry     getPageGeometry () const
                              { return fPageGeometry; }

    void                  setCredit (S_msrCredit credit)
                              { fCredit = credit; }
    S_msrCredit           getCredit () const
                              { return fCredit; }
        
    const list<S_msrPartGroup>&
                          getPartGroupsList () const
                              { return fPartGroupsList; }

    void                  setInhibitMeasureRepeatReplicasBrowsing ()
                              {
                                fInhibitMeasureRepeatReplicasBrowsing =
                                  true;
                              }
                            
    bool                  getInhibitMeasureRepeatReplicasBrowsing ()
                            const
                              {
                                return
                                  fInhibitMeasureRepeatReplicasBrowsing;
                              };

    void                  setInhibitMultipleRestMeasuresBrowsing ()
                              {
                                fInhibitMultipleRestMeasuresBrowsing =
                                  true;
                              }
                            
    bool                  getInhibitMultipleRestMeasuresBrowsing ()
                            const
                              {
                                return
                                  fInhibitMultipleRestMeasuresBrowsing;
                              };

    // services
    // ------------------------------------------------------

    void                  addPartGroupToScore (
                            S_msrPartGroup partGroup);

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);
    
    virtual void          printStructure (ostream& os);

  private:

    S_msrIdentification  fIdentification;

    S_msrPageGeometry    fPageGeometry;
    
    S_msrCredit          fCredit;
    
    list<S_msrPartGroup> fPartGroupsList;

    // in <measure-repeat/>, the measure replicas are explicit,
    // whereas LilyPond only needs the repeated measure
    bool                 fInhibitMeasureRepeatReplicasBrowsing;

    // in <multiple-rest/>, the rest measures are explicit,
    // whereas LilyPond only needs the number of rest measures
    bool                 fInhibitMultipleRestMeasuresBrowsing;
};
typedef SMARTP<msrScore> S_msrScore;
EXP ostream& operator<< (ostream& os, const S_msrScore& elt);

/*!
\brief A msr midi representation.

  A midi is represented by variable/value pairs
*/
//______________________________________________________________________________
class EXP msrMidi : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMidi> create (
      int    inputLineNumber,
      string midiTempoDuration,
      int    midiTempoPerSecond);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrMidi (
      int    inputLineNumber,
      string midiTempoDuration,
      int    midiTempoPerSecond);
      
    virtual ~msrMidi();
  
  public:

    // set and get
    // ------------------------------------------------------
    string                getMidiTempoDuration () const
                              { return fMidiTempoDuration; }

    int                   getMidiTempoPerSecond () const
                              { return fMidiTempoPerSecond; }

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    string          fMidiTempoDuration;
    int             fMidiTempoPerSecond;
};
typedef SMARTP<msrMidi> S_msrMidi;
EXP ostream& operator<< (ostream& os, const S_msrMidi& elt);


/*! @} */

}


    // creation from MusicXML
    // ------------------------------------------------------

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------



#endif
