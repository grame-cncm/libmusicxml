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
        - a text description with         printLPSR()
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

EXP ostream& operator<< (ostream& os, const S_lpsrElement& elt);

EXP ostream& operator<< (ostream& os, const S_lpsrLilypondVarValAssoc& elt);
EXP ostream& operator<< (ostream& os, const S_lpsrSchemeVarValAssoc& elt);

EXP ostream& operator<< (ostream& os, const S_lpsrDuration& dur);
EXP ostream& operator<< (ostream& os, const S_lpsrDynamics& dyn);
EXP ostream& operator<< (ostream& os, const S_lpsrWedge& wdg);
EXP ostream& operator<< (ostream& os, const S_lpsrChord& chrd);
EXP ostream& operator<< (ostream& os, const S_lpsrNote& chrd);

EXP ostream& operator<< (ostream& os, const S_lpsrPaper& chrd);
EXP ostream& operator<< (ostream& os, const S_lpsrLayout& chrd);


/*!
\brief Global variables.

  An global variable is implemented as a static member of this class.
*/
//______________________________________________________________________________
class EXP lpsrGlobalVariables {
  public:
    
    enum CodeGenerationKind {
      kLPSR, kMusicXML, kLilypondCode};

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
    
    virtual void print              (ostream& os);

    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);
    
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
  
     virtual void print              (ostream& os);
 
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
    string fTupletMemberType;

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
    
    string  absoluteOctaveAsLilypondString ();

    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

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
        string tupletMemberType);
    
    lpsrDuration(
        int num,
        int denom,
        int dots,
        string tupletMemberType);
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
    
    string  durationAsLilypondString ();

    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  private:

    int         fNum;
    int         fDenom;
    int         fDots;
    string fTupletMemberType;
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
                          
    static map<LpsrPitch, string> sDutchLilypondPitches;

    S_lpsrDuration getNoteLpsrDuration () { return fNoteLpsrDuration; }   

     string  notePitchAsLilypondString ();
    
   // dynamics and wedges
    void              addDynamics (S_lpsrDynamics dyn);
    void              addWedge    (S_lpsrWedge    wdg);

    list<S_lpsrDynamics> getNoteDynamics () { return fNoteDynamics; };
    list<S_lpsrWedge>    getNoteWedges   () { return fNoteWedges; };

    S_lpsrDynamics     removeFirstDynamics ();
    S_lpsrWedge        removeFirstWedge ();

//    void octaveRelativeTo (const lpsrAbsoluteOctave& otherAbsOct);
        
    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:
 
    lpsrNote (
      S_translationSettings& ts,
      musicXMLNoteData&      mxmldat);
    
    virtual ~lpsrNote();
    
  private:
  
    S_translationSettings     fTranslationSettings;

    // MusicXML informations
    musicXMLNoteData          fMusicXMLNoteData;
    MusicXMLDiatonicPitch     fMusicXMLDiatonicPitch; // JMI

 //   bool                     fCurrentStepIsARest;
 //   MusicXMLAlteration       fMusicXMLAlteration;
  //  int                      fMusicXMLOctave;

    // MusicXML durations are in in divisions per quarter note,
    // LilyPond durations are in whole notes, hence the "*4" multiplication
    S_lpsrDuration            fNoteLpsrDuration;

    // LilyPond informations
    LpsrPitch                 fNoteLpsrPitch;
    
    list<S_lpsrDynamics> fNoteDynamics;
    list<S_lpsrWedge>    fNoteWedges;
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

    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    lpsrParallelMusic(ElementsSeparator elementsSeparator);
    virtual ~lpsrParallelMusic();
    
  private:
  
    vector<S_lpsrElement> fParallelMusicElements;
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

    void          prependElementToSequence (S_lpsrElement elem)
                      { fSequenceElements.push_front(elem); }
    void          appendElementToSequence  (S_lpsrElement elem)
                      { fSequenceElements.push_back(elem); }
    
    S_lpsrElement getLastElementOfSequence()
                      { return fSequenceElements.back(); }
    void          removeLastElementOfVoiceSequence ()
                      { fSequenceElements.pop_back(); }

    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    lpsrSequence(ElementsSeparator elementsSeparator);
    virtual ~lpsrSequence();
    
  private:
  
    list<S_lpsrElement> fSequenceElements;
    ElementsSeparator   fElementsSeparator;

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

    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    lpsrChord (S_lpsrDuration chordDuration);
    virtual ~lpsrChord();
  
  private:
  
    vector<S_lpsrNote>   fChordNotes;
    
    S_lpsrDuration            fChordDuration;

    
    list<S_lpsrDynamics> fChordDynamics;
    list<S_lpsrWedge>    fChordWedges;
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
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    lpsrLilypondVarValAssoc(
              string     variableName,
              string     value, 
              VarValSeparator varValSeparator,
              QuotesKind      quotesKind,
              CommentedKind   commentedKind,
              string     unit = "");
    virtual ~lpsrLilypondVarValAssoc();
  
  private:

    string     fVariableName;
    string     fVariableValue;
    VarValSeparator fVarValSeparator;
    QuotesKind      fQuotesKind;
    CommentedKind   fCommentedKind;
    string     fUnit;
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
              string     variableName,
              string     value, 
              CommentedKind   commentKind );
    
    void    changeAssoc (string value);
    
    string getVariableValue () const { return fVariableValue; };

    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    lpsrSchemeVarValAssoc(
              string     variableName,
              string     value, 
              CommentedKind   commentedKind );
    virtual ~lpsrSchemeVarValAssoc();
  
  private:

    string     fVariableName;
    string     fVariableValue;
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
    
    void                      setWorkNumber      (string val);
    S_lpsrLilypondVarValAssoc getWorkNumber      () const;

    void                      setWorkTitle       (string val);
    S_lpsrLilypondVarValAssoc getWorkTitle       () const;

    void                      setMovementNumber  (string val);
    S_lpsrLilypondVarValAssoc getMovementNumber  () const;

    void                      setMovementTitle   (string val);
    S_lpsrLilypondVarValAssoc getMovementTitle   () const;

    void                      addCreator         (
                                string type, string val);
    vector<S_lpsrLilypondVarValAssoc>
                              getCreators        () const;

    void                      setRights          (string val);
    S_lpsrLilypondVarValAssoc getRights          () const;

    void                      addSoftware        (string val);
    vector<S_lpsrLilypondVarValAssoc>
                              getSoftwares       () const;

    void                      setEncodingDate    (string val);
    S_lpsrLilypondVarValAssoc getEncodingDate    () const;

    void                      setScoreInstrument (string val);
    S_lpsrLilypondVarValAssoc getScoreInstrument () const;

    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    lpsrHeader();
    virtual ~lpsrHeader();
  
  private:

    S_lpsrLilypondVarValAssoc              fWorkNumber;
    S_lpsrLilypondVarValAssoc              fWorkTitle;
    S_lpsrLilypondVarValAssoc              fMovementNumber;
    S_lpsrLilypondVarValAssoc              fMovementTitle;
    vector<S_lpsrLilypondVarValAssoc> fCreators;
    S_lpsrLilypondVarValAssoc              fRights;
    vector<S_lpsrLilypondVarValAssoc> fSoftwares;
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

    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);


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
    
    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    lpsrLayout();
    virtual ~lpsrLayout();
  
  private:
  
    vector<S_lpsrLilypondVarValAssoc> fLpsrLilypondVarValAssocs;
    vector<S_lpsrSchemeVarValAssoc>   fLpsrSchemeVarValAssocs;
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
    
    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    lpsrRepeat();
    virtual ~lpsrRepeat();
  
  private:
  
    S_lpsrSequence              fCommonPart;
    vector<S_lpsrSequence> fAlternateEndings;
    
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
    enum stanzaChunkType {
      kSingleChunk, kBeginChunk, kMiddleChunk, kEndChunk,
      kSkipChunk, kBreakChunk };

    static SMARTP<lpsrStanzaChunk> create (
        stanzaChunkType chunkType,
        string          chunkText);
     
    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    lpsrStanzaChunk (
        stanzaChunkType chunkType,
        string     chunkText);
    virtual ~lpsrStanzaChunk();

  private:
  
    stanzaChunkType fStanzaChunkType;
    string     fChunkText;
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
        string lyricsName,
        string voiceName);
    
//    string getStanzaName     () const { return fStanzaName; }
//    string getStanzaContents () const { return fStanzaContents; }

    void addChunkToStanza (S_lpsrStanzaChunk chunk)
            { fStanzaChunks.push_back (chunk); }
          

    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    lpsrStanza (
        string lyricsName,
        string voiceName);
    virtual ~lpsrStanza();
  
  private:

    string        fLyricsName;
    string        fVoiceName;
    vector<S_lpsrStanzaChunk>
                  fStanzaChunks;
};
typedef SMARTP<lpsrStanza> S_lpsrStanza;

/*!
\brief A lpsr lyrics representation.

  A lyrics is represented by a its string contents
*/
//______________________________________________________________________________
/*
class EXP lpsrLyrics : public lpsrElement {
  public:

    static SMARTP<lpsrLyrics> create (
                                string lyricsName,
                                string voiceName);

    string      getLyricsName () const { return fLyricsName; }
    
    map<int, S_lpsrStanza> 
                getLyricsStanzasMap () const { return fLyricsStanzasMap; }

    void        addStanzaToLyrics (int number, S_lpsrStanza stanza)
                  { fLyricsStanzasMap [number] = stanza; }

    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    lpsrLyrics (
        string lyricsName,
        string voiceName);
    virtual ~lpsrLyrics();
  
  private:

    string                 fLyricsName;
    string                 fVoiceName;
    map<int, S_lpsrStanza> fLyricsStanzasMap;
};
typedef SMARTP<lpsrLyrics> S_lpsrLyrics;
*/

/*!
\brief A lpsr voice representation.

  A vpoce is represented by a its string contents
*/
//______________________________________________________________________________
class EXP lpsrVoice : public lpsrElement {
  public:

    static SMARTP<lpsrVoice> create (
        string name,
        bool   absoluteCode,
        bool   generateNumericalTime);
    
//    void     addLyricsToVoice (S_lpsrLyrics lyr)
//                { fVoiceLyrics.push_back(lyr); }
                      
    void      addStanzaToVoice (int number, S_lpsrStanza stanza)
                { fVoiceStanzasMap [number] = stanza; }

 //   vector<S_lpsrLyrics>
   //                getVoiceLyrics () const { return fVoiceLyrics; }
    map<int, S_lpsrStanza>
              getVoiceStanzasMap () const { return fVoiceStanzasMap; }

    string    getVoiceName () const       { return fVoiceName; }
    bool        getAbsoluteCode () const    { return fVoiceAbsoluteCode; }

    void      appendElementToVoiceSequence (S_lpsrElement elem)
                { fVoiceSequence->appendElementToSequence(elem); }
                
    void      removeLastElementOfVoiceSequence ()
                { fVoiceSequence->removeLastElementOfVoiceSequence(); }

    S_lpsrSequence getVoiceSequence () const { return fVoiceSequence; }

    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    lpsrVoice (
        string name,
        bool   absoluteCode,
        bool   generateNumericalTime);
    virtual ~lpsrVoice();
  
  private:

    string            fVoiceName;
    bool              fVoiceAbsoluteCode;
    bool              fGenerateNumericalTime;
                       
    // the implicit sequence containing the code generated for the voice
    S_lpsrSequence    fVoiceSequence;
  
    // there can be lyrics associated to the voice
    map<int, S_lpsrStanza>
                      fVoiceStanzasMap;
//    vector<S_lpsrLyrics>
//                       fVoiceLyrics;

    // the implicit repeat at the beginning of the voice
    // will be ignored if the voice has no repeats at all
    S_lpsrRepeat      fVoiceLpsrRepeat;
};
typedef SMARTP<lpsrVoice> S_lpsrVoice;
typedef map<string, S_lpsrVoice> lpsrVoicesmap;

/*!
\brief A lpsr part representation.

  A part is represented by a its string contents
*/
//______________________________________________________________________________
class EXP lpsrPart : public lpsrElement {
  public:

    static SMARTP<lpsrPart> create (
        string name,
        string partInstrumentName,
        bool   absoluteCode,
        bool   generateNumericalTime);
    
    void          addVoiceToPart (int voiceID, S_lpsrVoice voice)
                      { fPartVoicesMap [voiceID] = (voice); }
    map<int, S_lpsrVoice>
                  getPartVoicesMap ()          { return fPartVoicesMap; }

    string        getPartName () const         { return fPartName; }
    bool          getAbsoluteCode () const     { return fPartAbsoluteCode; }

    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    lpsrPart (
        string name,
        string partInstrumentName,
        bool   absoluteCode,
        bool   generateNumericalTim);
    virtual ~lpsrPart();
  
  private:

    string             fPartName;
    string             fPartInstrumentName;
    bool               fPartAbsoluteCode;
    bool               fGenerateNumericalTime;

    // the part voices
    map<int, S_lpsrVoice>
                       fPartVoicesMap;
};
typedef SMARTP<lpsrPart> S_lpsrPart;
typedef map<string, S_lpsrPart> lpsrPartsmap;

/*!
\brief A lpsr barline representation.

  A barline is represented by the number of the next bar
*/
//______________________________________________________________________________
class EXP lpsrBarLine : public lpsrElement {
  public:
    
    static SMARTP<lpsrBarLine> create(int nextBarNumber);

    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

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

    static SMARTP<lpsrComment> create(string contents, GapKind gapKind = kNoGapAfterwards);

    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    lpsrComment(string contents, GapKind gapKind = kNoGapAfterwards);
    virtual ~lpsrComment();
  
  private:

    string fContents;
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

    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

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

    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

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

    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    lpsrTuplet();
    virtual ~lpsrTuplet();
  
  private:

    int fTupletNumber;
    
    int fActualNotes;
    int fNormalNotes;
    
    vector<S_lpsrElement> fTupletContents;
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

    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

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

    string  dynamicsKindAsLilypondString ();
    
    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

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

    string  wedgeKinsAsString ();

    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

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

    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

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
    
    enum { kStandardLine, kTrebleStdLine=2, kBassStdLine=4, kCStdLine=3, kTabStdLine=5 };

    static SMARTP<lpsrClef> create (
      string sign,
      int         line,
      int         octaveChange);

    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    lpsrClef (
      string clefName,
      int         line,
      int         octaveChange);
    virtual ~lpsrClef();
  
  private:

    string fSign;
    int         fLine;
    int         fOctaveChange;
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

    static SMARTP<lpsrKey> create (
        int fifths, string mode, int cancel);

    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    lpsrKey (
        int fifths, string mode, int cancel);
    virtual ~lpsrKey();
  
  private:

    int     fFifths;
    string  fMode;
    int     fCancel;

    string  fTonic;
    KeyMode fKeyMode;
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

    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

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

    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

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

    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    lpsrNewstaffCommand();
    virtual ~lpsrNewstaffCommand();
  
  private:
  
    vector<S_lpsrElement> fNewStaffElements;
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
        string lyricsName,
        string voiceName);
     
    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    lpsrNewlyricsCommand(
        string lyricsName,
        string voiceName);
    virtual ~lpsrNewlyricsCommand();
  
  private:
  
    string fLyricsName;
    string fVoiceName;
};
typedef SMARTP<lpsrNewlyricsCommand> S_lpsrNewlyricsCommand;

/*!
\brief A lpsr variable use representation.

  A variable use is represented by the name of the variable to use
*/
//______________________________________________________________________________
class EXP lpsrVariableUseCommand : public lpsrElement {
  public:

    static SMARTP<lpsrVariableUseCommand> create (string variableName);
    
    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    lpsrVariableUseCommand (string variableName);
    virtual ~lpsrVariableUseCommand();
  
  private:
  
    string fVariableName;
};
typedef SMARTP<lpsrVariableUseCommand> S_lpsrVariableUseCommand;

/*!
\brief A lyrics use representation.

  A lyrics use is represented by the lyrics to use
*/
//______________________________________________________________________________
class EXP lpsrUseLyricsCommand : public lpsrElement {
  public:

    static SMARTP<lpsrUseLyricsCommand> create (string lyricsName);
    
    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    lpsrUseLyricsCommand(string variableName);
    virtual ~lpsrUseLyricsCommand();
  
  private:
  
    string lyricsName;
};
typedef SMARTP<lpsrUseLyricsCommand> S_lpsrUseLyricsCommand;

/*!
\brief A context representation.

  A context is represented by its type, name and contents
  It may be:
    existing (such as an implicit Voice in a Score), \context Voice...
    or a new one, \new Voice...
*/
//______________________________________________________________________________
class EXP lpsrContext : public lpsrElement {
  public:

    enum ContextKind { kExistingContext, kNewContext };
    
    static SMARTP<lpsrContext> create (
        ContextKind    contextKind,
        string    contextType,
        string    contextName);
    
    void addElementToContext (S_lpsrElement elem)
        { fContextElements.push_back(elem); }

    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

  protected:

    lpsrContext(
        ContextKind    contextKind,
        string    contextType,
        string    contextName);
    virtual ~lpsrContext();
  
  private:
  
    ContextKind    fContextKind;
    string    fContextType;
    string    fContextName;

    vector<S_lpsrElement>
                   fContextElements;
};
typedef SMARTP<lpsrContext> S_lpsrContext;

//______________________________________________________________________________
class musicXMLBeatData {
  public:
  
     virtual void print              (ostream& os);
 
  public:
  
    string fBeatUnit;
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
    
    virtual void printMusicXML      (ostream& os);
    virtual void printLPSR          (ostream& os);
    virtual void printLilyPondCode  (ostream& os);

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
