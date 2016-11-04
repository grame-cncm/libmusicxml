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

class msrVarValAssoc;
// JMI class msrSchemeVarValAssoc;

class msrScore;

class msrPartgroup;

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

class msrMidi;

typedef SMARTP<msrElement>             S_msrElement;

typedef SMARTP<msrVarValAssoc>         S_msrVarValAssoc;
//typedef SMARTP<msrSchemeVarValAssoc>   S_msrSchemeVarValAssoc;

typedef SMARTP<msrScore>               S_msrScore;

typedef SMARTP<msrPartgroup>               S_msrPartgroup;

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

typedef SMARTP<msrMidi>                S_msrMidi;

EXP ostream& operator<< (ostream& os, const S_msrElement& elt);

EXP ostream& operator<< (ostream& os, const S_msrScore& elt);

EXP ostream& operator<< (ostream& os, const S_msrPartgroup& elt);

EXP ostream& operator<< (ostream& os, const S_msrClef& elt);
EXP ostream& operator<< (ostream& os, const S_msrKey& elt);
EXP ostream& operator<< (ostream& os, const S_msrTime& elt);

EXP ostream& operator<< (ostream& os, const S_msrVarValAssoc& elt);
//EXP ostream& operator<< (ostream& os, const S_msrSchemeVarValAssoc& elt);

EXP ostream& operator<< (ostream& os, const S_msrDuration& dur);
EXP ostream& operator<< (ostream& os, const S_msrDynamics& dyn);
EXP ostream& operator<< (ostream& os, const S_msrWedge& wdg);
EXP ostream& operator<< (ostream& os, const S_msrChord& chrd);
EXP ostream& operator<< (ostream& os, const S_msrNote& chrd);

EXP ostream& operator<< (ostream& os, const S_msrPaper& chrd);
EXP ostream& operator<< (ostream& os, const S_msrLayout& chrd);

EXP ostream& operator<< (ostream& os, const S_msrMidi& chrd);


//______________________________________________________________________________
/*!
\internal
\brief A macro to simplify indentation
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
    
    enum DisplayKind {
      kMusicXML, kMSR, kScoreSummary, kLilypondCode,
      k_NoDisplay};

    static DisplayKind getDisplayKind ()
      { return sDisplayKind; }
      
    static void setDisplayKind (DisplayKind kind)
      { sDisplayKind = kind; }
  
  private:
  
    static DisplayKind sDisplayKind;
};

/*!
\brief A generic msr element representation.

  An element is represented by its name and the
  list of its enclosed elements plus optional parameters.
*/
template<class C> class msrVisitor : virtual public basevisitor
{
  public:
  
    virtual ~msrVisitor() {}
    
    virtual void visitStart( C& elt ) {};
    virtual void visitEnd  ( C& elt ) {};
};

{
  public:
    virtual ~msrVisitor() {}

    virtual void visitStart( SMARTP<msrVisitor>& visitor ) {};
    virtual void visitEnd  ( SMARTP<msrVisitor>& visitor ) {};
};
typedef SMARTP<msrVisitor> S_msrVisitor;

/*!
\brief A generic msr element representation.

  An element is represented by its name and the
  list of its enclosed elements plus optional parameters.
*/
//______________________________________________________________________________

//class EXP msrVisitable : public visitable JMI
class EXP msrVisitable : public ctree<msrVisitable>, public visitable
{
  public:
   
 // JMI   static SMARTP<msrVisitable> create ();

    virtual void acceptIn  (msrVisitor& visitor);
    virtual void acceptOut (msrVisitor& visitor);

    // how to browse the data stored in this object
    // this method is overloaded in every concrete derived class
    virtual void browseData () = 0;

  protected:

    int fType; // the element type

    msrVisitable () : fType(0) {}
};
typedef SMARTP<msrVisitable> S_msrVisitable;

/*!
\brief A generic msr element representation.

  An element is represented by its name and the
  list of its enclosed elements plus optional parameters.
*/
//______________________________________________________________________________
// JMI class msrVisitor : public smartable
/*
template<class C> class visitor : virtual public basevisitor
{
  public:
  
    virtual ~visitor() {}
    
    virtual void visitStart( C& elt ) {};
    virtual void visitEnd  ( C& elt ) {};
};
*/

/*!
\brief A generic msr element representation.

  An element is represented by its name and the
  list of its enclosed elements plus optional parameters.
*/
//______________________________________________________________________________
class EXP msrBrowser : public smartable //JMI : public tree_browser<msrVisitable> 
{
  public:
  
    static SMARTP<msrBrowser> create (
      S_translationSettings& ts,
      S_msrVisitor&          visitor)
      {
        msrBrowser * o =
          new msrBrowser( 
            ts, visitor);
        assert(o!=0);
        return 0;
      }

    msrBrowser (
      S_translationSettings& ts,
      S_msrVisitor&          visitor)
        // JMI: tree_browser<msrVisitable> (v)
      {
        fTranslationSettings = ts;
        fMsrVisitor          = visitor;
      }
      
    virtual ~msrBrowser() {}
    
    virtual void browse (S_msrVisitable& t);

  protected:

    msrVisitor* fmsrVisitor;

    virtual void enter (S_msrVisitable& t)
      {
        t->acceptIn  (*fMsrVisitor);
      }
    virtual void leave (S_msrVisitable& t)
      {
        t->acceptOut (*fMsrVisitor);
      }
  
  private:

    S_translationSettings fTranslationSettings;

    S_msrVisitor          fMsrVisitor;
};
typedef SMARTP<msrBrowser> S_msrBrowser;

/*!
\brief A generic msr element representation.

  An element is represented by its name and the
  list of its enclosed elements plus optional parameters.
*/
//______________________________________________________________________________
class EXP msrElement : public msrVisitable //, public smartable
{
  public:
 
    static SMARTP<msrElement> create (
      S_translationSettings& ts, 
      int                    inputLineNumber);
    
    int getInputLineNumber ()
      { return fInputLineNumber; }

    virtual void acceptIn  (S_msrVisitor& visitor);
    virtual void acceptOut (S_msrVisitor& visitor);

    virtual void print             (ostream& os);

    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);
    
    static indenter idtr;

  protected:
     
    S_translationSettings fTranslationSettings;
    int                   fInputLineNumber;
    
    msrElement (
      S_translationSettings& ts, 
      int                    inputLineNumber);

    virtual void browseData () {};

    virtual ~msrElement();

  private:

    bool fDebug;
};
typedef SMARTP<msrElement> S_msrElement;
typedef vector<S_msrElement> msrElementsVector;
typedef list<S_msrElement> msrElementsList;

//______________________________________________________________________________
/*
template <typename T> class EXP ctree : virtual public smartable
{
  public:
  
    typedef SMARTP<T>                   treePtr;   ///< the node sub elements type
    
    typedef std::vector<treePtr>        branches;  ///< the node sub elements container type
    
    typedef typename branches::iterator literator; ///< the current level iterator type
    typedef treeIterator<treePtr>       iterator;  ///< the top -> bottom iterator type

    static treePtr new_tree ()
      { ctree<T>* o = new ctree<T>; assert(o!=0); return o; }
    
    branches& elements ()                { return fElements; }   
    const branches& elements () const    { return fElements; }
    
    virtual void push (const treePtr& t) { fElements.push_back(t); }
    
    virtual int  size  () const          { return fElements.size(); }
    virtual bool empty () const          { return fElements.size()==0; }

    iterator begin ()
      {
        treePtr start=dynamic_cast<T*>(this);
        return iterator(start);
      }
    iterator end ()
      {
        treePtr start=dynamic_cast<T*>(this);
        return iterator(start, true);
      }
    iterator erase (iterator i)
      {
        return i.erase();
      }
    iterator insert (iterator before, const treePtr& value)
      {
        return before.insert(value);
      }
    
    literator lbegin () { return fElements.begin(); }
    literator lend ()   { return fElements.end(); }

  protected:
  
    ctree() {}
    virtual ~ctree() {}

  private:
  
    branches  fElements;
};
*/

/*
//______________________________________________________________________________
template <typename T> class EXP tree_browser : public browser<T> 
{
  protected:
  
    basevisitor*  fVisitor;

    virtual void enter (T& t) { t.acceptIn  (*fVisitor); }
    virtual void leave (T& t) { t.acceptOut (*fVisitor); }

  public:
    typedef typename ctree<T>::treePtr treePtr;
    
    tree_browser (basevisitor* v) : fVisitor(v) {}
    
    virtual ~tree_browser() {}

    virtual void set (basevisitor* v) {  fVisitor = v; }
    
    virtual void browse (T& t) {
      enter (t);
      
      typename ctree<T>::literator iter;
      
      for (iter = t.lbegin(); iter != t.lend(); iter++)
        browse(**iter);
        
      leave (t);
    }
};
*/

/*
//______________________________________________________________________________
class EXP xml_tree_browser : public tree_browser<xmlelement> 
{
  public:
  
    xml_tree_browser (basevisitor* v)
      : tree_browser<xmlelement> (v)
      {}
      
    virtual ~xml_tree_browser() {}
    
    virtual void browse (xmlelement& t);
};
*/

//______________________________________________________________________________
/*
class EXP msrBrowser
{
  protected:
  
    basevisitor*  fVisitor;

    virtual void enter (S_msrElement& elem)
      { elem->acceptIn  (*fVisitor); }
      
    virtual void leave (S_msrElement& elem)
      { elem->acceptOut (*fVisitor); }

  public:
      
    msrBrowser (basevisitor* v) : fVisitor (v) {}
    
    virtual ~msrBrowser () {}

    virtual void set (basevisitor* v) {  fVisitor = v; }
    
    virtual void browse (S_msrElement& t)
    {
      enter (t);
      
      typename ctree<T>::literator iter;
      for (iter = t.lbegin(); iter != t.lend(); iter++)
        browse (**iter);
        
      leave (t);
    }
};
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

    bool        fMusicXMLNoteIsTied;

    int         fMusicXMLVoiceNumber;
};
  
/*!
\brief A msr absolute octave representation.
*/
//______________________________________________________________________________
class EXP msrAbsoluteOctave : public msrElement {
  public:
  
    static SMARTP<msrAbsoluteOctave> create (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      int                    musicxmlOctave);
    
    msrAbsoluteOctave (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      int                    musicxmlOctave);
      
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

    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

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
      S_translationSettings& ts, 
      int                    inputLineNumber,
      int                    num,
      int                    denom,
      int                    dots,
      string                 tupletMemberType);
    
    msrDuration (
      S_translationSettings& ts, 
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

    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

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
        kStaccato, kStaccatissimo, kFermata };

    static SMARTP<msrArticulation> create (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      ArticulationKind       articulationKind);

    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrArticulation (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      ArticulationKind       articulationKind);
      
    virtual ~msrArticulation();
  
  private:

    ArticulationKind
          fArticulationKind;
};
typedef SMARTP<msrArticulation> S_msrArticulation;
typedef list<S_msrArticulation> msrArticulationsList;

/*!
\brief A msr slur representation.

  A slur is represented by a SlurKind value (hairpins in LilyPond)
*/
//______________________________________________________________________________
class EXP msrSlur : public msrElement {
  public:

    enum SlurKind { kStartSlur, kContinueSlur, kStopSlur, k_NoSlur };
    
    static SMARTP<msrSlur> create (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      SlurKind               slurKind);

    SlurKind getSlurKind () const        { return fSlurKind; }

    string  slurKindAsString ();

    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrSlur (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      SlurKind               slurKind);
    virtual ~msrSlur();
  
  private:

    SlurKind fSlurKind;
};
typedef SMARTP<msrSlur> S_msrSlur;

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
    
    static SMARTP<msrDynamics> create (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      DynamicsKind           dynamicsKind);

    DynamicsKind getDynamicsKind () const { return fDynamicsKind; }

    string  dynamicsKindAsString ();
    string  dynamicsKindAsLilypondString ();
    
    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrDynamics (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      DynamicsKind           dynamicsKind);
      
    virtual ~msrDynamics();
  
  private:

    DynamicsKind fDynamicsKind;
};
typedef SMARTP<msrDynamics> S_msrDynamics;
typedef list<S_msrDynamics> msrDynamicssList;

/*!
\brief A msr wedge representation.

  A wedge is represented by a WedgeKind value (hairpins in LilyPond)
*/
//______________________________________________________________________________
class EXP msrWedge : public msrElement {
  public:

    enum WedgeKind { kCrescendoWedge, kDecrescendoWedge, kStopWedge };
    
    static SMARTP<msrWedge> create (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      WedgeKind              wedgeKind);

    WedgeKind getWedgeKind () const        { return fWedgeKind; }

    string  wedgeKindAsString ();

    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrWedge (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      WedgeKind              wedgeKind);
      
    virtual ~msrWedge();
  
  private:

    WedgeKind fWedgeKind;
};
typedef SMARTP<msrWedge> S_msrWedge;
typedef list<S_msrWedge> msrWedgesList;

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
        int                    inputLineNumber,
        musicXMLNoteData&      mxmldat,
        msrSlur::SlurKind      slurKind);
    
    // for rests
    bool getNoteIsARest ();
    
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

    int                 getNoteMusicXMLDuration () const
                            {
                              return
                                fMusicXMLNoteData.fMusicXMLDivisions;
                            }
    S_msrDuration       getNoteMsrDuration ()
                            { return fNoteMsrDuration; }   

    string              notePitchAsLilypondString ();

    // articulations
    void                addArticulation (S_msrArticulation art);
    msrArticulationsList
                        getNoteArticulations () const
                            { return fNoteArticulations; }
    
    // dynamics and wedges
    void                addDynamics (S_msrDynamics dyn);
    void                addWedge    (S_msrWedge    wdg);

    msrDynamicssList    getNoteDynamics () { return fNoteDynamics; };
    msrWedgesList       getNoteWedges   () { return fNoteWedges; };

    S_msrDynamics       removeFirstDynamics ();
    S_msrWedge          removeFirstWedge ();

//    void octaveRelativeTo (const msrAbsoluteOctave& otherAbsOct);
        
    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:
 
    msrNote (
        S_translationSettings& ts,
        int                    inputLineNumber,
        musicXMLNoteData&      mxmldat,
        msrSlur::SlurKind      slurKind);
    
    virtual ~msrNote();
    
  private:
  
    // MusicXML informations
    musicXMLNoteData           fMusicXMLNoteData;
    MusicXMLDiatonicPitch      fMusicXMLDiatonicPitch; // JMI

    // MusicXML durations are in in divisions per quarter note,
    // LilyPond durations are in whole notes, hence the "*4" multiplication
    S_msrDuration              fNoteMsrDuration;

    // LilyPond informations
    MsrPitch                   fNoteMsrPitch;

    msrArticulationsList       fNoteArticulations;
    
    msrDynamicssList           fNoteDynamics;
    msrWedgesList              fNoteWedges;

    msrSlur::SlurKind          fNoteSlurKind;
};
typedef SMARTP<msrNote> S_msrNote;
typedef vector<S_msrNote> msrNotesVector;

/*!
\brief The msr parallel music element
*/
//______________________________________________________________________________
class EXP msrParallelMusic : public msrElement {
  public:
    
    enum ElementsSeparator { kEndOfLine, kSpace };

    static SMARTP<msrParallelMusic> create (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      ElementsSeparator      elementsSeparator);

    void         addElementToParallelMusic (S_msrElement elem)
                    { fParallelMusicElements.push_back(elem); }
    S_msrElement getLastElementOfParallelMusic()
                    { return fParallelMusicElements.back(); }
    void         removeLastElementOfParallelMusic ()
                    { fParallelMusicElements.pop_back(); }

    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrParallelMusic (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      ElementsSeparator      elementsSeparator);
      
    virtual ~msrParallelMusic();
    
  private:
  
    msrElementsVector fParallelMusicElements;
    ElementsSeparator fElementsSeparator;

};
typedef SMARTP<msrParallelMusic> S_msrParallelMusic;

/*!
\brief The msr sequence element
*/
//______________________________________________________________________________
class EXP msrSequentialMusic : public msrElement {
  public:
    
   enum ElementsSeparator { kEndOfLine, kSpace };

    static SMARTP<msrSequentialMusic> create (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      ElementsSeparator      elementsSeparator);

    void          prependElementToSequentialMusic (S_msrElement elem)
                      { fSequentialMusicElements.push_front (elem); }
    void          appendElementToSequentialMusic  (S_msrElement elem)
                      { fSequentialMusicElements.push_back (elem); }
    
    S_msrElement  getLastElementOfSequentialMusic ()
                      { return fSequentialMusicElements.back (); }
                      
    void          removeElementFromSequentialMusic (S_msrElement elem);
    void          removeLastElementFromSequentialMusic ()
                      { fSequentialMusicElements.pop_back () ; }

    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrSequentialMusic (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      ElementsSeparator      elementsSeparator);
      
    virtual ~msrSequentialMusic();
    
  private:
  
    msrElementsList    fSequentialMusicElements;
    ElementsSeparator  fElementsSeparator;

};
typedef SMARTP<msrSequentialMusic> S_msrSequentialMusic;

/*!
\brief The msr chord element
*/
//______________________________________________________________________________
class EXP msrChord : public msrElement {
  public:

    static SMARTP<msrChord> create (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      S_msrDuration          chordDuration);
    
    void         addNoteToChord (S_msrNote note)
                    { fChordNotes.push_back(note); }

    void         addArticulation (S_msrArticulation art)
                    { fChordArticulations.push_back(art); }
    
    void         addDynamics (S_msrDynamics dyn)
                    { fChordDynamics.push_back(dyn); }
    void         addWedge    (S_msrWedge    wdg)
                    { fChordWedges.push_back(wdg); }

    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrChord (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      S_msrDuration          chordDuration);
      
    virtual ~msrChord();
  
  private:
  
    msrNotesVector            fChordNotes;
    
    S_msrDuration             fChordDuration;
                              
    msrArticulationsList      fChordArticulations;
    
    msrDynamicssList          fChordDynamics;
    msrWedgesList             fChordWedges;
};
typedef SMARTP<msrChord> S_msrChord;

/*!
\brief A msr LilyPond variable/value association representation.
*/
//______________________________________________________________________________
class EXP msrVarValAssoc : public msrElement {
  public:

    enum VarValSeparator   { kSpace, kEqualSign };
    enum QuotesKind        { kQuotesAroundValue, kNoQuotesAroundValue };
    enum CommentedKind     { kCommented, kUncommented };

    static SMARTP<msrVarValAssoc> create (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      string                 variableName,
      string                 value, 
      VarValSeparator        varValSeparator,
      QuotesKind             quotesKind,
      CommentedKind          commentedKind,
      string                 unit = "");
    
    void    changeAssoc (string value);
    
    string getVariableName  () const { return fVariableName; };
    string getVariableValue () const { return fVariableValue; };

    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrVarValAssoc (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      string                 variableName,
      string                 value, 
      VarValSeparator        varValSeparator,
      QuotesKind             quotesKind,
      CommentedKind          commentedKind,
      string                 unit = "");
      
    virtual ~msrVarValAssoc();
  
  private:

    string     fVariableName;
    string     fVariableValue;
    VarValSeparator fVarValSeparator;
    QuotesKind      fQuotesKind;
    CommentedKind   fCommentedKind;
    string     fUnit;
};
typedef SMARTP<msrVarValAssoc> S_msrVarValAssoc;
typedef vector<S_msrVarValAssoc> msrVarValAssocsVector;
/*!
\brief A msr Scheme variable/value association representation.
*/
//______________________________________________________________________________
/*
class EXP msrSchemeVarValAssoc : public msrElement {
  public:

    enum CommentedKind     { kCommented, kUncommented };

    static SMARTP<msrSchemeVarValAssoc> create (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      string                 variableName,
      string                 value, 
      CommentedKind          commentKind);
    
    void    changeAssoc (string value);
    
    string getVariableValue () const { return fVariableValue; };

    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrSchemeVarValAssoc (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      string                 variableName,
      string                 value, 
      CommentedKind          commentKind);
      
    virtual ~msrSchemeVarValAssoc();
  
  private:

    string     fVariableName;
    string     fVariableValue;
    CommentedKind   fCommentedKind;
};
typedef SMARTP<msrSchemeVarValAssoc> S_msrSchemeVarValAssoc;
*/

/*!
\brief A msr header representation.

  A header is represented by variable/value pairs
*/
//______________________________________________________________________________
class EXP msrHeader : public msrElement {
  public:

    static SMARTP<msrHeader> create (
      S_translationSettings& ts, 
      int                    inputLineNumber);
    
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
    
    msrVarValAssocsVector
                getCreators () const
                    { return fCreators; };
    
    S_msrVarValAssoc
                getRights () const
                    { return fRights; }
    
    msrVarValAssocsVector
                getSoftwares () const
                    { return fSoftwares; };
    
    S_msrVarValAssoc
                getEncodingDate () const
                    { return fEncodingDate; }
    
    S_msrVarValAssoc
                getScoreInstrument () const
                    { return fScoreInstrument; }
  
    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrHeader (
      S_translationSettings& ts, 
      int                    inputLineNumber);
      
    virtual ~msrHeader();
  
  private:

    S_msrVarValAssoc      fWorkNumber;
    S_msrVarValAssoc      fWorkTitle;
    S_msrVarValAssoc      fMovementNumber;
    S_msrVarValAssoc      fMovementTitle;
    msrVarValAssocsVector fCreators;
    S_msrVarValAssoc      fRights;
    msrVarValAssocsVector fSoftwares;
    S_msrVarValAssoc      fEncodingDate;
    S_msrVarValAssoc      fScoreInstrument;

};
typedef SMARTP<msrHeader> S_msrHeader;

/*!
\brief A msr paper representation.

  A paper is represented by variable/value pairs
*/
//______________________________________________________________________________
class EXP msrPaper : public msrElement {
  public:

    static SMARTP<msrPaper> create (
      S_translationSettings& ts, 
      int                    inputLineNumber);
    
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

    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);


  protected:

    msrPaper (
      S_translationSettings& ts, 
      int                    inputLineNumber);
      
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

    static SMARTP<msrLayout> create (
      S_translationSettings& ts, 
      int                    inputLineNumber);
    
    void  addmsrVarValAssoc (S_msrVarValAssoc assoc)
              { fmsrVarValAssocs.push_back(assoc); }
      
//    void addMsrSchemeVarValAssoc (S_msrSchemeVarValAssoc assoc)
//      { fMsrSchemeVarValAssocs.push_back(assoc); }
    
    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrLayout (
      S_translationSettings& ts, 
      int                    inputLineNumber);
      
    virtual ~msrLayout();
  
  private:
  
    msrVarValAssocsVector fmsrVarValAssocs;
};
typedef SMARTP<msrLayout> S_msrLayout;

/*!
\brief A msr repeat representation.

  A repeat is represented by:
    - a sequence of elements for the common part
    - a vector of sequences of elements for the alternate endings
*/
//______________________________________________________________________________
class EXP msrRepeat: public msrElement {
  public:

    static SMARTP<msrRepeat> create (
      S_translationSettings& ts, 
      int                    inputLineNumber);
    
    void    appendElementToCommonPart (S_msrElement elem)
              { fCommonPart->appendElementToSequentialMusic (elem); }
              
    void    appendElementToLastAlternateEnding  (S_msrElement elem)
              { fAlternateEndings.back()->appendElementToSequentialMusic (elem); }
                    
    void    appendNewAlternateEnding ()
              {
                fAlternateEndings.push_back(
                  msrSequentialMusic::create (
                    fTranslationSettings, fInputLineNumber,
                    msrSequentialMusic::kSpace));
              }

    void    setActuallyUsed ()
              { fActuallyUsed = true; }
    
    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrRepeat (
      S_translationSettings& ts, 
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

/*!
\brief A msr barline representation.

  A barline is represented by the number of the next bar
*/
//______________________________________________________________________________
class EXP msrBarLine : public msrElement {
  public:
    
    static SMARTP<msrBarLine> create (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      int                    nextBarNumber);

    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrBarLine (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      int                    nextBarNumber);
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
      S_translationSettings& ts, 
      int                    inputLineNumber,
      string                 contents,
      GapKind                gapKind = kNoGapAfterwards);

    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrComment (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      string                 contents,
      GapKind                gapKind = kNoGapAfterwards);
      
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
    
    static SMARTP<msrBreak> create (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      int                    nextBarNumber);

    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrBreak (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      int                    nextBarNumber);
      
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
    
    static SMARTP<msrBarNumberCheck> create (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      int                    nextBarNumber);

    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrBarNumberCheck (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      int                    nextBarNumber);
      
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
    
    static SMARTP<msrTuplet> create (
      S_translationSettings& ts,
      int                    inputLineNumber);

    enum TupletKind {
      kStartTuplet, kContinueTuplet, kStopTuplet, 
      k_NoTuplet };

    void updateTuplet (int number, int actualNotes, int normalNotes);
    
    int  getTupletNumber () const { return fTupletNumber; }

    void addElementToTuplet (S_msrElement elem)
            { fTupletContents.push_back(elem); }

    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrTuplet (
      S_translationSettings& ts,
      int                    inputLineNumber);
      
    virtual ~msrTuplet();
  
  private:

    int fTupletNumber;
    
    int fActualNotes;
    int fNormalNotes;
    
    msrElementsVector fTupletContents;
};
typedef SMARTP<msrTuplet> S_msrTuplet;
typedef stack<S_msrTuplet> msrTupletsStack;
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
    
    static SMARTP<msrBeam> create (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      int                    number,
      BeamKind               beamKind);

    BeamKind getBeamKind () const { return fBeamKind; }

    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrBeam (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      int                    number,
      BeamKind               beamKind);
      
    virtual ~msrBeam();
  
  private:

    int      fBeamNumber;
    BeamKind fBeamKind;
};
typedef SMARTP<msrBeam> S_msrBeam;

/*!
\brief A msr clef representation.

  A clef is represented by its name
*/
//______________________________________________________________________________
class EXP msrClef : public msrElement {
  public:
    
    enum { kStandardLine, kTrebleStdLine=2, kBassStdLine=4, kCStdLine=3, kTabStdLine=5 };

    static SMARTP<msrClef> create (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      string                 sign,
      int                    line,
      int                    octaveChange);

    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrClef (
      S_translationSettings& ts, 
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

/*!
\brief A msr key representation.

  A key is represented by the tonic and the mode
*/
//______________________________________________________________________________
class EXP msrKey : public msrElement {
  public:
    
    enum KeyMode { kMajor, kMinor };

    static SMARTP<msrKey> create (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      int                    fifths,
      string                 mode,
      int                    cancel);

    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrKey (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      int                    fifths,
      string                 mode,
      int                    cancel);
      
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
      S_translationSettings& ts, 
      int                    inputLineNumber,
      int                    numerator,
      int                    denominator);

    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrTime (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      int                    numerator,
      int                    denominator);
      
    virtual ~msrTime();
  
  private:

    rational fRational;
    bool     fGenerateNumericalTime;
};
typedef SMARTP<msrTime> S_msrTime;

/*!
\brief A tempo representation.

  A tempo is represented by the lyrics to use
*/
//______________________________________________________________________________
class EXP msrTempo : public msrElement {
  public:

    static SMARTP<msrTempo> create (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      int                    tempoUnit,
      int                    perMinute);
    
    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrTempo (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      int                    tempoUnit,
      int                    perMinute);
      
    virtual ~msrTempo();
  
  private:
  
    int  fTempoUnit;
    int  fPerMinute;
};
typedef SMARTP<msrTempo> S_msrTempo;

/*!
\brief A msr midi representation.

  A midi is represented by variable/value pairs
*/
//______________________________________________________________________________
class EXP msrMidi : public msrElement {
  public:

    static SMARTP<msrMidi> create (
      S_translationSettings& ts, 
      int                    inputLineNumber);

    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrMidi (
      S_translationSettings& ts, 
      int                    inputLineNumber);
      
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
      kSkipChunk, kSlurChunk, kTiedChunk,
      kBreakChunk,
      k_NoChunk };

    static SMARTP<msrLyricsChunk> create (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      LyricsChunkType        chunkType,
      string                 chunkText,
      S_msrDuration          msrDuration);
     
    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrLyricsChunk (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      LyricsChunkType        chunkType,
      string                 chunkText,
      S_msrDuration          msrDuration);
        
    virtual ~msrLyricsChunk();

  private:
  
    LyricsChunkType fLyricsChunkType;
    string          fChunkText;
    S_msrDuration  fChunkDuration;
};
typedef SMARTP<msrLyricsChunk> S_msrLyricsChunk;
typedef vector<S_msrLyricsChunk> msrLyricsChunksVector;

/*!
\brief A msr lyrics representation.

  A lyrics is represented by a list of lyrics chunks,
*/
//______________________________________________________________________________
class EXP msrLyrics : public msrElement {
  public:

    static SMARTP<msrLyrics> create (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      int                    lyricsNumber,
      S_msrVoice             lyricsVoice);
    
    int     getLyricsNumber () const
                { return fLyricsNumber; }
                
    string  getLyricsName () const;
                
    S_msrVoice
            getLyricsVoice () const
                { return fLyricsVoice; }
                
    msrLyricsChunksVector
            getLyricsChunks () const
                { return fLyricsChunks; }

    void    addTextChunkToLyrics (
              int                             inputLineNumber,
              string                          syllabic, // JMI ???
              msrLyricsChunk::LyricsChunkType chunkType,
              string                          text,
              bool                            elision,
              S_msrDuration                   msrDuration);
      
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

    void    addChunkToLyrics (S_msrLyricsChunk chunk)
                { fLyricsChunks.push_back (chunk); }
                
    int     getLyricsTextPresent() { return fLyricsTextPresent; }
    
    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrLyrics (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      int                    lyricsNumber,
      S_msrVoice             lyricsVoice);
    virtual ~msrLyrics();
  
  private:

    int                       fLyricsNumber;
    S_msrVoice                fLyricsVoice;
    
    msrLyricsChunksVector     fLyricsChunks;

    bool                      fLyricsTextPresent;
};
typedef SMARTP<msrLyrics> S_msrLyrics;
typedef map<int, S_msrLyrics> msrIntToLyricsMap;

/*!
\brief A msr voice representation.

  A vpoce is represented by a its string contents
*/
//______________________________________________________________________________
class EXP msrVoice : public msrElement {
  public:

    static SMARTP<msrVoice> create (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      int                    voiceNumber,
      int                    staffRelativeVoiceNumber,
      S_msrStaff             voiceStaff);
                          
    int     getVoiceNumber () const
                { return fVoiceNumber; }
                
    S_msrStaff
            getVoiceStaff () const
                { return fVoiceStaff; }
                
    map<int, S_msrLyrics>
            getVoiceLyricsMap () const
                { return fVoiceLyricsMap; }

    string  getVoiceName () const;

    S_msrLyrics
            addLyricsToVoice (
              int inputLineNumber,
              int lyricsNumber);
              
    S_msrLyrics
            fetchLyricsFromVoice (int lyricsNumber);

    S_msrLyrics
            getVoiceMasterLyrics () { return fVoiceMasterLyrics; }
               
    void    appendNoteToVoice       (S_msrNote note);
    void    appendChordToVoice      (S_msrChord chord);
    void    appendTupletToVoice     (S_msrTuplet tuplet);
    void    appendElementToVoice    (S_msrElement elem); // for others

    S_msrElement
            getVoiceSequentialMusicLastElement ()
                { return fVoiceSequentialMusic->getLastElementOfSequentialMusic (); }
                  
    void    removeLastElementFromVoiceSequentialMusic ()
                { fVoiceSequentialMusic->removeLastElementFromSequentialMusic (); }

    void    removeElementFromVoiceSequentialMusic (S_msrElement elem)
                { fVoiceSequentialMusic->removeElementFromSequentialMusic (elem); }


    S_msrSequentialMusic
            getVoiceSequentialMusic () const
                { return fVoiceSequentialMusic; }

    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrVoice (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      int                    voiceNumber,
      int                    staffRelativeVoiceNumber,
      S_msrStaff             voiceStaff);
    virtual ~msrVoice();
  
  private:

    int                       fVoiceNumber;
    int                       fStaffRelativeVoiceNumber;
    S_msrStaff                fVoiceStaff;

    bool                      fVoiceContainsActualNotes;

    // the lyrics map
    // [0] is used as a master lyrics, collecting skips along the way
    // to be used as a 'prelude' by actual lyrics that start at later points
    map<int, S_msrLyrics>     fVoiceLyricsMap;
    S_msrLyrics               fVoiceMasterLyrics;

    // the implicit sequence containing the code generated for the voice
    S_msrSequentialMusic      fVoiceSequentialMusic;
  

    // the implicit repeat at the beginning of the voice
    // will be ignored if the voice has no repeats at all
    S_msrRepeat               fVoiceMsrRepeat;
};
typedef SMARTP<msrVoice> S_msrVoice;

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
      int                    inputLineNumber,
      int                    staffNumber,
      S_msrPart              staffPart);
    
    int     getStaffNumber () const
                { return fStaffNumber; }
                
    S_msrPart
            getStaffPart () const
                { return fStaffPart; }

    map<int, S_msrVoice>
            getStaffVoicesMap ()
                { return fStaffVoicesMap; }

    string  getStaffName () const;

    S_msrVoice
            addVoiceToStaff (
              int inputLineNumber,
              int voiceNumber);
              
    S_msrVoice
            fetchVoiceFromStaff (int voiceNumber);
                              
    void    setStaffKey  (S_msrKey  key);
    void    setStaffTime (S_msrTime time);
    void    setStaffClef (S_msrClef clef);
              
    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrStaff (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      int                    staffNumber,
      S_msrPart              staffPart);
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
      int                    inputLineNumber,
      string                 partMusicXMLName,
      S_msrPartgroup         partPartgroup);

    void    setPartName (string partName)
                { fPartName = partName; }
                
    void    setPartAbbreviation (string partAbbreviation)
                { fPartAbbreviation = partAbbreviation; }
                
    void    setPartInstrumentName (string partInstrumentName)
                { fPartInstrumentName = partInstrumentName; }
                
// JMI    void    reusePartAs (
      //        string newPartMusicXMLName);
              
    string  getPartMusicXMLName () const
                { return fPartMusicXMLName; }
                
    S_msrPartgroup
            getPartPartgroup () const
                { return fPartPartgroup; }
                
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
            addStaffToPart (
              int inputLineNumber,
              int staffNumber);

    S_msrStaff
            fetchStaffFromPart (int staffNumber);

    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrPart (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      string                 partMusicXMLName,
      S_msrPartgroup         partPartgroup);
      
    virtual ~msrPart();
  
  private:
    
    string                  fPartMusicXMLName;
    S_msrPartgroup          fPartPartgroup;

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
class EXP msrPartgroup : public msrElement {
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

    enum PartgroupTypeKind {
        kStartPartgroupType, kStopPartgroupType,
        k_NoPartgroupType };
          
    enum PartgroupSymbolKind {
        kBracePartgroupSymbol, kBracketPartgroupSymbol,
        kLinePartgroupSymbol, kSquarePartgroupSymbol,
        k_NoPartgroupSymbol };
          
    static SMARTP<msrPartgroup> create (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      int                    partGroupNumber,
      string                 partGroupName,
      string                 partGroupAbbreviation,
      PartgroupSymbolKind    partGroupSymbolKind,
      int                    partGroupSymbolDefaultX,
      bool                   partGroupBarline);

    static int gPartgroupsCounter;
    
    int       getPartgroupNumber () const
                  { return fPartgroupNumber; }
    
    string    getPartgroupName () const
                  { return fPartgroupName; }

    string    getPartgroupAbbreviation () const
                  { return fPartgroupAbbreviation; }

    PartgroupSymbolKind
              getPartgroupSymbolKind () const
                  { return fPartgroupSymbolKind; }

    int       getPartgroupSymbolDefaultX () const
                  { return fPartgroupSymbolDefaultX; }

    bool      getPartgroupBarline () const
                  { return fPartgroupBarline; }
    
    string    getPartgroupCombinedName () const;

    S_msrPart addPartToPartgroup (
                int    inputLineNumber,
                string partMusicXMLName);
    
    void      prependSubPartgroupToPartgroup (
                S_msrPartgroup partGroup);

    S_msrPart fetchPartFromPartgroup (string partMusicXMLName);
                
    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrPartgroup (
      S_translationSettings& ts, 
      int                    inputLineNumber,
      int                    partGroupNumber,
      string                 partGroupName,
      string                 partGroupAbbreviation,
      PartgroupSymbolKind    partGroupSymbolKind,
      int                    partGroupSymbolDefaultX,
      bool                   partGroupBarline);
            
    virtual ~msrPartgroup();
  
  private:

    int                     fPartgroupAbsoluteNumber;
    
    int                     fPartgroupNumber;
        
    string                  fPartgroupName;
    string                  fPartgroupAbbreviation;

    PartgroupSymbolKind     fPartgroupSymbolKind;
    int                     fPartgroupSymbolDefaultX;

    bool                    fPartgroupBarline;

    // accessing parts by name
    msrPartsMap             fPartgroupPartsMap;
//    msrPartsList            fPartgroupPartsList; JMI

    // allowing for both parts and (sub-)part groups as elements
    msrElementsList         fPartgroupElements;
};
typedef SMARTP<msrPartgroup> S_msrPartgroup;
typedef list<S_msrPartgroup> msrPartgroupsList;
typedef map<int, S_msrPartgroup> msrPartgroupsMap;

/*!
\brief A msr score representation.

  A score is represented by a its string contents
*/
//______________________________________________________________________________
class EXP msrScore : public msrElement {
  public:

    static SMARTP<msrScore> create (
      S_translationSettings& ts, 
      int                    inputLineNumber);

    void addPartgroupToScore (S_msrPartgroup partGroup);

    virtual void printMusicXML     (ostream& os);
    virtual void printMSR          (ostream& os);
    virtual void printScoreSummary (ostream& os);
    virtual void printLilyPondCode (ostream& os);

  protected:

    msrScore (
      S_translationSettings& ts, 
      int                    inputLineNumber);
      
    virtual ~msrScore();
  
  private:

    msrPartgroupsList       fPartgroupsList;
    
//    msrPartgroupsStack     fScorePartgroupsStack; JMI
//    msrPartgroupsMap        fScorePartgroupsMap;
};
typedef SMARTP<msrScore> S_msrScore;


/*! @} */

}


#endif
