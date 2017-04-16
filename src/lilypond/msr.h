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

class msrNote;
typedef SMARTP<msrNote> S_msrNote;

class msrGracenotes;
typedef SMARTP<msrGracenotes> S_msrGracenotes;

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

class msrStaff;
typedef SMARTP<msrStaff> S_msrStaff;

class msrPart;
typedef SMARTP<msrPart> S_msrPart;

class msrPartgroup;
typedef SMARTP<msrPartgroup> S_msrPartgroup;

class msrRepeat;
typedef SMARTP<msrRepeat> S_msrRepeat;


/*!
\internal
\brief A macro to simplify indentation
*/
//______________________________________________________________________________
#define idtr indenter::gIndenter


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
enum msrQuartertonesPitch {
  k_NoQuaterTonesPitch,

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
  msrQuartertonesPitch quartertonesPitch,
  msrDiatonicPitch& diatonicPitch,
  msrAlteration& alteration);

msrQuartertonesPitch quatertonesPitchFromDiatonicPitchAndAlteration (
  int              inputLineNumber,
  msrDiatonicPitch diatonicPitch,
  msrAlteration    alteration);

msrDiatonicPitch msrDiatonicPitchFromQuatertonesPitch (
  int                  inputLineNumber,
  msrQuartertonesPitch quartertonesPitch);

// quarter tones pitches languages
//______________________________________________________________________________
enum msrQuatertonesPitchesLanguage {
  kNederlands, kCatalan, kDeutsch, kEnglish, kEspanol, kFrancais, 
  kItaliano, kNorsk, kPortugues, kSuomi, kSvenska, kVlaams};
  
string msrQuatertonesPitchesLanguageAsString (
  msrQuatertonesPitchesLanguage language);

string msrDiatonicPitchAsString (
  msrQuatertonesPitchesLanguage language,
  msrDiatonicPitch              diatonicPitch);

string msrDiatonicPitchAsString (
  msrQuatertonesPitchesLanguage language,
  msrDiatonicPitch              diatonicPitch);

string msrQuartertonesPitchAsString (
  msrQuatertonesPitchesLanguage language,
  msrQuartertonesPitch          quartertonesPitch);

// global variables
//______________________________________________________________________________

extern map<string, msrQuatertonesPitchesLanguage>
  gQuatertonesPitchesLanguagesMap;

extern map<msrQuartertonesPitch, string> gNederlandsPitchName;
extern map<msrQuartertonesPitch, string> gCatalanPitchName;
extern map<msrQuartertonesPitch, string> gDeutschPitchName;
extern map<msrQuartertonesPitch, string> gEnglishPitchName;
extern map<msrQuartertonesPitch, string> gEspanolPitchName;
extern map<msrQuartertonesPitch, string> gFrancaisPitchName;
extern map<msrQuartertonesPitch, string> gItalianoPitchName;
extern map<msrQuartertonesPitch, string> gNorskPitchName;
extern map<msrQuartertonesPitch, string> gPortuguesPitchName;
extern map<msrQuartertonesPitch, string> gSuomiPitchName;
extern map<msrQuartertonesPitch, string> gSvenskaPitchName;
extern map<msrQuartertonesPitch, string> gVlaamsPitchName;

string existingQuartertonesPitchesLanguages ();

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
class EXP msrOptions : public smartable
{
  public:

    static SMARTP<msrOptions> create ();
      
  public:

    // initialisation
    // ------------------------------------------------------

    void                          initializeMsrOptions (
                                    bool boolOptionsInitialValue);
    
    void                          printMsrOptionsHelp ();

    void                          printMsrOptionsValues (int fieldWidth);

  public:

    // constructors/destructor
    // ------------------------------------------------------

    msrOptions();
  
    virtual ~msrOptions();
 
    // set and get
    // ------------------------------------------------------

    bool                          setMsrQuartertonesPitchesLanguage (
                                    string language);    
 
  public:

    // trace
    bool                          fTraceMsr;
    bool                          fTraceMsrVisitors;
    
    // languages
    msrQuatertonesPitchesLanguage fMsrQuatertonesPitchesLanguage;
    
    // advanced options
    bool                          fCreateStaffRelativeVoiceNumbers;
    bool                          fDelayRestsDynamics;
    bool                          fDelayRestsWords; // JMI
    bool                          fDelayRestsSlurs; // JMI
    bool                          fDelayRestsLigatures; // JMI
    bool                          fDelayRestsWedges; // JMI

    // master voices and stanzas
    bool                          fKeepMasterVoices;
    bool                          fKeepMasterStanzas;
    
    // MSR display
    bool                          fDisplayMsr;

    // stanza display
    bool                          fDontDisplayMsrStanzas;
    
    // stanza display
    bool                          fDontDisplayMsrHarmonies; // JMI
    
    // MSR score summary
    bool                          fDisplayMsrSummary;

    // parts renaming
    map<string, string>           fPartsRenaming;

  private:

};
typedef SMARTP<msrOptions> S_msrOptions;

extern S_msrOptions gMsrOptions;

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

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v) = 0;

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

    virtual void printStructure (ostream& os) {}
    
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

    virtual void print (ostream& os);
 
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

    msrOctaveShiftKind  getOctaveShiftKind () const
                            { return fOctaveShiftKind; }

    int                 getOctaveShiftSize () const
                            { return fOctaveShiftSize; }

    // services
    // ------------------------------------------------------

    string              octaveShiftKindAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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

    msrArticulationKind
            getArticulationKind () const
                { return fArticulationKind; }
        
    // services
    // ------------------------------------------------------

    string  articulationKindAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    msrArticulationKind fArticulationKind;
};
typedef SMARTP<msrArticulation> S_msrArticulation;
EXP ostream& operator<< (ostream& os, const S_msrArticulation& elt);

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
        kArrow,
        kBend,
        kDoubleTongue,
        kDownBow,
        kFingering,
        kFingernails,
        kFret,
        kHammerOn,
        kHandbell,
        kHarmonic,
        kHeel,
        kHole,
        kOpenString,
        kOtherTechnical,
        kPluck,
        kPullOff,
        kSnapPizzicato,
        kStopped,
        kString,
        kTap,
        kThumbPosition,
        kToe,
        kTripleTongue,
        kUpBow};

    static string technicalKindAsString (
      msrTechnicalKind technicalKind);
      
    enum msrTechnicalPlacementKind {
      k_NoPlacementKind, kAbove, kBelow};

    static string technicalPlacementKindAsString (
      msrTechnicalPlacementKind technicalPlacementKind);
      
    enum msrTechnicalAccidentalMarkKind {
      kNatural, kSharp, kFlat};

    static string technicalAccidentalMarkKindAsString (
      msrTechnicalAccidentalMarkKind technicalAccidentalMarkKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTechnical> create (
      int              inputLineNumber,
      msrTechnicalKind technicalKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTechnical (
      int              inputLineNumber,
      msrTechnicalKind technicalKind);
      
    virtual ~msrTechnical();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrTechnicalKind getTechnicalKind () const
                        { return fTechnicalKind; }
        
    void            setTechnicalPlacementKind (
                      msrTechnicalPlacementKind technicalPlacementKind)
                        { fTechnicalPlacementKind = technicalPlacementKind; }
        
    msrTechnicalPlacementKind
                    getTechnicalPlacementKind () const
                        { return fTechnicalPlacementKind; }
        
    void            setTechnicalAccidentalMarkKind (
                      msrTechnicalAccidentalMarkKind
                        technicalAccidentalMarkKind)
                        {
                          fTechnicalAccidentalMarkKind =
                            technicalAccidentalMarkKind;
                        }
        
    msrTechnicalAccidentalMarkKind
                    getTechnicalAccidentalMarkKind () const
                        { return fTechnicalAccidentalMarkKind; }

    void            setTechnicalNoteUplink (S_msrNote note)
                        { fTechnicalNoteUplink = note; }

    S_msrNote       getTechnicalNoteUplink () const
                        { return fTechnicalNoteUplink; }
        
    // services
    // ------------------------------------------------------

    string          technicalKindAsString () const;

    string          technicalPlacementKindAsString () const;

    string          technicalAccidentalMarkKindAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    msrTechnicalKind                fTechnicalKind;

    msrTechnicalPlacementKind       fTechnicalPlacementKind;
    
    msrTechnicalAccidentalMarkKind  fTechnicalAccidentalMarkKind;

    S_msrNote                       fTechnicalNoteUplink;
};
typedef SMARTP<msrTechnical> S_msrTechnical;
EXP ostream& operator<< (ostream& os, const S_msrTechnical& elt);

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
      k_NoPlacementKind, kAbove, kBelow};

    static string ornamentPlacementKindAsString (
      msrOrnamentPlacementKind ornamentPlacementKind);
      
    enum msrOrnamentAccidentalMarkKind {
      kNatural, kSharp, kFlat};

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

    msrOrnamentKind getOrnamentKind () const
                        { return fOrnamentKind; }
        
    void            setOrnamentPlacementKind (
                      msrOrnamentPlacementKind ornamentPlacementKind)
                        { fOrnamentPlacementKind = ornamentPlacementKind; }
        
    msrOrnamentPlacementKind
                    getOrnamentPlacementKind () const
                        { return fOrnamentPlacementKind; }
        
    void            setOrnamentAccidentalMarkKind (
                      msrOrnamentAccidentalMarkKind
                        ornamentAccidentalMarkKind)
                        {
                          fOrnamentAccidentalMarkKind =
                            ornamentAccidentalMarkKind;
                        }
        
    msrOrnamentAccidentalMarkKind
                    getOrnamentAccidentalMarkKind () const
                        { return fOrnamentAccidentalMarkKind; }

    void            setOrnamentNoteUplink (S_msrNote note)
                        { fOrnamentNoteUplink = note; }

    S_msrNote       getOrnamentNoteUplink () const
                        { return fOrnamentNoteUplink; }
        
    // services
    // ------------------------------------------------------

    string          ornamentKindAsString () const;

    string          ornamentPlacementKindAsString () const;

    string          ornamentAccidentalMarkKindAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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

    enum msrSingleTremoloKind {
        kSingleTremolo, kDoubleTremolo };

    static string singleTremoloKindAsString (
      msrSingleTremoloKind singleTremoloKind);
      
    enum msrSingleTremoloPlacementKind {
      k_NoPlacementKind, kAbove, kBelow};

    static string SingleTremoloPlacementKindAsString (
      msrSingleTremoloPlacementKind singleTremoloPlacementKind);
            
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSingleTremolo> create (
      int                     inputLineNumber,
      msrSingleTremoloKind    singleTremoloKind,
      int                     singleTremoloMarksNumber,
      msrSingleTremoloPlacementKind singleTremoloPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSingleTremolo (
      int                     inputLineNumber,
      msrSingleTremoloKind    singleTremoloKind,
      int                     singleTremoloMarksNumber,
      msrSingleTremoloPlacementKind singleTremoloPlacementKind);
      
    virtual ~msrSingleTremolo();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrSingleTremoloKind  getSingleTremoloKind () const
                              { return fSingleTremoloKind; }
        
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

    string                singleTremoloKindAsString () const;

    string                singleTremoloPlacementKindAsString () const;
    
    string                singleTremoloAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    msrSingleTremoloKind          fSingleTremoloKind;

    int                           fSingleTremoloMarksNumber;

    msrSingleTremoloPlacementKind fSingleTremoloPlacementKind;
    
    S_msrNote                     fSingleTremoloNoteUplink;
};
typedef SMARTP<msrSingleTremolo> S_msrSingleTremolo;
EXP ostream& operator<< (ostream& os, const S_msrSingleTremolo& elt);

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

    msrRehearsalKind
            getRehearsalKind () const
                { return fRehearsalKind; }
        
    // services
    string
            getRehearsalText () const
                { return fRehearsalText; }
        
    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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

    msrTieKind  getTieKind () const
                    { return fTieKind; }

    // services
    // ------------------------------------------------------

    string     tieKindAsString () const // JMI
                  { return tieKindAsString (fTieKind); }

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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

    int         getSlurNumber () const { return fSlurNumber; }
    
    msrSlurKind getSlurKind () const { return fSlurKind; }

    // services
    // ------------------------------------------------------

    string      slurKindAsString ();

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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

    int               getLigatureNumber () const
                          { return fLigatureNumber; }
    
    msrLigatureKind getLigatureKind () const
                          { return fLigatureKind; }

    // services
    // ------------------------------------------------------

    string            ligatureKindAsString ();

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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

    msrDynamicsKind
              getDynamicsKind () const
                  { return fDynamicsKind; }

    string  dynamicsKindAsString ();

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    msrDynamicsKind fDynamicsKind;
};
typedef SMARTP<msrDynamics> S_msrDynamics;
EXP ostream& operator<< (ostream& os, const S_msrDynamics& elt);

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

    msrWedgeKind getWedgeKind () const { return fWedgeKind; }

    string  wedgeKindAsString ();

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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
      kTrebleClef, kAltoClef, kTenorClef, kBassClef,
      kTrebleLine1Clef,
      kTrebleMinus15Clef, kTrebleMinus8Clef,
      kTreblePlus8Clef, kTreblePlus15Clef, 
      kBassMinus15Clef, kBassMinus8Clef,
      kBassPlus8Clef, kBassPlus15Clef,
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

    msrClefKind   getClefKind () const
                      { return fClefKind; }
                
    // services
    // ------------------------------------------------------

    string        clefAsString () const;

    bool          clefIsATablatureClef () const;
    
    bool          clefIsAPercussionClef () const;

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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
class EXP msrKey : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrKeyModeKind { kMajorMode, kMinorMode };

    static string keyModeKindAsString (
      msrKeyModeKind keyModeKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrKey> create (
      int                  inputLineNumber,
      msrQuartertonesPitch keyTonicPitch,
      msrKeyModeKind       keyModeKind,
      int                  keyCancel);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrKey (
      int                  inputLineNumber,
      msrQuartertonesPitch keyTonicPitch,
      msrKeyModeKind       keyModeKind,
      int                  keyCancel);
      
    virtual ~msrKey();

  public:

    // set and get
    // ------------------------------------------------------

    msrQuartertonesPitch  getKeyTonicQuartertonesPitch () const
                              { return fKeyTonicQuartertonesPitch; }
                              
    msrKeyModeKind        getKeyModeKind () const
                              { return fKeyModeKind; }
                            
    int                   getKeyCancel () const
                              { return fKeyCancel; }

    // services
    // ------------------------------------------------------

    string                keyAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);
  
  private:

    msrQuartertonesPitch  fKeyTonicQuartertonesPitch;
    msrKeyModeKind        fKeyModeKind;
    int                   fKeyCancel;
};
typedef SMARTP<msrKey> S_msrKey;
EXP ostream& operator<< (ostream& os, const S_msrKey& elt);

/*!
\brief A msr time representation.

  A time is represented by the numerator and denominator
*/
//______________________________________________________________________________
class EXP msrTime : public msrElement
{
  public:
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTime> create (
      int           inputLineNumber,
      int           beatsNumber,
      int           beatsValue);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTime (
      int           inputLineNumber,
      int           beatsNumber,
      int           beatsValue);
      
    virtual ~msrTime();
  
  public:

    // set and get
    // ------------------------------------------------------

    int       getBeatsNumber () const
                  { return fBeatsNumber; }
                  
    int       getBeatsValue () const
                  { return fBeatsValue; }
                  
    // services
    // ------------------------------------------------------
                  
    string    timeAsShortString () const;
    
    string    timeAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    int      fBeatsNumber;
    int      fBeatsValue;
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
      kRegularMeasure,
      kIncompleteLeftMeasure,
      kIncompleteRightMeasure,
      kOverfullMeasure,
      kEmptyMeasure};
    
    static string measureKindAsString (
      msrMeasureKind measureKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasure> create (
      int           inputLineNumber,
      S_msrPart     measureDirectPartUplink,
      int           measureNumber,
      S_msrSegment  segmentUplink);
    
    SMARTP<msrMeasure> createMeasureBareClone (
      S_msrSegment clonedSegment);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrMeasure (
      int           inputLineNumber,
      int           measureNumber,
      S_msrSegment  segmentUplink);
      
    virtual ~msrMeasure();
  
  public:

    // set and get
    // ------------------------------------------------------

    // part direct uplink
    void                  setMeasureDirectPartUplink (S_msrPart part)
                              { fMeasureDirectPartUplink = part; }
                              
    S_msrPart             getMeasureDirectPartUplink () const
                             { return fMeasureDirectPartUplink; }

    void                  setMeasureNumber (int measureNumber)
                              { fMeasureNumber = measureNumber; }

    void                  incrementMeasureNumber ()
                              { fMeasureNumber++; }

    int                   getMeasureNumber () const
                              { return fMeasureNumber; }

    void                  setMeasureClef (S_msrClef clef)
                              { fMeasureClef = clef; }

    S_msrClef             getMeasureClef () const
                              { return fMeasureClef; }

    void                  setMeasureKey (S_msrKey key)
                              { fMeasureKey = key; }

    S_msrKey              getMeasureKey () const
                              { return fMeasureKey; }

    void                  setMeasureTime (S_msrTime time);

    S_msrTime             getMeasureTime () const
                              { return fMeasureTime; }

    int                   getMeasureDivisionsPerFullMeasure () const
                              { return fMeasureDivisionsPerFullMeasure; }

    void                  setMeasurePosition (
                            int inputLineNumber,
                            int measurePosition);

    int                   getMeasurePosition () const
                              { return fMeasurePosition; }
                      
    S_msrNote              getMeasureLastHandledNote () const
                              { return fMeasureLastHandledNote; }

    void                  setMeasureKind (msrMeasureKind measureKind)
                              { fMeasureKind = measureKind; }

    msrMeasureKind        getMeasureKind () const
                              { return fMeasureKind; }

    S_msrSegment          getMeasureSegmentUplink () const
                              { return fMeasureSegmentUplink; }
                      
    S_msrVoice            getMeasureVoiceDirectUplink () const
                              { return fMeasureVoiceDirectUplink; }

    const list<S_msrElement>&
                          getMeasureElementsList () const
                              { return fMeasureElementsList; }

    // services
    // ------------------------------------------------------

    void                  forceMeasureInputLineNumber (
                            int inputLineNumber)
                              { fInputLineNumber = inputLineNumber; }

    int                   getMeasureLength () const
                            // divisions, positions start at 1
                              { return fMeasurePosition - 1; }
                      
    string                getMeasureLengthAsString () const;
    
    string                getMeasureKindAsString () const;

    S_msrMeasure          appendMeasureIfOverflow (
                            int inputLineNumber);

    void                  appendBarlineToMeasure (S_msrBarline barline);
    void                  appendBarCheckToMeasure (S_msrBarCheck barCheck);
    
    void                  appendNoteToMeasure      (S_msrNote note);
    void                  appendNoteToMeasureClone (S_msrNote note);
    
    void                  appendChordToMeasure (S_msrChord chord);
    void                  appendTupletToMeasure (S_msrTuplet tuplet);
    
    void                  appendHarmonyToMeasure (S_msrHarmony harmony);
    
    void                  appendHarmonyToMeasureClone (S_msrHarmony harmony);

    void                  bringMeasureToMeasurePosition (
                            int inputLineNumber,
                            int measurePosition);

    void                  appendGracenotesToMeasure (
                            S_msrGracenotes gracenotes);
    void                  prependGracenotesToMeasure (
                            S_msrGracenotes gracenotes);
    
    void                  prependOtherElementToMeasure (S_msrElement elem);
                      
    void                  appendOtherElementToMeasure  (S_msrElement elem);

    S_msrElement          getLastElementOfMeasure () const
                              { return fMeasureElementsList.back (); }
                      
    S_msrElement          removeLastElementFromMeasure (
                            int inputLineNumber);

    void                  removeFirstChordNoteFromMeasure (
                            int       inputLineNumber,
                            S_msrNote note);

    void                  removeElementFromMeasure (S_msrElement elem); // JMI

    bool                  checkForIncompleteMeasure (
                            int                        inputLineNumber,
                            msrMeasure::msrMeasureKind measureKind);

    bool                  checkForOverfullMeasure (
                            int inputLineNumber);

    void                  finalizeMeasure (
                            int                        inputLineNumber,
                            msrMeasure::msrMeasureKind measureKind);

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    int                 fMeasureNumber;
    
    int                 fMeasureDivisionsPerFullMeasure;

    S_msrClef           fMeasureClef;
    S_msrKey            fMeasureKey;
    S_msrTime           fMeasureTime;

    int                 fMeasurePosition; // in divisions
    S_msrNote           fMeasureLastHandledNote; // for chords handling
    
    S_msrVoice          fMeasureVoiceDirectUplink; // to accelerate things
    S_msrPart           fMeasureDirectPartUplink; // to accelerate things

    msrMeasureKind      fMeasureKind;

    list<S_msrElement>  fMeasureElementsList;
    
    S_msrSegment        fMeasureSegmentUplink;
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

    SMARTP<msrSegment> createSegmentBareClone (
      S_msrVoice clonedVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSegment (
      int        inputLineNumber,
      S_msrVoice segmentVoicekUplink);
      
    virtual ~msrSegment();
    
  private:

    // initialization
    // ------------------------------------------------------

    void                  initializeSegment ();

  public:

    // set and get
    // ------------------------------------------------------

    // part direct uplink
    void                  setSegmentDirectPartUplink (S_msrPart part)
                              { fSegmentDirectPartUplink = part; }
                              
    S_msrPart             getSegmentDirectPartUplink () const
                             { return fSegmentDirectPartUplink; }

    int                   getSegmentAbsoluteNumber () const
                              { return fSegmentAbsoluteNumber; }
                      
    S_msrVoice            getSegmentVoiceUplink () const
                              { return fSegmentVoiceUplink; }
                      
    const list<S_msrMeasure>&
                          getSegmentMeasuresList () const
                              { return fSegmentMeasuresList; }
                                            
    void                  setSegmentTime (S_msrTime time)
                              { fSegmentTime = time; }

    S_msrTime             getSegmentTime () const
                              { return fSegmentTime; }

    // measure number
    void                  setSegmentMeasureNumber (
                            int inputLineNumber,
                            int measureNumber);
                      
    const int             getSegmentMeasureNumber () const
                              { return fSegmentMeasureNumber; }

    // services
    // ------------------------------------------------------

    void                  forceSegmentMeasureNumberTo (int measureNumber); // JMI
    void                  incrementSegmentLastMeasureNumber (
                            int inputLineNumber);

    string                segmentAsString ();

    void                  appendMeasureToSegment (S_msrMeasure measure);

    void                  appendMeasureToSegmentIfNeeded (  // JMI
                            int inputLineNumber,
                            int measureNumber);
  
    void                  appendClefToSegment (S_msrClef clef);
    void                  appendKeyToSegment (S_msrKey key);
    void                  appendTimeToSegment (S_msrTime time);

    void                  appendHarmonyToSegment (S_msrHarmony harmony);
    
    void                  appendHarmonyToSegmentClone (S_msrHarmony harmony);

    void                    bringSegmentToMeasurePosition (
                              int inputLineNumber,
                              int measurePosition);
  
    void                  appendNoteToSegment      (S_msrNote note);
    void                  appendNoteToSegmentClone (S_msrNote note);
    
    void                  appendChordToSegment (S_msrChord chord);
    void                  appendTupletToSegment (S_msrTuplet tuplet);
    
    void                  appendBarlineToSegment (
                            S_msrBarline barline);
    void                  appendBarCheckToSegment (
                            S_msrBarCheck barCheck);
    
    void                  appendGracenotesToSegment (
                            S_msrGracenotes gracenotes);
    void                  prependGracenotesToSegment (
                            S_msrGracenotes gracenotes);
    
    void                  appendOtherElementToSegment (
                            S_msrElement elem);
        
    void                  prependBarlineToSegment (
                            S_msrBarline barline);

    S_msrElement          removeLastElementFromSegment (
                            int inputLineNumber);

    void                  removeFirstChordNoteFromSegment (
                            int       inputLineNumber,
                            S_msrNote note);

    bool                  checkForIncompleteSegmentLastMeasure (
                            int                        inputLineNumber,
                            msrMeasure::msrMeasureKind measureKind);

    void                  finalizeLastMeasureOfSegment (
                            int inputLineNumber);

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    static int            gSegmentsCounter;

    S_msrPart             fSegmentDirectPartUplink;
    
    int                   fSegmentAbsoluteNumber;
    
    S_msrTime             fSegmentTime;
    
    // the measures in the segment contain the mmusic
    list<S_msrMeasure>    fSegmentMeasuresList;

    int                   fSegmentMeasureNumber;

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
class EXP msrGracenotes : public msrElement
{
  public:
        
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrGracenotes> create (
      int        inputLineNumber,
      bool       slashed,
      S_msrVoice gracenotesVoiceUplink);
    
    SMARTP<msrGracenotes> createGracenotesBareClone (
      S_msrVoice voiceClone);

    SMARTP<msrGracenotes> createSkipGracenotesClone (
      S_msrVoice voiceClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrGracenotes (
      int        inputLineNumber,
      bool       gracenoteIsSlashed,
      S_msrVoice gracenotesVoiceUplink);
      
    virtual ~msrGracenotes();
  
  public:

    // set and get
    // ------------------------------------------------------

    // part direct uplink
    void                  setGracenotesDirectPartUplink (
                            S_msrPart part)
                              {
                                fGracenotesDirectPartUplink =
                                  part;
                              }
                              
    S_msrPart             getGracenotesDirectPartUplink () const
                              { return fGracenotesDirectPartUplink; }

    list<S_msrNote>&      getGracenotesNotesList ()
                              { return fGracenotesNotesList; }

    bool                  getGracenotesIsSlashed () const
                              { return fGracenotesIsSlashed; }

    // services
    // ------------------------------------------------------

    void                  appendNoteToGracenotes (S_msrNote note);

    string                gracenotesAsShortString () const;
    
    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    list<S_msrNote>       fGracenotesNotesList;

    S_msrPart             fGracenotesDirectPartUplink;

    bool                  fGracenotesIsSlashed;

    S_msrVoice            fGracenotesVoiceUplink;
};
typedef SMARTP<msrGracenotes> S_msrGracenotes;
EXP ostream& operator<< (ostream& os, const S_msrGracenotes& elt);

/*!
\brief A msr repeat representation.

  A repeat is represented by:
    - a sequence of elements for the common segment
    - a vector of sequences of elements for the alternate endings
*/
//______________________________________________________________________________
class EXP msrAftergracenotes : public msrElement
{
  public:
        
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrAftergracenotes> create (
      int        inputLineNumber,
      bool       aftergracenoteIsSlashed,
      S_msrVoice aftergracenotesVoiceUplink);
    
    SMARTP<msrAftergracenotes> createAftergracenotesBareClone (
      S_msrVoice voiceClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrAftergracenotes (
      int        inputLineNumber,
      bool       slashed,
      S_msrVoice aftergracenotesVoiceUplink);
      
    virtual ~msrAftergracenotes();
  
  public:

    // set and get
    // ------------------------------------------------------

    // part direct uplink
    void                  setAftergracenotesDirectPartUplink (
                            S_msrPart part)
                              {
                                fAftergracenotesDirectPartUplink =
                                  part;
                              }
                              
    S_msrPart             getAftergracenotesDirectPartUplink () const
                              { return fAftergracenotesDirectPartUplink; }

    list<S_msrNote>&      getAftergracenotesNotesList ()
                              { return fAftergracenotesNotesList; }

    bool                  getAftergracenotesIsSlashed () const
                              { return fAftergracenotesIsSlashed; }

    // services
    // ------------------------------------------------------

    void                  appendNoteToAftergracenotes (S_msrNote note);

    string                aftergracenotesAsShortString () const;
    
    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    list<S_msrNote>       fAftergracenotesNotesList;

    S_msrPart             fAftergracenotesDirectPartUplink;
    
    bool                  fAftergracenotesIsSlashed;
    
    S_msrVoice            fAftergracenotesVoiceUplink;
};
typedef SMARTP<msrAftergracenotes> S_msrAftergracenotes;
EXP ostream& operator<< (ostream& os, const S_msrAftergracenotes& elt);

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
      kAbove, kBelow};

    static string wordsPlacementKindAsString (
      msrWordsPlacementKind wordsPlacementKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrWords> create (
      int                   inputLineNumber,
      msrWordsPlacementKind wordsPlacementKind,
      string                wordsContents,
      string                wordsFontStyle,
      string                wordsFontSize,
      string                wordsFontWeight,
      string                wordsFontXMLLang);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrWords (
      int                   inputLineNumber,
      msrWordsPlacementKind wordsPlacementKind,
      string                wordsContents,
      string                wordsFontStyle,
      string                wordsFontSize,
      string                wordsFontWeight,
      string                wordsFontXMLLang);
      
    virtual ~msrWords();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrWordsPlacementKind
              getWordsPlacementKind () const
                  { return fWordsPlacementKind; }

    string    getWordsContents () const
                  { return fWordsContents; }

    string    getWordsFontStyle () const
                  { return fWordsFontStyle; }

    string    getWordsFontSize () const
                  { return fWordsFontSize; }

    string    getWordsFontWeight () const
                  { return fWordsFontWeight; }

    string    getWordsFontXMLLang () const
                  { return fWordsFontXMLLang; }

    // services
    // ------------------------------------------------------

    string    wordsAsString () const;
    
    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:
                        
  msrWordsPlacementKind fWordsPlacementKind;
  
  string                fWordsContents;

  string                fWordsFontStyle;
  string                fWordsFontSize;
  string                fWordsFontWeight;
  string                fWordsFontXMLLang;
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
      kBarcheckSyllable, kBarnumberCheckSyllable,
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

    SMARTP<msrSyllable> createSyllableBareClone (S_msrPart part);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSyllable (
      int                   inputLineNumber,
      msrSyllableKind       syllableKind,
      string                syllableText,
      msrSyllableExtendKind syllableExtendKind,
      int                   divisions,
      S_msrStanza           syllableStanzaUplink);
        
    virtual ~msrSyllable();

  public:

    // set and get
    // ------------------------------------------------------

    // part direct uplink
    void                  setSyllableDirectPartUplink (S_msrPart part)
                              { fSyllableDirectPartUplink = part; }
                              
    S_msrPart             getSyllableDirectPartUplink () const
                             { return fSyllableDirectPartUplink; }

    msrSyllableKind
                      getSyllableKind () const
                          { return fSyllableKind; }

    void              setSyllableNoteUplink (S_msrNote note);

    S_msrNote         getSyllableNoteUplink () const
                          { return fSyllableNoteUplink; }

    string            getSyllableText () const
                          { return fSyllableText; }

    msrSyllableExtendKind
                      getSyllableExtendKind () const
                          { return fSyllableExtendKind; }

    int               getSyllableDivisions () const
                          { return fSyllableDivisions; }

    // services
    // ------------------------------------------------------
  
    string            syllableKindAsString () const;

    string            syllableDivisionsAsString () const;

    string            syllableNoteUplinkAsString () const;

    string            syllableAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:
  
    // divisions handling is done at the part level
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
      msrQuartertonesPitch harmonyRootQuartertonesPitch,
      msrHarmonyKind       harmonyKind,
      string               harmonyKindText,
      msrQuartertonesPitch harmonyBassQuartertonesPitch,
      int                  harmonyDivisions);
    
    SMARTP<msrHarmony> createHarmonyBareClone (
      S_msrPart clonedPart);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrHarmony (
      int                  inputLineNumber,
      msrQuartertonesPitch harmonyRootQuartertonesPitch,
      msrHarmonyKind       harmonyKind,
      string               harmonyKindText,
      msrQuartertonesPitch harmonyBassQuartertonesPitch,
      int                  harmonyDivisions);

    virtual ~msrHarmony();
  
  public:

    // set and get
    // ------------------------------------------------------

 // JMI   void                  setHarmonyDivisions (int divisions)
       //                       { fHarmonyDivisions = divisions; }

    int                   getHarmonyDivisions () const
                              { return fHarmonyDivisions; }

    msrQuartertonesPitch  getHarmonyRootQuartertonesPitch () const
                              { return fHarmonyRootQuartertonesPitch; }
                                
    msrHarmonyKind        getHarmonyKind () const
                              { return fHarmonyKind; }
                
    string                getHarmonyKindText () const
                              { return fHarmonyKindText; }
                
    msrQuartertonesPitch  getHarmonyBassQuartertonesPitch () const
                              { return fHarmonyBassQuartertonesPitch; }
                
    // part direct uplink
    void                  setHarmonyDirectPartUplink (S_msrPart part)
                              { fHarmonyDirectPartUplink = part; }
                              
    S_msrPart             getHarmonyDirectPartUplink () const
                             { return fHarmonyDirectPartUplink; }


    // services
    // ------------------------------------------------------

    string                harmonyKindAsString () const;
    string                harmonyKindAsShortString () const;
    
    string                harmonyAsString () const;
   
    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    S_msrPart                 fHarmonyDirectPartUplink;

    int                       fHarmonyDivisions;
    
    msrQuartertonesPitch      fHarmonyRootQuartertonesPitch;

    msrHarmonyKind            fHarmonyKind;
    string                    fHarmonyKindText;

    msrQuartertonesPitch      fHarmonyBassQuartertonesPitch;
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

    // data types
    // ------------------------------------------------------

    enum msrNoteKind {
      k_NoNoteKind,
      kRestNote, kSkipNote,
      kStandaloneNote,  kGraceNote,
      kChordMemberNote, kTupletMemberNote};
      
    static string noteKindAsString (
      msrNoteKind noteKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrNote> create (
      int                  inputLineNumber,
      S_msrPart            noteDirectPartUplink,
      
      msrNoteKind          noteKind,
    
      msrQuartertonesPitch noteQuatertonesPitch,
      int                  noteDivisions,
      int                  noteDisplayDivisions,
      int                  noteDotsNumber,
      msrDuration          noteGraphicDuration,
      
      int                  noteOctave,
      
      bool                 noteIsARest,
      bool                 noteIsUnpitched,
    
      bool                 noteIsAGraceNote);
    
    SMARTP<msrNote> createNoteBareClone (S_msrPart clonedPart);
    
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
      msrNoteKind          noteKind,
    
      msrQuartertonesPitch noteQuatertonesPitch,
      int                  noteDivisions,
      int                  noteDisplayDivisions,
      int                  noteDotsNumber,
      msrDuration          noteGraphicDuration,
      
      int                  noteOctave,
      
      bool                 noteIsARest,
      bool                 noteIsUnpitched,
    
      bool                 noteIsAGraceNote);
        
    virtual ~msrNote();
    
  public:

    // set and get
    // ------------------------------------------------------

    // basic note description
    // -------------------------------

    void                  setNoteKind (msrNoteKind noteKind)
                              { fNoteKind = noteKind; }

    msrNoteKind           getNoteKind () const
                              { return fNoteKind; }

    msrQuartertonesPitch  getQuatertonesPitch () const
                              { return fNoteQuatertonesPitch; }
                              
    int                   getNoteDivisions () const
                              { return fNoteDivisions; }

    void                  setNoteDisplayDivisions (int divisions)
                              { fNoteDisplayDivisions = divisions; }

    int                   getNoteDisplayDivisions () const
                              { return fNoteDisplayDivisions; }

    int                   getNoteDotsNumber () const
                              { return fNoteDotsNumber; }

    msrDuration           getNoteGraphicDuration () const
                              { return fNoteGraphicDuration; }

    int                   getNoteOctave () const
                              { return fNoteOctave; }

    bool                  getNoteIsARest () const
                            // useful for rest tuplet members
                            { return fNoteIsARest; }
                      
    bool                  getNoteIsUnpitched () const
                            // useful for rest tuplet members
                              { return fNoteIsUnpitched; }
                      
    // octave shifts
    void                  setNoteOctaveShift (
                             S_msrOctaveShift octaveShift)
                                { fNoteOctaveShift = octaveShift; }

    S_msrOctaveShift      getNoteOctaveShift () const
                              { return fNoteOctaveShift; }

    // note context
    // -------------------------------

    // part direct uplink
    void                  setNoteDirectPartUplink (S_msrPart part)
                              { fNoteDirectPartUplink = part; }
                              
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

    // ornaments
    const list<S_msrTechnical>&
                          getNoteTechnicals () const
                              { return fNoteTechnicals; }
                              
    // ornaments
    const list<S_msrOrnament>&
                          getNoteOrnaments () const
                              { return fNoteOrnaments; }
      /* JMI                
    list<S_msrOrnament>&  getNoteOrnamentsToModify ()
                              { return fNoteOrnaments; }
        */
        
    // singleTremolo
    S_msrSingleTremolo          getNoteSingleTremolo () const
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

    list<S_msrDynamics>&  getNoteDynamicsToModify ()
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
                            int measureNumber)
                              { fNoteMeasureNumber = measureNumber; }

    int                   getNoteMeasureNumber () const
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

    // note measure information
    // -------------------------------

    bool                  getNoteIsStemless () const
                              { return fNoteIsStemless; }
                  
    bool                  getNoteHasATrill () const
                              { return fNoteHasATrill; }
                  
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

    msrDiatonicPitch      getDiatonicPitch (
                            int inputLineNumber) const;

    string                notePitchAsString () const;

    string                noteAsShortStringWithRawDivisions () const;
    string                noteAsShortString () const;
    string                noteAsString () const;
      
    msrDiatonicPitch      noteDiatonicPitch (
                            int inputLineNumber) const;
                            
    string                noteDiatonicPitchAsString (
                            int inputLineNumber) const;

    string                noteDivisionsAsMsrString () const;
    string                skipDivisionsAsMsrString () const;
    
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
    void                  addTechnicalToNote (S_msrTechnical art);
    
    // ornaments
    void                  addOrnamentToNote (S_msrOrnament art);
    
    // singleTremolo
    void                  addSingleTremoloToNote (S_msrSingleTremolo trem);
    
    // dynamics
    void                  addDynamicsToNote (S_msrDynamics dynamics);

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

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    // basic note description
    // ------------------------------------------------------

    msrNoteKind               fNoteKind;

    msrQuartertonesPitch      fNoteQuatertonesPitch;
    int                       fNoteDivisions;
    int                       fNoteDisplayDivisions;
    int                       fNoteDotsNumber;
    msrDuration               fNoteGraphicDuration;
    
    int                       fNoteOctave;
    
    bool                      fNoteIsARest;
    bool                      fNoteIsUnpitched;
  
    bool                      fNoteIsAGraceNote;
    
    S_msrOctaveShift          fNoteOctaveShift; // JMI ???

    // note context
    // ------------------------------------------------------

    // divisions handling is done at the part level
    S_msrPart                 fNoteDirectPartUplink;
    
    int                       fNoteStaffNumber;
    int                       fNoteVoiceNumber;

    bool                      fNoteBelongsToAChord;
    
    bool                      fNoteBelongsToATuplet;
    S_msrTuplet               fNoteTupletUplink;

    // note lyrics
    // ------------------------------------------------------

   list<S_msrSyllable>        fNoteSyllables;
    msrSyllable::msrSyllableExtendKind
                              fNoteSyllableExtendKind; // MEGA JMI
    
    // elements attached to the note
    // ------------------------------------------------------

    S_msrStem                 fNoteStem;

    list<S_msrBeam>           fNoteBeams;
                                      
    list<S_msrArticulation>   fNoteArticulations;

    list<S_msrTechnical>      fNoteTechnicals;
    
    list<S_msrOrnament>       fNoteOrnaments;
    
    S_msrSingleTremolo        fNoteSingleTremolo;

    S_msrTie                  fNoteTie;
    
    list<S_msrDynamics>       fNoteDynamics;
    list<S_msrWords>          fNoteWords;
    list<S_msrWedge>          fNoteWedges;

    list<S_msrSlur>           fNoteSlurs;
    list<S_msrLigature>       fNoteLigatures;

    S_msrHarmony              fNoteHarmony;

    // note measure information
    // ------------------------------------------------------

    int                       fNoteMeasureNumber;
    int                       fNotePositionInMeasure;
    bool                      fNoteOccupiesAFullMeasure;
    S_msrMeasure              fNoteMeasureUplink;

    // note redundant information (for speed)
    // ------------------------------------------------------

    // this is needed to handle stemless notes,
    // because the <stem> is visited after 'visitorStart ( S_msrNote )' 
    bool                      fNoteIsStemless;

    // this is useful to produce a nice \aftergrace in LilyPond 
    bool                      fNoteHasATrill;

    // this is needed to produce a delayed turn/inverted-turn in LilyPond 
    bool                      fNoteHasADelayedOrnament;
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
      int         chordDivisions,
      msrDuration chordGraphicDuration);

    // creation from MusicXML
    // ------------------------------------------------------

    SMARTP<msrChord>      createChordBareClone (
                            S_msrPart partClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrChord (
      int         inputLineNumber,
      int         chordDivisions,
      msrDuration chordGraphicDuration);
      
    virtual ~msrChord();
  
  public:

    // set and get
    // ------------------------------------------------------

    // part direct uplink
    void                  setChordDirectPartUplink (S_msrPart part)
                              { fChordDirectPartUplink = part; }
                              
    S_msrPart             getChordDirectPartUplink () const
                             { return fChordDirectPartUplink; }

    msrDuration           getChordGraphicDuration () const
                              { return fChordGraphicDuration; }
            
    const vector<S_msrNote>&
                          getChordNotes () const
                              { return fChordNotes; }

    // ties
    void                  setChordTie (
                            const S_msrTie tie)
                              { fChordTie = tie; }

    S_msrTie              getChordTie () const
                              { return fChordTie; }

    const list<S_msrArticulation>&
                          getChordArticulations () const
                              { return fChordArticulations; }

    // singleTremolo
    S_msrSingleTremolo          getChordSingleTremolo () const
                              { return fChordSingleTremolo; }

    // technicals
    const list<S_msrTechnical>&
                          getChordTechnicals () const
                              { return fChordTechnicals; }
    
    // ornaments
    const list<S_msrOrnament>&
                          getChordOrnaments () const
                              { return fChordOrnaments; }
    
    const list<S_msrDynamics>&
                          getChordDynamics () const
                              { return fChordDynamics; }
                      
    const list<S_msrWords>&
                          getChordWords () const
                              { return fChordWords; }
                      
    const list<S_msrSlur>&
                          getChordSlurs () const
                              { return fChordSlurs; }
                      
    const list<S_msrLigature>&
                          getChordLigatures () const
                              { return fChordLigatures; }
                      
    const list<S_msrWedge>&
                          getChordWedges () const
                              { return fChordWedges; }

    // harmony
    void                  setChordHarmony (S_msrHarmony harmony)
                              { fChordHarmony = harmony; }
                      
    const S_msrHarmony&   getChordHarmony () const
                              { return fChordHarmony; };
                      
     // divisions
    void                  setChordDivisions (int divisions)
                              { fChordDivisions = divisions; }
            
    int                   getChordDivisions () const
                              { return fChordDivisions; }
            
    // measure uplink
    void                  setChordMeasureUplink (
                            const S_msrMeasure& measure)
                              { fChordMeasureUplink = measure; }
                      
    S_msrMeasure          getChordMeasureUplink () const
                            { return fChordMeasureUplink; }

    // measure number
    void                  setChordMeasureNumber (
                            int measureNumber)
                              { fChordMeasureNumber = measureNumber; }
    
    int                   getChordMeasureNumber () const
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

    void                  setChordFirstNotePositionInMeasure (
                            int position);
                    
    void                  setChordFirstNoteMeasureNumber (
                            int measureNumber);
                    
 // JMI   S_msrNote     chordLastNote () const
       //               { return fChordNotes.back (); }

    void                  addArticulationToChord (S_msrArticulation art);
     
    void                  addSingleTremoloToChord (S_msrSingleTremolo trem);
    
    void                  addTechnicalToChord (S_msrTechnical tech);
    
    void                  addOrnamentToChord (S_msrOrnament orn);
     
    void                  addDynamicsToChord (S_msrDynamics dyn)
                              { fChordDynamics.push_back (dyn); }
                    
    void                  addWordsToChord (S_msrWords dyn)
                              { fChordWords.push_back (dyn); }
                    
    void                  addSlurToChord (S_msrSlur slur)
                              { fChordSlurs.push_back (slur); }
                      
    void                  addLigatureToChord (S_msrLigature ligature)
                              { fChordLigatures.push_back (ligature); }
                      
    void                  addWedgeToChord (S_msrWedge wedge)
                              { fChordWedges.push_back (wedge); }

    string                chordDivisionsAsMsrString () const;

    // tuplet members
    void                  applyTupletMemberDisplayFactorToChordMembers (
                            int actualNotes, int normalNotes);

    string                chordAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    // divisions handling is done at the part level
    S_msrPart                 fChordDirectPartUplink;

    // sounding duration
    int                       fChordDivisions;
                                  
    // graphic duration is needed for grace notes,
    // since they don't have any note (sounding) duration
    msrDuration               fChordGraphicDuration;
    
    vector<S_msrNote>         fChordNotes;

    S_msrMeasure              fChordMeasureUplink;

    int                       fChordMeasureNumber;
    int                       fChordPositionInMeasure;
    
    list<S_msrArticulation>   fChordArticulations;

    S_msrSingleTremolo        fChordSingleTremolo;
    
    list<S_msrTechnical>      fChordTechnicals;
    
    list<S_msrOrnament>       fChordOrnaments;
    
    S_msrTie                  fChordTie;
    
    list<S_msrDynamics>       fChordDynamics;
    
    list<S_msrWords>          fChordWords;
    
    list<S_msrSlur>           fChordSlurs;

    list<S_msrLigature>       fChordLigatures;

    list<S_msrWedge>          fChordWedges;

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

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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

    void        setWorkNumber (
                  int    inputLineNumber,
                  string val);

    void        setWorkTitle (
                  int    inputLineNumber,
                  string val);

    void        setMovementNumber (
                  int    inputLineNumber,
                  string val);

    void        setMovementTitle (
                  int    inputLineNumber,
                  string val);

    void        addComposer (
                  int    inputLineNumber,
                  string type,
                  string val);

    void        addArranger (
                  int    inputLineNumber,
                  string type,
                  string val);

    void        addLyricist (
                  int    inputLineNumber,
                  string type,
                  string val);

    void        setRights (
                  int    inputLineNumber,
                  string val);

    void        addSoftware (
                  int    inputLineNumber,
                  string val);

    void        setEncodingDate (
                  int    inputLineNumber,
                  string val);

    void        setScoreInstrumentAssoc (
                  int    inputLineNumber,
                  string val);

    S_msrVarValAssoc
                getWorkNumber () const
                    { return fWorkNumber; }
    
    S_msrVarValAssoc
                getWorkTitle () const
                    { return fWorkTitle; }
    
    S_msrVarValAssoc
                getMovementNumber () const
                    { return fMovementNumber; }
    
    S_msrVarValAssoc
                getMovementTitle () const
                    { return fMovementTitle; }
    
    const vector<S_msrVarValAssoc>&
                getComposers () const
                    { return fComposers; };
                    
    const vector<S_msrVarValAssoc>&
                getArrangers () const
                    { return fArrangers; };
                    
    const vector<S_msrVarValAssoc>&
                getLyricists () const
                    { return fLyricists; };
    
    S_msrVarValAssoc
                getRights () const
                    { return fRights; }
    
    const vector<S_msrVarValAssoc>&
                getSoftwares () const
                    { return fSoftwares; };
    
    S_msrVarValAssoc
                getEncodingDate () const
                    { return fEncodingDate; }
    
    S_msrVarValAssoc
                getScoreInstrumentAssoc () const
                    { return fScoreInstrumentAssoc; }

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    S_msrVarValAssoc         fWorkNumber;
    S_msrVarValAssoc         fWorkTitle;
    S_msrVarValAssoc         fMovementNumber;
    S_msrVarValAssoc         fMovementTitle;
    vector<S_msrVarValAssoc> fComposers;
    vector<S_msrVarValAssoc> fArrangers;
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

    void    setPaperWidth         (float val) { fPaperWidth = val; }
    void    setPaperHeight        (float val) { fPaperHeight = val; }
    void    setTopMargin          (float val) { fTopMargin = val; }
    void    setBottomMargin       (float val) { fBottomMargin = val; }
    void    setLeftMargin         (float val) { fLeftMargin = val; }
    void    setRightMargin        (float val) { fRightMargin = val; }
    void    setBetweenSystemSpace (float val) { fBetweenSystemSpace = val; }
    void    setPageTopSpace       (float val) { fPageTopSpace = val; }
    void    setMillimeters        (float val) { fMillimeters = val; }
    void    setTenths             (int val)   { fTenths = val; }

    float   getPaperWidth         () const    { return fPaperWidth; }
    float   getPaperHeight        () const    { return fPaperHeight; }
    float   getTopMargin          () const    { return fTopMargin; }
    float   getBottomMargin       () const    { return fBottomMargin; }
    float   getLeftMargin         () const    { return fLeftMargin; }
    float   getRightMargin        () const    { return fRightMargin; }
    float   getBetweenSystemSpace () const    { return fBetweenSystemSpace; }
    float   getPageTopSpace       () const    { return fPageTopSpace; }
    int     getMillimeters        () const    { return fMillimeters; }
    float   getTenths             () const    { return fTenths; }
    
    // services
    // ------------------------------------------------------

    float   globalStaffSize       () const;

    // visitors
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);


  private:

    // page height, margins and the like in centimeters are in centimeters
    float             fPaperWidth;
    float             fPaperHeight;
    float             fTopMargin;
    float             fBottomMargin;
    float             fLeftMargin;
    float             fRightMargin;

    int               fMillimeters;
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
    string            getCreditWordsContents () const
                          { return fCreditWordsContents; }

    int               getCreditWordsFontSize () const
                          { return fCreditWordsFontSize; }

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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

    int               getCreditPageNumber () const
                          { return fCreditPageNumber; }

    const vector<S_msrCreditWords>&
                      getCreditWordsList () const
                          { return fCreditWordsList; }

    // services
    // ------------------------------------------------------

    void              appendCreditWordsToCredit (
                        S_msrCreditWords creditWords);

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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

    void  addMsrVarValAssoc (S_msrVarValAssoc assoc)
              { fVarValAssocs.push_back(assoc); } // JMI ???
      
    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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

    string        getContents () const
                      { return fContents; }
                    
    msrCommentGapKind
                  getCommentGapKind  () const
                      { return fCommentGapKind; }
    
    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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
      int           inputLineNumber,
      int           nextBarNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrBreak (
      int           inputLineNumber,
      int           nextBarNumber);
      
    virtual ~msrBreak();
  
  public:

    // set and get
    // ------------------------------------------------------

    int         getNextBarNumber () const
                  { return fNextBarNumber; }

    // services
    // ------------------------------------------------------

    string      breakAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    int fNextBarNumber;
};
typedef SMARTP<msrBreak> S_msrBreak;
EXP ostream& operator<< (ostream& os, const S_msrBreak& elt);

/*!
\brief A msr barnumbercheck representation.

  A barnumbercheck is represented by the number of the next bar
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

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    int fNextBarNumber;
};
typedef SMARTP<msrBarCheck> S_msrBarCheck;
EXP ostream& operator<< (ostream& os, const S_msrBarCheck& elt);

/*!
\brief A msr barnumbercheck representation.

  A barnumbercheck is represented by the number of the next bar
*/
//______________________________________________________________________________
class EXP msrBarnumberCheck : public msrElement
{
  public:
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBarnumberCheck> create (
      int           inputLineNumber,
      int           nextBarNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrBarnumberCheck (
      int           inputLineNumber,
      int           nextBarNumber);
      
    virtual ~msrBarnumberCheck();
  
  public:

    // set and get
    // ------------------------------------------------------

    int         getNextBarNumber () const
                    { return fNextBarNumber; }

    // services
    // ------------------------------------------------------

    string      barnumberCheckAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    int fNextBarNumber;
};
typedef SMARTP<msrBarnumberCheck> S_msrBarnumberCheck;
EXP ostream& operator<< (ostream& os, const S_msrBarnumberCheck& elt);

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
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTuplet> create (
      int           inputLineNumber,
      int           number,
      int           actualNotes,
      int           normalNotes,
      int           notePositionInMeasure);

    SMARTP<msrTuplet> createTupletBareClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTuplet (
      int           inputLineNumber,
      int           number,
      int           actualNotes,
      int           normalNotes,
      int           notePositionInMeasure);
      
    virtual ~msrTuplet();
  
  public:

    // set and get
    // ------------------------------------------------------
    
    int           getTupletNumber () const
                      { return fTupletNumber; }

    int           getTupletActualNotes () const
                      { return fTupletActualNotes; }
    int           getTupletNormalNotes () const
                      { return fTupletNormalNotes; }
    
    const list<S_msrElement>&
                  getTupletElements () const
                      { return fTupletElements; }

    int           getTupletDivisions () const
                      { return fTupletDivisions; }
            
    // measure uplink
    void          setTupletMeasureUplink (
                    const S_msrMeasure& measure)
                      { fTupletMeasureUplink = measure; }
                      
    S_msrMeasure  getTupletMeasureUplink () const
                      { return fTupletMeasureUplink; }

    // measure number
    void          setTupletMeasureNumber (int measureNumber);

    int           getTupletMeasureNumber () const
                      { return fTupletMeasureNumber; }
 
    // position in measure
    int           setTupletPositionInMeasure (int position);
                    // returns the position after the tuplet

    const int     getTupletPositionInMeasure () const
                      { return fTupletPositionInMeasure; }
                      
    // services
    // ------------------------------------------------------

    void          addNoteToTuplet (S_msrNote note);
    void          addChordToTuplet (S_msrChord chord);
    void          addTupletToTuplet (S_msrTuplet tuplet);
    
    void          addTupletToTupletClone (S_msrTuplet tuplet);
    
    void          removeFirstNoteFromTuplet (
                    int       inputLineNumber,
                    S_msrNote note);

    void          applyDisplayFactorToTupletMembers ();
    
    void          unapplyDisplayFactorToTupletMembers (
                    int containingTupletActualNotes,
                    int containingTupletNormalNotes);

    string        tupletAsShortString () const;
    string        tupletAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    int                  fTupletNumber;
    
    int                  fTupletActualNotes;
    int                  fTupletNormalNotes;

    int                  fTupletDivisions;
    int                  fTupletDisplayDivisions;

    S_msrMeasure         fTupletMeasureUplink;

    int                  fTupletMeasureNumber;
    int                  fTupletPositionInMeasure;
    
    list<S_msrElement>   fTupletElements;
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

    int       getTransposeDiatonic () const
                  { return fTransposeDiatonic; }
                  
    int       getTransposeChromatic () const
                  { return fTransposeChromatic; }
                  
    // services
    // ------------------------------------------------------

    string    transposeAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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

    string    getTempoIndication () const
                  { return fTempoIndication; }

    void      setTempoIndication (string indication);

    int       getTempoUnit () const
                  { return fTempoUnit; }

    int       getPerMinute () const
                  { return fPerMinute; }

    // services
    // ------------------------------------------------------

    string    tempoAsString () const;
    
    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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

    // data types
    // ------------------------------------------------------

    enum msrStanzaKind {
       kMasterStanza, kRegularStanza };

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
    
    SMARTP<msrStanza> createStanzaBareClone (
      S_msrVoice clonedVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrStanza (
      int           inputLineNumber,
      int           stanzaNumber,
      msrStanzaKind stanzaKind,
      S_msrVoice    stanzaVoiceUplink);

  public:
  
    virtual ~msrStanza();
  
  public:

    // set and get
    // ------------------------------------------------------

    // part direct uplink
    void                  setStanzaDirectPartUplink (S_msrPart part)
                              { fStanzaDirectPartUplink = part; }
                              
    S_msrPart             getStanzaDirectPartUplink () const
                             { return fStanzaDirectPartUplink; }

    int             getStanzaNumber () const
                        { return fStanzaNumber; }
                
    string          getStanzaName () const;
                
    S_msrVoice      getStanzaVoiceUplink () const
                        { return fStanzaVoiceUplink; }
                
    msrStanzaKind   getStanzaKind () const
                        { return fStanzaKind; }
                
    const vector<S_msrSyllable>&
                    getSyllables () const
                        { return fSyllables; }

    void            setStanzaTextPresent ()
                        { fStanzaTextPresent = true; }

    bool            getStanzaTextPresent () const
                        { return fStanzaTextPresent; }


    // services
    // ------------------------------------------------------

    void            appendSyllableToStanza (
                      S_msrSyllable syllable);
      
    S_msrSyllable   appendRestSyllableToStanza (
                      int inputLineNumber,
                      int divisions);

    S_msrSyllable   appendSkipSyllableToStanza (
                      int inputLineNumber,
                      int divisions);

    S_msrSyllable   appendTiedSyllableToStanza ( // JMI
                      int inputLineNumber,
                      int divisions);

    S_msrSyllable   appendSlurSyllableToStanza ( // JMI
                      int inputLineNumber,
                      int divisions);

    S_msrSyllable   appendSlurBeyondEndSyllableToStanza ( // JMI
                      int inputLineNumber,
                      int divisions);

    S_msrSyllable   appendLigatureSyllableToStanza ( // JMI
                      int inputLineNumber,
                      int divisions);

    S_msrSyllable   appendLigatureBeyondEndSyllableToStanza ( // JMI
                      int inputLineNumber,
                      int divisions);

    S_msrSyllable   appendBarnumberCheckSyllableToStanza (
                      int inputLineNumber,
                      int divisions);

    S_msrSyllable   appendBarcheckSyllableToStanza (
                      int inputLineNumber,
                      int divisions);

    S_msrSyllable   appendBreakSyllableToStanza (
                      int inputLineNumber,
                      int divisions);
                
    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    // part direct uplink
    S_msrPart                 fStanzaDirectPartUplink;

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
    
    // position in measure

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:
};
typedef SMARTP<msrCoda> S_msrCoda;
EXP ostream& operator<< (ostream& os, const S_msrCoda& elt);

/*!
\brief A msr barline representation.

  A barline is represented by the number of the next bar
*/
//______________________________________________________________________________
class EXP msrEyeglasses : public msrElement
{
  public:
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrEyeglasses> create (
      int                       inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrEyeglasses (
      int                       inputLineNumber);
      
    virtual ~msrEyeglasses();
  
  public:

    // set and get
    // ------------------------------------------------------

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:
};
typedef SMARTP<msrEyeglasses> S_msrEyeglasses;
EXP ostream& operator<< (ostream& os, const S_msrEyeglasses& elt);

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
    
    msrPedalType
                getPedalType () const
                    { return fPedalType; }
                    
    msrPedalLine
                getPedalLine () const
                    { return fPedalLine; }
                    
    // services
    // ------------------------------------------------------

    string        pedalTypeAsString ();
    string        pedalLineAsString ();
    
    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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
      kLeft, kMiddle, kRight}; // kRight by default
        
    static string barlineLocationAsString (
      msrBarlineLocation barlineLocation);
      
    enum msrBarlineStyle {
      k_NoStyle,
      kRegular,  // by default
      kDotted, kDashed, kHeavy,
      kLightLight, kLightHeavy, kHeavyLight, kHeavyHeavy,
      kTick, kShort};

    static string barlineStyleAsString (
      msrBarlineStyle barlineStyle);
      
    enum msrBarlineEndingType {
      k_NoEndingType,
      kStart, kStop, kDiscontinue};

    static string barlineEndingTypeAsString (
      msrBarlineEndingType barlineEndingType);
      
    enum msrBarlineRepeatDirection {
      k_NoRepeatDirection,
      kForward, kBackward};

    static string barlineRepeatDirectionAsString (
      msrBarlineRepeatDirection barlineRepeatDirection);
      
    enum msrBarlineRepeatWinged {
      k_NoRepeatWinged,
      kNone,
      kStraight, kCurved, kDoubleStraight, kDoubleCurved };

    static string barlineRepeatWingedAsString (
      msrBarlineRepeatWinged barlineRepeatWinged);
      
    enum msrBarlineCategory {
      kStandaloneBar,
      kRepeatStart, kRepeatEnd,
      kHookedEndingStart, kHookedEndingEnd,
      kHooklessEndingStart, kHooklessEndingEnd};
      
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

    bool          getBarlineHasSegno () const
                      { return fBarlineHasSegno; }
    bool          getBarlineHasCoda () const
                      { return fBarlineHasCoda; }

    msrBarlineLocation
                  getLocation () const
                      { return fLocation; }
                    
    msrBarlineStyle
                  getStyle () const
                      { return fStyle; }
                    
    msrBarlineEndingType
                  getEndingType () const
                      { return fEndingType; }
                    
    string        getEndingNumber () const
                      { return fEndingNumber; }
                    
    msrBarlineRepeatDirection
                  getRepeatDirection () const
                      { return fRepeatDirection; }
                    
    msrBarlineRepeatWinged
                  getRepeatWinged () const
                      { return fRepeatWinged; }
                    
    const list<int>&
                  getEndingNumbersList () const
                      { return fEndingNumbersList; }
                        
    msrBarlineCategory
                  getBarlineCategory () const
                      { return fBarlineCategory; }

    void          setBarlineCategory (
                  msrBarlineCategory barlineCategory)
                    { fBarlineCategory = barlineCategory; }
    
    // measure number
    void          setBarlineMeasureNumber (
                    int positionInMeasure)
                      { fBarlineMeasureNumber= positionInMeasure; }
                      
    int           getBarlineMeasureNumber() const
                      { return fBarlineMeasureNumber; }

    // position in measure
    void          setBarlinePositionInMeasure (
                    int positionInMeasure)
                      { fBarlinePositionInMeasure = positionInMeasure; }
                      
    int           getBarlinePositionInMeasure () const
                      { return fBarlinePositionInMeasure; }

    // services
    // ------------------------------------------------------

    string        barlineAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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
class EXP msrRepeatending : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrRepeatendingKind {
      kHookedEnding,
      kHooklessEnding};

    static string repeatendingKindAsString (
      msrRepeatendingKind repeatendingKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeatending> create (
      int                 inputLineNumber,
      string              repeatendingNumber, // may be "1, 2"
      msrRepeatendingKind repeatendingKind,
      S_msrSegment        segment,
      S_msrRepeat         repeatUplink);
    
    SMARTP<msrRepeatending> createRepeatendingBareClone (
      S_msrRepeat clonedRepeat);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRepeatending (
      int                 inputLineNumber,
      string              repeatendingNumber, // may be "1, 2"
      msrRepeatendingKind repeatendingKind,
      S_msrSegment        segment,
      S_msrRepeat         repeatUplink);
      
    virtual ~msrRepeatending();
  
  public:

    // set and get
    // ------------------------------------------------------

    string          getRepeatendingNumber () const
                        { return fRepeatendingNumber; }
                
    void            setRepeatendingNumber (int repeatendingNumber)
                        { fRepeatendingNumber = repeatendingNumber; }

    void            setRepeatendingInternalNumber (
                      int repeatendingInternalNumber)
                        {
                          fRepeatendingInternalNumber =
                            repeatendingInternalNumber;
                        }
                      
    int             getRepeatendingInternalNumber () const
                        { return fRepeatendingInternalNumber; }
                
    msrRepeatendingKind
                    getRepeatendingKind () const
                        { return fRepeatendingKind; }
                
    S_msrSegment    getRepeatendingSegment () const
                        { return fRepeatendingSegment; }
                
    S_msrRepeat     getRepeatendingRepeatUplink () const
                        { return fRepeatendingRepeatUplink; }

    // services
    // ------------------------------------------------------
  
    string          repeatendingAsString () const;

    void            appendElementToRepeatending (S_msrElement elem);
                    
    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:
  
    string              fRepeatendingNumber; // may be "1, 2"
    int                 fRepeatendingInternalNumber; // internally assigned
    
    msrRepeatendingKind fRepeatendingKind;
    
    S_msrSegment        fRepeatendingSegment;

    S_msrRepeat         fRepeatendingRepeatUplink;
};
typedef SMARTP<msrRepeatending> S_msrRepeatending;
EXP ostream& operator<< (ostream& os, const S_msrRepeatending& elt);

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
    
    SMARTP<msrRepeat> createRepeatBareClone (
      S_msrVoice clonedVoice);

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

    void            setRepeatCommonSegment (
                      S_msrSegment repeatCommonSegment);
                  
    S_msrSegment    getRepeatCommonSegment () const
                        { return fRepeatCommonSegment; }

    const vector<S_msrRepeatending>&
                    getRepeatEndings () const
                      { return fRepeatEndings; }

    S_msrVoice      getRepeatVoiceUplink () const
                      { return fRepeatVoiceUplink; }

    // services
    // ------------------------------------------------------

    void            addRepeatending (S_msrRepeatending repeatending);

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    S_msrSegment              fRepeatCommonSegment;
    
    vector<S_msrRepeatending> fRepeatEndings;
    int                       fRepeatEndingsInternalCounter;
    
    S_msrVoice                fRepeatVoiceUplink;
};
typedef SMARTP<msrRepeat> S_msrRepeat;
EXP ostream& operator<< (ostream& os, const S_msrRepeat& elt);

/*!
\brief A msr voice representation.

  A vpoce is represented by a its string contents
*/
//______________________________________________________________________________
class EXP msrVoice : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrVoiceKind {
        kRegularVoice, kHarmonyVoice, kMasterVoice};
          
    static string voiceKindAsString (
      msrVoiceKind voiceKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrVoice> create (
      int          inputLineNumber,
      S_msrPart    voiceDirectPartUplink,
      msrVoiceKind voiceKind,
      int          externalVoiceNumber,
      S_msrStaff   voiceStaffUplink);
    
    SMARTP<msrVoice> createVoiceBareClone (
      S_msrStaff clonedStaff);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    // for regular voices
    msrVoice (
      int          inputLineNumber,
      msrVoiceKind voiceKind,
      int          externalVoiceNumber,
      S_msrStaff   voiceStaffUplink);

    virtual ~msrVoice();

  private:

    // initialisation
    // ------------------------------------------------------

    void            initializeVoice (int inputLineNumber);
  
  public:

    // set and get
    // ------------------------------------------------------

    // part direct uplink
    void                  setVoiceDirectPartUplink (S_msrPart part)
                              { fVoiceDirectPartUplink = part; }
                              
    S_msrPart             getVoiceDirectPartUplink () const
                             { return fVoiceDirectPartUplink; }

    msrVoiceKind    getVoiceKind () const
                        { return fVoiceKind; }

    int             getExternalVoiceNumber () const
                        { return fExternalVoiceNumber; }
                
    void            setExternalVoiceNumber (int externalVoiceNumber)
                        { fExternalVoiceNumber = externalVoiceNumber; }

    int             getStaffRelativeVoiceNumber () const
                        { return fStaffRelativeVoiceNumber; }
                
    S_msrStaff      getVoiceStaffUplink () const
                        { return fVoiceStaffUplink; }
                
    const map<int, S_msrStanza>&
                    getVoiceStanzasMap () const
                        { return fVoiceStanzasMap; }

    string          getVoiceName () const;

    S_msrSegment    getVoiceLastSegment () const
                        { return fVoiceLastSegment; }

    S_msrStanza     getVoiceStanzaMaster () const
                        { return fVoiceStanzaMaster; }
               
    int             getVoiceActualNotesCounter () const
                        { return fVoiceActualNotesCounter; }

    int             getVoiceActualHarmoniesCounter () const
                        { return fVoiceActualHarmoniesCounter; }

    int             getMeasureZeroHasBeenMetInVoice () const
                        { return fMeasureZeroHasBeenMetInVoice; }

    S_msrTime       getVoiceTime () const
                        { return fVoiceTime; }
               
     // measure number
    void            setVoiceMeasureNumber (
                      int inputLineNumber,
                      int measureNumber);
                      
    const int       getVoiceMeasureNumber () const
                        { return fVoiceMeasureNumber; }

    // has music been inserted in the voice?
    void            setMusicHasBeenInsertedInVoice ()
                        { fMusicHasBeenInsertedInVoice = true; }

    bool            getMusicHasBeenInsertedInVoice () const
                        { return fMusicHasBeenInsertedInVoice; }

    // services
    // ------------------------------------------------------

    string          voiceKindAsString () const;

    void            forceVoiceMeasureNumberTo (int measureNumber); // JMI
                      
    void            appendClefToVoice (S_msrClef clef);
    void            appendKeyToVoice  (S_msrKey  key);
    void            appendTimeToVoice (S_msrTime time);
    
    void            appendTransposeToVoice (
                      S_msrTranspose transpose);

// JMI    void          appendWordsToVoice  (S_msrWords words);
    
    void            appendTempoToVoice (
                      S_msrTempo tempo);
    
    void            appendRehearsalToVoice (
                      S_msrRehearsal rehearsal);
    
    void            appendOctaveShiftToVoice (
                      S_msrOctaveShift octaveShift);
    
    void            appendNoteToVoice      (S_msrNote note);
    void            appendNoteToVoiceClone (S_msrNote note);

    void            appendChordToVoice  (S_msrChord chord);
    void            appendTupletToVoice (S_msrTuplet tuplet);

    void            appendHarmonyToVoice (S_msrHarmony harmony);
    
    void            appendHarmonyToVoiceClone (S_msrHarmony harmony);

    void            bringVoiceToMeasurePosition (
                      int inputLineNumber,
                      int measurePosition);
  
    void            appendGracenotesToVoice (
                      S_msrGracenotes gracenotes);
    void            prependGracenotesToVoice (
                      S_msrGracenotes gracenotes);

    void            appendSyllableToVoice (
                      int           inputLineNumber,
                      int           stanzaNumber,
                      S_msrSyllable syllable);
                              
    void            appendBarCheckToVoice (
                      S_msrBarCheck barCheck);
                    
    void            appendBarnumberCheckToVoice (
                      S_msrBarnumberCheck barNumberCheck);

    void            appendBreakToVoice (S_msrBreak break_);

    void            appendRepeatToVoice (int inputLineNumber);
    
    void            appendRepeatCloneToVoice (
                      int         inputLineNumber,
                      S_msrRepeat repeatCLone);
    
    void            appendRepeatendingToVoice (
                      int       inputLineNumber,
                      string    repeatendingNumber, // may be "1, 2"
                      msrRepeatending::msrRepeatendingKind
                                repeatendingKind);

    void            appendRepeatendingCloneToVoice (
                      S_msrRepeatending repeatendingClone);

    void            prependBarlineToVoice (S_msrBarline barline);
    void            appendBarlineToVoice (S_msrBarline barline);
    
    void            appendSegnoToVoice (S_msrSegno segno);
    void            appendCodaToVoice (S_msrCoda coda);
    void            appendEyeglassesToVoice (
                      S_msrEyeglasses eyeglasses);
    void            appendPedalToVoice (S_msrPedal pedal);
    
    void            appendOtherElementToVoice (S_msrElement elem);
                      // for other types of elements not known
                      // in this header file, such as LPSR elements

    S_msrElement    removeLastElementFromVoice (
                      int inputLineNumber);

    void            removeFirstChordNoteFromVoice (
                      int       inputLineNumber,
                      S_msrNote note);

    S_msrStanza     addStanzaToVoiceByItsNumber (
                      int inputLineNumber,
                      int stanzaNumber);

    void            addStanzaToVoiceWithoutCatchUp (S_msrStanza stanza);

    void            catchUpWithVoiceStanzaMaster (
                      S_msrStanza stanza);
                    
    void            addStanzaToVoiceWithCatchUp (S_msrStanza stanza);

    S_msrStanza     createStanzaInVoiceIfNeeded (
                      int inputLineNumber,
                      int stanzaNumber);
    
    bool            checkForIncompleteVoiceLastMeasure (
                      int inputLineNumber);

    void            createNewLastSegmentForVoice (int inputLineNumber);

    void            finalizeLastMeasureOfVoice (int inputLineNumber);

    void            finalizeVoice (int inputLineNumber);
    
    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    // part direct uplink
    S_msrPart                 fVoiceDirectPartUplink;

    msrVoiceKind              fVoiceKind;

    string                    fVoiceName;
    
    // voice numbers in MusicXML may be greater than 4
    // while there can only be 4 in a staff
    // we thus have to cope with that
    int                       fExternalVoiceNumber;
    int                       fStaffRelativeVoiceNumber;

    S_msrStaff                fVoiceStaffUplink;

    int                       fVoiceActualNotesCounter;
    int                       fVoiceActualHarmoniesCounter;

    S_msrTime                 fVoiceTime;

    int                       fVoiceMeasureNumber;
    
    // anacrusis detection
    bool                      fMeasureZeroHasBeenMetInVoice;
    bool                      fMeasureNumberHasBeenSetInVoice;
    bool                      fMusicHasBeenInsertedInVoice;
    
    // fVoiceLastSegment contains the music
    // not yet stored in fVoiceInitialRepeatsAndSegments,
    // it is thus logically the end of the latter,
    // and is created implicitly for every voice.
    // Is is needed 'outside' of the 'list<S_msrElement>'
    // because it is not a mere S_msrElement, but a S_msrSegment
    list<S_msrElement>        fVoiceInitialRepeatsAndSegments;
    S_msrSegment              fVoiceLastSegment;

    // fVoiceFirstSegment is used to work around LilyPond issue 34
    S_msrSegment              fVoiceFirstSegment;

    // fVoiceCurrentRepeat is null or
    // the last msrRepeat in fVoiceInitialRepeatsAndSegments
    S_msrRepeat               fVoiceCurrentRepeat;

    // the stanza master, collecting skips along the way,
    // to be used as a 'prelude' by actual stanzas
    // that start at later points
    S_msrStanza               fVoiceStanzaMaster;

    // the harmony master, collecting harmonies and skips along the way
    // JMI
    
    // the stanza map
    map<int, S_msrStanza>     fVoiceStanzasMap;
};
EXP ostream& operator<< (ostream& os, const S_msrVoice& elt);

/*!
\brief A msr staff representation.

  A staff is represented by a its string contents
*/

/*
Staff assignment is only needed for music notated on multiple staves. Used by both notes and directions. Staff values are numbers, with 1 referring to the top-most staff in a part.
*/
//______________________________________________________________________________
class EXP msrStafftuning : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStafftuning> create (
      int                  inputLineNumber,
      int                  stafftuningLineNumber,
      msrQuartertonesPitch quartertonesPitch,
      int                  stafftuningOctave);
    
    SMARTP<msrStafftuning> createStafftuningBareClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrStafftuning (
      int                  inputLineNumber,
      int                  stafftuningLineNumber,
      msrQuartertonesPitch quartertonesPitch,
      int                  stafftuningOctave);
         
    ~ msrStafftuning ();
  
  public:

    // set and get
    // ------------------------------------------------------

    int             getStafftuningLineNumber () const
                        { return fStafftuningLineNumber; }

    msrQuartertonesPitch
                    getStafftuningQuartertonesPitch () const
                        { return fStafftuningQuartertonesPitch; }

    int             getStafftuningOctave () const
                        { return fStafftuningOctave; }

    // services
    // ------------------------------------------------------

    string          stafftuningAsString () const;
         
    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);
    
  public:
  
    // data
    // ------------------------------------------------------
    
    int                   fStafftuningLineNumber;
    
    msrQuartertonesPitch  fStafftuningQuartertonesPitch;
    int                   fStafftuningOctave;
};
typedef SMARTP<msrStafftuning> S_msrStafftuning;
EXP ostream& operator<< (ostream& os, const S_msrStafftuning& elt);

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
      kTablatureStaff, kPercussionStaff,
      kHarmonyStaff};

    static string staffKindAsString (
      msrStaffKind staffKind);
      
    enum msrStaffTypeKind {
      kOssiaStaffType, kCueStaffType, kEditorialStaffType,
      kRegularStaffType, kAlternateStaffType};

     /*
    static string staffTypeKindAsString (
      msrStaffTypeKind staffTypeKind);
      */
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStaff> create (
      int          inputLineNumber,
      S_msrPart    staffDirectPartUplink,
      msrStaffKind staffKind,
      int          staffNumber,
      S_msrPart    staffPartUplink);
    
    SMARTP<msrStaff> createStaffBareClone (
      S_msrPart clonedPart);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrStaff (
      int          inputLineNumber,
      msrStaffKind staffKind,
      int          staffNumber,
      S_msrPart    staffPartUplink);
      
    virtual ~msrStaff();
  
  public:

    // set and get
    // ------------------------------------------------------

    // part direct uplink
    void            setStaffDirectPartUplink ( S_msrPart part)
                        { fStaffDirectPartUplink = part; }

    S_msrPart       getStaffDirectPartUplink () const
                        { return fStaffDirectPartUplink; }

    msrStaffKind    getStaffKind () const
                        { return fStaffKind; }
                
    int             getStaffNumber () const
                        { return fStaffNumber; }
                
    string          getStaffName () const;

// JMI    string          getStaffInstrumentName () const
      //                  { return fStaffInstrumentName; }

    S_msrClef       getStaffClef () const { return fStaffClef; };
    S_msrKey        getStaffKey  () const { return fStaffKey; };
    S_msrTime       getStaffTime () const { return fStaffTime; };
    
    S_msrTranspose  getStaffTranspose () const
                        { return fStaffTranspose; };
    
    void            setStaffClef (S_msrClef clef);
    void            setStaffKey  (S_msrKey  key);
    void            setStaffTime (S_msrTime time);

    void            setStaffTranspose (S_msrTranspose transpose);

    const map<int, S_msrVoice>&
                    getStaffVoiceRelativeNumberToVoiceMap () const
                        { return fStaffVoiceRelativeNumberToVoiceMap; }

    const map<int, S_msrVoice>&
                    getStaffAllVoicesMap () const
                        { return fStaffAllVoicesMap; }

    const list<S_msrStafftuning>&
                    getStafftuningsList ()
                        { return fStafftuningsList; }

    // measure number
    void            setStaffMeasureNumber (
                      int inputLineNumber,
                      int measureNumber);
                      
    const int       getStaffMeasureNumber () const
                        { return fStaffMeasureNumber; }

    // voice master
    const S_msrVoice
                    getStaffMasterVoice () const
                        { return fStaffMasterVoice; }

    // services
    // ------------------------------------------------------

/*
    S_msrVoice      addVoiceMasterToStaff (
                      int inputLineNumber);
  */
  
    const int       getStaffNumberOfMusicVoices () const;

    S_msrVoice      addVoiceToStaffByItsRelativeNumber (
                      int                    inputLineNumber,
                      msrVoice::msrVoiceKind voiceKind,
                      int                    voiceRelativeNumber);
  
    S_msrVoice      addVoiceToStaffByItsExternalNumber (
                      int inputLineNumber,
                      int externalVoiceNumber);
  
    string          staffKindAsString () const;
    
    S_msrVoice      createVoiceInStaffByItsExternalNumber (
                      int inputLineNumber,
                      int externalVoiceNumber);

    void            registerVoiceInStaff (
                      int inputLineNumber,
                      S_msrVoice voice);

    S_msrVoice      fetchVoiceFromStaffByItsExternalNumber (
                      int inputLineNumber,
                      int externalVoiceNumber);
                              
    void            appendClefToAllStaffVoices (S_msrClef clef);
    void            appendKeyToAllStaffVoices  (S_msrKey   key);
    void            appendTimeToAllStaffVoices (S_msrTime time);
    
    void            appendRepeatToStaff (int inputLineNumber);
    
    void            appendRepeatCloneToStaff (
                      int         inputLineNumber,
                      S_msrRepeat repeatCLone);

    void            appendRepeatendingToStaff (
                      int       inputLineNumber,
                      string    repeatendingNumber, // may be "1, 2"
                      msrRepeatending::msrRepeatendingKind
                                repeatendingKind);

    void            appendRepeatendingCloneToStaff (
                      S_msrRepeatending repeatendingClone);

    void            appendBarlineToStaff (S_msrBarline barline);
    
    void            appendRepeatToAllStaffVoices (int inputLineNumber);
    
 //   void            appendHarmonyToStaff (S_msrHarmony harmony); // JMI

    void            appendTransposeToAllStaffVoices (
                      S_msrTranspose transpose);

    void            addStafftuningToStaff (
                      S_msrStafftuning stafftuning)
                        { fStafftuningsList.push_back (stafftuning); }
  
    void            finalizeLastMeasureOfStaff (int inputLineNumber);
    
    void            finalizeStaff (int inputLineNumber);

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

    virtual void printStructure (ostream& os);

  private:

    // divisions handling is done at the part level
    S_msrPart               fStaffDirectPartUplink;

    string                  fStaffName;
    
    msrStaffKind            fStaffKind;
    
    static int              gMaxStaffVoices;

    int                     fStaffNumber;

    map<int, S_msrVoice>    fStaffVoiceRelativeNumberToVoiceMap;
                              //numbered 1 to gMaxStaffVoices
                              
    map<int, S_msrVoice>    fStaffAllVoicesMap;
                              // [0] is used for the staff voice master

 // JMI   string                  fStaffInstrumentName;

    int                     fStaffMeasureNumber;
    
    S_msrVoice              fStaffMasterVoice;

    S_msrClef               fStaffClef;
    S_msrKey                fStaffKey;
    S_msrTime               fStaffTime;

    S_msrTranspose          fStaffTranspose;

    int                     fRegisteredVoicesCounter;

    list<S_msrStafftuning>  fStafftuningsList;
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
      S_msrPartgroup partPartgroupUplink);
                
    SMARTP<msrPart> createPartBareClone (
      S_msrPartgroup clonedPartgroup);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrPart (
      int            inputLineNumber,
      string         partID,
      S_msrPartgroup partPartgroupUplink);
      
    virtual ~msrPart();
  
  public:

    // set and get
    // ------------------------------------------------------

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

    void                  setPartAbbreviation (string partAbbreviation)
                              { fPartAbbreviation = partAbbreviation; }
                
    string                getPartAbbreviation () const
                              { return fPartAbbreviation; }
                
    void                  setPartInstrumentName (string partInstrumentName)
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
                
    S_msrPartgroup        getPartPartgroupUplink () const
                              { return fPartPartgroupUplink; }
              
    S_msrStaff            getPartHarmonyStaff () const
                              { return fPartHarmonyStaff; }
                  
    S_msrVoice            getPartHarmonyVoice () const
                              { return fPartHarmonyVoice; }
                  
    void                  setPartHarmoniesSupplierVoice (
                            int        inputLineNumber,
                            S_msrVoice partHarmoniesSupplierVoice);

    S_msrVoice            getPartHarmoniesSupplierVoice () const
                              { return fPartHarmoniesSupplierVoice; }
                  
    const map<int, S_msrStaff>&
                          getPartStavesMap ()
                              { return fPartStavesMap; }

    string                getPartCombinedName () const;

    // divisions per quarter note
    void                  setPartDivisionsPerQuarterNote (
                            int divisionsPerQuarterNote);
                      
    const int             getPartDivisionsPerQuarterNote () const
                              { return fPartDivisionsPerQuarterNote; }
          
    void                  setPartClef (S_msrClef clef);
    void                  setPartKey  (S_msrKey  key);
    void                  setPartTime (S_msrTime time);

    S_msrClef             getPartClef () const { return fPartClef; };
    S_msrKey              getPartKey  () const { return fPartKey; };
    S_msrTime             getPartTime () const { return fPartTime; };
    
    void                  setPartTranspose (S_msrTranspose transpose);

    S_msrTranspose        getPartTranspose () const { return fPartTranspose; };
    
    // measure number
    void                  setPartMeasureNumber (
                            int inputLineNumber,
                            int measureNumber);
                      
    const int             getPartMeasureNumber () const
                              { return fPartMeasureNumber; }

    const bool            getMeasureZeroHasBeenMetInPart () const
                              { return fMeasureZeroHasBeenMetInPart; }

    void                  setPartMeasurePositionHighTide (
                            int inputLineNumber,
                            int measurePosition);
                      
    void                  updatePartMeasurePositionHighTide (
                            int inputLineNumber,
                            int measurePosition);
                    
    const int             getPartMeasurePositionHighTide () const
                              { return fPartMeasurePositionHighTide; }

    const int             getPartMeasureNumberMin () const
                              { return fPartMeasureNumberMin; }
                      
    const int             getPartMeasureNumberMax () const
                              { return fPartMeasureNumberMax; }

    // services
    // ------------------------------------------------------

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
  
    void                  testDivisionsAndDurations ();
    void                  testTupletDivisionsAndDurations ();
  
    void                  createPartHarmonyStaffAndVoice (
                            int inputLineNumber);
        
    void                  appendRepeatToPart (int inputLineNumber);
    
    void                  appendRepeatCloneToPart (
                            int         inputLineNumber,
                            S_msrRepeat repeatCLone);
    
    void                  appendRepeatendingToPart (
                            int       inputLineNumber,
                            string    repeatendingNumber, // may be "1, 2"
                            msrRepeatending::msrRepeatendingKind
                                      repeatendingKind);
    
    void                  appendRepeatendingCloneToPart (
                            S_msrRepeatending repeatendingCLone);

//    void            appendRepeatendingToPart (int inputLineNumber);

    void                  appendBarlineToPart (S_msrBarline barline);
              
    S_msrStaff            addStaffToPartByItsNumber (
                            int                    inputLineNumber,
                            msrStaff::msrStaffKind staffKind,
                            int                    staffNumber);
    
    void                  addStaffToPartCloneByItsNumber (S_msrStaff staff);

    S_msrStaff            fetchStaffFromPart (int staffNumber);

    void                  appendHarmonyToPart (
                            S_msrVoice   harmoniesSupplierVoice,
                            S_msrHarmony harmony);

    void                  appendHarmonyToPartClone (
                            S_msrVoice   harmoniesSupplierVoice,
                            S_msrHarmony harmony);

    void                  handleBackup (int divisions);

    void                  removePartEmptyVoices ();

    void                  finalizeLastMeasureOfPart (int inputLineNumber);

    void                  finalizePart (int inputLineNumber);

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

    virtual void printStructure (ostream& os);

  private:
    
    string                  fPartID; // native
    string                  fPartMsrName;
                              // may be different than fPartID
                              // if renamed,
                              // coined in constructor

    string                  fPartName; // from '<part-name/>'
    string                  fPartAbbreviation;
    
    string                  fPartInstrumentName;
    string                  fPartInstrumentAbbreviation;

    S_msrPartgroup          fPartPartgroupUplink;

    map<int, S_msrStaff>    fPartStavesMap;

    int                     fPartDivisionsPerQuarterNote;
    list<pair<msrDuration, int> >
                            fPartDurationsToDivisions;

    int                     fPartMeasureNumber;
    bool                    fMeasureZeroHasBeenMetInPart;
    int                     fPartMeasurePositionHighTide;

    int                     fPartMeasureNumberMin;
    int                     fPartMeasureNumberMax;
    
    S_msrClef               fPartClef;
    S_msrKey                fPartKey;
    S_msrTime               fPartTime;

    S_msrTranspose          fPartTranspose;

    S_msrStaff              fPartHarmonyStaff;
    S_msrVoice              fPartHarmonyVoice;
    S_msrVoice              fPartHarmoniesSupplierVoice;
};
typedef SMARTP<msrPart> S_msrPart;
EXP ostream& operator<< (ostream& os, const S_msrPart& elt);

/*!
\brief A msr part group representation.

  A part group is represented by a its string contents
*/
//______________________________________________________________________________
class EXP msrPartgroup : public msrElement
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

    enum msrPartgroupTypeKind {
        k_NoPartgroupType,
        kStartPartgroupType, kStopPartgroupType};
          
    static string partgroupTypeKindAsString (
      msrPartgroupTypeKind partgroupTypeKind);
      
    enum msrPartgroupSymbolKind {
        k_NoPartgroupSymbol,
        kBracePartgroupSymbol, kBracketPartgroupSymbol,
        kLinePartgroupSymbol, kSquarePartgroupSymbol};
          
    static string partgroupSymbolKindAsString (
      msrPartgroupSymbolKind partgroupSymbolKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrPartgroup> create (
      int                    inputLineNumber,
      int                    partgroupNumber,
      string                 partgroupName,
      string                 partgroupDisplayText,
      string                 partgroupAccidentalText,
      string                 partgroupAbbreviation,
      msrPartgroupSymbolKind partgroupSymbolKind,
      int                    partgroupSymbolDefaultX,
      bool                   partgroupBarline,
      S_msrPartgroup         partgroupPartgroupUplink);

    SMARTP<msrPartgroup> createPartgroupBareClone (
      S_msrPartgroup clonedPartgroup); // the uplink

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrPartgroup (
      int                    inputLineNumber,
      int                    partgroupNumber,
      string                 partgroupName,
      string                 partgroupDisplayText,
      string                 partgroupAccidentalText,
      string                 partgroupAbbreviation,
      msrPartgroupSymbolKind partgroupSymbolKind,
      int                    partgroupSymbolDefaultX,
      bool                   partgroupBarline,
      S_msrPartgroup         partgroupPartgroupUplink);
            
    virtual ~msrPartgroup();
  
  public:

    // set and get
    // ------------------------------------------------------

    int           getPartgroupNumber () const
                      { return fPartgroupNumber; }
    
    string        getPartgroupDisplayText () const
                      { return fPartgroupDisplayText; }

    string        getPartgroupAccidentalText () const
                      { return fPartgroupAccidentalText; }

    string        getPartgroupName () const
                      { return fPartgroupName; }

    string        getPartgroupAbbreviation () const
                      { return fPartgroupAbbreviation; }

    msrPartgroupSymbolKind
                  getPartgroupSymbolKind () const
                      { return fPartgroupSymbolKind; }

    static string pargroupSymbolKindAsString (
                  msrPartgroupSymbolKind partgroupSymbolKind);

    string        pargroupSymbolKindAsString ()
                      {
                        return
                          pargroupSymbolKindAsString (
                            fPartgroupSymbolKind);
                      }

    int           getPartgroupSymbolDefaultX () const
                      { return fPartgroupSymbolDefaultX; }

    bool          getPartgroupBarline () const
                      { return fPartgroupBarline; }
    
    string        getPartgroupCombinedName () const;

    void          setPartgroupInstrumentName (string name)
                      { fPartgroupInstrumentName = name; }
                
    string        getPartgroupInstrumentName () const
                      { return fPartgroupInstrumentName; }
                
    const list<S_msrElement>&
                  getPartgroupElements () const
                      { return fPartgroupElements; }

    S_msrPartgroup
                  getPartgroupPartgroupUplink () const
                      { return fPartgroupPartgroupUplink; }

    // services
    // ------------------------------------------------------

    S_msrPart     addPartToPartgroupByItsID (
                    int    inputLineNumber,
                    string partID);
    
    void          addPartToPartgroup (S_msrPart part);
                
    void          prependSubPartgroupToPartgroup (
                    S_msrPartgroup partgroup);

    void          appendSubPartgroupToPartgroup (
                    S_msrPartgroup partgroup);

    S_msrPart     fetchPartFromPartgroup (string partID);

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

    virtual void printStructure (ostream& os);

  private:

    static int              gPartgroupsCounter;
    
    int                     fPartgroupAbsoluteNumber;
    
    int                     fPartgroupNumber;
        
    string                  fPartgroupName;

    string                  fPartgroupDisplayText;
    string                  fPartgroupAccidentalText;
    
    string                  fPartgroupAbbreviation;

    msrPartgroupSymbolKind  fPartgroupSymbolKind;
    int                     fPartgroupSymbolDefaultX;

    bool                    fPartgroupBarline;

    string                  fPartgroupInstrumentName;

    // accessing parts by name
    map<string, S_msrPart>  fPartgroupPartsMap;

    // allowing for both parts and (sub-)part groups as elements
    list<S_msrElement>      fPartgroupElements;
    
    S_msrPartgroup          fPartgroupPartgroupUplink;
};
typedef SMARTP<msrPartgroup> S_msrPartgroup;
EXP ostream& operator<< (ostream& os, const S_msrPartgroup& elt);

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

    SMARTP<msrScore> createScoreBareClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrScore (
      int inputLineNumber);
      
    virtual ~msrScore();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrIdentification
                    getIdentification () const
                        { return fIdentification; }

    S_msrPageGeometry
                    getPageGeometry () const
                        { return fPageGeometry; }

    void            setCredit (S_msrCredit credit)
                        { fCredit = credit; }
                        
    S_msrCredit     getCredit () const
                        { return fCredit; }
    
    const list<S_msrPartgroup>&
                    getPartgroupsList () const
                        { return fPartgroupsList; }

    // services
    // ------------------------------------------------------

    void addPartgroupToScore (S_msrPartgroup partgroup);

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);
    
    virtual void printStructure (ostream& os);

  private:

    S_msrIdentification  fIdentification;

    S_msrPageGeometry    fPageGeometry;
    
    S_msrCredit          fCredit;
    
    list<S_msrPartgroup> fPartgroupsList;
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
    string        getMidiTempoDuration () const
                      { return fMidiTempoDuration; }

    int           getMidiTempoPerSecond () const
                      { return fMidiTempoPerSecond; }

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

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
