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
              
    - the base class is msrElement
    - the msrElement tree can be converted to:
        - a text description with         printMSR()
        - a MusicXML text equivalent with printMusicXML()
        - LilyPond source code with       printLilypondCode()
*/

//______________________________________________________________________________
// PRE-declarations for class dependencies

class msrNote;
typedef SMARTP<msrNote> S_msrNote;

class msrLyrics;
typedef SMARTP<msrLyrics> S_msrLyrics;

class msrVoice;
typedef SMARTP<msrVoice> S_msrVoice;

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
      messageIfFalse << endl << flush; \
    assert(condition); \
  } \
}

/*!
\internal
\brief A macro to emit warning messages regarding MusicXML data
*/
//______________________________________________________________________________
#define msrMusicXMLWarning( inputSourceName, inputLineNumber, message ) \
{ \
  cerr << endl << endl; \
\
  cerr << \
    "!!! MusicXML WARNING !!!, " << inputSourceName << \
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
#define msrMusicXMLError( inputSourceName, inputLineNumber, message ) \
{ \
  cerr << endl << endl; \
\
  cerr << \
    "### MusicXML ERROR ###, " << inputSourceName << \
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
#define msrInternalError( inputSourceName, inputLineNumber, message ) \
{ \
  cerr << endl << endl; \
\
  cerr << \
    "[[[ MSR INTERNAL ERROR ]]], " << inputSourceName << \
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
\brief Global variables.

  An global variable is implemented as a static member of this class.
*/
//______________________________________________________________________________
/*
class EXP msrGlobalVariables {
  public:
    
    enum msrDisplayKind {
      kMusicXML, kMSR, kScoreSummary, kLilypondCode,
      k_NoDisplay};

    // set and get
    // ------------------------------------------------------

    static msrDisplayKind getDisplayKind ()
      { return sDisplayKind; }
      
    static void setDisplayKind (msrDisplayKind kind)
      { sDisplayKind = kind; }
  
  private:
  
    static msrDisplayKind sDisplayKind;
};
*/

//______________________________________________________________________________
/*!
  \brief The LilyPond note names language.
*/
enum msrNoteNamesLanguage {
  kNederlands, kCatalan, kDeutsch, kEnglish, kEspanol, kItaliano, 
  kFrancais, kNorsk, kPortugues, kSuomi, kSvenska, kVlaams};

typedef map<string, msrNoteNamesLanguage>
  msrNoteNamesLanguageMap;
  
static msrNoteNamesLanguageMap gMsrNoteNamesLanguageMap;

void                 initializeMsrNoteNamesLanguage ();
msrNoteNamesLanguage getMsrNoteNamesLanguage (string lang);
  
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
    
  protected:
  
    msrOptions();
  
    virtual ~msrOptions();
 
  public:

    // General
    // ------------------------------------------------------

    // synthetic view
    string                          fCommandLineOptions;
    string                          fInputSourceName;
    string                          fTranslationDate;
    

    // interactive mode
    bool                            fInteractive;
    
    // trace
    bool                            fTrace;
    
    // debug
    bool                            fDebug;
    bool                            fDebugDebug;

    // forcing debug information at specific places in the code
    bool                            fForceDebug;
    
    // measure number-selective debug
    set<int>                        fDebugMeasureNumbersSet;
    bool                            fSaveDebug;
    bool                            fSaveDebugDebug;

    // MSR
    // ------------------------------------------------------

    // languages
    string                          fMsrNoteNamesLanguageAsString;
    msrNoteNamesLanguage            fMsrNoteNamesLanguage;
    
    // advanced options
    bool                            fCreateStaffRelativeVoiceNumbers;
    bool                            fDelayRestsDynamics;

    // MSR display
    bool                            fDisplayMSR;

    // lyrics display
    bool                            fDontDisplayMSRLyrics;
    
    // MSR score summary
    bool                            fDisplayMSRScoreSummary;

    // parts renaming
    map<string, string>             fPartsRenaming;
};
typedef SMARTP<msrOptions> S_msrOptions;

/*!
\brief A generic msr element representation.

  An element is represented by its name and the
  list of its enclosed elements plus optional parameters.
*/
//______________________________________________________________________________
class msrMeasureLocation
{
  public:

    int         fMeasureNumber;
    int         fPositionInMeasure; // divisions

    bool        operator==
                  (const msrMeasureLocation& other) const 
                    {
                      return
                        fMeasureNumber == other.fMeasureNumber
                          &&
                        fPositionInMeasure == other.fPositionInMeasure;
                    }    
};

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
      S_msrOptions& msrOpts, 
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

    virtual void print (ostream& os);
    
  protected:

    S_msrOptions   fMsrOptions;
     
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
\brief A note description for MusicXML.
*/
//______________________________________________________________________________
class msrMusicXMLNoteData
{
  public:

    /*
      The alter element represents chromatic alteration
      in number of semitones (e.g., -1 for flat, 1 for sharp).
      Decimal values like 0.5 (quarter tone sharp) are used for microtones.
    
      The following table lists note names for quarter-tone accidentals
      in various languages; here the pre- fixes semi- and sesqui-
      respectively mean ‘half’ and ‘one and a half’.
      
      Languages that do not appear in this table do not provide special note names yet.
      * 
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
  
    enum msrDiatonicPitch {
      // starting at C for LilyPond relative octave calculations
      kC, kD, kE, kF, kG, kA, kB}; // JMI, k_NoDiatonicPitch};
    
    enum msrMusicXMLAlterationKind {
      kSesquiFlat, kFlat, kSemiFlat,
      kNatural,
      kSemiSharp, kSharp, kSesquiSharp};

    enum msrMusicXMLTieKind {
        k_NoTie,
        kStartTie, kContinueTie, kStopTie};
        
    msrMusicXMLNoteData ();

    string        musicXMLTieKindAsString () const;

    virtual void  print (ostream& os);
 
  public:
  
    char                      fMusicXMLStep;
    bool                      fMusicXMLStepIsARest;
    bool                      fMusicXMLStepIsUnpitched;

    // The alter element represents chromatic alteration
    // in number of semitones (e.g., -1 for flat, 1 for sharp).
    // Decimal values like 0.5 (quarter tone sharp) are used for microtones.
    msrDiatonicPitch          fDiatonicPitch;
    float                     fMusicXMLAlter;
    msrMusicXMLAlterationKind fMusicXMLAlteration;
                          
    int                       fMusicXMLOctave;

    // MusicXML durations are in divisions per quarter note.
    // LilyPond durations are in whole notes,
    // hence the "* 4" multiplications
    
    // the note duration when played
    int                       fMusicXMLDivisions;

    // tuplets member notes need another value for display
    int                       fMusicXMLDisplayDivisions;
    string                    fMusicXMLType; // "whole", "32nd", ...

    int                       fMusicXMLDotsNumber;
    
    bool                      fMusicXMLNoteIsAGraceNote;
    
    bool                      fMusicXMLNoteBelongsToAChord;
    
    bool                      fMusicXMLNoteBelongsToATuplet;

    msrMusicXMLTieKind        fMusicXMLTieKind;
                    
    int                       fMusicXMLVoiceNumber;
};
EXP ostream& operator<< (ostream& os, msrMusicXMLNoteData& elt);

/*!
\brief A beat description for MusicXML.
*/
//______________________________________________________________________________
class musicXMLBeatData // JMI ???
{
  public:
  
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

    enum msrOctaveShiftKind {
      kOctaveShiftUp, kOctaveShiftDown,
      kOctaveShiftStop};

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrOctaveShift> create (
      S_msrOptions&      msrOpts, 
      int                inputLineNumber,
      msrOctaveShiftKind octaveShiftKind,
      int                octaveShiftSize);

  protected:

    msrOctaveShift (
      S_msrOptions&      msrOpts, 
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

    enum msrStemKind {
      k_NoStem,
      kStemUp, kStemDown};

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStem> create (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      msrStemKind   stemKind);

  protected:

    msrStem (
      S_msrOptions& msrOpts, 
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

    enum msrBeamKind {
      k_NoBeam,
      kBeginBeam, kContinueBeam, kEndBeam,
      kForwardHookBeam, kBackwardHookBeam};
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBeam> create (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           number,
      msrBeamKind   beamKind);

  protected:

    msrBeam (
      S_msrOptions& msrOpts, 
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
    
    enum msrArticulationKind {
        kStaccato, kStaccatissimo,
        kFermata,
        kTrill,
        kArpeggiato };

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrArticulation> create (
      S_msrOptions&       msrOpts, 
      int                 inputLineNumber,
      msrArticulationKind articulationKind);

  protected:

    msrArticulation (
      S_msrOptions&       msrOpts, 
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

    virtual void print (ostream& os);

  private:

    msrArticulationKind fArticulationKind;
};
typedef SMARTP<msrArticulation> S_msrArticulation;
EXP ostream& operator<< (ostream& os, const S_msrArticulation& elt);

/*!
\brief A msr rehearsal representation.

  An rehearsal is represented by the numerator and denominator
*/
//______________________________________________________________________________
class EXP msrRehearsal : public msrElement
{
  public:
    
    enum msrRehearsalKind {
        kNone,
        kRectangle, kOval, kCircle, kBracket, kTriangle, kDiamond };

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRehearsal> create (
      S_msrOptions&    msrOpts, 
      int              inputLineNumber,
      msrRehearsalKind rehearsalKind,
      string           rehearsalText);

  protected:

    msrRehearsal (
      S_msrOptions&    msrOpts, 
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

    virtual void print (ostream& os);

  private:

    msrRehearsalKind fRehearsalKind;

    string           fRehearsalText;
};
typedef SMARTP<msrRehearsal> S_msrRehearsal;
EXP ostream& operator<< (ostream& os, const S_msrRehearsal& elt);

/*!
\brief A msr slur representation.

  A slur is represented by a SlurKind value (hairpins in LilyPond)
*/
//______________________________________________________________________________
class EXP msrSlur : public msrElement
{
  public:

    enum msrSlurKind { kStartSlur, kContinueSlur, kStopSlur, k_NoSlur };
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSlur> create (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      msrSlurKind   slurKind);

  protected:

    msrSlur (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      msrSlurKind   slurKind);
      
    virtual ~msrSlur();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrSlurKind getSlurKind () const { return fSlurKind; }

    string  slurKindAsString ();

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  private:

    msrSlurKind fSlurKind;
};
typedef SMARTP<msrSlur> S_msrSlur;
EXP ostream& operator<< (ostream& os, const S_msrSlur& elt);

/*!
\brief A msr dynamics representation.

  A dynamics is represented by a msrDynamicsKind value
*/
//______________________________________________________________________________
class EXP msrDynamics : public msrElement
{
  public:

    enum msrDynamicsKind {
          kF, kFF, kFFF, kFFFF, kFFFFF, kFFFFFF,
          kP, kPP, kPPP, kPPPP, kPPPPP, kPPPPPP,
          kMF, kMP, kFP, kFZ, kRF, kSF, kRFZ, kSFZ, kSFP, kSFPP, kSFFZ,
          k_NoDynamics};
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrDynamics> create (
      S_msrOptions& msrOpts, 
      int                    inputLineNumber,
      msrDynamicsKind           dynamicsKind);

  protected:

    msrDynamics (
      S_msrOptions& msrOpts, 
      int                    inputLineNumber,
      msrDynamicsKind           dynamicsKind);
      
    virtual ~msrDynamics();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrDynamicsKind
              getDynamicsKind () const
                  { return fDynamicsKind; }

    string  dynamicsKindAsString ();
    string  dynamicsKindAsLilypondString ();

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

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

    enum msrWedgeKind
      { kCrescendoWedge, kDecrescendoWedge, kStopWedge };
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrWedge> create (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      msrWedgeKind  wedgeKind);

  protected:

    msrWedge (
      S_msrOptions& msrOpts, 
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

    virtual void print (ostream& os);

  private:

    msrWedgeKind fWedgeKind;
};
typedef SMARTP<msrWedge> S_msrWedge;
EXP ostream& operator<< (ostream& os, const S_msrWedge& elt);

/*!
\brief The msr sequential music element
*/
//______________________________________________________________________________
class EXP msrVoicechunk : public msrElement
{
  public:
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrVoicechunk> create (
      S_msrOptions& msrOpts, 
      int           inputLineNumber);

    SMARTP<msrVoicechunk> createEmptyClone ();

  protected:

    msrVoicechunk (
      S_msrOptions& msrOpts, 
      int           inputLineNumber);
      
    virtual ~msrVoicechunk();
    
  public:

    // set and get
    // ------------------------------------------------------

    const list<S_msrElement>&
                  getVoicechunkElements () const
                      { return fVoicechunkElementsList; }
                      
    string        voicechunkAsString ();

    // services
    // ------------------------------------------------------

    void          prependElementToVoicechunk (S_msrElement elem)
                      { fVoicechunkElementsList.push_front (elem); }
    void          appendElementToVoicechunk  (S_msrElement elem)
                      { fVoicechunkElementsList.push_back (elem); }
    
    S_msrElement  getLastElementOfVoicechunk () const
                      { return fVoicechunkElementsList.back (); }
                      
    void          removeLastElementFromVoicechunk ()
                      { fVoicechunkElementsList.pop_back (); }

//    void          removeElementFromVoicechunk (S_msrElement elem);

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  private:

    list<S_msrElement>   fVoicechunkElementsList;
};
typedef SMARTP<msrVoicechunk> S_msrVoicechunk;
EXP ostream& operator<< (ostream& os, const S_msrVoicechunk& elt);

/*!
\brief A msr repeat representation.

  A repeat is represented by:
    - a sequence of elements for the common part
    - a vector of sequences of elements for the alternate endings
*/
//______________________________________________________________________________
class EXP msrGraceexpression : public msrElement
{
  public:
        
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrGraceexpression> create (
      S_msrOptions&   msrOpts, 
      int             inputLineNumber,
      bool            slashed);
    
    SMARTP<msrGraceexpression> createEmptyClone ();

  protected:

    msrGraceexpression (
      S_msrOptions&   msrOpts, 
      int             inputLineNumber,
      bool            slashed);
      
    virtual ~msrGraceexpression();
  
  public:

    // set and get
    // ------------------------------------------------------

    bool
                getGraceexpressionIsSlashed () const
                    { return fGraceexpressionIsSlashed; }

    S_msrVoicechunk
                getGraceexpressionVoicechunk () const
                    { return fGraceexpressionVoicechunk; }

    // services
    // ------------------------------------------------------

    void      appendNoteToGraceexpression (S_msrNote note)
                  {
                    fGraceexpressionVoicechunk->
                      appendElementToVoicechunk (note);
                  }
    
    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  private:

    bool                     fGraceexpressionIsSlashed;
    
    S_msrVoicechunk          fGraceexpressionVoicechunk;
};
typedef SMARTP<msrGraceexpression> S_msrGraceexpression;
EXP ostream& operator<< (ostream& os, const S_msrGraceexpression& elt);

/*!
\brief A words representation.

  A words is represented by the lyrics to use
*/
//______________________________________________________________________________
class EXP msrWords : public msrElement
{
  public:

    enum msrWordsPlacementKind {
      kAbove, kBelow};

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrWords> create (
      S_msrOptions&         msrOpts, 
      int                   inputLineNumber,
      msrWordsPlacementKind wordsPlacementKind,
      string                wordsContents,
      string                wordsFontStyle,
      string                wordsFontSize,
      string                wordsFontWeight,
      string                wordsFontXMLLang);

  protected:

    msrWords (
      S_msrOptions&         msrOpts, 
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
\brief A msr note representation.

  A note is represented by its name, optional accidentals,
    duration (in the form of numerator/denominator) and optional dots.
*/
//______________________________________________________________________________
class EXP msrNote : public msrElement
{
  public:

    enum msrNoteKind {
      k_NoNoteKind,
      kRestNote, 
      kStandaloneNote,  kGraceNote,
      kChordMemberNote, kTupletMemberNote};
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrNote> createFromMusicXMLData (
        S_msrOptions&        msrOpts,
        int                  inputLineNumber,
        msrMusicXMLNoteData& musicXMLNoteData,
        msrSlur::msrSlurKind slurKind);

    SMARTP<msrNote> createBareNoteClone ();
    
    // creation from xml2Msr
    // ------------------------------------------------------

    static SMARTP<msrNote> createRest (
        S_msrOptions& msrOpts,
        int           inputLineNumber,
        int           divisions,
        int           voiceNumber);
    
  protected:
 
    msrNote (
        S_msrOptions&        msrOpts,
        int                  inputLineNumber,
        msrMusicXMLNoteData& musicXMLNoteData,
        msrSlur::msrSlurKind slurKind);
    
    virtual ~msrNote();
    
  public:

    // set and get
    // ------------------------------------------------------

    S_msrOptions  getMsrOptions () const
                      { return fMsrOptions; }

    bool          getNoteIsUnpitched () const
                    // useful for rest tuplet members
                      {
                        return
                          fMusicXMLNoteData.fMusicXMLStepIsUnpitched;
                      }
                      
    bool          getNoteIsARest () const
                    // useful for rest tuplet members
                      {
                        return
                          fMusicXMLNoteData.fMusicXMLStepIsARest;
                      }
                      
    void          setNoteKind (msrNoteKind noteKind)
                      { fNoteKind = noteKind; }

    msrNoteKind   getNoteKind () const
                      { return fNoteKind; }

    int           getNoteMusicXMLDivisions () const
                      {
                        return
                          fMusicXMLNoteData.fMusicXMLDivisions;
                      }

    void          setNoteMusicXMLDisplayDivisions (int divisions)
                      {
                        fMusicXMLNoteData.fMusicXMLDisplayDivisions =
                          divisions;
                      }

    int           getNoteMusicXMLDisplayDivisions () const
                      {
                        return
                          fMusicXMLNoteData.fMusicXMLDisplayDivisions;
                      }

    int           getNoteMusicXMLDotsNumber () const
                      {
                        return
                          fMusicXMLNoteData.fMusicXMLDotsNumber;
                      }

    int           getNoteMusicXMLOctave () const
                      {
                        return
                          fMusicXMLNoteData.fMusicXMLOctave;
                      }

    msrMusicXMLNoteData::msrDiatonicPitch
                  getDiatonicPitch () const
                      {
                        return
                          fMusicXMLNoteData.fDiatonicPitch;
                      }

    // octave shifts
    void          setOctaveShift (S_msrOctaveShift stem)
                      { fNoteOctaveShift = stem; }

    S_msrOctaveShift
                  getNoteOctaveShift () const
                      { return fNoteOctaveShift; }

    // stems
    void          setStem (S_msrStem stem)
                      { fNoteStem = stem; }

    S_msrStem     getNoteStem () const
                      { return fNoteStem; }

    // divisions per whole note
    void          setDivisionsPerWholeNote (int divisionsPerWholeNote)
                      {
                        fDivisionsPerWholeNote =
                          divisionsPerWholeNote;
                      }
                      
    const int     getDivisionsPerWholeNote () const
                      { return fDivisionsPerWholeNote; }
          
    // articulations
    const list<S_msrArticulation>&
                  getNoteArticulations () const
                      { return fNoteArticulations; }
                      
    list<S_msrArticulation>&
                  getNoteArticulationsToModify ()
                      { return fNoteArticulations; }
                      
    // dynamics
    const list<S_msrDynamics>&
                  getNoteDynamics () const
                      { return fNoteDynamics; };

    list<S_msrDynamics>&
                  getNoteDynamicsToModify ()
                      { return fNoteDynamics; };

    // words
    const list<S_msrWords>&
                  getNoteWords () const
                      { return fNoteWords; };
                      
    list<S_msrWords>&
                  getNoteWordsToModify ()
                      { return fNoteWords; };
                      
    // wedges
    const list<S_msrWedge>&
                  getNoteWedges () const
                      { return fNoteWedges; };

    list<S_msrWedge>&
                  getNoteWedgesToModify ()
                      { return fNoteWedges; };

    // chord members
    void          setNoteBelongsToAChord ();
/*
    void          setNoteIsChordFirstNote (bool value)
                      { fNoteIsChordFirstNote = value; }
*/
    bool          getNoteBelongsToAChord () const
                      {
                        return
                          fMusicXMLNoteData.fMusicXMLNoteBelongsToAChord;
                      }
/*
    bool          getNoteIsChordFirstNote () const
                      { return fNoteIsChordFirstNote; }
*/
    // ties
    msrMusicXMLNoteData::msrMusicXMLTieKind
                  getNoteTieKind () const
                      {
                        return
                          fMusicXMLNoteData.fMusicXMLTieKind;
                      }

    // slurs
    msrSlur::msrSlurKind
                  getNoteSlurKind () const { return fNoteSlurKind; }

    // beams
    void          setBeam (S_msrBeam beam)  { fNoteBeam = beam; }
    S_msrBeam     getBeam () const          { return fNoteBeam; }

    // location in measure
    void          setNoteMeasureLocation (
                    const msrMeasureLocation& location)
                      { fNoteMeasureLocation = location; }
                      
    const msrMeasureLocation&
                  getNoteMeasureLocation () const
                      { return fNoteMeasureLocation; }

    // services
    // ------------------------------------------------------

    // tuplet members
    void          applyTupletMemberDisplayFactor (
                    int actualNotes, int normalNotes);
                    
    string        notePitchAsString () const;

    string        noteAsString () const;
    
    string        noteDiatonicPitchAsString () const;

    string        noteDivisionsAsMSRString () const;
    string        noteTypeAsMSRString () const;

    // articulations
    void          addArticulationToNote (S_msrArticulation art);
    
    // dynamics
    void          addDynamicsToNote (S_msrDynamics dynamics);

    // words
    void          addWordsToNote (S_msrWords words);
    
    //  wedges
    void          addWedgeToNote (S_msrWedge wedge);

    S_msrDynamics removeFirstDynamics (); // ???
    S_msrWedge    removeFirstWedge (); // JMI

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  private:

    // MusicXML informations
    
    msrMusicXMLNoteData       fMusicXMLNoteData;

    // LilyPond informations

    msrNoteKind               fNoteKind;

    S_msrOctaveShift          fNoteOctaveShift;
    
    S_msrStem                 fNoteStem;

    S_msrBeam                 fNoteBeam;
                                      
    list<S_msrArticulation>   fNoteArticulations;
        
    list<S_msrDynamics>       fNoteDynamics;
    list<S_msrWords>          fNoteWords;
    list<S_msrWedge>          fNoteWedges;


    msrSlur::msrSlurKind      fNoteSlurKind;

    int                       fDivisionsPerWholeNote;
    
    msrMeasureLocation        fNoteMeasureLocation;

 // JMI   bool                      fNoteIsChordFirstNote;
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
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           chordDivisions);

    // creation from MusicXML
    // ------------------------------------------------------

    SMARTP<msrChord> createEmptyChordClone ();

  protected:

    msrChord (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           chordDivisions);
      
    virtual ~msrChord();
  
  public:

    // set and get
    // ------------------------------------------------------

    const vector<S_msrNote>&
                  getChordNotes () const
                      { return fChordNotes; }

    void          setChordDivisions (int divisions)
                      { fChordDivisions = divisions; }
            
    int           getChordDivisions () const
                      { return fChordDivisions; }
            
    // divisions per whole note
    void          setDivisionsPerWholeNote (int divisionsPerWholeNote)
                      {
                        fDivisionsPerWholeNote =
                          divisionsPerWholeNote;
                      }
                      
    const int     getDivisionsPerWholeNote () const
                      { return fDivisionsPerWholeNote; }
          
    // location in measure
    void          setChordMeasureLocation (
                    const msrMeasureLocation& location)
                      { fChordMeasureLocation = location; }
                      
    const msrMeasureLocation&
                  getChordMeasureLocation () const
                      { return fChordMeasureLocation; }

    // ties
    void          setChordTieKind (
                    const
                      msrMusicXMLNoteData::msrMusicXMLTieKind kind)
                      { fChordTieKind = kind; }

    msrMusicXMLNoteData::msrMusicXMLTieKind
                  getChordTieKind () const
                      { return fChordTieKind; }

    const list<S_msrArticulation>&
                  getChordArticulations () const
                      { return fChordArticulations; }

    // services
    // ------------------------------------------------------

    void          addNoteToChord (S_msrNote note);

    void          addArticulationToChord (S_msrArticulation art);
     
    void          addDynamicsToChord (S_msrDynamics dyn)
                      { fChordDynamics.push_back (dyn); }
                    
    void          addWordsToChord (S_msrWords dyn)
                      { fChordWords.push_back (dyn); }
                    
    void          addWedgeToChord (S_msrWedge wdg)
                      { fChordWedges.push_back (wdg); }

    string        chordDivisionsAsMSRString () const;

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  private:
  
    vector<S_msrNote>         fChordNotes;

    int                       fDivisionsPerWholeNote;

    msrMeasureLocation        fChordMeasureLocation;
    
    int                       fChordDivisions;
                              

    list<S_msrArticulation>   fChordArticulations;
    
    list<S_msrDynamics>       fChordDynamics;
    
    list<S_msrWords>          fChordWords;
    
    list<S_msrWedge>          fChordWedges;

    msrMusicXMLNoteData::msrMusicXMLTieKind
                              fChordTieKind;
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
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      string        variableName,
      string        value);
    
  protected:

    msrVarValAssoc (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      string        variableName,
      string        value);
      
    virtual ~msrVarValAssoc();
  
  public:

    // set and get
    // ------------------------------------------------------

    void      changeAssocValue (string value)
                  { fVariableValue = value; }

    string    getVariableName  () const
                  { return fVariableName; };
    string    getVariableValue () const
                  { return fVariableValue; };

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

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
      S_msrOptions&  msrOpts, 
      int            inputLineNumber);
    
  protected:

    msrIdentification (
      S_msrOptions&  msrOpts, 
      int            inputLineNumber);
      
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

    void        addCreator (
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
                getCreators () const
                    { return fCreators; };
    
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

    virtual void print (ostream& os);

  private:

    S_msrVarValAssoc         fWorkNumber;
    S_msrVarValAssoc         fWorkTitle;
    S_msrVarValAssoc         fMovementNumber;
    S_msrVarValAssoc         fMovementTitle;
    vector<S_msrVarValAssoc> fCreators;
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
      S_msrOptions& msrOpts, 
      int           inputLineNumber);
    
  protected:

    msrPageGeometry (
      S_msrOptions& msrOpts, 
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
\brief A msr layout representation.

  A layout is represented by variable/value pairs
*/
//______________________________________________________________________________
class EXP msrLayout : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrLayout> create (
      S_msrOptions& msrOpts, 
      int           inputLineNumber);
    
  protected:

    msrLayout (
      S_msrOptions& msrOpts, 
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
    
    enum msrGapKind { kGapAfterwards, kNoGapAfterwards };

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrComment> create (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      string        contents,
      msrGapKind    gapKind = kNoGapAfterwards);

  protected:

    msrComment (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      string        contents,
      msrGapKind    gapKind = kNoGapAfterwards);
      
    virtual ~msrComment();
  
  public:

    // set and get
    // ------------------------------------------------------

    string      getContents () const { return fContents; }
    msrGapKind  getGapKind  () const { return fGapKind; }
    
    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  private:

    string     fContents;
    msrGapKind fGapKind;
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
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           nextBarNumber);

  protected:

    msrBreak (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           nextBarNumber);
      
    virtual ~msrBreak();
  
  public:

    // set and get
    // ------------------------------------------------------

    int getNextBarNumber () const
            { return fNextBarNumber; }

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

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
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           nextBarNumber);

  protected:

    msrBarCheck (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           nextBarNumber);
      
    virtual ~msrBarCheck();
  
  public:

    // set and get
    // ------------------------------------------------------

    int getNextBarNumber () const
            { return fNextBarNumber; }

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

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
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           nextBarNumber);

  protected:

    msrBarnumberCheck (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           nextBarNumber);
      
    virtual ~msrBarnumberCheck();
  
  public:

    // set and get
    // ------------------------------------------------------

    int getNextBarNumber () const
            { return fNextBarNumber; }

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

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
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTuplet> create (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           number,
      int           actualNotes,
      int           normalNotes,
      S_msrNote     firstNote);

    SMARTP<msrTuplet> createEmptyClone ();

    enum msrTupletKind {
      kStartTuplet, kContinueTuplet, kStopTuplet, 
      k_NoTuplet };

  protected:

    msrTuplet (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           number,
      int           actualNotes,
      int           normalNotes,
      S_msrNote     firstNote);
      
    virtual ~msrTuplet();
  
  public:

    // set and get
    // ------------------------------------------------------
    
    int           getTupletNumber () const { return fTupletNumber; }

    int           getActualNotes () const { return fActualNotes; }
    int           getNormalNotes () const { return fNormalNotes; }
    
    const vector<S_msrElement>&
                  getTupletElements () const
                      { return fTupletElements; }

    int           getTupletDivisions () const
                      { return fTupletDivisions; }
            
    // divisions per whole note
    void        setDivisionsPerWholeNote (int divisionsPerWholeNote)
                    {
                      fDivisionsPerWholeNote =
                        divisionsPerWholeNote;
                    }
                      
    const int   getDivisionsPerWholeNote () const
                    { return fDivisionsPerWholeNote; }
          
    // location in measure
    void          setTupletMeasureLocation (
                    const msrMeasureLocation& location)
                      { fTupletMeasureLocation = location; }
                      
    const msrMeasureLocation&
                  getTupletMeasureLocation () const
                      { return fTupletMeasureLocation; }

    // services
    // ------------------------------------------------------

    void  addElementToTuplet (S_msrElement elem)
             { fTupletElements.push_back(elem); }

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  private:

    int                  fTupletNumber;
    
    int                  fActualNotes;
    int                  fNormalNotes;

    int                  fTupletDivisions;
    int                  fTupletDisplayDivisions;

    int                  fDivisionsPerWholeNote;

    msrMeasureLocation   fTupletMeasureLocation;
    
    vector<S_msrElement> fTupletElements;
};
typedef SMARTP<msrTuplet> S_msrTuplet;
EXP ostream& operator<< (ostream& os, const S_msrTuplet& elt);

/*!
\brief A msr clef representation.

  A clef is represented by its name
*/
//______________________________________________________________________________
class EXP msrClef : public msrElement
{
  public:
    
    enum msrClefLineKind {
      kStandardLine,
      kTrebleStdLine=2, kBassStdLine=4, kCStdLine=3, kTabStdLine=5 };

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrClef> create (
      S_msrOptions& msrOpts, 
      int          inputLineNumber,
      string       sign,
      int          line,
      int          octaveChange);

  protected:

    msrClef (
      S_msrOptions& msrOpts, 
      int          inputLineNumber,
      string       sign,
      int          line,
      int          octaveChange);
      
    virtual ~msrClef();
  
  public:

    // set and get
    // ------------------------------------------------------

    string  getSign () const
                { return fSign; }
    int     getLine () const
                { return fLine; }
    int     getOctaveChange () const
                { return fOctaveChange; }
                
    // services
    // ------------------------------------------------------

    string  clefAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  private:

    string fSign;
    int    fLine;
    int    fOctaveChange;
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
    
    enum msrKeyMode { kMajor, kMinor };

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrKey> create (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           fifths,
      string        mode,
      int           cancel);

  protected:

    msrKey (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           fifths,
      string        mode,
      int           cancel);
      
    virtual ~msrKey();

  public:

    // set and get
    // ------------------------------------------------------

    int         getFifths () const
                    { return fFifths; }
    string      getMode () const
                    { return fMode; }
    int         getCancel () const
                    { return fCancel; }

    string      getTonic () const
                    { return fTonic; }
    msrKeyMode  getKeyMode () const
                    { return fKeyMode; }

    const msrKeyMode&
                getMeasureLocation () const
                    { return fMeasureLocation; }

    // services
    // ------------------------------------------------------

    string      keyAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);
  
  private:

    int                 fFifths;
    string              fMode;
    int                 fCancel;

    string              fTonic;
    msrKeyMode          fKeyMode;
                        
    msrMeasureLocation  fMeasureLocation;
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
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           beatsNumber,
      int           beatsValue);

  protected:

    msrTime (
      S_msrOptions& msrOpts, 
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

    string    timeAsString () const;

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  private:

    int      fBeatsNumber;
    int      fBeatsValue;

    bool     fGenerateNumericalTime; // JMI
};
typedef SMARTP<msrTime> S_msrTime;
EXP ostream& operator<< (ostream& os, const S_msrTime& elt);

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
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           transposeDiatonic,
      int           transposeChromatic);

  protected:

    msrTranspose (
      S_msrOptions& msrOpts, 
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

    virtual void print (ostream& os);

  private:

    int      fTransposeDiatonic;
    int      fTransposeChromatic;
};
typedef SMARTP<msrTranspose> S_msrTranspose;
EXP ostream& operator<< (ostream& os, const S_msrTranspose& elt);

/*!
\brief A tempo representation.

  A tempo is represented by the lyrics to use
*/
//______________________________________________________________________________
class EXP msrTempo : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTempo> create (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           tempoUnit,
      int           perMinute);

  protected:

    msrTempo (
      S_msrOptions& msrOpts, 
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

    virtual void print (ostream& os);

  private:

    string fTempoIndication;
    
    int    fTempoUnit; // 0 to indicate there's only an indication
    int    fPerMinute;
};
typedef SMARTP<msrTempo> S_msrTempo;
EXP ostream& operator<< (ostream& os, const S_msrTempo& elt);

/*!
\brief A msr lyrics chunk representation.

  A lyrics chunk is represented by a type and and a string.
  In the case of "single", the list contains only one string
*/
//______________________________________________________________________________
class EXP msrLyricschunk : public msrElement
{
  public:

    // we want to end the line in the LilyPond code at a break
    enum msrLyricschunkKind {
      k_NoChunk,
      kSingleChunk, kBeginChunk, kMiddleChunk, kEndChunk,
      kSkipChunk,
      kTiedChunk,
      kSlurChunk, kSlurBeyondEndChunk,
      kBarcheckChunk, kBreakChunk};

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrLyricschunk> create (
      S_msrOptions&      msrOpts, 
      int                inputLineNumber,
      msrLyricschunkKind lyricschunkKind,
      string             chunkText,
      int                divisions,
      S_msrNote          lyricschunkNote,
      S_msrLyrics        lyricschunkLyricsUplink);

    SMARTP<msrLyricschunk> createEmptyClone ();

  protected:

    msrLyricschunk (
      S_msrOptions&      msrOpts, 
      int                inputLineNumber,
      msrLyricschunkKind lyricschunkKind,
      string             chunkText,
      int                divisions,
      S_msrNote          lyricschunkNote,
      S_msrLyrics        lyricschunkLyricsUplink);
        
    virtual ~msrLyricschunk();

  public:

    // set and get
    // ------------------------------------------------------

    msrLyricschunkKind
              getLyricschunkKind () const
                  { return fLyricschunkKind; }

    S_msrNote getLyricschunkNote () const
                  { return fLyricschunkNote; }

    string    getChunkText () const
                  { return fChunkText; }

    int       getChunkDivisions () const
                  { return fChunkDivisions; }

    // services
    // ------------------------------------------------------

    string    lyricschunkKindAsString ();

    string    lyricschunkAsString ();

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  private:
  
    msrLyricschunkKind fLyricschunkKind;
    string             fChunkText;
    int                fChunkDivisions;
    
    S_msrNote          fLyricschunkNote;
    S_msrLyrics        fLyricschunkLyricsUplink;
};
typedef SMARTP<msrLyricschunk> S_msrLyricschunk;
EXP ostream& operator<< (ostream& os, const S_msrLyricschunk& elt);

/*!
\brief A msr lyrics representation.

  A lyrics is represented by a list of lyrics chunks,
*/
//______________________________________________________________________________
class EXP msrLyrics : public msrElement
{
  public:

    enum msrLyricsMasterStatus {
       kMasterLyrics, kRegularLyrics };

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrLyrics> create (
      S_msrOptions&         msrOpts, 
      int                   inputLineNumber,
      int                   lyricsNumber,
      msrLyricsMasterStatus lyricsMasterStatus,
      S_msrVoice            lyricsVoiceUplink);
    
    SMARTP<msrLyrics> createEmptyClone (
      S_msrVoice clonedVoice);

  protected:

    msrLyrics (
      S_msrOptions&         msrOpts, 
      int                   inputLineNumber,
      int                   lyricsNumber,
      msrLyricsMasterStatus lyricsMasterStatus,
      S_msrVoice            lyricsVoiceUplink);

    virtual ~msrLyrics();
  
  public:

    // set and get
    // ------------------------------------------------------

    int     getLyricsNumber () const
                { return fLyricsNumber; }
                
    string  getLyricsName () const;
                
    S_msrVoice
            getLyricsVoiceUplink () const
                { return fLyricsVoiceUplink; }
                
    msrLyricsMasterStatus
            getLyricsMasterStatus () const
                { return fLyricsMasterStatus; }
                
    const vector<S_msrLyricschunk>&
            getLyricschunks () const
                { return fLyricschunks; }

    void    setLyricsTextPresent ()
                { fLyricsTextPresent = true; }

    bool    getLyricsTextPresent () const
                { return fLyricsTextPresent; }


    // services
    // ------------------------------------------------------

    void    addTextChunkToLyrics (
              int       inputLineNumber,
              string    syllabic,
              msrLyricschunk::msrLyricschunkKind
                        lyricschunkKind,
              string    text,
              bool      elision,
              int       divisions,
              S_msrNote note);
      
    void    addSkipChunkToLyrics (
              int       inputLineNumber,
              int       divisions,
              S_msrNote note);

    void    addTiedChunkToLyrics (
              int       inputLineNumber,
              int       divisions,
              S_msrNote note);

    void    addSlurChunkToLyrics (
              int       inputLineNumber,
              int       divisions,
              S_msrNote note);

    void    addSlurBeyondEndChunkToLyrics (
              int       inputLineNumber,
              int       divisions,
              S_msrNote note);

    void    addBarcheckChunkToLyrics (
              int inputLineNumber,
              int nextMeasureNumber);

    void    addBreakChunkToLyrics (
              int inputLineNumber,
              int nextMeasureNumber);

    void    addChunkToLyrics (S_msrLyricschunk chunk);
                
    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  private:

    int                       fLyricsNumber;
    msrLyricsMasterStatus     fLyricsMasterStatus;

    vector<S_msrLyricschunk>  fLyricschunks;

    bool                      fLyricsTextPresent;

    S_msrVoice                fLyricsVoiceUplink;

};
typedef SMARTP<msrLyrics> S_msrLyrics;
EXP ostream& operator<< (ostream& os, const S_msrLyrics& elt);

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

    enum msrBarlineLocation {
      k_NoLocation,
      kLeft, kMiddle, kRight}; // kRight by default
        
    enum msrBarlineStyle {
      k_NoStyle,
      kRegular,  // by default
      kDotted, kDashed, kHeavy,
      kLightLight, kLightHeavy, kHeavyLight, kHeavyHeavy,
      kTick, kShort};

    enum msrBarlineEndingType {
      k_NoEndingType,
      kStart, kStop, kDiscontinue};

    enum msrBarlineRepeatDirection {
      k_NoRepeatDirection,
      kForward, kBackward};

    enum msrBarlineRepeatWinged {
      k_NoRepeatWinged,
      kStraight, kCurved, kDoubleStraight, kDoubleCurved };

    enum msrBarlineCategory {
      kStandaloneBar,
      kRepeatStart, kRepeatEnd,
      kEndingStart, kHookedEndingEnd, kHooklessEndingEnd};
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrBarline> create (
      S_msrOptions&             msrOpts, 
      int                       inputLineNumber,
      msrBarlineLocation        location,
      msrBarlineStyle           style,
      msrBarlineEndingType      endingType,
      string                    endingMusicXMLNumber,
      msrBarlineRepeatDirection repeatDirection,
      msrBarlineRepeatWinged    repeatWinged);

  protected:

    msrBarline (
      S_msrOptions&             msrOpts, 
      int                       inputLineNumber,
      msrBarlineLocation        location,
      msrBarlineStyle           style,
      msrBarlineEndingType      endingType,
      string                    endingMusicXMLNumber,
      msrBarlineRepeatDirection repeatDirection,
      msrBarlineRepeatWinged    repeatWinged);
      
    virtual ~msrBarline();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrBarlineLocation
                getLocation () const
                    { return fLocation; }
                    
    msrBarlineStyle
                getStyle () const
                    { return fStyle; }
                    
    msrBarlineEndingType
                getEndingType () const
                    { return fEndingType; }
                    
    string
                getEndingNumber () const
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

    void        setBarlineCategory (
                  msrBarlineCategory barlineCategory)
                    { fBarlineCategory = barlineCategory; }
    
    // position in measure
    void          setBarlineMeasureLocation (
                    const msrMeasureLocation& location)
                      { fBarlineMeasureLocation = location; }
                      
    const msrMeasureLocation&
                  getBarlineMeasureLocation () const
                      { return fBarlineMeasureLocation; }

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  private:

    msrBarlineLocation          fLocation;
    msrBarlineStyle             fStyle;
    msrBarlineEndingType        fEndingType;
    string                      fEndingNumber; // may be "1, 2"
    msrBarlineRepeatDirection   fRepeatDirection;
    msrBarlineRepeatWinged      fRepeatWinged;

    msrBarlineCategory          fBarlineCategory;

    msrMeasureLocation          fBarlineMeasureLocation;

    // the numbers extracted from fEndingNumber
    list<int>                   fEndingNumbersList;
};
typedef SMARTP<msrBarline> S_msrBarline;
EXP ostream& operator<< (ostream& os, const S_msrBarline& elt);

/*!
\brief A msr repeat representation.

  A repeat is represented by:
    - a sequence of elements for the common part
    - a vector of sequences of elements for the alternate endings
*/
//______________________________________________________________________________
class EXP msrRepeatending : public msrElement
{
  public:

    enum msrRepeatendingKind {
      kHookedEnding,
      kHooklessEnding};

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeatending> create (
      S_msrOptions&       msrOpts, 
      int                 inputLineNumber,
      string              repeatendingMusicXMLNumber, // may be "1, 2"
      msrRepeatendingKind repeatendingKind,
      S_msrVoicechunk     voicechunk,
      S_msrRepeat         repeatUplink);
    
    SMARTP<msrRepeatending> createEmptyClone (
      S_msrRepeat clonedRepeat);

  protected:

    msrRepeatending (
      S_msrOptions&       msrOpts, 
      int                 inputLineNumber,
      string              repeatendingMusicXMLNumber, // may be "1, 2"
      msrRepeatendingKind repeatendingKind,
      S_msrVoicechunk     voicechunk,
      S_msrRepeat         repeatUplink);
      
    virtual ~msrRepeatending();
  
  public:

    // set and get
    // ------------------------------------------------------

    string    getRepeatendingMusicXMLNumber () const
                  { return fRepeatendingMusicXMLNumber; }
                
    void      setRepeatendingNumber (int repeatendingNumber)
                  { fRepeatendingNumber = repeatendingNumber; }
                
    int       getRepeatendingNumber () const
                  { return fRepeatendingNumber; }
                
    S_msrVoicechunk
              getRepeatendingVoicechunk () const
                  { return fRepeatendingVoicechunk; }
                
    S_msrRepeat
              getRepeatendingRepeatUplink () const
                { return fRepeatendingRepeatUplink; }

    // services
    // ------------------------------------------------------

    void      appendElementToVoicechunk  (S_msrElement elem)
                  {
                    fRepeatendingVoicechunk->
                      appendElementToVoicechunk (elem);
                  }
                    
    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  private:
  
    string              fRepeatendingMusicXMLNumber; // may be "1, 2"
    int                 fRepeatendingNumber; // internally assigned
    
    msrRepeatendingKind fRepeatendingKind;
    
    S_msrVoicechunk     fRepeatendingVoicechunk;

    S_msrRepeat         fRepeatendingRepeatUplink;
};
typedef SMARTP<msrRepeatending> S_msrRepeatending;
EXP ostream& operator<< (ostream& os, const S_msrRepeatending& elt);

/*!
\brief A msr repeat representation.

  A repeat is represented by:
    - a sequence of elements for the common part
    - a vector of sequences of elements for the alternate endings
*/
//______________________________________________________________________________
class EXP msrRepeat : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeat> create (
      S_msrOptions&   msrOpts, 
      int             inputLineNumber,
      S_msrVoicechunk commonPart,
      S_msrVoice      voiceUplink);
    
    SMARTP<msrRepeat> createEmptyClone (
      S_msrVoice clonedVoice);

  protected:

    msrRepeat (
      S_msrOptions&   msrOpts, 
      int             inputLineNumber,
      S_msrVoicechunk commonPart,
      S_msrVoice      voiceUplink);
      
    virtual ~msrRepeat();
  
  public:

    // set and get
    // ------------------------------------------------------

    void      setRepeatCommonPart (
                S_msrVoicechunk repeatCommonPart);
                  
    S_msrVoicechunk
              getRepeatCommonPart () const
                { return fRepeatCommonPart; }

    const vector<S_msrRepeatending>&
              getRepeatEndings () const
                { return fRepeatEndings; }

    S_msrVoice
              getRepeatVoiceUplink () const
                { return fRepeatVoiceUplink; }

    // services
    // ------------------------------------------------------

    void      addRepeatending (S_msrRepeatending repeatending);

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  private:

    S_msrVoicechunk           fRepeatCommonPart;
    
    vector<S_msrRepeatending> fRepeatEndings;
    int                       fRepeatEndingsCounter;
    
    S_msrVoice                fRepeatVoiceUplink;
};
typedef SMARTP<msrRepeat> S_msrRepeat;
EXP ostream& operator<< (ostream& os, const S_msrRepeat& elt);

/*!
\brief A msr repeat representation.

  A repeat is represented by:
    - a sequence of elements for the common part
    - a vector of sequences of elements for the alternate endings
*/
//______________________________________________________________________________
class EXP msrUpbeat : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrUpbeat> create (
      S_msrOptions&   msrOpts, 
      int             inputLineNumber,
      int             divisions,
      S_msrVoice      voiceUplink);
    
    SMARTP<msrUpbeat> createEmptyClone (
      S_msrVoice clonedVoice);

  protected:

    msrUpbeat (
      S_msrOptions&   msrOpts, 
      int             inputLineNumber,
      int             divisions,
      S_msrVoice      voiceUplink);
      
    virtual ~msrUpbeat();
  
  public:

    // set and get
    // ------------------------------------------------------

//    void          setUpbeatDivisions (int divisions)
//                      { fUpbeatDivisions = divisions; }

    int           getUpbeatDivisions () const
                      { return fUpbeatDivisions; }

    string        getUpbeatDivisionsAsString () const;

    S_msrVoice    getUpbeatVoiceUplink () const
                      { return fUpbeatVoiceUplink; }

    // services
    // ------------------------------------------------------

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  private:

    int                       fUpbeatDivisions;
    
    S_msrVoice                fUpbeatVoiceUplink;
};
typedef SMARTP<msrUpbeat> S_msrUpbeat;
EXP ostream& operator<< (ostream& os, const S_msrUpbeat& elt);

/*!
\brief A msr voice representation.

  A vpoce is represented by a its string contents
*/
//______________________________________________________________________________
class EXP msrVoice : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrVoice> create (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           voiceNumber,
      int           staffRelativeVoiceNumber,
      S_msrStaff    voiceStaffUplink);
                          
    static SMARTP<msrVoice> createRest (
      S_msrOptions& msrOpts,
      int           inputLineNumber,
      int           divisions,
      int           voiceNumber);
    
    SMARTP<msrVoice> createEmptyClone (
      S_msrStaff clonedStaff);

  protected:

    msrVoice (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           voiceNumber,
      int           staffRelativeVoiceNumber,
      S_msrStaff    voiceStaffUplink);
      
    virtual ~msrVoice();
  
  public:

    // set and get
    // ------------------------------------------------------

    int           getVoiceNumber () const
                      { return fVoiceNumber; }
                
    S_msrStaff    getVoiceStaffUplink () const
                      { return fVoiceStaffUplink; }
                
    const map<int, S_msrLyrics>&
                  getVoiceLyricsMap () const
                      { return fVoiceLyricsMap; }

    string        getVoiceName () const;

    void          setVoiceAnacrusis (
                    const S_msrUpbeat& upbeat)
                      { fVoiceAnacrusis = upbeat; }

    S_msrUpbeat   getVoiceAnacrusis () const
                      { return fVoiceAnacrusis; }

    void          setNewVoicechunkForVoice (int inputLineNumber);

    S_msrVoicechunk
                  getVoicechunk () const
                      { return fVoicechunk; }

    S_msrLyrics   getVoiceMasterLyrics () const
                      { return fVoiceMasterLyrics; }
               
    // divisions per whole note
    void          setDivisionsPerWholeNote (int divisionsPerWholeNote)
                      {
                        fDivisionsPerWholeNote =
                          divisionsPerWholeNote;
                      }
                      
    const int     getDivisionsPerWholeNote () const
                      { return fDivisionsPerWholeNote; }
          
    // location in measure
    void          setPositionInMeasure (int positionInMeasure)
                      {
                        fVoiceMeasureLocation.fPositionInMeasure =
                          positionInMeasure;
                      }
                        
    void          incrementPositionInMeasure (int increment)
                      {
                        fVoiceMeasureLocation.fPositionInMeasure +=
                          increment;
                      }

    const int     getVoicePositionInMeasure () const
                      {
                        return fVoiceMeasureLocation.fPositionInMeasure;
                      }
          
    void          setMeasureNumber (
                    int inputLineNumber, int measureNumber);

    const int     getMeasureNumber () const
                      {
                        return fVoiceMeasureLocation.fMeasureNumber;
                      }
          
    // location in measure
    void          setVoiceMeasureLocation (
                    const msrMeasureLocation& location)
                      { fVoiceMeasureLocation = location; }
                      
    const msrMeasureLocation&
                  getVoiceMeasureLocation () const
                      { return fVoiceMeasureLocation; }

    // services
    // ------------------------------------------------------

    void          handleForward       (int duration); // JMI
    
    void          appendClefToVoice   (S_msrClef clef);
    void          appendKeyToVoice    (S_msrKey  key);
    void          appendTimeToVoice   (S_msrTime time);
    
    void          appendTransposeToVoice (S_msrTranspose transpose);

// JMI    void          appendWordsToVoice  (S_msrWords words);
    
    void          appendTempoToVoice  (S_msrTempo tempo);
    
    void          appendRehearsalToVoice (S_msrRehearsal rehearsal);
    
    void          appendOctaveShiftToVoice (S_msrOctaveShift octaveShift);
    
    void          appendNoteToVoice   (S_msrNote note);
    void          appendChordToVoice  (S_msrChord chord);
    void          appendTupletToVoice (S_msrTuplet tuplet);
    
    void          appendGraceexpressionToVoice
                                      (S_msrGraceexpression
                                        graceexpression);
    
    void          addTextLyricschunkToVoice (
                    int       lyricsNumber,
                    string    syllabic,
                    msrLyricschunk::msrLyricschunkKind
                              lyricschunkKind,
                    string    text,
                    bool      elision,
                    int       divisions,
                    S_msrNote newNote);
    
    void          addSkipLyricschunkToVoice (
                    int       lyricsNumber,
                    int       divisions,
                    S_msrNote newNote);
    
    void          addTiedLyricschunkToVoice (
                    int       lyricsNumber,
                    int       divisions,
                    S_msrNote newNote);
    
    void          addSlurLyricschunkToVoice (
                    int       lyricsNumber,
                    int       divisions,
                    S_msrNote newNote);
    
    void          addSlurBeyondEndLyricschunkToVoice (
                    int       lyricsNumber,
                    int       divisions,
                    S_msrNote newNote);
    
    void          appendBarCheckToVoice (S_msrBarCheck bnc);
    void          appendBarnumberCheckToVoice
                                        (S_msrBarnumberCheck bnc);

    void          appendBreakToVoice    (S_msrBreak break_);

    void          appendRepeatToVoice   (S_msrRepeat repeat);
    
    void          prependBarlineToVoice (S_msrBarline barline);
    void          appendBarlineToVoice  (S_msrBarline barline);
    
    void          removeLastElementFromVoice ();

    S_msrLyrics
                  addLyricsToVoice (
                    int inputLineNumber,
                    int lyricsNumber);

    void          addLyricsToVoice (S_msrLyrics lyrics);
    
    S_msrLyrics   fetchLyricsFromVoice (int lyricsNumber);
    
    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  private:

    int                       fVoiceNumber;
    int                       fStaffRelativeVoiceNumber;
    S_msrStaff                fVoiceStaffUplink;

    bool                      fVoiceContainsActualNotes;

    int                       fDivisionsPerWholeNote;

    msrMeasureLocation        fVoiceMeasureLocation;

    // anacrusis detection
    bool                      fMeasureZeroHasBeenMetInVoice;
    bool                      fMeasureNumberHasBeenSetInVoice;
    bool                      fMusicHasBeenInsertedInVoice;
    S_msrUpbeat               fVoiceAnacrusis;
    
    // the chunk in the voice contain the music elements
    // it is created implicitly for every voice,
    S_msrVoicechunk           fVoicechunk;

    // the master lyrics, collecting skips along the way,
    // to be used as a 'prelude' by actual lyrics
    // that start at later points
    S_msrLyrics               fVoiceMasterLyrics;
    
    // the lyrics map
    map<int, S_msrLyrics>     fVoiceLyricsMap;
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
class EXP msrStaff : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStaff> create (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           staffNumber,
      S_msrPart     fStaffPartUplink);
    
    SMARTP<msrStaff> createEmptyClone (
      S_msrPart clonedPart);

  protected:

    msrStaff (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           staffNumber,
      S_msrPart     fStaffPartUplink);
      
    virtual ~msrStaff();
  
  public:

    // set and get
    // ------------------------------------------------------

    int         getStaffNumber () const
                    { return fStaffNumber; }
                
    S_msrPart   getStaffPartUplink () const
                    { return fStaffPartUplink; }

    string      getStaffName () const;

    string      getStaffInstrumentName () const
                    { return fStaffInstrumentName; }

    S_msrClef   getStaffClef () const { return fStaffClef; };
    S_msrKey    getStaffKey  () const { return fStaffKey; };
    S_msrTime   getStaffTime () const { return fStaffTime; };
    
    S_msrTranspose
                getStaffTranspose () const { return fStaffTranspose; };
    
    void        setStaffClef (S_msrClef clef);
    void        setStaffKey  (S_msrKey  key);
    void        setStaffTime (S_msrTime time);
    
    void        setStaffTranspose (S_msrTranspose transpose);

    const map<int, S_msrVoice>&
                getStaffVoicesMap ()
                    { return fStaffVoicesMap; }

    // divisions per whole note
    void        setDivisionsPerWholeNote (
                  int divisionsPerWholeNote)
                    {
                      fDivisionsPerWholeNote =
                        divisionsPerWholeNote;

                      setAllStaffVoicesDivisionsPerWholeNote (
                        divisionsPerWholeNote);
                    }
                      
    const int   getDivisionsPerWholeNote () const
                    { return fDivisionsPerWholeNote; }
          
    // services
    // ------------------------------------------------------

    void        setAllStaffVoicesDivisionsPerWholeNote (
                  int divisions);

    S_msrVoice  addVoiceToStaff (
                  int inputLineNumber,
                  int voiceNumber);

    void        addVoiceToStaff (S_msrVoice voice);
    
    S_msrVoice  fetchVoiceFromStaff (int voiceNumber);
                              
    void        appendClefToAllStaffVoices (S_msrClef clef);
    void        appendKeyToAllStaffVoices  (S_msrKey   key);
    void        appendTimeToAllStaffVoices (S_msrTime time);
    
    void        appendTransposeToAllStaffVoices (S_msrTranspose transpose);
              
    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  private:

    static int              gMaxStaffVoices;

    int                     fStaffNumber;
    S_msrPart               fStaffPartUplink;

    map<int, S_msrVoice>    fStaffVoicesMap;

    string                  fStaffInstrumentName;

    S_msrClef               fStaffClef;
    S_msrKey                fStaffKey;
    S_msrTime               fStaffTime;

    S_msrTranspose          fStaffTranspose;

    int                     fNextRelativeStaffVoiceNumber;

    int                     fDivisionsPerWholeNote;    
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
      S_msrOptions&  msrOpts, 
      int            inputLineNumber,
      string         partMusicXMLID,
      S_msrPartgroup partPartgroupUplink);
                
    SMARTP<msrPart> createEmptyPartClone (
      S_msrPartgroup clonedPartgroup);

  protected:

    msrPart (
      S_msrOptions&  msrOpts, 
      int            inputLineNumber,
      string         partMusicXMLID,
      S_msrPartgroup partPartgroupUplink);
      
    virtual ~msrPart();
  
  public:

    // set and get
    // ------------------------------------------------------

    void          setPartAbbreviation (string partAbbreviation)
                      { fPartAbbreviation = partAbbreviation; }
                
    void          setPartInstrumentName (string partInstrumentName)
                      { fPartInstrumentName = partInstrumentName; }
                              
    void          setPartMusicXMLID (string partMusicXMLID)
                      { fPartMusicXMLID = partMusicXMLID; }
    
    void          setPartMSRName (string partMSRName);
    
    void          setPartName (string partName)
                      { fPartName = partName; }
    
    string        getPartMusicXMLID () const
                      { return fPartMusicXMLID; }

    string        getPartMSRName () const
                      { return fPartMSRName; }

    string        getPartName () const
                      { return fPartName; }

    string        getPartAbbreviation () const
                      { return fPartAbbreviation; }
                
    string        getPartInstrumentName () const
                      { return fPartInstrumentName; }
                
    S_msrPartgroup
                  getPartPartgroupUplink () const
                      { return fPartPartgroupUplink; }
                
    const map<int, S_msrStaff>&
                  getPartStavesMap ()
                      { return fPartStavesMap; }

    string        getPartCombinedName () const;

    // divisions per whole note
    void          setDivisionsPerWholeNote (
                    int divisionsPerWholeNote)
                      {
                        fDivisionsPerWholeNote =
                          divisionsPerWholeNote;

                        setAllPartStavesDivisionsPerWholeNote (
                          divisionsPerWholeNote);
                      }
                      
    const int     getDivisionsPerWholeNote () const
                      { return fDivisionsPerWholeNote; }
          
    S_msrClef     getPartClef () const { return fPartClef; };
    S_msrKey      getPartKey  () const { return fPartKey; };
    S_msrTime     getPartTime () const { return fPartTime; };
    
    S_msrTranspose
                  getPartTranspose () const { return fPartTranspose; };
    
    void          setPartClef (S_msrClef clef);
    void          setPartKey  (S_msrKey  key);
    void          setPartTime (S_msrTime time);

    void          setPartTranspose (S_msrTranspose transpose);
    
     // services
    // ------------------------------------------------------

    void          setAllPartStavesDivisionsPerWholeNote (
                    int divisions);
    
    void          setAllPartStavesClef (S_msrClef clef);
              
    void          setAllPartStavesKey  (S_msrKey  key);
              
    void          setAllPartStavesTime (S_msrTime time);
              
    void          setAllPartStavesTranspose (S_msrTranspose transpose);
              
    S_msrStaff    addStaffToPart (
                    int inputLineNumber,
                    int staffNumber);
    
    void          addStaffToPart (S_msrStaff staff);

    S_msrStaff    fetchStaffFromPart (int staffNumber);

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  private:
    
    string                  fPartMusicXMLID; // native
    string                  fPartMSRName;
                              // may be different than fPartMusicXMLID
                              // if renamed,
                              // coined in constructor

    string                  fPartName; // from '<part-name/>'
    string                  fPartAbbreviation;
    string                  fPartInstrumentName;

    S_msrPartgroup          fPartPartgroupUplink;

    map<int, S_msrStaff>    fPartStavesMap;

    int                     fDivisionsPerWholeNote;

    S_msrClef               fPartClef;
    S_msrKey                fPartKey;
    S_msrTime               fPartTime;

    S_msrTranspose          fPartTranspose;
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

    enum msrPartgroupTypeKind {
        k_NoPartgroupType,
        kStartPartgroupType, kStopPartgroupType};
          
    enum msrPartgroupSymbolKind {
        k_NoPartgroupSymbol,
        kBracePartgroupSymbol, kBracketPartgroupSymbol,
        kLinePartgroupSymbol, kSquarePartgroupSymbol};
          
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrPartgroup> create (
      S_msrOptions&          msrOpts, 
      int                    inputLineNumber,
      int                    partgroupNumber,
      string                 partgroupName,
      string                 partgroupAbbreviation,
      msrPartgroupSymbolKind partgroupSymbolKind,
      int                    partgroupSymbolDefaultX,
      bool                   partgroupBarline,
      S_msrPartgroup         partgroupPartgroupUplink);

    SMARTP<msrPartgroup> createEmptyPartgroupClone (
      S_msrPartgroup clonedPartgroup); // the uplink

  protected:

    msrPartgroup (
      S_msrOptions&          msrOpts, 
      int                    inputLineNumber,
      int                    partgroupNumber,
      string                 partgroupName,
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
    
    string        getPartgroupName () const
                      { return fPartgroupName; }

    string        getPartgroupAbbreviation () const
                      { return fPartgroupAbbreviation; }

    msrPartgroupSymbolKind
                  getPartgroupSymbolKind () const
                      { return fPartgroupSymbolKind; }

    static string pargroupSymbolKindAsString (
                  msrPartgroupSymbolKind partgroupSymbolKind);

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

    S_msrPart addPartToPartgroup (
                int    inputLineNumber,
                string partMusicXMLID);
    
    void      addPartToPartgroup (S_msrPart part);
                
    void      prependSubPartgroupToPartgroup (
                S_msrPartgroup partgroup);

    S_msrPart fetchPartFromPartgroup (string partMusicXMLID);

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  private:

    static int              gPartgroupsCounter;
    
    int                     fPartgroupAbsoluteNumber;
    
    int                     fPartgroupNumber;
        
    string                  fPartgroupName;
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
      S_msrOptions& msrOpts, 
      int           inputLineNumber);

    SMARTP<msrScore> createEmptyClone ();

  protected:

    msrScore (
      S_msrOptions& msrOpts, 
      int           inputLineNumber);
      
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

    virtual void print (ostream& os);

  private:

    S_msrIdentification  fIdentification;

    S_msrPageGeometry    fPageGeometry;
    
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
      S_msrOptions& msrOpts, 
      int                    inputLineNumber);

  protected:

    msrMidi (
      S_msrOptions& msrOpts, 
      int                    inputLineNumber);
      
    virtual ~msrMidi();
  
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

    virtual void print (ostream& os);

  private:
  
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
