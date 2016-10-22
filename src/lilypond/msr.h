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

#include "msrUtilities.h"

#include "musicxml2msr.h"

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

class msrVisitor;

class msrElement;

class msrLilypondVarValAssoc;
class msrSchemeVarValAssoc;

class msrScore;

class msrPart;
class msrStaff;
class msrClef;
class msrKey;
class msrTime;
class msrVoice;
class msrLyrics;

class msrLyrics;
class msrDuration;
class msrDynamics;
class msrWedge;
class msrChord;
class msrNote;

class msrPaper;
class msrLayout;

typedef SMARTP<msrElement>             S_msrElement;

typedef SMARTP<msrLilypondVarValAssoc> S_msrLilypondVarValAssoc;
typedef SMARTP<msrSchemeVarValAssoc>   S_msrSchemeVarValAssoc;

typedef SMARTP<msrScore>               S_msrScore;

typedef SMARTP<msrPart>                S_msrPart;
typedef SMARTP<msrStaff>               S_msrStaff;
typedef SMARTP<msrClef>                S_msrClef;
typedef SMARTP<msrKey>                 S_msrKey;
typedef SMARTP<msrTime>                S_msrTime;
typedef SMARTP<msrVoice>               S_msrVoice;
typedef SMARTP<msrLyrics>              S_msrLyrics;


typedef SMARTP<msrLyrics>              S_msrLyrics;
typedef SMARTP<msrDuration>            S_msrDuration;
typedef SMARTP<msrDynamics>            S_msrDynamics;
typedef SMARTP<msrWedge>               S_msrWedge;
typedef SMARTP<msrChord>               S_msrChord;
typedef SMARTP<msrNote>                S_msrNote;

typedef SMARTP<msrPaper>               S_msrPaper;
typedef SMARTP<msrLayout>              S_msrLayout;

EXP ostream& operator<< (ostream& os, const S_msrElement& elt);

EXP ostream& operator<< (ostream& os, const S_msrScore& elt);

EXP ostream& operator<< (ostream& os, const S_msrClef& elt);
EXP ostream& operator<< (ostream& os, const S_msrKey& elt);
EXP ostream& operator<< (ostream& os, const S_msrTime& elt);

EXP ostream& operator<< (ostream& os, const S_msrLilypondVarValAssoc& elt);
EXP ostream& operator<< (ostream& os, const S_msrSchemeVarValAssoc& elt);

EXP ostream& operator<< (ostream& os, const S_msrDuration& dur);
EXP ostream& operator<< (ostream& os, const S_msrDynamics& dyn);
EXP ostream& operator<< (ostream& os, const S_msrWedge& wdg);
EXP ostream& operator<< (ostream& os, const S_msrChord& chrd);
EXP ostream& operator<< (ostream& os, const S_msrNote& chrd);

EXP ostream& operator<< (ostream& os, const S_msrPaper& chrd);
EXP ostream& operator<< (ostream& os, const S_msrLayout& chrd);


/*!
\brief Global variables.

  An global variable is implemented as a static member of this class.
*/
//______________________________________________________________________________
class EXP msrGlobalVariables {
  public:
    
    enum CodeGenerationKind {
      kMSR, kMusicXML, kLilypondCode};

    static CodeGenerationKind getCodeGenerationKind ()
      { return sCodeGenerationKind; }
      
    static void setCodeGenerationKind (CodeGenerationKind kind)
      { sCodeGenerationKind = kind; }
  
  private:
  
    static CodeGenerationKind sCodeGenerationKind;
};

/*!
\brief A generic msr element representation.

  An element is represented by its name and the
  list of its enclosed elements plus optional parameters.
*/
//______________________________________________________________________________
class EXP msrElement : public smartable {
  public:
 
    static SMARTP<msrElement> create (bool debug);
    
    virtual void print              (ostream& os);

    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);
    
    static indenter idtr;

  protected:
     
    msrElement (bool debug);
    virtual ~msrElement();

  private:
  
    bool fDebug;
};
typedef SMARTP<msrElement> S_msrElement;
typedef list<S_msrElement> msrElementList;

//______________________________________________________________________________
/*!
\internal
\brief A macro to simplify indentation
*/

#define idtr msrElement::idtr

/*!
\brief A beat description for MusicXML.
*/
//______________________________________________________________________________
class musicXMLBeatData { // JMI ???
  public:
  
     virtual void print              (ostream& os);
 
  public:
  
    string fBeatUnit;
    int         fDots;
};


/*!
\brief A note description for MusicXML.
*/
//______________________________________________________________________________
class musicXMLNoteData {
  public:
  
     virtual void print              (ostream& os);
 
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

    int         fMusicXMLVoiceNumber;
};
  
/*!
\brief A msr absolute octave representation.
*/
//______________________________________________________________________________
class EXP msrAbsoluteOctave : public msrElement {
  public:
  
    static SMARTP<msrAbsoluteOctave> create(int musicxmlOctave);
    
    msrAbsoluteOctave(int musicxmlOctave);
    virtual ~msrAbsoluteOctave();
    
    msrAbsoluteOctave& operator= (const msrAbsoluteOctave& absOct)
      {
        fMsrOctave = absOct.fMsrOctave;
        return *this;
      }
          
    bool operator!= (const msrAbsoluteOctave& otherAbsOct) const 
      { 
        return fMsrOctave != otherAbsOct.fMsrOctave;
      }
    
    string  absoluteOctaveAsLilypondString ();

    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  private:

    int  fMsrOctave;
};
typedef SMARTP<msrAbsoluteOctave> S_msrAbsoluteOctave;

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
class EXP msrDuration : public msrElement {
  public:
  
    static SMARTP<msrDuration> create (
        int    num,
        int    denom,
        int    dots,
        string tupletMemberType);
    
    msrDuration(
        int    num,
        int    denom,
        int    dots,
        string tupletMemberType);
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

    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  private:

    int         fNum;
    int         fDenom;
    int         fDots;
    string      fTupletMemberNoteType;
};
typedef SMARTP<msrDuration> S_msrDuration;

/*!
\brief A msr articulation representation.

  An articulation is represented by the numerator and denominator
*/
//______________________________________________________________________________
class EXP msrArticulation : public msrElement {
  public:
    
    enum ArticulationKind {
        kStaccato, kStaccatissimo };

    static SMARTP<msrArticulation> create (
              ArticulationKind articulationKink);

    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrArticulation (
        ArticulationKind articulationKink);
    virtual ~msrArticulation();
  
  private:

    ArticulationKind
          fArticulationKind;
};
typedef SMARTP<msrArticulation> S_msrArticulation;

/*!
\brief A msr slur representation.

  A slur is represented by a SlurKind value (hairpins in LilyPond)
*/
//______________________________________________________________________________
class EXP msrSlur : public msrElement {
  public:

    enum SlurKind { kStartSlur, kContinueSlur, kStopSlur, k_NoSlur };
    
    static SMARTP<msrSlur> create (SlurKind kind);

    SlurKind getSlurKind () const        { return fSlurKind; }

    string  slurKindAsString ();

    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrSlur (SlurKind kind);
    virtual ~msrSlur();
  
  private:

    SlurKind fSlurKind;
};
typedef SMARTP<msrSlur> S_msrSlur;

/*!
\brief A msr note representation.

  A note is represented by its name, optional accidentals,
    duration (in the form of numerator/denominator) and optional dots.
*/
//______________________________________________________________________________
class EXP msrNote : public msrElement {
  public:

    enum MusicXMLDiatonicPitch {
      kA, kB, kC, kD, kE, kF, kG, kRest, k_NoDiatonicPitch};
    
    enum MusicXMLAlteration {
      // kDoubleFlat=-2 as in MusicXML, to faciliting testing
      kDoubleFlat=-2, kFlat, kNatural, kSharp, kDoubleSharp,
      k_NoAlteration};
            
    // for standalone notes
    static SMARTP<msrNote> createFromMusicXMLData (
        S_translationSettings& ts,
        musicXMLNoteData&      mxmldat,
        msrSlur::SlurKind      slurKind);
    
    // for chord members
    void setNoteBelongsToAChord ();
    
    // we use dutch pitches names for the enumeration below
    // the following is a series of Cs with increasing pitches:
    // \relative c'' { ceseh ces ceh c cih cis cisih }

    enum MsrPitch {
      k_aeseh, k_aes, k_aeh, k_a, k_aih, k_ais, k_aisih,
      k_beseh, k_bes, k_beh, k_b, k_bih, k_bis, k_bisih, 
      k_ceseh, k_ces, k_ceh, k_c, k_cih, k_cis, k_cisih,
      k_deseh, k_des, k_deh, k_d, k_dih, k_dis, k_disih,
      k_eeseh, k_ees, k_eeh, k_e, k_eih, k_eis, k_eisih, 
      k_feseh, k_fes, k_feh, k_f, k_fih, k_fis, k_fisih,
      k_geseh, k_ges, k_geh, k_g, k_gih, k_gis, k_gisih,
      k_NoMsrPitch};
    
    MsrPitch computeNoteMsrPitch (
        int                         noteQuatertonesFromA,
        msrNote::MusicXMLAlteration alteration);
                          
    static map<MsrPitch, string> sDutchLilypondPitches;

    S_msrDuration       getNoteMsrDuration ()
                            { return fNoteMsrDuration; }   

    string              notePitchAsLilypondString ();

    // articulations
    void                addArticulation (S_msrArticulation art);
    list<S_msrArticulation>
                        getNoteArticulations () const
                            { return fNoteArticulations; }
    
    // dynamics and wedges
    void                addDynamics (S_msrDynamics dyn);
    void                addWedge    (S_msrWedge    wdg);

    list<S_msrDynamics> getNoteDynamics () { return fNoteDynamics; };
    list<S_msrWedge>    getNoteWedges   () { return fNoteWedges; };

    S_msrDynamics       removeFirstDynamics ();
    S_msrWedge          removeFirstWedge ();

//    void octaveRelativeTo (const msrAbsoluteOctave& otherAbsOct);
        
    virtual void printMusicXML      (ostream& os);
    virtual void printMSR           (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:
 
    msrNote (
        S_translationSettings& ts,
        musicXMLNoteData&      mxmldat,
        msrSlur::SlurKind      slurKind);
    
    virtual ~msrNote();
    
  private:
  
    S_translationSettings      fTranslationSettings;

    // MusicXML informations
    musicXMLNoteData           fMusicXMLNoteData;
    MusicXMLDiatonicPitch      fMusicXMLDiatonicPitch; // JMI

    // MusicXML durations are in in divisions per quarter note,
    // LilyPond durations are in whole notes, hence the "*4" multiplication
    S_msrDuration              fNoteMsrDuration;

    // LilyPond informations
    MsrPitch                   fNoteMsrPitch;

    list<S_msrArticulation>    fNoteArticulations;
    
    list<S_msrDynamics>        fNoteDynamics;
    list<S_msrWedge>           fNoteWedges;

    msrSlur::SlurKind          fNoteSlurKind;
};
typedef SMARTP<msrNote> S_msrNote;

/*!
\brief The msr parallel music element
*/
//______________________________________________________________________________
class EXP msrParallelMusic : public msrElement {
  public:
    
    enum ElementsSeparator { kEndOfLine, kSpace };

    static SMARTP<msrParallelMusic> create(ElementsSeparator elementsSeparator);

    void         addElementToParallelMusic (S_msrElement elem)
                    { fParallelMusicElements.push_back(elem); }
    S_msrElement getLastElementOfParallelMusic()
                    { return fParallelMusicElements.back(); }
    void         removeLastElementOfParallelMusic ()
                    { fParallelMusicElements.pop_back(); }

    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrParallelMusic(ElementsSeparator elementsSeparator);
    virtual ~msrParallelMusic();
    
  private:
  
    vector<S_msrElement> fParallelMusicElements;
    ElementsSeparator         fElementsSeparator;

};
typedef SMARTP<msrParallelMusic> S_msrParallelMusic;

/*!
\brief The msr sequence element
*/
//______________________________________________________________________________
class EXP msrSequence : public msrElement {
  public:
    
   enum ElementsSeparator { kEndOfLine, kSpace };

    static SMARTP<msrSequence> create(ElementsSeparator elementsSeparator);

    void          prependElementToSequence (S_msrElement elem)
                      { fSequenceElements.push_front (elem); }
    void          appendElementToSequence  (S_msrElement elem)
                      { fSequenceElements.push_back (elem); }
    
    S_msrElement  getLastElementOfSequence ()
                      { return fSequenceElements.back (); }
                      
    void          removeElementFromSequence (S_msrElement elem);
    void          removeLastElementFromSequence ()
                      { fSequenceElements.pop_back () ; }

    virtual void printMusicXML      (ostream& os);
    virtual void printMSR           (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrSequence(ElementsSeparator elementsSeparator);
    virtual ~msrSequence();
    
  private:
  
    msrElementList     fSequenceElements;
    ElementsSeparator  fElementsSeparator;

};
typedef SMARTP<msrSequence> S_msrSequence;

/*!
\brief The msr chord element
*/
//______________________________________________________________________________
class EXP msrChord : public msrElement {
  public:

    static SMARTP<msrChord> create(S_msrDuration chordDuration);
    
    void         addNoteToChord (S_msrNote note)
                    { fChordNotes.push_back(note); }

    void         addArticulation (S_msrArticulation art)
                    { fChordArticulations.push_back(art); }
    
    void         addDynamics (S_msrDynamics dyn)
                    { fChordDynamics.push_back(dyn); }
    void         addWedge    (S_msrWedge    wdg)
                    { fChordWedges.push_back(wdg); }

    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrChord (S_msrDuration chordDuration);
    virtual ~msrChord();
  
  private:
  
    vector<S_msrNote>         fChordNotes;
    
    S_msrDuration             fChordDuration;

    list<S_msrArticulation>   fChordArticulations;
    
    list<S_msrDynamics>       fChordDynamics;
    list<S_msrWedge>          fChordWedges;
};
typedef SMARTP<msrChord> S_msrChord;

/*!
\brief A msr LilyPond variable/value association representation.
*/
//______________________________________________________________________________
class EXP msrLilypondVarValAssoc : public msrElement {
  public:

    enum VarValSeparator   { kSpace, kEqualSign };
    enum QuotesKind        { kQuotesAroundValue, kNoQuotesAroundValue };
    enum CommentedKind     { kCommented, kUncommented };

    static SMARTP<msrLilypondVarValAssoc> create(
              string     variableName,
              string     value, 
              VarValSeparator varValSeparator,
              QuotesKind      quotesKind,
              CommentedKind   commentKind,
              string     unit = "");
    
    void    changeAssoc (string value);
    
    string getVariableName  () const { return fVariableName; };
    string getVariableValue () const { return fVariableValue; };

    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrLilypondVarValAssoc(
              string     variableName,
              string     value, 
              VarValSeparator varValSeparator,
              QuotesKind      quotesKind,
              CommentedKind   commentedKind,
              string     unit = "");
    virtual ~msrLilypondVarValAssoc();
  
  private:

    string     fVariableName;
    string     fVariableValue;
    VarValSeparator fVarValSeparator;
    QuotesKind      fQuotesKind;
    CommentedKind   fCommentedKind;
    string     fUnit;
};
typedef SMARTP<msrLilypondVarValAssoc> S_msrLilypondVarValAssoc;

/*!
\brief A msr Scheme variable/value association representation.
*/
//______________________________________________________________________________
class EXP msrSchemeVarValAssoc : public msrElement {
  public:

    enum CommentedKind     { kCommented, kUncommented };

    static SMARTP<msrSchemeVarValAssoc> create(
              string     variableName,
              string     value, 
              CommentedKind   commentKind );
    
    void    changeAssoc (string value);
    
    string getVariableValue () const { return fVariableValue; };

    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrSchemeVarValAssoc(
              string     variableName,
              string     value, 
              CommentedKind   commentedKind );
    virtual ~msrSchemeVarValAssoc();
  
  private:

    string     fVariableName;
    string     fVariableValue;
    CommentedKind   fCommentedKind;
};
typedef SMARTP<msrSchemeVarValAssoc> S_msrSchemeVarValAssoc;

/*!
\brief A msr header representation.

  A header is represented by variable/value pairs
*/
//______________________________________________________________________________
class EXP msrHeader : public msrElement {
  public:

    static SMARTP<msrHeader> create();
    
    void                      setWorkNumber      (string val);
    S_msrLilypondVarValAssoc getWorkNumber      () const;

    void                      setWorkTitle       (string val);
    S_msrLilypondVarValAssoc getWorkTitle       () const;

    void                      setMovementNumber  (string val);
    S_msrLilypondVarValAssoc getMovementNumber  () const;

    void                      setMovementTitle   (string val);
    S_msrLilypondVarValAssoc getMovementTitle   () const;

    void                      addCreator         (
                                string type, string val);
    vector<S_msrLilypondVarValAssoc>
                              getCreators        () const;

    void                      setRights          (string val);
    S_msrLilypondVarValAssoc getRights          () const;

    void                      addSoftware        (string val);
    vector<S_msrLilypondVarValAssoc>
                              getSoftwares       () const;

    void                      setEncodingDate    (string val);
    S_msrLilypondVarValAssoc getEncodingDate    () const;

    void                      setScoreInstrument (string val);
    S_msrLilypondVarValAssoc getScoreInstrument () const;

    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrHeader();
    virtual ~msrHeader();
  
  private:

    S_msrLilypondVarValAssoc              fWorkNumber;
    S_msrLilypondVarValAssoc              fWorkTitle;
    S_msrLilypondVarValAssoc              fMovementNumber;
    S_msrLilypondVarValAssoc              fMovementTitle;
    vector<S_msrLilypondVarValAssoc> fCreators;
    S_msrLilypondVarValAssoc              fRights;
    vector<S_msrLilypondVarValAssoc> fSoftwares;
    S_msrLilypondVarValAssoc              fEncodingDate;
    S_msrLilypondVarValAssoc              fScoreInstrument;

};
typedef SMARTP<msrHeader> S_msrHeader;

/*!
\brief A msr paper representation.

  A paper is represented by variable/value pairs
*/
//______________________________________________________________________________
class EXP msrPaper : public msrElement {
  public:

    static SMARTP<msrPaper> create();
    
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

    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);


  protected:

    msrPaper();
    virtual ~msrPaper();
  
  private:

    // page height, margins and the like in centimeters are in centimeters
    float             fPaperWidth;
    float             fPaperHeight;
    float             fTopMargin;
    float             fBottomMargin;
    float             fLeftMargin;
    float             fRightMargin;
    
    float             fBetweenSystemSpace;
    float             fPageTopSpace; 
};
typedef SMARTP<msrPaper> S_msrPaper;

/*!
\brief A msr layout representation.

  A layout is represented by variable/value pairs
*/
//______________________________________________________________________________
class EXP msrLayout : public msrElement {
  public:

    static SMARTP<msrLayout> create();
    
    void  addmsrLilypondVarValAssoc (S_msrLilypondVarValAssoc assoc)
              { fMsrLilypondVarValAssocs.push_back(assoc); }
      
//    void addMsrSchemeVarValAssoc (S_msrSchemeVarValAssoc assoc)
//      { fMsrSchemeVarValAssocs.push_back(assoc); }
    
    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrLayout();
    virtual ~msrLayout();
  
  private:
  
    vector<S_msrLilypondVarValAssoc> fMsrLilypondVarValAssocs;
// JMI    vector<S_msrSchemeVarValAssoc>   fMsrSchemeVarValAssocs;
};
typedef SMARTP<msrLayout> S_msrLayout;

/*!
\brief A msr repeat representation.

  A repeat is represented by:
    - a sequence of elements for the common part
    - a vector os sequences of elements for the alternate endings
*/
//______________________________________________________________________________
class EXP msrRepeat: public msrElement {
  public:

    static SMARTP<msrRepeat> create();
    
    void    appendElementToCommonPart (S_msrElement elem)
              { fCommonPart->appendElementToSequence (elem); }
              
    void    appendElementToLastAlternateEnding  (S_msrElement elem)
              { fAlternateEndings.back()->appendElementToSequence (elem); }
                    
    void    appendNewAlternateEnding ()
              {
                fAlternateEndings.push_back(
                  msrSequence::create (msrSequence::kSpace));
              }

    void    setActuallyUsed ()
              { fActuallyUsed = true; }
    
    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrRepeat();
    virtual ~msrRepeat();
  
  private:
  
    S_msrSequence              fCommonPart;
    vector<S_msrSequence> fAlternateEndings;
    
    // the implicit msrRepeat is not used unless there are
    // actual repeats in the part
    bool                       fActuallyUsed;
};
typedef SMARTP<msrRepeat> S_msrRepeat;

/*!
\brief A msr barline representation.

  A barline is represented by the number of the next bar
*/
//______________________________________________________________________________
class EXP msrBarLine : public msrElement {
  public:
    
    static SMARTP<msrBarLine> create (int nextBarNumber);

    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrBarLine (int nextBarNumber);
    virtual ~msrBarLine();
  
  private:

    int fNextBarNumber;
};
typedef SMARTP<msrBarLine> S_msrBarLine;

/*!
\brief A msr comment representation.

  A comment is represented by its contents
*/
//______________________________________________________________________________
class EXP msrComment : public msrElement {
  public:
    
    enum GapKind { kGapAfterwards, kNoGapAfterwards };

    static SMARTP<msrComment> create (
              string contents, GapKind gapKind = kNoGapAfterwards);

    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrComment (
        string contents, GapKind gapKind = kNoGapAfterwards);
    virtual ~msrComment();
  
  private:

    string fContents;
    GapKind     fGapKind;
};
typedef SMARTP<msrComment> S_msrComment;

/*!
\brief A msr break representation.

  A break is represented by the number of the next bar
*/
//______________________________________________________________________________
class EXP msrBreak : public msrElement {
  public:
    
    static SMARTP<msrBreak> create(int nextBarNumber);

    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrBreak(int nextBarNumber);
    virtual ~msrBreak();
  
  private:

    int fNextBarNumber;
};
typedef SMARTP<msrBreak> S_msrBreak;

/*!
\brief A msr barnumbercheck representation.

  A barnumbercheck is represented by the number of the next bar
*/
//______________________________________________________________________________
class EXP msrBarNumberCheck : public msrElement {
  public:
    
    static SMARTP<msrBarNumberCheck> create(int nextBarNumber);

    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrBarNumberCheck(int nextBarNumber);
    virtual ~msrBarNumberCheck();
  
  private:

    int fNextBarNumber;
};
typedef SMARTP<msrBarNumberCheck> S_msrBarNumberCheck;

/*!
\brief A msr tuplet representation.

  A tuplet is represented by the number of actual notes and
  normal notes, i.e. a triplet is a tuplet with 3 actual notes
  played for the duration of 2 actual notes
*/
//______________________________________________________________________________
class EXP msrTuplet : public msrElement {
  public:
    
    static SMARTP<msrTuplet> create();

    enum TupletKind {
      kStartTuplet, kContinueTuplet, kStopTuplet, 
      k_NoTuplet };

    void updateTuplet (int number, int actualNotes, int normalNotes);
    
    int  getTupletNumber () const { return fTupletNumber; }

    void addElementToTuplet (S_msrElement elem)
            { fTupletContents.push_back(elem); }

    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrTuplet();
    virtual ~msrTuplet();
  
  private:

    int fTupletNumber;
    
    int fActualNotes;
    int fNormalNotes;
    
    vector<S_msrElement> fTupletContents;
};
typedef SMARTP<msrTuplet> S_msrTuplet;

/*!
\brief A msr beam representation.

  A beam is represented by a BeamKind value
*/
//______________________________________________________________________________
class EXP msrBeam : public msrElement {
  public:

    enum BeamKind {
            kBeginBeam, kContinueBeam, kEndBeam, 
            k_NoBeam };
    
    static SMARTP<msrBeam> create(int number, BeamKind kind);

    BeamKind getBeamKind () const { return fBeamKind; }

    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrBeam(int number, BeamKind kind);
    virtual ~msrBeam();
  
  private:

    int      fBeamNumber;
    BeamKind fBeamKind;
};
typedef SMARTP<msrBeam> S_msrBeam;

/*!
\brief A msr dynamics representation.

  A dynamics is represented by a DynamicsKind value
*/
//______________________________________________________________________________
class EXP msrDynamics : public msrElement {
  public:

    enum DynamicsKind {
          kF, kFF, kFFF, kFFFF, kFFFFF, kFFFFFF,
          kP, kPP, kPPP, kPPPP, kPPPPP, kPPPPPP,
          kMF, kMP, kFP, kFZ, kRF, kSF, kRFZ, kSFZ, kSFP, kSFPP, kSFFZ,
          k_NoDynamics };
    
    static SMARTP<msrDynamics> create(DynamicsKind kind);

    DynamicsKind getDynamicsKind () const { return fDynamicsKind; }

    string  dynamicsKindAsLilypondString ();
    
    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrDynamics(DynamicsKind kind);
    virtual ~msrDynamics();
  
  private:

    DynamicsKind fDynamicsKind;
};
typedef SMARTP<msrDynamics> S_msrDynamics;

/*!
\brief A msr wedge representation.

  A wedge is represented by a WedgeKind value (hairpins in LilyPond)
*/
//______________________________________________________________________________
class EXP msrWedge : public msrElement {
  public:

    enum WedgeKind { kCrescendoWedge, kDecrescendoWedge, kStopWedge };
    
    static SMARTP<msrWedge> create (WedgeKind kind);

    WedgeKind getWedgeKind () const        { return fWedgeKind; }

    string  wedgeKindAsString ();

    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrWedge (WedgeKind kind);
    virtual ~msrWedge();
  
  private:

    WedgeKind fWedgeKind;
};
typedef SMARTP<msrWedge> S_msrWedge;

/*!
\brief A msr clef representation.

  A clef is represented by its name
*/
//______________________________________________________________________________
class EXP msrClef : public msrElement {
  public:
    
    enum { kStandardLine, kTrebleStdLine=2, kBassStdLine=4, kCStdLine=3, kTabStdLine=5 };

    static SMARTP<msrClef> create (
      string sign,
      int    line,
      int    octaveChange);

    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrClef (
      string clefName,
      int    line,
      int    octaveChange);
    virtual ~msrClef();
  
  private:

    string fSign;
    int    fLine;
    int    fOctaveChange;
};
typedef SMARTP<msrClef> S_msrClef;

/*!
\brief A msr key representation.

  A key is represented by the tonic and the mode
*/
//______________________________________________________________________________
class EXP msrKey : public msrElement {
  public:
    
    enum KeyMode { kMajor, kMinor };

    static SMARTP<msrKey> create (
        int fifths, string mode, int cancel);

    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrKey (
        int fifths, string mode, int cancel);
    virtual ~msrKey();
  
  private:

    int     fFifths;
    string  fMode;
    int     fCancel;

    string  fTonic;
    KeyMode fKeyMode;
};
typedef SMARTP<msrKey> S_msrKey;

/*!
\brief A msr time representation.

  A time is represented by the numerator and denominator
*/
//______________________________________________________________________________
class EXP msrTime : public msrElement {
  public:
    
    static SMARTP<msrTime> create (
              int numerator, int denominator, bool generateNumericalTime);

    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrTime(int numerator, int denominator, bool generateNumericalTime);
    virtual ~msrTime();
  
  private:

    rational fRational;
    bool     fGenerateNumericalTime;
};
typedef SMARTP<msrTime> S_msrTime;

/*!
\brief A msr midi representation.

  A midi is represented by variable/value pairs

/*!
\brief A msr midi representation.

  A midi is represented by variable/value pairs

*/
//______________________________________________________________________________
class EXP msrMidi : public msrElement {
  public:

    static SMARTP<msrMidi> create();

    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrMidi();
    virtual ~msrMidi();
  
  private:
  
};
typedef SMARTP<msrMidi> S_msrMidi;

/*!
\brief A msr lyrics chunk representation.

  A lyrics chunk is represented by a type and and a string.
  In the case of "single", the list contains only one string
*/
//______________________________________________________________________________
class EXP msrLyricsChunk : public msrElement {
  public:

    // we want to end the line in the LilyPond code at a break
    enum LyricsChunkType {
      kSingleChunk, kBeginChunk, kMiddleChunk, kEndChunk,
      kSkipChunk, kBreakChunk };

    static SMARTP<msrLyricsChunk> create (
            LyricsChunkType chunkType,
            string          chunkText,
            S_msrDuration  msrDuration);
     
    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrLyricsChunk (
        LyricsChunkType chunkType,
        string          chunkText,
        S_msrDuration  msrDuration);
        
    virtual ~msrLyricsChunk();

  private:
  
    LyricsChunkType fLyricsChunkType;
    string          fChunkText;
    S_msrDuration  fChunkDuration;
};
typedef SMARTP<msrLyricsChunk> S_msrLyricsChunk;

/*!
\brief A msr lyrics representation.

  A lyrics is represented by a list of lyrics chunks,
*/
//______________________________________________________________________________
class EXP msrLyrics : public msrElement {
  public:

    static SMARTP<msrLyrics> create (
            S_translationSettings& ts,
            int                    lyricsNumber,
            S_msrVoice            lyricsVoice);
    
    int     getLyricsNumber () const
                { return fLyricsNumber; }
                
    string  getLyricsName () const;
                
    S_msrVoice
            getLyricsVoice () const
                { return fLyricsVoice; }
                
    vector<S_msrLyricsChunk>
            getLyricsChunks () const
                { return fLyricsChunks; }

    void    addTextChunkToLyrics (
              string          syllabic,
              string          text,
              bool            elision,
              S_msrDuration  duration);
      
    void    addSkipChunkToLyrics (
              S_msrDuration  duration);

    void    addBreakChunkToLyrics (
              int nextMeasureNumber);

    int     getLyricsTextPresent() { return fLyricsTextPresent; }
    
    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrLyrics (
        S_translationSettings& ts,
        int                    lyricsNumber,
        S_msrVoice            lyricsVoice);
    virtual ~msrLyrics();
  
  private:

    S_translationSettings     fTranslationSettings;

    int                       fLyricsNumber;
    S_msrVoice               fLyricsVoice;
    
    vector<S_msrLyricsChunk> fLyricsChunks;

    bool                      fLyricsTextPresent;
};
typedef SMARTP<msrLyrics> S_msrLyrics;
typedef map<int, S_msrLyrics> msrLyricsMap;

/*!
\brief A msr voice representation.

  A vpoce is represented by a its string contents
*/
//______________________________________________________________________________
class EXP msrVoice : public msrElement {
  public:

    static SMARTP<msrVoice> create (
            S_translationSettings& ts,
            int                    voiceNumber,
            int                    staffRelativeVoiceNumber,
            S_msrStaff             voiceStaff);
                          
    int     getVoiceNumber () const
                { return fVoiceNumber; }
                
    S_msrStaff
            getVoiceStaff () const
                { return fVoiceStaff; }
                
    msrLyricsMap
            getVoiceLyricsMap () const
                { return fVoiceLyricsMap; }

    string  getVoiceName () const;

    S_msrLyrics
            addLyricsToVoice (int lyricsNumber);
              
    S_msrLyrics
            voiceContainsLyrics (int lyricsNumber);
               
    void    appendElementToVoiceSequence (S_msrElement elem)
                { fVoiceSequence->appendElementToSequence (elem); }

    S_msrElement
            getVoiceSequenceLastElement ()
                { return fVoiceSequence->getLastElementOfSequence (); }
                  
    void    removeLastElementFromVoiceSequence ()
                { fVoiceSequence->removeLastElementFromSequence (); }

    void    removeElementFromVoiceSequence (S_msrElement elem)
                { fVoiceSequence->removeElementFromSequence (elem); }


    S_msrSequence
            getVoiceSequence () const
                { return fVoiceSequence; }

    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrVoice (
        S_translationSettings& ts,
        int                    voiceNumber,
        int                    staffRelativeVoiceNumber,
        S_msrStaff             voiceStaff);
    virtual ~msrVoice();
  
  private:

    S_translationSettings     fTranslationSettings;

    int                       fVoiceNumber;
    int                       fStaffRelativeVoiceNumber;
    S_msrStaff                fVoiceStaff;

    msrLyricsMap              fVoiceLyricsMap;

    // the implicit sequence containing the code generated for the voice
    S_msrSequence             fVoiceSequence;
  

    // the implicit repeat at the beginning of the voice
    // will be ignored if the voice has no repeats at all
    S_msrRepeat               fVoiceMsrRepeat;
};
typedef SMARTP<msrVoice> S_msrVoice;
typedef map<int, S_msrVoice> msrVoicesMap;

/*!
\brief A msr staff representation.

  A staff is represented by a its string contents
*/

/*
Staff assignment is only needed for music notated on multiple staves. Used by both notes and directions. Staff values are numbers, with 1 referring to the top-most staff in a part.
*/
//______________________________________________________________________________
class EXP msrStaff : public msrElement {
  public:

    static SMARTP<msrStaff> create (
        S_translationSettings& ts,
        int                    staffNumber,
        S_msrPart             staffPart);
    
    int     getStaffNumber () const
                { return fStaffNumber; }
                
    S_msrPart
            getStaffPart () const
                { return fStaffPart; }

    msrVoicesMap
            getStaffVoicesMap ()
                { return fStaffVoicesMap; }

    string  getStaffName () const;

    S_msrVoice
            addVoiceToStaff (int voiceNumber);
              
    S_msrVoice
            fetchVoiceFromStaff (int voiceNumber);
                              
    void    setStaffKey  (S_msrKey  key);
    void    setStaffTime (S_msrTime time);
    void    setStaffClef (S_msrClef clef);
              
    virtual void printMusicXML      (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrStaff (
        S_translationSettings& ts,
        int                    staffNumber,
        S_msrPart              staffPart);
    virtual ~msrStaff();
  
  private:

    S_translationSettings   fTranslationSettings;

    int                     fStaffNumber;
    S_msrPart               fStaffPart;

    msrVoicesMap            fStaffVoicesMap;

    string                  fStaffInstrumentName;

    S_msrKey                fStaffKey;
    S_msrTime               fStaffTime;
    S_msrClef               fStaffClef;

    int                     fNextRelativeStaffVoiceNumber;
};
typedef SMARTP<msrStaff> S_msrStaff;
typedef map<int, S_msrStaff> msrStaffsMap;

/*!
\brief A msr part representation.

  A part is represented by a its string contents
*/
//______________________________________________________________________________
class EXP msrPart : public msrElement {
  public:

    static SMARTP<msrPart> create (
              S_translationSettings& ts,
              string                 partMusicXMLName);

    void    setPartName (string partName)
                { fPartName = partName; }
                
    void    setPartAbbreviation (string partAbbreviation)
                { fPartAbbreviation = partAbbreviation; }
                
    void    setPartInstrumentName (string partInstrumentName)
                { fPartInstrumentName = partInstrumentName; }
                
    void    changePartMusicXMLName (
              string newPartMusicXMLName);
              
    string  getPartMusicXMLName () const
                { return fPartMusicXMLName; }
                
    msrStaffsMap
            getPartStavesMap ()
                { return fPartStavesMap; }

    string  getPartMSRName () const
                { return fPartMSRName; }

    string  getPartCombinedName () const
                { return
                    "\"" + fPartMusicXMLName + "\"" +
                    " (" + fPartMSRName + ")";
                }
                    
    void    setPartDivisions (int  musicXMLDivisions)
                { fPartMusicXMLDivisions = musicXMLDivisions; }
    
    void    setAllPartStavesKey  (S_msrKey  key);
              
    void    setAllPartStavesTime (S_msrTime time);
              
    void    setAllPartStavesClef (S_msrClef clef);
              
    S_msrStaff
            addStaffToPart (int staffNumber);

    S_msrStaff
            fetchStaffFromPart (int staffNumber);

    virtual void printMusicXML      (ostream& os);
    virtual void printMSR           (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrPart (
        S_translationSettings& ts,
        string                 partMusicXMLName);
    virtual ~msrPart();
  
  private:

    S_translationSettings   fTranslationSettings;

    string                  fPartMusicXMLName;
    msrStaffsMap            fPartStavesMap;

    string                  fPartMSRName; // coined in constructor

    string                  fPartName;
    string                  fPartAbbreviation;
    string                  fPartInstrumentName;

    int                     fPartMusicXMLDivisions;
};
typedef SMARTP<msrPart> S_msrPart;
typedef list<S_msrPart> msrPartsList;
typedef map<string, S_msrPart> msrPartsMap;

/*!
\brief A msr part group representation.

  A part group is represented by a its string contents
*/
//______________________________________________________________________________
class EXP msrPartGroup : public msrElement {
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

    enum PartGroupTypeKind {
        kStartPartGroupType, kStopPartGroupType,
        k_NoPartGroupType };
          
    enum PartGroupSymbolKind {
        kBracePartGroupSymbol, kBracketPartGroupSymbol,
         k_NoPartGroupSymbol };
          
    static SMARTP<msrPartGroup> create (
        S_translationSettings& ts,
        int                    partGroupNumber,
        PartGroupTypeKind      partPartGroupTypeKind,
        string                 partGroupName,
        string                 partGroupAbbreviation,
        PartGroupSymbolKind    partGroupSymbolKind,
        int                    partGroupSymbolDefaultX,
        bool                   partGroupBarline);

/*            
    string    getPartGroupName () const
                { return fPartGroupName; }

    string    getPartGroupAbbreviation () const
                { return fPartGroupAbbreviation; }

    string    getPartGroupSymbol () const
                { return fPartGroupSymbol; }

    string    getPartGroupBarline () const
                { return fPartGroupBarline; }
*/

    S_msrPart addPartToPartGroup (int partNumber);
    
    S_msrPart addPartToPartGroup (string partMusicXMLName);

    S_msrPart fetchPartFromPartGroup (string partMusicXMLName);

    /* JMI
     map<int, S_msrPart>
            getPartGroupPartsMap ()
                { return fPartGroupPartsMap; }
    */
                
    virtual void printMusicXML      (ostream& os);
    virtual void printMSR           (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrPartGroup (
        S_translationSettings& ts,
        int                    partGroupNumber,
        PartGroupTypeKind      partPartGroupTypeKind,
        string                 partGroupName,
        string                 partGroupAbbreviation,
        PartGroupSymbolKind    partGroupSymbolKind,
        int                    partGroupSymbolDefaultX,
        bool                   partGroupBarline);
            
    virtual ~msrPartGroup();
  
  private:

    S_msrPart tryAndReUseInitialAnonymousPart (
                string partMusicXMLName);

    S_translationSettings   fTranslationSettings;
    
    int                     fPartGroupNumber;
    PartGroupTypeKind       fPartPartGroupTypeKind;
        
    string                  fPartGroupName;
    string                  fPartGroupAbbreviation;

    PartGroupSymbolKind     fPartGroupSymbolKind;
    int                     fPartGroupSymbolDefaultX;

    bool                    fPartGroupBarline;

    msrPartsMap             fPartGroupPartsMap;
    msrPartsList            fPartGroupPartsList;
};
typedef SMARTP<msrPartGroup> S_msrPartGroup;
typedef list<S_msrPartGroup> msrPartGroupsList;
typedef map<int, S_msrPartGroup> msrPartGroupsMap;

/*!
\brief A msr score representation.

  A score is represented by a its string contents
*/
//______________________________________________________________________________
class EXP msrScore : public msrElement {
  public:

    static SMARTP<msrScore> create (
        S_translationSettings& ts);

    void addPartGroupToScore (S_msrPartGroup partGroup);

    virtual void printMusicXML      (ostream& os);
    virtual void printMSR           (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    msrScore (
        S_translationSettings& ts);
    virtual ~msrScore();
  
  private:

    S_translationSettings   fTranslationSettings;

    msrPartGroupsList       fPartGroupsList;
    
//    msrPartGroupsStack     fScorePartGroupsStack; JMI
//    msrPartGroupsMap        fScorePartGroupsMap;
};
typedef SMARTP<msrScore> S_msrScore;


/*! @} */

}


#endif
