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

class msrVoice;
typedef SMARTP<msrVoice> S_msrVoice;

class msrStaff;
typedef SMARTP<msrStaff> S_msrStaff;

class msrPart;
typedef SMARTP<msrPart> S_msrPart;

class msrPartgroup;
typedef SMARTP<msrPartgroup> S_msrPartgroup;

//______________________________________________________________________________
/*!
\internal
\brief A macro to exit when a condition if false
*/
// a macro to abort with code source line information,
// as assert() itself

#define msrAssert( condition, messageIfFalse ) \
{ \
  if (! condition) { \
    cout << flush; \
    cerr << \
      messageIfFalse << endl << flush; \
    assert(condition); \
  } \
}
// JMI       endl <<"--> assertLilypond() causes exit, " <<

/*!
\internal
\brief A function to emit warning messages regarding MusicXML data
*/
//______________________________________________________________________________
void msrMusicXMLWarning (int inputLineNumber, string message);

/*!
\internal
\brief A macro to emit error messages regarding MusicXML data and exit
*/
//______________________________________________________________________________
/*
#define msrMusicXMLError( errorMessage ) \
{ \
  cerr << \
    "--> MusicXML ERROR : " << errorMessage << " !!!" << endl; \
  assert(false); \
}
*/

void msrMusicXMLError (int inputLineNumber, string message);
  
void msrInternalError (int inputLineNumber, string message);
  
/*!
\brief Global variables.

  An global variable is implemented as a static member of this class.
*/
//______________________________________________________________________________
class EXP msrGlobalVariables {
  public:
    
    enum msrDisplayKind {
      kMusicXML, kMSR, kScoreSummary, kLilypondCode,
      k_NoDisplay};

    static msrDisplayKind getDisplayKind ()
      { return sDisplayKind; }
      
    static void setDisplayKind (msrDisplayKind kind)
      { sDisplayKind = kind; }
  
  private:
  
    static msrDisplayKind sDisplayKind;
};

//______________________________________________________________________________
/*!
  \brief The LilyPond note names language.
*/
enum msrNoteNamesLanguage {
  kNederlands, kCatalan, kDeutsch, kEnglish, kEspanol, kItaliano, 
  kFrancais, kNorsk, kPortugues, kSuomi, kSvenska, kVlaams};

typedef std::map<std::string, msrNoteNamesLanguage>
  msrNoteNamesLanguageMap;
  
static msrNoteNamesLanguageMap gMsrNoteNamesLanguageMap;

void                 initializeMsrNoteNamesLanguage ();
msrNoteNamesLanguage getMsrNoteNamesLanguage (std::string lang);
  
//______________________________________________________________________________
/*!
  \brief The MSR code generation options.

  A class is used to avoid passing arguments one by one
  to the various methods that need them.
*/
class EXP msrOptions : public smartable {
  public:

    static SMARTP<msrOptions> create ();
    
  public:
  
    msrOptions();
    virtual ~msrOptions();
 
  public:

    // synthetic view
    string                          fCommandLineOptions;

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
    set<string>                     fPartRenamingSpecsSet;
    
    // interactive mode
    bool                            fInteractive;
    
    // trace
    bool                            fTrace;
    
    // debug
    bool                            fDebug;
    bool                            fDebugDebug;
    
    // measure number-selective debug
    set<int>                        fDebugMeasureNumbersSet;
    bool                            fSaveDebug;
    bool                            fSaveDebugDebug;
};
typedef SMARTP<msrOptions> S_msrOptions;

/*!
\brief A generic msr element representation.

  An element is represented by its name and the
  list of its enclosed elements plus optional parameters.
*/
//______________________________________________________________________________
class musicXMLLocation
{
  public:
/* JMI
    musicXMLLocation (
      int         inputLineNumber,
      int         measureNumber,
      int         positionInMeasure);
      */
  public:

    int         fInputLineNumber;
    int         fMeasureNumber;
    int         fPositionInMeasure; // divisions
};

extern musicXMLLocation  gCurrentMusicXMLLocation;

/*!
\brief A generic msr element representation.

  An element is represented by its name and the
  list of its enclosed elements plus optional parameters.
*/
//______________________________________________________________________________
class EXP msrElement : public smartable
{
  public:

    int getInputLineNumber ()
      { return fInputLineNumber; }

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v) = 0;

    virtual void print (ostream& os);
    
    static indenter gIndenter;

  protected:
     
    S_msrOptions   fMsrOptions;
    int            fInputLineNumber;
    
    msrElement (
      S_msrOptions& msrOpts, 
      int           inputLineNumber);

    virtual ~msrElement();

  private:
};
typedef SMARTP<msrElement> S_msrElement;
EXP ostream& operator<< (ostream& os, const S_msrElement& elt);

/*!
\internal
\brief A macro to simplify indentation
*/
//______________________________________________________________________________
#define idtr msrElement::gIndenter

/*!
\brief A generic msr element representation.

  An element is represented by its name and the
  list of its enclosed elements plus optional parameters.
*/
//______________________________________________________________________________
template <typename T> class EXP msrBrowser : public browser<T> 
{
  protected:
  
    basevisitor*  fVisitor;

    virtual void enter (T& t) { t.acceptIn  (fVisitor); }
    virtual void leave (T& t) { t.acceptOut (fVisitor); }

  public:
    
    msrBrowser (basevisitor* v) : fVisitor (v) {}
    
    virtual ~msrBrowser() {}

    virtual void set (basevisitor* v) { fVisitor = v; }
    
    virtual void browse (T& t) {
      enter (t);

      t.browseData (fVisitor);
      
      leave (t);
    }
};

/*!
\brief A note description for MusicXML.
*/
//______________________________________________________________________________
class musicXMLNoteData
{
  public:
  
    virtual void print (ostream& os);
 
  public:
  
    char        fMusicXMLStep;
    bool        fMusicXMLStepIsARest;
    bool        fMusicXMLStepIsUnpitched;
    
    int         fMusicXMLAlteration;
    
    int         fMusicXMLOctave;
    
    int         fMusicXMLDivisions;
    int         fMusicXMLDuration;
    int         fMusicXMLDotsNumber;
    
    bool        fMusicXMLNoteIsAGraceNote;
    
    bool        fMusicXMLNoteBelongsToAChord;
    
    bool        fMusicXMLNoteBelongsToATuplet;
    string      fMusicXMLTupletMemberNoteType;

    bool        fMusicXMLNoteIsTied;

    int         fMusicXMLVoiceNumber;
};
EXP ostream& operator<< (ostream& os, musicXMLNoteData& elt);

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
\brief A msr note duration representation.

  Musical notation duration is commonly represented as fractions. 

  Duration is a positive number specified in division units.
 
  The divisions element indicates how many divisions per quarter note 
  are used to indicate a note's duration. 
  For example, if duration = 1 and divisions = 2, this is an eighth note duration. 
   
  While MusicXML durations are in in divisions per quarter note,
  LilyPond durations are in whole notes, hence the "*4" multiplications
  
  A note duration is represented by:
    - a numerator (the number of beats)
    - a denominator (the beat value)
    - and optional dots.
*/
//______________________________________________________________________________
class EXP msrDuration : public msrElement
{
  public:
  
    static SMARTP<msrDuration> create (
      S_msrOptions&          msrOpts, 
      int                    inputLineNumber,
      int                    num,
      int                    denom,
      int                    dots,
      string                 tupletMemberType);
    
    msrDuration (
      S_msrOptions&          msrOpts, 
      int                    inputLineNumber,
      int                    num,
      int                    denom,
      int                    dots,
      string                 tupletMemberType);
        
    virtual ~msrDuration();
    
    void scaleNumByFraction (int num, int denom);
        
    msrDuration& operator= (const msrDuration& dur)
      {
        fNum=dur.fNum; fDenom=dur.fDenom; fDots=dur.fDots; 
        return *this;
      }
          
    bool operator!= (const msrDuration& dur) const 
      { 
        return
          (fNum!=dur.fNum) || (fDenom!=dur.fDenom) || (fDots!=dur.fDots);
      }
    
    rational durationAsRational ();

    string   durationAsMSRString ();

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  private:

    int         fNum;
    int         fDenom;
    int         fDots;
    string      fTupletMemberNoteType;
};
typedef SMARTP<msrDuration> S_msrDuration;
EXP ostream& operator<< (ostream& os, const S_msrDuration& elt);

/*!
\brief A msr beam representation.

  A beam is represented by a msrBeamKind value
*/
//______________________________________________________________________________
class EXP msrBeam : public msrElement
{
  public:

    enum msrBeamKind {
            kBeginBeam, kContinueBeam, kEndBeam, 
            k_NoBeam };
    
    static SMARTP<msrBeam> create (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           number,
      msrBeamKind   beamKind);

    msrBeamKind getBeamKind   () const { return fBeamKind; }
    int         getBeamNumber () const { return fBeamNumber; }

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrBeam (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           number,
      msrBeamKind   beamKind);
      
    virtual ~msrBeam();
  
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
        kStaccato, kStaccatissimo, kFermata };

    static SMARTP<msrArticulation> create (
      S_msrOptions&       msrOpts, 
      int                 inputLineNumber,
      msrArticulationKind articulationKind);

    msrArticulationKind
            getArticulationKind () const
                { return fArticulationKind; }
        
    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrArticulation (
      S_msrOptions&       msrOpts, 
      int                 inputLineNumber,
      msrArticulationKind articulationKind);
      
    virtual ~msrArticulation();
  
  private:

    msrArticulationKind fArticulationKind;
};
typedef SMARTP<msrArticulation> S_msrArticulation;
EXP ostream& operator<< (ostream& os, const S_msrArticulation& elt);

/*!
\brief A msr slur representation.

  A slur is represented by a SlurKind value (hairpins in LilyPond)
*/
//______________________________________________________________________________
class EXP msrSlur : public msrElement
{
  public:

    enum msrSlurKind { kStartSlur, kContinueSlur, kStopSlur, k_NoSlur };
    
    static SMARTP<msrSlur> create (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      msrSlurKind   slurKind);

    msrSlurKind getSlurKind () const { return fSlurKind; }

    string  slurKindAsString ();

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrSlur (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      msrSlurKind   slurKind);
    virtual ~msrSlur();
  
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
          k_NoDynamics };
    
    static SMARTP<msrDynamics> create (
      S_msrOptions& msrOpts, 
      int                    inputLineNumber,
      msrDynamicsKind           dynamicsKind);

    msrDynamicsKind
              getDynamicsKind () const
                  { return fDynamicsKind; }

    string  dynamicsKindAsString ();
    string  dynamicsKindAsLilypondString ();

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrDynamics (
      S_msrOptions& msrOpts, 
      int                    inputLineNumber,
      msrDynamicsKind           dynamicsKind);
      
    virtual ~msrDynamics();
  
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
    
    static SMARTP<msrWedge> create (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      msrWedgeKind  wedgeKind);

    msrWedgeKind getWedgeKind () const { return fWedgeKind; }

    string  wedgeKindAsString ();

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrWedge (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      msrWedgeKind  wedgeKind);
      
    virtual ~msrWedge();
  
  private:

    msrWedgeKind fWedgeKind;
};
typedef SMARTP<msrWedge> S_msrWedge;
EXP ostream& operator<< (ostream& os, const S_msrWedge& elt);

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
      kStandaloneNote, kRestNote, kChordMemberNote, kTupletMemberNote};
      
    static SMARTP<msrNote> createFromMusicXMLData (
        S_msrOptions&        msrOpts,
        int                  inputLineNumber,
        musicXMLNoteData&    mxmlNoteData,
        msrSlur::msrSlurKind slurKind);
    
    enum musicXMLDiatonicPitch {
      kA, kB, kC, kD, kE, kF, kG, kRest, k_NoDiatonicPitch};
    
    enum musicXMLAlteration {
      // kDoubleFlat=-2 as in MusicXML, to faciliting testing
      kDoubleFlat=-2, kFlat, kNatural, kSharp, kDoubleSharp,
      k_NoAlteration};

    msrNoteKind getNoteKind () const
                    { return fNoteKind; }
    void        setNoteKind (
                  msrNoteKind noteKind)
                    { fNoteKind = noteKind; }

    // for rests
  // JMI  bool getNoteIsARest ();
    
    // for chord members
    void setNoteBelongsToAChord ();
    
    // we use dutch pitches names for the enumeration below
    // the following is a series of Cs with increasing pitches:
    // \relative c'' { ceseh ces ceh c cih cis cisih }

    enum msrPitch {
      k_aeseh, k_aes, k_aeh, k_a, k_aih, k_ais, k_aisih,
      k_beseh, k_bes, k_beh, k_b, k_bih, k_bis, k_bisih, 
      k_ceseh, k_ces, k_ceh, k_c, k_cih, k_cis, k_cisih,
      k_deseh, k_des, k_deh, k_d, k_dih, k_dis, k_disih,
      k_eeseh, k_ees, k_eeh, k_e, k_eih, k_eis, k_eisih, 
      k_feseh, k_fes, k_feh, k_f, k_fih, k_fis, k_fisih,
      k_geseh, k_ges, k_geh, k_g, k_gih, k_gis, k_gisih,
      k_NoMsrPitch};
    
    msrPitch computeNoteMsrPitch (
        int                         noteQuatertonesFromA,
        msrNote::musicXMLAlteration alteration);
                          
    static map<msrPitch, string> sDutchLilypondPitches;

    int           getNoteMusicXMLDuration () const
                      {
                        return
                          fMusicXMLNoteData.fMusicXMLDivisions;
                      }
    int           getNoteMusicXMLOctave () const
                      {
                        return
                          fMusicXMLNoteData.fMusicXMLOctave;
                      }

    msrPitch      getNoteMsrPitch () const
                      { return fNoteMsrPitch; }
    S_msrDuration getNoteMsrDuration () const
                      { return fNoteMsrDuration; }   

    string        noteMsrPitchAsString ();

    // beam
    void          setBeam (S_msrBeam beam)  { fNoteBeam = beam; }
    S_msrBeam     getBeam () const          { return fNoteBeam; }
    
    // articulations
    void          addArticulation (S_msrArticulation art);
    list<S_msrArticulation>
                  getNoteArticulations () const
                            { return fNoteArticulations; }
    
    // dynamics and wedges
    void          addDynamics (S_msrDynamics dyn);
    void          addWedge    (S_msrWedge    wdg);

    list<S_msrDynamics>
                  getNoteDynamics () const { return fNoteDynamics; };
    list<S_msrWedge>
                  getNoteWedges   () const { return fNoteWedges; };

    msrSlur::msrSlurKind
                  getNoteSlurKind () const { return fNoteSlurKind; }

    S_msrDynamics removeFirstDynamics ();
    S_msrWedge    removeFirstWedge ();

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:
 
    msrNote (
        S_msrOptions&        msrOpts,
        int                  inputLineNumber,
        musicXMLNoteData&    mxmlNoteData,
        msrSlur::msrSlurKind slurKind);
    
    virtual ~msrNote();
    
  private:

    msrNoteKind                fNoteKind;
    
    // MusicXML informations
    musicXMLNoteData           fMusicXMLNoteData;
    musicXMLDiatonicPitch      fMusicXMLDiatonicPitch; // JMI

    // MusicXML durations are in divisions per quarter note.
    // LilyPond durations are in whole notes,
    // hence the "*4" multiplications
    S_msrDuration              fNoteMsrDuration;

    // LilyPond informations
    msrPitch                   fNoteMsrPitch;

    S_msrBeam                  fNoteBeam;
    
    list<S_msrArticulation>    fNoteArticulations;
    
    list<S_msrDynamics>        fNoteDynamics;
    list<S_msrWedge>           fNoteWedges;

    msrSlur::msrSlurKind       fNoteSlurKind;
};
typedef SMARTP<msrNote> S_msrNote;
EXP ostream& operator<< (ostream& os, const S_msrNote& elt);

/*!
\brief The msr parallel music element
*/
//______________________________________________________________________________
class EXP msrParallelMusic : public msrElement
{
  public:
    
    enum msrElementsSeparator { kEndOfLine, kSpace };

    static SMARTP<msrParallelMusic> create (
      S_msrOptions&        msrOpts, 
      int                  inputLineNumber,
      msrElementsSeparator elementsSeparator);

    void         addElementToParallelMusic (S_msrElement elem)
                    { fParallelMusicElements.push_back(elem); }
    S_msrElement getLastElementOfParallelMusic()
                    { return fParallelMusicElements.back(); }
    void         removeLastElementOfParallelMusic ()
                    { fParallelMusicElements.pop_back(); }

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrParallelMusic (
      S_msrOptions&        msrOpts, 
      int                  inputLineNumber,
      msrElementsSeparator elementsSeparator);
      
    virtual ~msrParallelMusic();
    
  private:
  
    vector<S_msrElement> fParallelMusicElements;
    msrElementsSeparator fElementsSeparator;

};
typedef SMARTP<msrParallelMusic> S_msrParallelMusic;
EXP ostream& operator<< (ostream& os, const S_msrParallelMusic& elt);

/*!
\brief The msr sequential music element
*/
//______________________________________________________________________________
class EXP msrSequentialMusic : public msrElement
{
  public:
    
   enum msrElementsSeparator { kEndOfLine, kSpace };

    static SMARTP<msrSequentialMusic> create (
      S_msrOptions&        msrOpts, 
      int                  inputLineNumber,
      msrElementsSeparator elementsSeparator);

    SMARTP<msrSequentialMusic> createEmptyClone ();

    list<S_msrElement>
                  getSequentialMusicElements () const
                      { return fSequentialMusicElements; }

    void          prependElementToSequentialMusic (S_msrElement elem)
                      { fSequentialMusicElements.push_front (elem); }
    void          appendElementToSequentialMusic  (S_msrElement elem)
                      { fSequentialMusicElements.push_back (elem); }
    
    S_msrElement  getLastElementOfSequentialMusic () const
                      { return fSequentialMusicElements.back (); }
                      
    void          removeElementFromSequentialMusic (S_msrElement elem);
    void          removeLastElementFromSequentialMusic ()
                      { fSequentialMusicElements.pop_back () ; }

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrSequentialMusic (
      S_msrOptions&        msrOpts, 
      int                  inputLineNumber,
      msrElementsSeparator elementsSeparator);
      
    virtual ~msrSequentialMusic();
    
  private:
  
    list<S_msrElement>   fSequentialMusicElements;
    msrElementsSeparator fElementsSeparator;

};
typedef SMARTP<msrSequentialMusic> S_msrSequentialMusic;
EXP ostream& operator<< (ostream& os, const S_msrSequentialMusic& elt);

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
      S_msrDuration chordDuration);

    SMARTP<msrChord> createEmptyClone ();

    vector<S_msrNote>
                getChordNotes () const
                    { return fChordNotes; }

    S_msrDuration
                getChordDuration () const
                    { return fChordDuration; }
            
    void        addNoteToChord (S_msrNote note)
                    { fChordNotes.push_back(note); }

    void        addArticulation (S_msrArticulation art)
                    { fChordArticulations.push_back(art); }
    
    void        addDynamics (S_msrDynamics dyn)
                    { fChordDynamics.push_back(dyn); }
    void        addWedge    (S_msrWedge    wdg)
                    { fChordWedges.push_back(wdg); }

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrChord (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      S_msrDuration chordDuration);
      
    virtual ~msrChord();
  
  private:
  
    vector<S_msrNote>         fChordNotes;
    
    S_msrDuration             fChordDuration;
                              
    list<S_msrArticulation>   fChordArticulations;
    
    list<S_msrDynamics>       fChordDynamics;
    list<S_msrWedge>          fChordWedges;
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

    static SMARTP<msrVarValAssoc> create (
      S_msrOptions&      msrOpts, 
      int                inputLineNumber,
      string             variableName,
      string             value);
    
    void      changeAssocValue (string value)
                  { fVariableValue = value; }

    string    getVariableName  () const
                  { return fVariableName; };
    string    getVariableValue () const
                  { return fVariableValue; };

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrVarValAssoc (
      S_msrOptions&      msrOpts, 
      int                inputLineNumber,
      string             variableName,
      string             value);
      
    virtual ~msrVarValAssoc();
  
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

    static SMARTP<msrIdentification> create (
      S_msrOptions&  msrOpts, 
      int            inputLineNumber);
    
    void        setWorkNumber (
                  int    inputLineNumber,
                  string val);

    void        setWorkTitle (
                  int    inputLineNumber,
                  string val);

    void        setMovementNumber (
                  int    inputLineNumber,
                  string vall);

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

    void        setScoreInstrument (
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
    
    vector<S_msrVarValAssoc>
                getCreators () const
                    { return fCreators; };
    
    S_msrVarValAssoc
                getRights () const
                    { return fRights; }
    
    vector<S_msrVarValAssoc>
                getSoftwares () const
                    { return fSoftwares; };
    
    S_msrVarValAssoc
                getEncodingDate () const
                    { return fEncodingDate; }
    
    S_msrVarValAssoc
                getScoreInstrument () const
                    { return fScoreInstrument; }

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrIdentification (
      S_msrOptions&  msrOpts, 
      int            inputLineNumber);
      
    virtual ~msrIdentification();
  
  private:

    S_msrVarValAssoc         fWorkNumber;
    S_msrVarValAssoc         fWorkTitle;
    S_msrVarValAssoc         fMovementNumber;
    S_msrVarValAssoc         fMovementTitle;
    vector<S_msrVarValAssoc> fCreators;
    S_msrVarValAssoc         fRights;
    vector<S_msrVarValAssoc> fSoftwares;
    S_msrVarValAssoc         fEncodingDate;
    S_msrVarValAssoc         fScoreInstrument;

};
typedef SMARTP<msrIdentification> S_msrIdentification;
EXP ostream& operator<< (ostream& os, const S_msrIdentification& elt);

/*!
\brief A MSR page geometry representation.

  A paper is represented by variable/value pairs
*/
//______________________________________________________________________________
class EXP msrPageGeometry : public msrElement
{
  public:

    static SMARTP<msrPageGeometry> create (
      S_msrOptions& msrOpts, 
      int           inputLineNumber);
    
    void    setPaperWidth         (float val) { fPaperWidth = val; }
    float   getPaperWidth         () const    { return fPaperWidth; }

    void    setPaperHeight        (float val) { fPaperHeight = val; }
    float   getPaperHeight        () const    { return fPaperHeight; }

    void    setTopMargin          (float val) { fTopMargin = val; }
    float   getTopMargin          () const    { return fTopMargin; }

    void    setBottomMargin       (float val) { fBottomMargin = val; }
    float   getBottomMargin       () const    { return fBottomMargin; }

    void    setLeftMargin         (float val) { fLeftMargin = val; }
    float   getLeftMargin         () const    { return fLeftMargin; }

    void    setRightMargin        (float val) { fRightMargin = val; }
    float   getRightMargin        () const    { return fRightMargin; }

    void    setBetweenSystemSpace (float val) { fBetweenSystemSpace = val; }
    float   getBetweenSystemSpace () const    { return fBetweenSystemSpace; }

    void    setPageTopSpace       (float val) { fPageTopSpace = val; }
    float   getPageTopSpace       () const    { return fPageTopSpace; }

    int     setMillimeters        (float val) { fMillimeters = val; }
    int     getMillimeters        () const    { return fMillimeters; }

    void    setTenths             (int val)   { fTenths = val; }
    float   getTenths             () const    { return fTenths; }

    float   globalStaffSize       () const;

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);


  protected:

    msrPageGeometry (
      S_msrOptions& msrOpts, 
      int           inputLineNumber);
      
    virtual ~msrPageGeometry();
  
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

    static SMARTP<msrLayout> create (
      S_msrOptions& msrOpts, 
      int           inputLineNumber);
    
    void  addmsrVarValAssoc (S_msrVarValAssoc assoc)
              { fmsrVarValAssocs.push_back(assoc); }
      
    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrLayout (
      S_msrOptions& msrOpts, 
      int           inputLineNumber);
      
    virtual ~msrLayout();
  
  private:
  
    vector<S_msrVarValAssoc> fmsrVarValAssocs;
};
typedef SMARTP<msrLayout> S_msrLayout;
EXP ostream& operator<< (ostream& os, const S_msrLayout& elt);

/*!
\brief A msr repeat representation.

  A repeat is represented by:
    - a sequence of elements for the common part
    - a vector of sequences of elements for the alternate endings
*/
//______________________________________________________________________________
class EXP msrRepeat: public msrElement
{
  public:

    static SMARTP<msrRepeat> create (
      S_msrOptions& msrOpts, 
      int                    inputLineNumber);
    
    void    appendElementToCommonPart (S_msrElement elem)
              { fCommonPart->appendElementToSequentialMusic (elem); }
              
    void    appendElementToLastAlternateEnding  (S_msrElement elem)
              { fAlternateEndings.back()->appendElementToSequentialMusic (elem); }
                    
    void    appendNewAlternateEnding ()
              {
                fAlternateEndings.push_back(
                  msrSequentialMusic::create (
                    fMsrOptions, fInputLineNumber,
                    msrSequentialMusic::kSpace));
              }

    void    setActuallyUsed ()
              { fActuallyUsed = true; }

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrRepeat (
      S_msrOptions& msrOpts, 
      int                    inputLineNumber);
      
    virtual ~msrRepeat();
  
  private:
  
    S_msrSequentialMusic         fCommonPart;
    vector<S_msrSequentialMusic> fAlternateEndings;
    
    // the implicit msrRepeat is not used unless there are
    // actual repeats in the part
    bool                         fActuallyUsed;
};
typedef SMARTP<msrRepeat> S_msrRepeat;
EXP ostream& operator<< (ostream& os, const S_msrRepeat& elt);

/*!
\brief A msr barline representation.

  A barline is represented by the number of the next bar
*/
//______________________________________________________________________________
class EXP msrBarLine : public msrElement
{
  public:
    
    static SMARTP<msrBarLine> create (
      S_msrOptions& msrOpts, 
      int                    inputLineNumber,
      int                    nextBarNumber);

    int        getNextBarNumber () const
                  { return fNextBarNumber; }
                  
    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrBarLine (
      S_msrOptions& msrOpts, 
      int                    inputLineNumber,
      int                    nextBarNumber);
    virtual ~msrBarLine();
  
  private:

    int fNextBarNumber;
};
typedef SMARTP<msrBarLine> S_msrBarLine;
EXP ostream& operator<< (ostream& os, const S_msrBarLine& elt);

/*!
\brief A msr comment representation.

  A comment is represented by its contents
*/
//______________________________________________________________________________
/*
class EXP msrComment : public msrElement
{
  public:
    
    enum msrGapKind { kGapAfterwards, kNoGapAfterwards };

    static SMARTP<msrComment> create (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      string        contents,
      msrGapKind    gapKind = kNoGapAfterwards);

    string      getContents () const { return fContents; }
    msrGapKind  getGapKind  () const { return fGapKind; }
    
    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrComment (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      string        contents,
      msrGapKind    gapKind = kNoGapAfterwards);
      
    virtual ~msrComment();
  
  private:

    string     fContents;
    msrGapKind fGapKind;
};
typedef SMARTP<msrComment> S_msrComment;
EXP ostream& operator<< (ostream& os, const S_msrComment& elt);
*/

/*!
\brief A msr break representation.

  A break is represented by the number of the next bar
*/
//______________________________________________________________________________
class EXP msrBreak : public msrElement
{
  public:
    
    static SMARTP<msrBreak> create (
      S_msrOptions& msrOpts, 
      int                    inputLineNumber,
      int                    nextBarNumber);

    int getNextBarNumber () const
            { return fNextBarNumber; }

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrBreak (
      S_msrOptions& msrOpts, 
      int                    inputLineNumber,
      int                    nextBarNumber);
      
    virtual ~msrBreak();
  
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
class EXP msrBarNumberCheck : public msrElement
{
  public:
    
    static SMARTP<msrBarNumberCheck> create (
      S_msrOptions& msrOpts, 
      int                    inputLineNumber,
      int                    nextBarNumber);

    int getNextBarNumber () const
            { return fNextBarNumber; }

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrBarNumberCheck (
      S_msrOptions& msrOpts, 
      int                    inputLineNumber,
      int                    nextBarNumber);
      
    virtual ~msrBarNumberCheck();
  
  private:

    int fNextBarNumber;
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
    
    static SMARTP<msrTuplet> create (
      S_msrOptions& msrOpts,
      int           inputLineNumber);

    SMARTP<msrTuplet> createEmptyClone ();

    enum msrTupletKind {
      kStartTuplet, kContinueTuplet, kStopTuplet, 
      k_NoTuplet };

    void  updateTuplet (int number, int actualNotes, int normalNotes);
    
    int   getTupletNumber () const { return fTupletNumber; }

    int   getActualNotes () const { return fActualNotes; }
    int   getNormalNotes () const { return fNormalNotes; }
    
    void  addElementToTuplet (S_msrElement elem)
             { fTupletContents.push_back(elem); }

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrTuplet (
      S_msrOptions& msrOpts,
      int           inputLineNumber);
      
    virtual ~msrTuplet();
  
  private:

    int fTupletNumber;
    
    int fActualNotes;
    int fNormalNotes;
    
    vector<S_msrElement> fTupletContents;
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
    
    enum
     // JMI   msrClefLineKind
     {
      kStandardLine,
      kTrebleStdLine=2, kBassStdLine=4, kCStdLine=3, kTabStdLine=5 };

    static SMARTP<msrClef> create (
      S_msrOptions& msrOpts, 
      int                    inputLineNumber,
      string                 sign,
      int                    line,
      int                    octaveChange);

    string  getSign () const
                { return fSign; }
    int     getLine () const
                { return fLine; }
    int     getOctaveChange () const
                { return fOctaveChange; }
                
    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrClef (
      S_msrOptions& msrOpts, 
      int                    inputLineNumber,
      string                 sign,
      int                    line,
      int                    octaveChange);
      
    virtual ~msrClef();
  
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

    static SMARTP<msrKey> create (
      S_msrOptions& msrOpts, 
      int                    inputLineNumber,
      int                    fifths,
      string                 mode,
      int                    cancel);

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

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrKey (
      S_msrOptions& msrOpts, 
      int                    inputLineNumber,
      int                    fifths,
      string                 mode,
      int                    cancel);
      
    virtual ~msrKey();
  
  private:

    int        fFifths;
    string     fMode;
    int        fCancel;

    string     fTonic;
    msrKeyMode fKeyMode;
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
    
    static SMARTP<msrTime> create (
      S_msrOptions& msrOpts, 
      int                    inputLineNumber,
      int                    numerator,
      int                    denominator);

    rational  getRational () const
                  { return fRational; }
                  
    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrTime (
      S_msrOptions& msrOpts, 
      int                    inputLineNumber,
      int                    numerator,
      int                    denominator);
      
    virtual ~msrTime();
  
  private:

    rational fRational;
    bool     fGenerateNumericalTime;
};
typedef SMARTP<msrTime> S_msrTime;
EXP ostream& operator<< (ostream& os, const S_msrTime& elt);

/*!
\brief A tempo representation.

  A tempo is represented by the lyrics to use
*/
//______________________________________________________________________________
class EXP msrTempo : public msrElement
{
  public:

    static SMARTP<msrTempo> create (
      S_msrOptions& msrOpts, 
      int                    inputLineNumber,
      int                    tempoUnit,
      int                    perMinute);

    int       getTempoUnit () const
                  { return fTempoUnit; }

    int       getPerMinute () const
                  { return fPerMinute; }

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrTempo (
      S_msrOptions& msrOpts, 
      int                    inputLineNumber,
      int                    tempoUnit,
      int                    perMinute);
      
    virtual ~msrTempo();
  
  private:
  
    int  fTempoUnit;
    int  fPerMinute;
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
    enum msrLyricschunkType {
      kSingleChunk, kBeginChunk, kMiddleChunk, kEndChunk,
      kSkipChunk, kSlurChunk, kTiedChunk,
      kBreakChunk,
      k_NoChunk };

    static SMARTP<msrLyricschunk> create (
      S_msrOptions&      msrOpts, 
      int                inputLineNumber,
      msrLyricschunkType chunkType,
      string             chunkText,
      S_msrDuration      msrDuration);

    msrLyricschunkType
              getLyricschunkType () const
                  { return fLyricschunkType; }

    string    getChunkText () const
                  { return fChunkText; }

    S_msrDuration
              getChunkDuration () const
                  { return fChunkDuration; }

    SMARTP<msrLyricschunk> createEmptyClone ();

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrLyricschunk (
      S_msrOptions&      msrOpts, 
      int                inputLineNumber,
      msrLyricschunkType chunkType,
      string             chunkText,
      S_msrDuration      msrDuration);
        
    virtual ~msrLyricschunk();

  private:
  
    msrLyricschunkType fLyricschunkType;
    string             fChunkText;
    S_msrDuration      fChunkDuration;
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

    static SMARTP<msrLyrics> create (
      S_msrOptions&         msrOpts, 
      int                   inputLineNumber,
      int                   lyricsNumber,
      S_msrVoice            lyricsVoice,
      msrLyricsMasterStatus lyricsMasterStatus);
    
    SMARTP<msrLyrics> createEmptyClone (S_msrVoice clonedVoice);

    int     getLyricsNumber () const
                { return fLyricsNumber; }
                
    string  getLyricsName () const;
                
    S_msrVoice
            getLyricsVoice () const
                { return fLyricsVoice; }
                
    msrLyricsMasterStatus
            getLyricsMasterStatus () const
                { return fLyricsMasterStatus; }
                
    vector<S_msrLyricschunk>
            getLyricschunks () const
                { return fLyricschunks; }

    void    addTextChunkToLyrics (
              int
                  inputLineNumber,
              string
                  syllabic, // JMI ???
              msrLyricschunk::msrLyricschunkType
                  chunkType,
              string
                  text,
              bool
                  elision,
              S_msrDuration
                  msrDuration);
      
    void    addSkipChunkToLyrics (
              int            inputLineNumber,
              S_msrDuration  duration);

    void    addSlurChunkToLyrics (
              int            inputLineNumber,
              S_msrDuration  duration);

    void    addTiedChunkToLyrics (
              int            inputLineNumber,
              S_msrDuration  duration);

    void    addBreakChunkToLyrics (
              int inputLineNumber,
              int nextMeasureNumber);

    void    addChunkToLyrics (S_msrLyricschunk chunk);
                
    int     getLyricsTextPresent() { return fLyricsTextPresent; }

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrLyrics (
      S_msrOptions&         msrOpts, 
      int                   inputLineNumber,
      int                   lyricsNumber,
      S_msrVoice            lyricsVoice,
      msrLyricsMasterStatus lyricsMasterStatus);
    virtual ~msrLyrics();
  
  private:

    int                       fLyricsNumber;
    S_msrVoice                fLyricsVoice;
    msrLyricsMasterStatus     fLyricsMasterStatus;

    vector<S_msrLyricschunk>  fLyricschunks;

    bool                      fLyricsTextPresent;
};
typedef SMARTP<msrLyrics> S_msrLyrics;
EXP ostream& operator<< (ostream& os, const S_msrLyrics& elt);

/*!
\brief A msr voice representation.

  A vpoce is represented by a its string contents
*/
//______________________________________________________________________________
class EXP msrVoice : public msrElement
{
  public:

    static SMARTP<msrVoice> create (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           voiceNumber,
      int           staffRelativeVoiceNumber,
      S_msrStaff    voiceStaff);
                          
    SMARTP<msrVoice> createEmptyClone (S_msrStaff clonedStaff);

    int       getVoiceNumber () const
                  { return fVoiceNumber; }
                
    S_msrStaff
              getVoiceStaff () const
                  { return fVoiceStaff; }
                
    map<int, S_msrLyrics>
              getVoiceLyricsMap () const
                  { return fVoiceLyricsMap; }

    string    getVoiceName () const;

    S_msrLyrics
              addLyricsToVoice (
                int inputLineNumber,
                int lyricsNumber);

    void      addLyricsToVoice (S_msrLyrics lyrics);
    
    S_msrLyrics
              fetchLyricsFromVoice (int lyricsNumber);

    S_msrLyrics
              getVoiceMasterLyrics () { return fVoiceMasterLyrics; }
               
    void      appendClefToVoice       (S_msrClef clef);
    void      appendKeyToVoice        (S_msrKey  key);
    void      appendTimeToVoice       (S_msrTime time);
    
    void      appendTempoToVoice      (S_msrTempo tempo);
    
    void      appendNoteToVoice       (S_msrNote note);
    void      appendChordToVoice      (S_msrChord chord);
    void      appendTupletToVoice     (S_msrTuplet tuplet);
    
    void      appendElementToVoice    (S_msrElement elem); // for others

    S_msrElement
              getVoiceSequentialMusicLastElement ()
                  {
                    return fVoiceSequentialMusic->
                      getLastElementOfSequentialMusic ();
                  }
                  
    void      removeLastElementFromVoiceSequentialMusic ();

    void      removeElementFromVoiceSequentialMusic (S_msrElement elem);

    S_msrSequentialMusic
              getVoiceSequentialMusic () const
                  { return fVoiceSequentialMusic; }

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrVoice (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           voiceNumber,
      int           staffRelativeVoiceNumber,
      S_msrStaff    voiceStaff);
      
    virtual ~msrVoice();
  
  private:

    int                       fVoiceNumber;
    int                       fStaffRelativeVoiceNumber;
    S_msrStaff                fVoiceStaff;

    bool                      fVoiceContainsActualNotes;

    // the master lyrics, collecting skips along the way,
    // to be used as a 'prelude' by actual lyrics that start at later points
    S_msrLyrics               fVoiceMasterLyrics;
    
    // the lyrics map
    map<int, S_msrLyrics>     fVoiceLyricsMap;

    // the implicit sequence containing the code generated for the voice
    S_msrSequentialMusic      fVoiceSequentialMusic;
  

    // the implicit repeat at the beginning of the voice
    // will be ignored if the voice has no repeats at all
    S_msrRepeat               fVoiceMsrRepeat;
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

    static SMARTP<msrStaff> create (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           staffNumber,
      S_msrPart     staffPart);
    
    SMARTP<msrStaff> createEmptyClone (S_msrPart clonedPart);

    int       getStaffNumber () const
                  { return fStaffNumber; }
                
    S_msrPart getStaffPart () const
                  { return fStaffPart; }

    string    getStaffName () const;

    string    getStaffInstrumentName () const
                  { return fStaffInstrumentName; }

    S_msrClef getStaffClef () const
                  { return fStaffClef; };
    S_msrKey  getStaffKey () const
                  { return fStaffKey; };
    S_msrTime getStaffTime () const
                  { return fStaffTime; };

    void      setStaffKey  (S_msrKey  key);
    void      setStaffTime (S_msrTime time);
    void      setStaffClef (S_msrClef clef);

    map<int, S_msrVoice>
              getStaffVoicesMap ()
                  { return fStaffVoicesMap; }

    S_msrVoice
              addVoiceToStaff (
                int inputLineNumber,
                int voiceNumber);

    void      addVoiceToStaff (S_msrVoice voice);
    
    S_msrVoice
              fetchVoiceFromStaff (int voiceNumber);
                              
    void      appendClefToAllStaffVoices (S_msrClef clef);
    void      appendKeyToAllStaffVoices  (S_msrKey   key);
    void      appendTimeToAllStaffVoices (S_msrTime time);
              
    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrStaff (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           staffNumber,
      S_msrPart     staffPart);
    virtual ~msrStaff();
  
  private:

    static int              gMaxStaffVoices;

    int                     fStaffNumber;
    S_msrPart               fStaffPart;

    map<int, S_msrVoice>    fStaffVoicesMap;

    string                  fStaffInstrumentName;

    S_msrKey                fStaffKey;
    S_msrTime               fStaffTime;
    S_msrClef               fStaffClef;

    int                     fNextRelativeStaffVoiceNumber;
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

    static SMARTP<msrPart> create (
      S_msrOptions&  msrOpts, 
      int            inputLineNumber,
      string         partMusicXMLID,
      S_msrPartgroup partPartgroup);
                
    SMARTP<msrPart> createEmptyClone (S_msrPartgroup clonedPartgroup);

    void      setPartAbbreviation (string partAbbreviation)
                  { fPartAbbreviation = partAbbreviation; }
                
    void      setPartInstrumentName (string partInstrumentName)
                  { fPartInstrumentName = partInstrumentName; }
                              
    void      setPartDivisions (int  musicXMLDivisions)
                  { fPartMusicXMLDivisions = musicXMLDivisions; }
    
    void      setPartName (string  partName)
                  { fPartName = partName; }
    
    string    getPartMusicXMLID () const
                  { return fPartMusicXMLID; }

    string    getPartName () const
                  { return fPartName; }

    string    getPartAbbreviation () const
                  { return fPartAbbreviation; }
                
    string    getPartInstrumentName () const
                  { return fPartInstrumentName; }
                
    int       getPartMusicXMLDivisions () const
                  { return fPartMusicXMLDivisions; }

    string    getPartMSRName () const
                  { return fPartMSRName; }

    S_msrPartgroup
              getPartPartgroup () const
                  { return fPartPartgroup; }
                
    map<int, S_msrStaff>
              getPartStavesMap ()
                  { return fPartStavesMap; }

    string    getPartCombinedName () const;

    void      setAllPartStavesKey  (S_msrKey  key);
              
    void      setAllPartStavesTime (S_msrTime time);
              
    void      setAllPartStavesClef (S_msrClef clef);
              
    S_msrStaff
              addStaffToPart (
                int inputLineNumber,
                int staffNumber);

    void      addStaffToPart (S_msrStaff staff);

    S_msrStaff
              fetchStaffFromPart (int staffNumber);

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrPart (
      S_msrOptions&  msrOpts, 
      int            inputLineNumber,
      string         partMusicXMLID,
      S_msrPartgroup partPartgroup);
      
    virtual ~msrPart();
  
  private:
    
    string                  fPartMusicXMLID;
    string                  fPartName;
    string                  fPartAbbreviation;
    string                  fPartInstrumentName;

    S_msrPartgroup          fPartPartgroup;

    map<int, S_msrStaff>    fPartStavesMap;

    string                  fPartMSRName; // coined in constructor

    int                     fPartMusicXMLDivisions;
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
        kStartPartgroupType, kStopPartgroupType,
        k_NoPartgroupType };
          
    enum msrPartgroupSymbolKind {
        kBracePartgroupSymbol, kBracketPartgroupSymbol,
        kLinePartgroupSymbol, kSquarePartgroupSymbol,
        k_NoPartgroupSymbol };
          
    static SMARTP<msrPartgroup> create (
      S_msrOptions&          msrOpts, 
      int                    inputLineNumber,
      int                    partGroupNumber,
      string                 partGroupName,
      string                 partGroupAbbreviation,
      msrPartgroupSymbolKind partGroupSymbolKind,
      int                    partGroupSymbolDefaultX,
      bool                   partGroupBarline);

    SMARTP<msrPartgroup> createEmptyClone ();

    static int gPartgroupsCounter;
    
    int       getPartgroupNumber () const
                  { return fPartgroupNumber; }
    
    string    getPartgroupName () const
                  { return fPartgroupName; }

    string    getPartgroupAbbreviation () const
                  { return fPartgroupAbbreviation; }

    msrPartgroupSymbolKind
              getPartgroupSymbolKind () const
                  { return fPartgroupSymbolKind; }

    int       getPartgroupSymbolDefaultX () const
                  { return fPartgroupSymbolDefaultX; }

    bool      getPartgroupBarline () const
                  { return fPartgroupBarline; }
    
    string    getPartgroupCombinedName () const;

    list<S_msrElement>
              getPartgroupElements () const
                  { return fPartgroupElements; }

    S_msrPart addPartToPartgroup (
                int    inputLineNumber,
                string partMusicXMLID);
    
    void       addPartToPartgroup (S_msrPart part);
                
    void      prependSubPartgroupToPartgroup (
                S_msrPartgroup partGroup);

    S_msrPart fetchPartFromPartgroup (string partMusicXMLID);

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrPartgroup (
      S_msrOptions&          msrOpts, 
      int                    inputLineNumber,
      int                    partGroupNumber,
      string                 partGroupName,
      string                 partGroupAbbreviation,
      msrPartgroupSymbolKind partGroupSymbolKind,
      int                    partGroupSymbolDefaultX,
      bool                   partGroupBarline);
            
    virtual ~msrPartgroup();
  
  private:

    int                     fPartgroupAbsoluteNumber;
    
    int                     fPartgroupNumber;
        
    string                  fPartgroupName;
    string                  fPartgroupAbbreviation;

    msrPartgroupSymbolKind  fPartgroupSymbolKind;
    int                     fPartgroupSymbolDefaultX;

    bool                    fPartgroupBarline;

    // accessing parts by name
    map<string, S_msrPart>  fPartgroupPartsMap;

    // allowing for both parts and (sub-)part groups as elements
    list<S_msrElement>      fPartgroupElements;
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

    static SMARTP<msrScore> create (
      S_msrOptions& msrOpts, 
      int           inputLineNumber);

    SMARTP<msrScore> createEmptyClone ();

    S_msrIdentification
                getIdentification () const
                    { return fIdentification; }

    S_msrPageGeometry
                getPageGeometry () const
                    { return fPageGeometry; }
    
    list<S_msrPartgroup>
                getPartgroupsList () const
                    { return fPartgroupsList; }

    void addPartgroupToScore (S_msrPartgroup partGroup);

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrScore (
      S_msrOptions& msrOpts, 
      int           inputLineNumber);
      
    virtual ~msrScore();
  
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

    static SMARTP<msrMidi> create (
      S_msrOptions& msrOpts, 
      int                    inputLineNumber);

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    virtual void print (ostream& os);

  protected:

    msrMidi (
      S_msrOptions& msrOpts, 
      int                    inputLineNumber);
      
    virtual ~msrMidi();
  
  private:
  
};
typedef SMARTP<msrMidi> S_msrMidi;
EXP ostream& operator<< (ostream& os, const S_msrMidi& elt);


/*! @} */

}


#endif
