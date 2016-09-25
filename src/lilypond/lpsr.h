/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __lpsr__
#define __lpsr__

#include <sstream>
#include <vector>
#include <string>
#include <list>
#include <map>

#include "smartpointer.h"
#include "rational.h"
#include "exports.h"
#include "typedefs.h"

#include "utilities.h"

namespace MusicXML2 
{

/*!
\addtogroup lpsr
@{
*/

/*
  The classes in this file implement the
  
              LPSR (LilyPond Semantic Representation)
              
    - the base class is lpsrElement
    - printStructure() methods produce a text of the lpsrElement tree
    - printLilypondCode() methods produce the LilyPond code
*/

class lpsrVisitor;

class lpsrElement;
class lpsrPart;
class lpsrLyrics;
class lpsrDuration;
class lpsrDynamics;
class lpsrWedge;
class lpsrChord;
class lpsrNote;

class lpsrPaper;
class lpsrLayout;

typedef SMARTP<lpsrElement>      SlpsrElement;
typedef SMARTP<lpsrPart>         SlpsrPart;
typedef SMARTP<lpsrLyrics>       SlpsrLyrics;
typedef SMARTP<lpsrDuration> SlpsrDuration;
typedef SMARTP<lpsrDynamics>     SlpsrDynamics;
typedef SMARTP<lpsrWedge>        SlpsrWedge;
typedef SMARTP<lpsrChord>        SlpsrChord;
typedef SMARTP<lpsrNote>         SlpsrNote;

typedef SMARTP<lpsrPaper>        SlpsrPaper;
typedef SMARTP<lpsrLayout>       SlpsrLayout;

EXP std::ostream& operator<< (std::ostream& os, const SlpsrElement& elt);
EXP std::ostream& operator<< (std::ostream& os, const SlpsrDuration& dur);
EXP std::ostream& operator<< (std::ostream& os, const SlpsrDynamics& dyn);
EXP std::ostream& operator<< (std::ostream& os, const SlpsrWedge& wdg);
EXP std::ostream& operator<< (std::ostream& os, const SlpsrChord& chrd);
EXP std::ostream& operator<< (std::ostream& os, const SlpsrNote& chrd);

EXP std::ostream& operator<< (std::ostream& os, const SlpsrPaper& chrd);
EXP std::ostream& operator<< (std::ostream& os, const SlpsrLayout& chrd);


/*!
\brief A generic lpsr element representation.

  An element is represented by its name and the
  list of its enclosed elements plus optional parameters.
*/
//______________________________________________________________________________
class EXP lpsrElement : public smartable {
  public:
 
    static SMARTP<lpsrElement> create(bool debug);
    
    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:
 
    static haendel hdl;
    
    lpsrElement(bool debug);
    virtual ~lpsrElement();

  private:
  
    bool fDebug;
};

/*!
\brief A lpsr note duration representation.

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
class EXP lpsrDuration : public lpsrElement {
  public:
  
    static SMARTP<lpsrDuration> create(int num, int denom, int dots=0);
    
    lpsrDuration(int num, int denom, int dots=0);
    virtual ~lpsrDuration();
    
    void scaleNumByFraction (int num, int denom);
    void sett (int num, int denom, int dots=0);
        
    lpsrDuration& operator= (const lpsrDuration& dur)
      {
        fNum=dur.fNum; fDenom=dur.fDenom; fDots=dur.fDots; 
        return *this;
      }
          
    bool operator!= (const lpsrDuration& dur) const 
      { 
        return
          (fNum!=dur.fNum) || (fDenom!=dur.fDenom) || (fDots!=dur.fDots);
      }
    
    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  private:

    int  fNum;
    int  fDenom;
    int  fDots;
};
typedef SMARTP<lpsrDuration> SlpsrDuration;

/*!
\brief A lpsr note representation.

  A note is represented by its name, optional accidentals,
    duration (in the form of numerator/denominator) and optional dots.
*/
//______________________________________________________________________________
class EXP lpsrNote : public lpsrElement {
  public:

    enum DiatonicPitch {
      kA, kB, kC, kD, kE, kF, kG, kRest, kNoDiatonicPitch};
    
    enum Alteration { // kDoubleFlat=-2 as in MusicXML, to help testing
      kDoubleFlat=-2, kFlat, kNatural, kSharp, kDoubleSharp, kNoAlteration};
        
    // the following is a series of Cs with increasing pitches:
    // \relative c'' { ceseh ces ceh c cih cis cisih }

    enum LpsrPitch {
      k_aeseh, k_aes, k_aeh, k_a, k_aih, k_ais, k_aisih,
      k_beseh, k_bes, k_beh, k_b, k_bih, k_bis, k_bisih, 
      k_ceseh, k_ces, k_ceh, k_c, k_cih, k_cis, k_cisih,
      k_deseh, k_des, k_deh, k_d, k_dih, k_dis, k_disih,
      k_eeseh, k_ees, k_eeh, k_e, k_eih, k_eis, k_eisih, 
      k_feseh, k_fes, k_feh, k_f, k_fih, k_fis, k_fisih,
      k_geseh, k_ges, k_geh, k_g, k_gih, k_gis, k_gisih,
      k_NoLpsrPitch};
      
    static SMARTP<lpsrNote> create();// JMI  Note note, int voice) 

    // for regular notes
    void updateNote( 
          bool              currentStepIsRest,
          DiatonicPitch     diatonicNote,
          Alteration        alteration,
          int               octave,
          SlpsrDuration     dur,
          LpsrPitch         lpsrPitch,
          int               voice,
          bool              noteBelongsToAChord);
    
    // for tuplet elements
    void updateNoteDuration (int actualNotes, int normalNotes);
          
    void setNoteBelongsToAChord ();
    
    void              addDynamics (SlpsrDynamics dyn);
    void              addWedge    (SlpsrWedge    wdg);

    SlpsrDynamics     removeFirstDynamics ();
    SlpsrWedge        removeFirstWedge ();

    std::list<SlpsrDynamics> getNoteDynamics () { return fNoteDynamics; };
    std::list<SlpsrWedge>    getNoteWedges   () { return fNoteWedges; };

    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:
 
    lpsrNote();
    
    virtual ~lpsrNote();
    
  private:

    // MusicXML informations
    bool                 fCurrentStepIsRest;
    DiatonicPitch        fDiatonicPitch;
    Alteration           fAlteration;
    int                  fOctave;

    // MusicXML durations are in in divisions per quarter note,
    // LilyPond durations are in whole notes, hence the "*4" multiplication
    SlpsrDuration        fLpsrDuration;

    // LilyPond informations
    LpsrPitch            fLpsrPitch;
    
    std::list<SlpsrDynamics> fNoteDynamics;
    std::list<SlpsrWedge>    fNoteWedges;
    
    bool                 fNoteBelongsToAChord;

    int                  fVoice;
};
typedef SMARTP<lpsrNote> SlpsrNote;

/*!
\brief The lpsr parallel element
*/
//______________________________________________________________________________
class EXP lpsrParallel : public lpsrElement {
  public:
    
    enum ElementsSeparator { kEndOfLine, kSpace };

    static SMARTP<lpsrParallel> create(ElementsSeparator elementsSeparator);

    void         addElementToParallel (SlpsrElement elem) { fParallelElements.push_back(elem); }
    SlpsrElement getLastElementOfParallel() { return fParallelElements.back(); }
    void         removeLastElementOfParallel () { fParallelElements.pop_back(); }

    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:

    lpsrParallel(ElementsSeparator elementsSeparator);
    virtual ~lpsrParallel();
    
  private:
  
    std::vector<SlpsrElement> fParallelElements;
    ElementsSeparator         fElementsSeparator;

};
typedef SMARTP<lpsrParallel> SlpsrParallel;

/*!
\brief The lpsr sequence element
*/
//______________________________________________________________________________
class EXP lpsrSequence : public lpsrElement {
  public:
    
    enum ElementsSeparator { kEndOfLine, kSpace };

    static SMARTP<lpsrSequence> create(ElementsSeparator elementsSeparator);

    void         appendElementToSequence (SlpsrElement elem) { fSequenceElements.push_back(elem); }
    SlpsrElement getLastElementOfSequence() { return fSequenceElements.back(); }
    void         removeLastElementOfSequence () { fSequenceElements.pop_back(); }

    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:

    lpsrSequence(ElementsSeparator elementsSeparator);
    virtual ~lpsrSequence();
    
  private:
  
    std::vector<SlpsrElement> fSequenceElements;
    ElementsSeparator         fElementsSeparator;

};
typedef SMARTP<lpsrSequence> SlpsrSequence;

/*!
\brief The lpsr chord element
*/
//______________________________________________________________________________
class EXP lpsrChord : public lpsrElement {
  public:

    static SMARTP<lpsrChord> create(SlpsrDuration chordduration);
    
    void addNoteToChord (SlpsrNote note) { fChordNotes.push_back(note); }

    void addDynamics (SlpsrDynamics dyn);
    void addWedge    (SlpsrWedge    wdg);

    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:

    lpsrChord (SlpsrDuration chordduration);
    virtual ~lpsrChord();
  
  private:
  
    std::vector<SlpsrNote>   fChordNotes;
    
    SlpsrDuration            fChordDuration;

    
    std::list<SlpsrDynamics> fChordDynamics;
    std::list<SlpsrWedge>    fChordWedges;
};
typedef SMARTP<lpsrChord> SlpsrChord;

/*!
\brief A lpsr variable/value association representation.
*/
//______________________________________________________________________________
class EXP lpsrVariableValueAssociation : public lpsrElement {
  public:

    enum VarValSeparator   { kSpace, kEqualSign };
    enum QuotesKind        { kQuotesAroundValue, kNoQuotesAroundValue };
    enum CommentedKind     { kCommented, kUncommented };

    static SMARTP<lpsrVariableValueAssociation> create(
              std::string     variableName,
              std::string     value, 
              VarValSeparator varValSeparator,
              QuotesKind      quotesKind,
              CommentedKind   commentKind );
    
    void    changeAssociation (std::string value);
    
    std::string getVariableValue () const { return fVariableValue; };

    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:

    lpsrVariableValueAssociation(
              std::string     variableName,
              std::string     value, 
              VarValSeparator varValSeparator,
              QuotesKind      quotesKind,
              CommentedKind   commentedKind );
    virtual ~lpsrVariableValueAssociation();
  
  private:

    std::string     fVariableName;
    std::string     fVariableValue;
    VarValSeparator fVarValSeparator;
    QuotesKind      fQuotesKind;
    CommentedKind   fCommentedKind;
};
typedef SMARTP<lpsrVariableValueAssociation> SlpsrVariableValueAssociation;

/*!
\brief A lpsr Scheme variable/value association representation.
*/
//______________________________________________________________________________
class EXP lpsrSchemeVariableValueAssociation : public lpsrElement {
  public:

    enum CommentedKind     { kCommented, kUncommented };

    static SMARTP<lpsrSchemeVariableValueAssociation> create(
              std::string     variableName,
              std::string     value, 
              CommentedKind   commentKind );
    
    void    changeAssociation (std::string value);
    
    std::string getVariableValue () const { return fVariableValue; };

    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:

    lpsrSchemeVariableValueAssociation(
              std::string     variableName,
              std::string     value, 
              CommentedKind   commentedKind );
    virtual ~lpsrSchemeVariableValueAssociation();
  
  private:

    std::string     fVariableName;
    std::string     fVariableValue;
    CommentedKind   fCommentedKind;
};
typedef SMARTP<lpsrSchemeVariableValueAssociation> SlpsrSchemeVariableValueAssociation;

/*!
\brief A lpsr header representation.

  A header is represented by variable/value pairs
*/
//______________________________________________________________________________
class EXP lpsrHeader : public lpsrElement {
  public:

    static SMARTP<lpsrHeader> create();
    
    void                    setScorePartwise   (S_score_partwise val)   { fScorePartwise = val; }
    S_score_partwise        getScorePartwise   () const                 { return fScorePartwise; }

    void                    setWorkNumber      (S_work_number val)      { fWorkNumber = val; }
    S_work_number           getWorkNumber      () const                 { return fWorkNumber; }

    void                    setWorkTitle       (S_work_title val)       { fWorkTitle = val; }
    S_work_title            getWorkTitle       () const                 { return fWorkTitle; }

    void                    setMovementNumber  (S_movement_number val)  { fMovementNumber = val; }
    S_movement_number       getMovementNumber  () const                 { return fMovementNumber; }

    void                    setMovementTitle   (S_movement_title val)   { fMovementTitle = val; }
    S_movement_title        getMovementTitle   () const                 { return fMovementTitle; }

    void                    addCreator         (S_creator val)          { fCreators.push_back(val); }
    std::vector<S_creator>  getCreators        () const                 { return fCreators; };

    void                    setRights          (S_rights val)           { fRights = val; }
    S_rights                getRights          () const                 { return fRights; }

    void                    addSoftware        (S_software val)         { fSoftwares.push_back(val); }
    std::vector<S_software> getSoftwares       () const                 { return fSoftwares; };

    void                    setEncodingDate    (S_encoding_date val)    { fEncodingDate = val; }
    S_encoding_date         getEncodingDate    () const                 { return fEncodingDate; }

    void                    setScoreInstrument (S_score_instrument val) { fScoreInstrument = val; }
    S_score_instrument      getScoreInstrument () const                 { return fScoreInstrument; }

    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:

    lpsrHeader();
    virtual ~lpsrHeader();
  
  private:

    S_score_partwise        fScorePartwise; // may contain MusicXML version
    S_work_number           fWorkNumber;
    S_work_title            fWorkTitle;
    S_movement_number       fMovementNumber;
    S_movement_title        fMovementTitle;
    std::vector<S_creator>  fCreators;
    S_rights                fRights;
    std::vector<S_software> fSoftwares;
    S_encoding_date         fEncodingDate;
    S_score_instrument      fScoreInstrument;

};
typedef SMARTP<lpsrHeader> SlpsrHeader;

/*!
\brief A lpsr paper representation.

  A paper is represented by variable/value pairs
*/
//______________________________________________________________________________
class EXP lpsrPaper : public lpsrElement {
  public:

    static SMARTP<lpsrPaper> create();
    
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

    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:

    lpsrPaper();
    virtual ~lpsrPaper();
  
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
typedef SMARTP<lpsrPaper> SlpsrPaper;

/*!
\brief A lpsr layout representation.

  A layout is represented by variable/value pairs
*/
//______________________________________________________________________________
class EXP lpsrLayout : public lpsrElement {
  public:

    static SMARTP<lpsrLayout> create();
    
    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:

    lpsrLayout();
    virtual ~lpsrLayout();
  
  private:
};
typedef SMARTP<lpsrLayout> SlpsrLayout;

/*!
\brief A lpsr part representation.

  A part is represented by a its string contents
*/
//______________________________________________________________________________
class EXP lpsrPart : public lpsrElement {
  public:

    static SMARTP<lpsrPart> create(
      std::string name, bool absoluteCode, bool generateNumericalTime);
    
    std::string   getPartName () const         { return fPartName; }
    bool          getAbsoluteCode () const     { return fPartAbsoluteCode; }
    SlpsrSequence getPartLpsrsequence () const { return fPartLpsrSequence; }

    void          addLyricsToPart (SlpsrLyrics lyr) { fPartLyrics.push_back(lyr); }

    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:

    lpsrPart(std::string name, bool absoluteCode, bool generateNumericalTime);
    virtual ~lpsrPart();
  
  private:

    std::string       fPartName;
    bool              fPartAbsoluteCode;
    bool              fGenerateNumericalTime;
    
    // the implicit sequence containing the code generated for the part
    SlpsrSequence     fPartLpsrSequence;
    
    // the lyrics stanzas, if any, associated with the part
    std::vector<SlpsrLyrics> fPartLyrics;
};
typedef SMARTP<lpsrPart> SlpsrPart;
typedef std::map<std::string, SlpsrPart> lpsrPartsmap;

/*!
\brief A lpsr barline representation.

  A barline is represented by the number of the next bar
*/
//______________________________________________________________________________
class EXP lpsrBarLine : public lpsrElement {
  public:
    
    static SMARTP<lpsrBarLine> create(int nextBarNumber);

    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:

    lpsrBarLine(int nextBarNumber);
    virtual ~lpsrBarLine();
  
  private:

    int fNextBarNumber;
};
typedef SMARTP<lpsrBarLine> SlpsrBarLine;

/*!
\brief A lpsr comment representation.

  A comment is represented by its contents
*/
//______________________________________________________________________________
class EXP lpsrComment : public lpsrElement {
  public:
    
    enum GapKind { kGapAfterwards, kNoGapAfterwards };

    static SMARTP<lpsrComment> create(std::string contents, GapKind gapKind = kNoGapAfterwards);

    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:

    lpsrComment(std::string contents, GapKind gapKind = kNoGapAfterwards);
    virtual ~lpsrComment();
  
  private:

    std::string fContents;
    GapKind     fGapKind;
};
typedef SMARTP<lpsrComment> SlpsrComment;

/*!
\brief A lpsr break representation.

  A break is represented by the number of the next bar
*/
//______________________________________________________________________________
class EXP lpsrBreak : public lpsrElement {
  public:
    
    static SMARTP<lpsrBreak> create(int nextBarNumber);

    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:

    lpsrBreak(int nextBarNumber);
    virtual ~lpsrBreak();
  
  private:

    int fNextBarNumber;
};
typedef SMARTP<lpsrBreak> SlpsrBreak;

/*!
\brief A lpsr barnumbercheck representation.

  A barnumbercheck is represented by the number of the next bar
*/
//______________________________________________________________________________
class EXP lpsrBarNumberCheck : public lpsrElement {
  public:
    
    static SMARTP<lpsrBarNumberCheck> create(int nextBarNumber);

    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:

    lpsrBarNumberCheck(int nextBarNumber);
    virtual ~lpsrBarNumberCheck();
  
  private:

    int fNextBarNumber;
};
typedef SMARTP<lpsrBarNumberCheck> SlpsrBarNumberCheck;

/*!
\brief A lpsr tuplet representation.

  A tuplet is represented by the number of actual notes and
  normal notes, i.e. a triplet is a tuplet with 3 actual notes
  played for the duration of 2 actual notes
*/
//______________________________________________________________________________
class EXP lpsrTuplet : public lpsrElement {
  public:
    
    static SMARTP<lpsrTuplet> create();

    enum TupletKind {
      kStartTuplet, kContinueTuplet, kStopTuplet, 
      k_NoTuplet };

    void updateTuplet (int number, int actualNotes, int normalNotes);
    
    int getTupletNumber () const { return fTupletNumber; }

    void addElementToTuplet (SlpsrElement elem) { fTupletContents.push_back(elem); }

    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:

    lpsrTuplet();
    virtual ~lpsrTuplet();
  
  private:

    int fTupletNumber;
    
    int fActualNotes;
    int fNormalNotes;
    
    std::vector<SlpsrElement> fTupletContents;
};
typedef SMARTP<lpsrTuplet> SlpsrTuplet;

/*!
\brief A lpsr beam representation.

  A beam is represented by a BeamKind value
*/
//______________________________________________________________________________
class EXP lpsrBeam : public lpsrElement {
  public:

    enum BeamKind {
            kBeginBeam, kContinueBeam, kEndBeam, 
            k_NoBeam };
    
    static SMARTP<lpsrBeam> create(int number, BeamKind kind);

    BeamKind getBeamKind () const { return fBeamKind; }

    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:

    lpsrBeam(int number, BeamKind kind);
    virtual ~lpsrBeam();
  
  private:

    int      fBeamNumber;
    BeamKind fBeamKind;
};
typedef SMARTP<lpsrBeam> SlpsrBeam;

/*!
\brief A lpsr dynamics representation.

  A dynamics is represented by a DynamicsKind value
*/
//______________________________________________________________________________
class EXP lpsrDynamics : public lpsrElement {
  public:

    enum DynamicsKind {
            kFDynamics, kPDynamics, 
            k_NoDynamics };
    
    static SMARTP<lpsrDynamics> create(DynamicsKind kind);

    DynamicsKind getDynamicsKind () const { return fDynamicsKind; }

    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:

    lpsrDynamics(DynamicsKind kind);
    virtual ~lpsrDynamics();
  
  private:

    DynamicsKind fDynamicsKind;
};
typedef SMARTP<lpsrDynamics> SlpsrDynamics;

/*!
\brief A lpsr wedge representation.

  A wedge is represented by a WedgeKind value (hairpins in LilyPond)
*/
//______________________________________________________________________________
class EXP lpsrWedge : public lpsrElement {
  public:

    enum WedgeKind { kCrescendoWedge, kDecrescendoWedge, kStopWedge };
    
    static SMARTP<lpsrWedge> create(WedgeKind kind);

    WedgeKind getWedgeKind () const        { return fWedgeKind; }

    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:

    lpsrWedge(WedgeKind kind);
    virtual ~lpsrWedge();
  
  private:

    WedgeKind fWedgeKind;
};
typedef SMARTP<lpsrWedge> SlpsrWedge;

/*!
\brief A lpsr lyrics representation.

  A lyrics is represented by a its string contents
*/
//______________________________________________________________________________
class EXP lpsrLyrics : public lpsrElement {
  public:

    static SMARTP<lpsrLyrics> create(std::string name, std::string contents);
    
    std::string getContents () const { return fLyricsContents; }

    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:

    lpsrLyrics(std::string name, std::string contents);
    virtual ~lpsrLyrics();
  
  private:

    std::string fLyricsName;
    std::string fLyricsContents;
};
typedef SMARTP<lpsrLyrics> SlpsrLyrics;



/*!
\brief A lpsr time representation.

  A time is represented by the numerator and denominator
*/
//______________________________________________________________________________
class EXP lpsrTime : public lpsrElement {
  public:
    
    static SMARTP<lpsrTime> create(int numerator, int denominator, bool generateNumericalTime);

    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:

    lpsrTime(int numerator, int denominator, bool generateNumericalTime);
    virtual ~lpsrTime();
  
  private:

    rational fRational;
    bool     fGenerateNumericalTime;
};
typedef SMARTP<lpsrTime> SlpsrTime;

/*!
\brief A lpsr clef representation.

  A clef is represented by its name
*/
//______________________________________________________________________________
class EXP lpsrClef : public lpsrElement {
  public:
    
    static SMARTP<lpsrClef> create(std::string clefName);

    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:

    lpsrClef(std::string clefName);
    virtual ~lpsrClef();
  
  private:

    std::string fClefName;
};
typedef SMARTP<lpsrClef> SlpsrClef;

/*!
\brief A lpsr key representation.

  A key is represented by the tonic and the mode
*/
//______________________________________________________________________________
class EXP lpsrKey : public lpsrElement {
  public:
    
    enum KeyMode { kMajor, kMinor };

    static SMARTP<lpsrKey> create(std::string tonic, KeyMode keyMode);

    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:

    lpsrKey(std::string tonic, KeyMode keyMode);
    virtual ~lpsrKey();
  
  private:

    std::string   fTonic;
    KeyMode       fKeyMode;
};
typedef SMARTP<lpsrKey> SlpsrKey;

/*!
\brief A lpsr midi representation.

  A midi is represented by variable/value pairs

*/
//______________________________________________________________________________
class EXP lpsrMidi : public lpsrElement {
  public:

    static SMARTP<lpsrMidi> create();

    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:

    lpsrMidi();
    virtual ~lpsrMidi();
  
  private:
  
};
typedef SMARTP<lpsrMidi> SlpsrMidi;

/*!
\brief A lpsr score representation.

  A score is represented by parallel music, score layout and midi
*/
//______________________________________________________________________________
class EXP lpsrScore : public lpsrElement {
  public:

    static SMARTP<lpsrScore> create();
     
    SlpsrParallel getScoreParallelMusic () const { return fScoreParallelMusic; }
    SlpsrLayout   getScoreLayout        () const { return fScoreLayout; }
    SlpsrMidi     getScoreMidi          () const { return fScoreMidi; }

    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:

    lpsrScore();
    virtual ~lpsrScore();
  
  private:
  
    SlpsrParallel fScoreParallelMusic;
    SlpsrLayout   fScoreLayout;
    SlpsrMidi     fScoreMidi;    
};
typedef SMARTP<lpsrScore> SlpsrScore;

/*!
\brief A lpsr new staff representation.

  A new staff is represented by a sequence of elements
*/
//______________________________________________________________________________
class EXP lpsrNewstaffCommand : public lpsrElement {
  public:

    static SMARTP<lpsrNewstaffCommand> create();
     
    void addElementToNewStaff (SlpsrElement elem) { fNewStaffElements.push_back(elem); }

    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:

    lpsrNewstaffCommand();
    virtual ~lpsrNewstaffCommand();
  
  private:
  
    std::vector<SlpsrElement> fNewStaffElements;
};
typedef SMARTP<lpsrNewstaffCommand> SlpsrNewstaffCommand;

/*!
\brief A lpsr new lyrics representation.

  A new lyrics is represented by a sequence of strings
*/
//______________________________________________________________________________
class EXP lpsrNewlyricsCommand : public lpsrElement {
  public:

    static SMARTP<lpsrNewlyricsCommand> create();
     
    void addElementToNewStaff (SlpsrElement elem) { fNewStaffElements.push_back(elem); }

    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:

    lpsrNewlyricsCommand();
    virtual ~lpsrNewlyricsCommand();
  
  private:
  
    std::vector<SlpsrElement> fNewStaffElements;
};
typedef SMARTP<lpsrNewlyricsCommand> SlpsrNewlyricsCommand;

/*!
\brief A lpsr new lyrics representation.

  A new lyrics is represented by a sequence of strings
*/
//______________________________________________________________________________
class EXP lpsrVariableUseCommand : public lpsrElement {
  public:

    static SMARTP<lpsrVariableUseCommand> create(std::string variableName);
    
    virtual void printStructure (std::ostream& os);
    virtual void printLilyPondCode (std::ostream& os);

  protected:

    lpsrVariableUseCommand(std::string variableName);
    virtual ~lpsrVariableUseCommand();
  
  private:
  
    std::string fVariableName;
};
typedef SMARTP<lpsrVariableUseCommand> SlpsrVariableUseCommand;


/*! @} */

}


#endif
