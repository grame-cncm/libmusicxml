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

#include "lpsrUtilities.h"
#include "musicxml2lpsr.h"

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
    - the lpsrElement tree can be converted to:
        - a text description with         printLpsrStructure()
        - a MusicXML text equivalent with printMusicXML()
        - LilyPond source code with       printLilypondCode()
*/

class lpsrVisitor;

class lpsrElement;

class lpsrLilypondVarValAssoc;
class lpsrSchemeVarValAssoc;

class lpsrPart;
class lpsrLyrics;
class lpsrDuration;
class lpsrDynamics;
class lpsrWedge;
class lpsrChord;
class lpsrNote;

class lpsrPaper;
class lpsrLayout;

typedef SMARTP<lpsrElement>             S_lpsrElement;

typedef SMARTP<lpsrLilypondVarValAssoc> S_lpsrLilypondVarValAssoc;
typedef SMARTP<lpsrSchemeVarValAssoc>   S_lpsrSchemeVarValAssoc;

typedef SMARTP<lpsrPart>                S_lpsrPart;
typedef SMARTP<lpsrLyrics>              S_lpsrLyrics;
typedef SMARTP<lpsrDuration>            S_lpsrDuration;
typedef SMARTP<lpsrDynamics>            S_lpsrDynamics;
typedef SMARTP<lpsrWedge>               S_lpsrWedge;
typedef SMARTP<lpsrChord>               S_lpsrChord;
typedef SMARTP<lpsrNote>                S_lpsrNote;

typedef SMARTP<lpsrPaper>               S_lpsrPaper;
typedef SMARTP<lpsrLayout>              S_lpsrLayout;

EXP std::ostream& operator<< (std::ostream& os, const S_lpsrElement& elt);

EXP std::ostream& operator<< (std::ostream& os, const S_lpsrLilypondVarValAssoc& elt);
EXP std::ostream& operator<< (std::ostream& os, const S_lpsrSchemeVarValAssoc& elt);

EXP std::ostream& operator<< (std::ostream& os, const S_lpsrDuration& dur);
EXP std::ostream& operator<< (std::ostream& os, const S_lpsrDynamics& dyn);
EXP std::ostream& operator<< (std::ostream& os, const S_lpsrWedge& wdg);
EXP std::ostream& operator<< (std::ostream& os, const S_lpsrChord& chrd);
EXP std::ostream& operator<< (std::ostream& os, const S_lpsrNote& chrd);

EXP std::ostream& operator<< (std::ostream& os, const S_lpsrPaper& chrd);
EXP std::ostream& operator<< (std::ostream& os, const S_lpsrLayout& chrd);


/*!
\brief Global variables.

  An global variable is implemented as a static member of this class.
*/
//______________________________________________________________________________
class EXP lpsrGlobalVariables {
  public:
    
    enum CodeGenerationKind {
      kLpsrStructure, kMusicXML, kLilypondCode};

    static CodeGenerationKind getCodeGenerationKind ()
      { return sCodeGenerationKind; }
      
    static void setCodeGenerationKind (CodeGenerationKind kind)
      { sCodeGenerationKind = kind; }
  
  private:
  
    static CodeGenerationKind sCodeGenerationKind;
};

/*!
\brief A generic lpsr element representation.

  An element is represented by its name and the
  list of its enclosed elements plus optional parameters.
*/
//______________________________________________________________________________
class EXP lpsrElement : public smartable {
  public:
 
    static SMARTP<lpsrElement> create(bool debug);
    
    virtual void print              (std::ostream& os);

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);
    
    static indenter idtr;

  protected:
     
    lpsrElement(bool debug);
    virtual ~lpsrElement();

  private:
  
    bool fDebug;
};
typedef SMARTP<lpsrElement> S_lpsrElement;

/*!
\brief A note description for MusicXML.
*/
//______________________________________________________________________________
class musicXMLNoteData {
  public:
  
     virtual void print              (std::ostream& os);
 
  public:
  
    char        fMusicxmlStep;
    bool        fMusicxmlStepIsARest;
    int         fMusicxmlAlteration;
    int         fMusicxmlOctave;
    int         fMusicxmlDivisions;
    int         fMusicxmlDuration;
    int         fDotsNumber;
    
    bool        fNoteBelongsToAChord;
    
    bool        fNoteBelongsToATuplet;
    std::string fTupletMemberType;

    int         fVoiceNumber;
};
  
/*!
\brief A lpsr absolute octave representation.
*/
//______________________________________________________________________________
class EXP lpsrAbsoluteOctave : public lpsrElement {
  public:
  
    static SMARTP<lpsrAbsoluteOctave> create(int musicxmlOctave);
    
    lpsrAbsoluteOctave(int musicxmlOctave);
    virtual ~lpsrAbsoluteOctave();
    
    lpsrAbsoluteOctave& operator= (const lpsrAbsoluteOctave& absOct)
      {
        fLpsrOctave = absOct.fLpsrOctave;
        return *this;
      }
          
    bool operator!= (const lpsrAbsoluteOctave& otherAbsOct) const 
      { 
        return fLpsrOctave != otherAbsOct.fLpsrOctave;
      }
    
    std::string  absoluteOctaveAsLilypondString ();

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  private:

    int  fLpsrOctave;
};
typedef SMARTP<lpsrAbsoluteOctave> S_lpsrAbsoluteOctave;

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
  
    static SMARTP<lpsrDuration> create (
        int num,
        int denom,
        int dots,
        std::string tupletMemberType);
    
    lpsrDuration(
        int num,
        int denom,
        int dots,
        std::string tupletMemberType);
    virtual ~lpsrDuration();
    
    void scaleNumByFraction (int num, int denom);
        
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
    
    std::string  durationAsLilypondString ();

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  private:

    int         fNum;
    int         fDenom;
    int         fDots;
    std::string fTupletMemberType;
};
typedef SMARTP<lpsrDuration> S_lpsrDuration;

/*!
\brief A lpsr note representation.

  A note is represented by its name, optional accidentals,
    duration (in the form of numerator/denominator) and optional dots.
*/
//______________________________________________________________________________
class EXP lpsrNote : public lpsrElement {
  public:

    enum MusicXMLDiatonicPitch {
      kA, kB, kC, kD, kE, kF, kG, kRest, k_NoDiatonicPitch};
    
    enum MusicXMLAlteration {
      // kDoubleFlat=-2 as in MusicXML, to faciliting testing
      kDoubleFlat=-2, kFlat, kNatural, kSharp, kDoubleSharp, k_NoAlteration};
            
    // for standalone notes
    static SMARTP<lpsrNote> createFromMusicXMLData (
                              S_translationSettings& ts,
                              musicXMLNoteData&      mxmldat);
    
    // for chord members
    void setNoteBelongsToAChord ();
    
    // we use dutch pitches names for the enumeration below
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
    
    LpsrPitch computeNoteLpsrPitch (
                int                          noteQuatertonesFromA,
                lpsrNote::MusicXMLAlteration alteration);
                          
    static std::map<LpsrPitch, std::string> sDutchLilypondPitches;

    S_lpsrDuration getNoteLpsrDuration () { return fNoteLpsrDuration; }   

     std::string  notePitchAsLilypondString ();
    
   // dynamics and wedges
    void              addDynamics (S_lpsrDynamics dyn);
    void              addWedge    (S_lpsrWedge    wdg);

    std::list<S_lpsrDynamics> getNoteDynamics () { return fNoteDynamics; };
    std::list<S_lpsrWedge>    getNoteWedges   () { return fNoteWedges; };

    S_lpsrDynamics     removeFirstDynamics ();
    S_lpsrWedge        removeFirstWedge ();

//    void octaveRelativeTo (const lpsrAbsoluteOctave& otherAbsOct);
        
    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:
 
    lpsrNote (
      S_translationSettings& ts,
      musicXMLNoteData&      mxmldat);
    
    virtual ~lpsrNote();
    
  private:
  
    S_translationSettings     fTranslationSettings;

    // MusicXML informations
    musicXMLNoteData          fMusicXMLNoteData;
    MusicXMLDiatonicPitch     fMusicXMLDiatonicPitch;

 //   bool                     fCurrentStepIsARest;
 //   MusicXMLAlteration       fMusicXMLAlteration;
  //  int                      fMusicXMLOctave;

    // MusicXML durations are in in divisions per quarter note,
    // LilyPond durations are in whole notes, hence the "*4" multiplication
    S_lpsrDuration            fNoteLpsrDuration;

    // LilyPond informations
    LpsrPitch                 fNoteLpsrPitch;
    
    std::list<S_lpsrDynamics> fNoteDynamics;
    std::list<S_lpsrWedge>    fNoteWedges;
};
typedef SMARTP<lpsrNote> S_lpsrNote;

/*!
\brief The lpsr parallel music element
*/
//______________________________________________________________________________
class EXP lpsrParallelMusic : public lpsrElement {
  public:
    
    enum ElementsSeparator { kEndOfLine, kSpace };

    static SMARTP<lpsrParallelMusic> create(ElementsSeparator elementsSeparator);

    void         addElementToParallelMusic (S_lpsrElement elem) { fParallelMusicElements.push_back(elem); }
    S_lpsrElement getLastElementOfParallelMusic() { return fParallelMusicElements.back(); }
    void         removeLastElementOfParallelMusic () { fParallelMusicElements.pop_back(); }

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrParallelMusic(ElementsSeparator elementsSeparator);
    virtual ~lpsrParallelMusic();
    
  private:
  
    std::vector<S_lpsrElement> fParallelMusicElements;
    ElementsSeparator         fElementsSeparator;

};
typedef SMARTP<lpsrParallelMusic> S_lpsrParallelMusic;

/*!
\brief The lpsr sequence element
*/
//______________________________________________________________________________
class EXP lpsrSequence : public lpsrElement {
  public:
    
    enum ElementsSeparator { kEndOfLine, kSpace };

    static SMARTP<lpsrSequence> create(ElementsSeparator elementsSeparator);

    void         prependElementToSequence (S_lpsrElement elem) { fSequenceElements.push_front(elem); }
    void         appendElementToSequence  (S_lpsrElement elem) { fSequenceElements.push_back(elem); }
    
    S_lpsrElement getLastElementOfSequence() { return fSequenceElements.back(); }
    void         removeLastElementOfSequence () { fSequenceElements.pop_back(); }

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrSequence(ElementsSeparator elementsSeparator);
    virtual ~lpsrSequence();
    
  private:
  
    std::list<S_lpsrElement> fSequenceElements;
    ElementsSeparator       fElementsSeparator;

};
typedef SMARTP<lpsrSequence> S_lpsrSequence;

/*!
\brief The lpsr chord element
*/
//______________________________________________________________________________
class EXP lpsrChord : public lpsrElement {
  public:

    static SMARTP<lpsrChord> create(S_lpsrDuration chordDuration);
    
    void addNoteToChord (S_lpsrNote note) { fChordNotes.push_back(note); }

    void addDynamics (S_lpsrDynamics dyn);
    void addWedge    (S_lpsrWedge    wdg);

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrChord (S_lpsrDuration chordDuration);
    virtual ~lpsrChord();
  
  private:
  
    std::vector<S_lpsrNote>   fChordNotes;
    
    S_lpsrDuration            fChordDuration;

    
    std::list<S_lpsrDynamics> fChordDynamics;
    std::list<S_lpsrWedge>    fChordWedges;
};
typedef SMARTP<lpsrChord> S_lpsrChord;

/*!
\brief A lpsr LilyPond variable/value association representation.
*/
//______________________________________________________________________________
class EXP lpsrLilypondVarValAssoc : public lpsrElement {
  public:

    enum VarValSeparator   { kSpace, kEqualSign };
    enum QuotesKind        { kQuotesAroundValue, kNoQuotesAroundValue };
    enum CommentedKind     { kCommented, kUncommented };

    static SMARTP<lpsrLilypondVarValAssoc> create(
              std::string     variableName,
              std::string     value, 
              VarValSeparator varValSeparator,
              QuotesKind      quotesKind,
              CommentedKind   commentKind,
              std::string     unit = "");
    
    void    changeAssoc (std::string value);
    
    std::string getVariableName  () const { return fVariableName; };
    std::string getVariableValue () const { return fVariableValue; };

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrLilypondVarValAssoc(
              std::string     variableName,
              std::string     value, 
              VarValSeparator varValSeparator,
              QuotesKind      quotesKind,
              CommentedKind   commentedKind,
              std::string     unit = "");
    virtual ~lpsrLilypondVarValAssoc();
  
  private:

    std::string     fVariableName;
    std::string     fVariableValue;
    VarValSeparator fVarValSeparator;
    QuotesKind      fQuotesKind;
    CommentedKind   fCommentedKind;
    std::string     fUnit;
};
typedef SMARTP<lpsrLilypondVarValAssoc> S_lpsrLilypondVarValAssoc;

/*!
\brief A lpsr Scheme variable/value association representation.
*/
//______________________________________________________________________________
class EXP lpsrSchemeVarValAssoc : public lpsrElement {
  public:

    enum CommentedKind     { kCommented, kUncommented };

    static SMARTP<lpsrSchemeVarValAssoc> create(
              std::string     variableName,
              std::string     value, 
              CommentedKind   commentKind );
    
    void    changeAssoc (std::string value);
    
    std::string getVariableValue () const { return fVariableValue; };

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrSchemeVarValAssoc(
              std::string     variableName,
              std::string     value, 
              CommentedKind   commentedKind );
    virtual ~lpsrSchemeVarValAssoc();
  
  private:

    std::string     fVariableName;
    std::string     fVariableValue;
    CommentedKind   fCommentedKind;
};
typedef SMARTP<lpsrSchemeVarValAssoc> S_lpsrSchemeVarValAssoc;

/*!
\brief A lpsr header representation.

  A header is represented by variable/value pairs
*/
//______________________________________________________________________________
class EXP lpsrHeader : public lpsrElement {
  public:

    static SMARTP<lpsrHeader> create();
    
    void                      setWorkNumber      (std::string val);
    S_lpsrLilypondVarValAssoc getWorkNumber      () const;

    void                      setWorkTitle       (std::string val);
    S_lpsrLilypondVarValAssoc getWorkTitle       () const;

    void                      setMovementNumber  (std::string val);
    S_lpsrLilypondVarValAssoc getMovementNumber  () const;

    void                      setMovementTitle   (std::string val);
    S_lpsrLilypondVarValAssoc getMovementTitle   () const;

    void                      addCreator         (std::string val);
    std::vector<S_lpsrLilypondVarValAssoc>
                              getCreators        () const;

    void                      setRights          (std::string val);
    S_lpsrLilypondVarValAssoc getRights          () const;

    void                      addSoftware        (std::string val);
    std::vector<S_lpsrLilypondVarValAssoc>
                              getSoftwares       () const;

    void                      setEncodingDate    (std::string val);
    S_lpsrLilypondVarValAssoc getEncodingDate    () const;

    void                      setScoreInstrument (std::string val);
    S_lpsrLilypondVarValAssoc getScoreInstrument () const;

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrHeader();
    virtual ~lpsrHeader();
  
  private:

    S_lpsrLilypondVarValAssoc              fWorkNumber;
    S_lpsrLilypondVarValAssoc              fWorkTitle;
    S_lpsrLilypondVarValAssoc              fMovementNumber;
    S_lpsrLilypondVarValAssoc              fMovementTitle;
    std::vector<S_lpsrLilypondVarValAssoc> fCreators;
    S_lpsrLilypondVarValAssoc              fRights;
    std::vector<S_lpsrLilypondVarValAssoc> fSoftwares;
    S_lpsrLilypondVarValAssoc              fEncodingDate;
    S_lpsrLilypondVarValAssoc              fScoreInstrument;

};
typedef SMARTP<lpsrHeader> S_lpsrHeader;

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

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);


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
typedef SMARTP<lpsrPaper> S_lpsrPaper;

/*!
\brief A lpsr layout representation.

  A layout is represented by variable/value pairs
*/
//______________________________________________________________________________
class EXP lpsrLayout : public lpsrElement {
  public:

    static SMARTP<lpsrLayout> create();
    
    void addlpsrLilypondVarValAssoc (S_lpsrLilypondVarValAssoc assoc)
      { fLpsrLilypondVarValAssocs.push_back(assoc); }
      
    void addLpsrSchemeVarValAssoc (S_lpsrSchemeVarValAssoc assoc)
      { fLpsrSchemeVarValAssocs.push_back(assoc); }
    
    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrLayout();
    virtual ~lpsrLayout();
  
  private:
  
    std::vector<S_lpsrLilypondVarValAssoc> fLpsrLilypondVarValAssocs;
    std::vector<S_lpsrSchemeVarValAssoc>   fLpsrSchemeVarValAssocs;
};
typedef SMARTP<lpsrLayout> S_lpsrLayout;

/*!
\brief A lpsr repeat representation.

  A repeat is represented by:
    - a sequence of elements for the common part
    - a vector os sequences of elements for the alternate endings
*/
//______________________________________________________________________________
class EXP lpsrRepeat: public lpsrElement {
  public:

    static SMARTP<lpsrRepeat> create();
    
    void    appendElementToCommonPart (S_lpsrElement elem)
              { fCommonPart->appendElementToSequence (elem); }
              
    void    appendElementToLastAlternateEnding  (S_lpsrElement elem)
              { fAlternateEndings.back()->appendElementToSequence (elem); }
                    
    void    appendNewAlternateEnding ()
              {
                fAlternateEndings.push_back(
                  lpsrSequence::create (lpsrSequence::kSpace));
              }

    void    setActuallyUsed ()
              { fActuallyUsed = true; }
    
    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrRepeat();
    virtual ~lpsrRepeat();
  
  private:
  
    S_lpsrSequence              fCommonPart;
    std::vector<S_lpsrSequence> fAlternateEndings;
    
    // the implicit lpsrRepeat is not used unless there are
    // actual repeats in the part
    bool                       fActuallyUsed;
};
typedef SMARTP<lpsrRepeat> S_lpsrRepeat;

/*!
\brief A lpsr stanza chunk representation.

  A stanza chunk is represented by a type and and a string.
  In the case of "single", the list contains only one string
*/
//______________________________________________________________________________
class EXP lpsrStanzaChunk : public lpsrElement {
  public:

    // we want to end the line in the LilyPond code at a break
    enum stanzaChunkType { kWordChunk, kSkipChunk, kBreakChunk };

    static SMARTP<lpsrStanzaChunk> create (
        stanzaChunkType chunkType,
        std::string     chunkText);
     
    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrStanzaChunk (
        stanzaChunkType chunkType,
        std::string     chunkText);
    virtual ~lpsrStanzaChunk();

  private:
  
    stanzaChunkType fStanzaChunkType;
    std::string     fChunkText;
};
typedef SMARTP<lpsrStanzaChunk> S_lpsrStanzaChunk;

/*!
\brief A lpsr stanza representation.

  A stanza is represented by a list of stanza chunks,
*/
//______________________________________________________________________________
class EXP lpsrStanza : public lpsrElement {
  public:

    static SMARTP<lpsrStanza> create (
        std::string lyricsName,
        std::string voiceName);
    
//    std::string getStanzaName     () const { return fStanzaName; }
//    std::string getStanzaContents () const { return fStanzaContents; }

    void addChunkToStanza (S_lpsrStanzaChunk chunk)
            { fStanzaChunks.push_back (chunk);
          

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrStanza (
        std::string lyricsName,
        std::string voiceName);
    virtual ~lpsrStanza();
  
  private:

    std::string        fLyricsName;
    std::string        fVoiceName;
    std::vector<S_lpsrStanzaChunk>
                       fStanzaChunks;
};
typedef SMARTP<lpsrStanza> S_lpsrStanza;

//    std::string fStanzaName;
//    std::string fStanzaContents;


 /*   
    std::map<std::string, stanzaContents> 
                          fStanzas;    // <text /> occurs after <syllabic />

    // a stanza is represented by a list words,
    // which are represented by a list of their components
    typedef std::list<std::list<std::string> > stanzaContents;

    virtual std::map<std::string, xmlPartSummaryVisitor::stanzaContents>& 
                  getStanzas();
    void          clearStanzas ();
    virtual std::string   getStanza (std::string name, std::string separator) const;
*/

/*!
\brief A lpsr lyrics representation.

  A lyrics is represented by a its string contents
*/
//______________________________________________________________________________
class EXP lpsrLyrics : public lpsrElement {
  public:

    static SMARTP<lpsrLyrics> create (
        std::string lyricsName,
        std::string voiceName);
    
    std::string getLyricsName    () const { return fLyricsName; }
    std::vector<S_lpsrStanza>
                getLyricsStanzas () const { return fLyricsStanzas; }

    void addStanzaToLyrics (S_lpsrStanza stanza)
            { fLyricsStanzas.push_back (stanza); }

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrLyrics (
        std::string lyricsName,
        std::string voiceName);
    virtual ~lpsrLyrics();
  
  private:

    std::string               fLyricsName;
    std::string               fVoiceName;
    std::vector<S_lpsrStanza> fLyricsStanzas;
};
typedef SMARTP<lpsrLyrics> S_lpsrLyrics;

/*!
\brief A lpsr voice representation.

  A voice is represented by its name and lyrics
*/
//______________________________________________________________________________
class EXP lpsrVoice : public lpsrElement {
  public:

    static SMARTP<lpsrVoice> create (std::string voiceName);

    std::string    getVoiceName () { return fVoiceName; }
    
    void           addLyricsToVoice (S_lpsrLyrics lyr)
                      { fVoiceLyrics.push_back(lyr); }
                      
    std::vector<S_lpsrLyrics>
                   getVoiceLyrics () const
                      { return fVoiceLyrics; }

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrVoice(std::string voiceName);
    virtual ~lpsrVoice();
  
  private:
  
    std::string               fVoiceName;
    std::vector<S_lpsrLyrics> fVoiceLyrics;
};
typedef SMARTP<lpsrVoice> S_lpsrVoice;

/*!
\brief A lpsr part representation.

  A part is represented by a its string contents
*/
//______________________________________________________________________________
class EXP lpsrPart : public lpsrElement {
  public:

    static SMARTP<lpsrPart> create(
      std::string name, bool absoluteCode, bool generateNumericalTime);
    
    void          addVoiceToPart (S_lpsrVoice voice)
                      { fPartVoices.push_back (voice); }
    std::vector<S_lpsrVoice>
                  getPartVoices ()
                      { return fPartVoices; }

    std::string    getPartName () const         { return fPartName; }
    bool           getAbsoluteCode () const     { return fPartAbsoluteCode; }
    S_lpsrSequence getPartLpsrSequence () const { return fPartLpsrSequence; }

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrPart(std::string name, bool absoluteCode, bool generateNumericalTime);
    virtual ~lpsrPart();
  
  private:

    std::string        fPartName;
    bool               fPartAbsoluteCode;
    bool               fGenerateNumericalTime;

    // the part voices
    std::vector<S_lpsrVoice>
                       fPartVoices;
                       
    // the implicit sequence containing the code generated for the part
    S_lpsrSequence     fPartLpsrSequence;
  
    // the implicit repeat at the beginning of the part
    // will be ignored if the part has no repeats at all
    S_lpsrRepeat       fPartLpsrRepeat;
};
typedef SMARTP<lpsrPart> S_lpsrPart;
typedef std::map<std::string, S_lpsrPart> lpsrPartsmap;

/*!
\brief A lpsr barline representation.

  A barline is represented by the number of the next bar
*/
//______________________________________________________________________________
class EXP lpsrBarLine : public lpsrElement {
  public:
    
    static SMARTP<lpsrBarLine> create(int nextBarNumber);

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrBarLine(int nextBarNumber);
    virtual ~lpsrBarLine();
  
  private:

    int fNextBarNumber;
};
typedef SMARTP<lpsrBarLine> S_lpsrBarLine;

/*!
\brief A lpsr comment representation.

  A comment is represented by its contents
*/
//______________________________________________________________________________
class EXP lpsrComment : public lpsrElement {
  public:
    
    enum GapKind { kGapAfterwards, kNoGapAfterwards };

    static SMARTP<lpsrComment> create(std::string contents, GapKind gapKind = kNoGapAfterwards);

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrComment(std::string contents, GapKind gapKind = kNoGapAfterwards);
    virtual ~lpsrComment();
  
  private:

    std::string fContents;
    GapKind     fGapKind;
};
typedef SMARTP<lpsrComment> S_lpsrComment;

/*!
\brief A lpsr break representation.

  A break is represented by the number of the next bar
*/
//______________________________________________________________________________
class EXP lpsrBreak : public lpsrElement {
  public:
    
    static SMARTP<lpsrBreak> create(int nextBarNumber);

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrBreak(int nextBarNumber);
    virtual ~lpsrBreak();
  
  private:

    int fNextBarNumber;
};
typedef SMARTP<lpsrBreak> S_lpsrBreak;

/*!
\brief A lpsr barnumbercheck representation.

  A barnumbercheck is represented by the number of the next bar
*/
//______________________________________________________________________________
class EXP lpsrBarNumberCheck : public lpsrElement {
  public:
    
    static SMARTP<lpsrBarNumberCheck> create(int nextBarNumber);

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrBarNumberCheck(int nextBarNumber);
    virtual ~lpsrBarNumberCheck();
  
  private:

    int fNextBarNumber;
};
typedef SMARTP<lpsrBarNumberCheck> S_lpsrBarNumberCheck;

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

    void addElementToTuplet (S_lpsrElement elem) { fTupletContents.push_back(elem); }

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrTuplet();
    virtual ~lpsrTuplet();
  
  private:

    int fTupletNumber;
    
    int fActualNotes;
    int fNormalNotes;
    
    std::vector<S_lpsrElement> fTupletContents;
};
typedef SMARTP<lpsrTuplet> S_lpsrTuplet;

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

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrBeam(int number, BeamKind kind);
    virtual ~lpsrBeam();
  
  private:

    int      fBeamNumber;
    BeamKind fBeamKind;
};
typedef SMARTP<lpsrBeam> S_lpsrBeam;

/*!
\brief A lpsr dynamics representation.

  A dynamics is represented by a DynamicsKind value
*/
//______________________________________________________________________________
class EXP lpsrDynamics : public lpsrElement {
  public:

    enum DynamicsKind {
          kF, kFF, kFFF, kFFFF, kFFFFF, kFFFFFF,
          kP, kPP, kPPP, kPPPP, kPPPPP, kPPPPPP,
          kMF, kMP, kFP, kFZ, kRF, kSF, kRFZ, kSFZ, kSFP, kSFPP, kSFFZ,
          k_NoDynamics };
    
    static SMARTP<lpsrDynamics> create(DynamicsKind kind);

    DynamicsKind getDynamicsKind () const { return fDynamicsKind; }

    std::string  dynamicsKindAsLilypondString ();
    
    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrDynamics(DynamicsKind kind);
    virtual ~lpsrDynamics();
  
  private:

    DynamicsKind fDynamicsKind;
};
typedef SMARTP<lpsrDynamics> S_lpsrDynamics;

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

    std::string  wedgeKinsAsString ();

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrWedge(WedgeKind kind);
    virtual ~lpsrWedge();
  
  private:

    WedgeKind fWedgeKind;
};
typedef SMARTP<lpsrWedge> S_lpsrWedge;

/*!
\brief A lpsr time representation.

  A time is represented by the numerator and denominator
*/
//______________________________________________________________________________
class EXP lpsrTime : public lpsrElement {
  public:
    
    static SMARTP<lpsrTime> create(int numerator, int denominator, bool generateNumericalTime);

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrTime(int numerator, int denominator, bool generateNumericalTime);
    virtual ~lpsrTime();
  
  private:

    rational fRational;
    bool     fGenerateNumericalTime;
};
typedef SMARTP<lpsrTime> S_lpsrTime;

/*!
\brief A lpsr clef representation.

  A clef is represented by its name
*/
//______________________________________________________________________________
class EXP lpsrClef : public lpsrElement {
  public:
    
    static SMARTP<lpsrClef> create(std::string clefName);

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrClef(std::string clefName);
    virtual ~lpsrClef();
  
  private:

    std::string fClefName;
};
typedef SMARTP<lpsrClef> S_lpsrClef;

/*!
\brief A lpsr key representation.

  A key is represented by the tonic and the mode
*/
//______________________________________________________________________________
class EXP lpsrKey : public lpsrElement {
  public:
    
    enum KeyMode { kMajor, kMinor };

    static SMARTP<lpsrKey> create(std::string tonic, KeyMode keyMode);

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrKey(std::string tonic, KeyMode keyMode);
    virtual ~lpsrKey();
  
  private:

    std::string   fTonic;
    KeyMode       fKeyMode;
};
typedef SMARTP<lpsrKey> S_lpsrKey;

/*!
\brief A lpsr midi representation.

  A midi is represented by variable/value pairs

*/
//______________________________________________________________________________
class EXP lpsrMidi : public lpsrElement {
  public:

    static SMARTP<lpsrMidi> create();

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrMidi();
    virtual ~lpsrMidi();
  
  private:
  
};
typedef SMARTP<lpsrMidi> S_lpsrMidi;

/*!
\brief A lpsr score representation.

  A score is represented by parallel music, score layout and midi
*/
//______________________________________________________________________________
class EXP lpsrScore : public lpsrElement {
  public:

    static SMARTP<lpsrScore> create();
     
    S_lpsrParallelMusic getScoreParallelMusic () const { return fScoreParallelMusic; }
    S_lpsrLayout   getScoreLayout        () const { return fScoreLayout; }
    S_lpsrMidi     getScoreMidi          () const { return fScoreMidi; }

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrScore();
    virtual ~lpsrScore();
  
  private:
  
    S_lpsrParallelMusic fScoreParallelMusic;
    S_lpsrLayout   fScoreLayout;
    S_lpsrMidi     fScoreMidi;    
};
typedef SMARTP<lpsrScore> S_lpsrScore;

/*!
\brief A lpsr new staff representation.

  A new staff is represented by a vactor of elements
*/
//______________________________________________________________________________
class EXP lpsrNewstaffCommand : public lpsrElement {
  public:

    static SMARTP<lpsrNewstaffCommand> create();
     
    void addElementToNewStaff (S_lpsrElement elem)
        { fNewStaffElements.push_back(elem); }

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrNewstaffCommand();
    virtual ~lpsrNewstaffCommand();
  
  private:
  
    std::vector<S_lpsrElement> fNewStaffElements;
};
typedef SMARTP<lpsrNewstaffCommand> S_lpsrNewstaffCommand;

/*!
\brief A lpsr new lyrics representation.

  A new lyrics is represented by the voice name and the part name
*/
//______________________________________________________________________________
class EXP lpsrNewlyricsCommand : public lpsrElement {
  public:

    static SMARTP<lpsrNewlyricsCommand> create(
        std::string lyricsName,
        std::string partName);
     
    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrNewlyricsCommand(
        std::string lyricsName,
        std::string partName);
    virtual ~lpsrNewlyricsCommand();
  
  private:
  
    std::string fLyricsName;
    std::string fPartName;
};
typedef SMARTP<lpsrNewlyricsCommand> S_lpsrNewlyricsCommand;

/*!
\brief A lpsr variable use representation.

  A variable use is represented by the name of the variable to use
*/
//______________________________________________________________________________
class EXP lpsrVariableUseCommand : public lpsrElement {
  public:

    static SMARTP<lpsrVariableUseCommand> create (std::string variableName);
    
    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrVariableUseCommand (std::string variableName);
    virtual ~lpsrVariableUseCommand();
  
  private:
  
    std::string fVariableName;
};
typedef SMARTP<lpsrVariableUseCommand> S_lpsrVariableUseCommand;

/*!
\brief A lyrics use representation.

  A lyrics use is represented by the lyrics to use
*/
//______________________________________________________________________________
class EXP lpsrUseLyricsCommand : public lpsrElement {
  public:

    static SMARTP<lpsrUseLyricsCommand> create (std::string lyricsName);
    
    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrUseLyricsCommand(std::string variableName);
    virtual ~lpsrUseLyricsCommand();
  
  private:
  
    std::string lyricsName;
};
typedef SMARTP<lpsrUseLyricsCommand> S_lpsrUseLyricsCommand;

/*!
\brief A context representation.

  A context is represented by its type, name and contents
*/
//______________________________________________________________________________
class EXP lpsrContext : public lpsrElement {
  public:

    static SMARTP<lpsrContext> create (
        std::string    contextType,
        std::string    contextName);
    
    void addElementToContext (S_lpsrElement elem)
        { fContextElements.push_back(elem); }

    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrContext(
        std::string    contextType,
        std::string    contextName);
    virtual ~lpsrContext();
  
  private:
  
    std::string    fContextType;
    std::string    fContextName;
    std::vector<S_lpsrElement>
                   fContextElements;
};
typedef SMARTP<lpsrContext> S_lpsrContext;

//______________________________________________________________________________
class musicXMLBeatData {
  public:
  
     virtual void print              (std::ostream& os);
 
  public:
  
    std::string fBeatUnit;
    int         fDots;
};

/*!
\brief A tempo command representation.

  A tempo command is represented by the lyrics to use
*/
//______________________________________________________________________________
class EXP lpsrTempoCommand : public lpsrElement {
  public:

    static SMARTP<lpsrTempoCommand> create (
        int tempoUnit, int perMinute);
    
    virtual void printMusicXML      (std::ostream& os);
    virtual void printLpsrStructure (std::ostream& os);
    virtual void printLilyPondCode  (std::ostream& os);

  protected:

    lpsrTempoCommand (
        int tempoUnit, int perMinute);
    virtual ~lpsrTempoCommand();
  
  private:
  
    int  fTempoUnit;
    int  fPerMinute;
};
typedef SMARTP<lpsrTempoCommand> S_lpsrTempoCommand;


/*! @} */

}


#endif
