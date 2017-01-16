/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __xmlPartsInformation__
#define __xmlPartsInformation__

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

//JMI #include "musicxml2msr.h"

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

class msrLyrics;
typedef SMARTP<msrLyrics> S_msrLyrics;

class msrVoice;
typedef SMARTP<msrVoice> S_msrVoice;

class msrStaff;
typedef SMARTP<msrStaff> S_msrStaff;

class msrPart;
typedef SMARTP<msrPart> S_msrPart;


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
  
    // constructors/destructor
    // ------------------------------------------------------

    msrOptions();
  
    virtual ~msrOptions();
 
  public:

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
    bool                            fDisplaySummary;

    // parts renaming
    map<string, string>             fPartsRenaming;
};
typedef SMARTP<msrOptions> S_msrOptions;

//______________________________________________________________________________
class EXP xmlPartsInformationElement : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

  protected:

     xmlPartsInformationElement (
      S_msrOptions& msrOpts, 
      int           inputLineNumber);

    virtual ~xmlPartsInformationElement();

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
    
  protected:

    S_msrOptions   fMsrOptions;
     
    int            fInputLineNumber;
};
typedef SMARTP<xmlPartsInformationElement> S_xmlPartsInformationElement;
EXP ostream& operator<< (ostream& os, const S_xmlPartsInformationElement& elt);


/*!
\brief A words representation.

  A words is represented by the lyrics to use
*/
//______________________________________________________________________________
class EXP msrWords : public xmlPartsInformationElement
{
  public:

    // data types
    // ------------------------------------------------------

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

    // constructors/destructor
    // ------------------------------------------------------

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
\brief A MSR page geometry representation.

  A page geometry is represented by variable/value pairs
*/
//______________________________________________________________________________
class EXP msrPageGeometry : public xmlPartsInformationElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrPageGeometry> create (
      S_msrOptions& msrOpts, 
      int           inputLineNumber);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

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
\brief A msr lyrics chunk representation.

  A lyrics chunk is represented by a type and and a string.
  In the case of "single", the list contains only one string
*/
//______________________________________________________________________________
class EXP msrLyricschunk : public xmlPartsInformationElement
{
  public:

    // data types
    // ------------------------------------------------------

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
      S_msrLyrics        lyricschunkLyricsUplink);

    SMARTP<msrLyricschunk> createLyricschunkBareClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrLyricschunk (
      S_msrOptions&      msrOpts, 
      int                inputLineNumber,
      msrLyricschunkKind lyricschunkKind,
      string             chunkText,
      int                divisions,
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

    void      setLyricschunkNote (S_msrNote note)
                  { fLyricschunkNote = note; }

    // services
    // ------------------------------------------------------

    string    lyricschunkKindAsString ();

    string    lyricschunkAsString ();

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

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
class EXP msrLyrics : public xmlPartsInformationElement
{
  public:

    // data types
    // ------------------------------------------------------

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
    
    SMARTP<msrLyrics> createLyricsBareClone (
      S_msrVoice clonedVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

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

    // print
    // ------------------------------------------------------

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
\brief A msr harmony representation.

  A harmony is represented by a list of lyrics chunks,
*/
//______________________________________________________________________________
class EXP msrHarmony : public xmlPartsInformationElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrHarmonyKind {
      kMajor, kMinor, kDominant,
      kSuspendedFourth,
      kMajorSeventh,kMinorSeventh,
      kMajorNinth, kMinorNinth};

    enum msrHarmonyDegreeTypeKind {
      kAdd, kAlter, kSubstract};

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrHarmony> create (
      S_msrOptions&         msrOpts, 
      int                   inputLineNumber,
      char                  harmonyRootStep,
      float                 harmonyRootAlter,
      msrHarmonyKind        harmonyKind,
      string                harmonyKindText,
      char                  harmonyBassStep,
      float                 harmonyBassAlter,
      S_msrPart             harmonyPartUplink);
    
    SMARTP<msrHarmony> createHarmonyBareClone (
      S_msrPart clonedPart);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrHarmony (
      S_msrOptions&         msrOpts, 
      int                   inputLineNumber,
      char                  harmonyRootStep,
      float                 harmonyRootAlter,
      msrHarmonyKind        harmonyKind,
      string                harmonyKindText,
      char                  harmonyBassStep,
      float                 harmonyBassAlter,
      S_msrPart             harmonyPartUplink);

    virtual ~msrHarmony();
  
  public:

    // set and get
    // ------------------------------------------------------

    char        getHarmonyRootStep () const
                  { return fHarmonyRootStep; }
                
    float       getHarmonyRootAlter () const
                  { return fHarmonyRootAlter; }
                
    msrHarmonyKind
                getHarmonyKind () const
                  { return fHarmonyKind; }
                
    string      getHarmonyKindText () const
                  { return fHarmonyKindText; }
                
                
    char        getHarmonyBassStep () const
                  { return fHarmonyBassStep; }
                
    float       getHarmonyBassAlter () const
                  { return fHarmonyBassAlter; }
                
    S_msrPart  getHarmonyVoiceUplink () const
                  { return fHarmonyPartUplink; }
                

    // services
    // ------------------------------------------------------

    string    harmonyKindAsString () const;
    
    string    harmonyAsString () const;
   
    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    char                      fHarmonyRootStep;
    float                     fHarmonyRootAlter;

    msrHarmonyKind            fHarmonyKind;
    string                    fHarmonyKindText;

    char                      fHarmonyBassStep;
    float                     fHarmonyBassAlter;
    
    S_msrPart                 fHarmonyPartUplink;
};
typedef SMARTP<msrHarmony> S_msrHarmony;
EXP ostream& operator<< (ostream& os, const S_msrHarmony& elt);

/*!
\brief A msr barline representation.

  A barline is represented by the number of the next bar
*/
//______________________________________________________________________________
class EXP msrBarline : public xmlPartsInformationElement
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
      kNone,
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
      S_msrOptions&             msrOpts, 
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

    bool
                getBarlineHasSegno () const
                    { return fBarlineHasSegno; }
    bool
                getBarlineHasCoda () const
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
\brief A msr voice representation.

  A vpoce is represented by a its string contents
*/
//______________________________________________________________________________
class EXP msrVoice : public xmlPartsInformationElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrVoice> create (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           externalVoiceNumber,
      S_msrStaff    voiceStaffUplink);

                          /* JMI
    static SMARTP<msrVoice> createRest (
      S_msrOptions& msrOpts,
      int           inputLineNumber,
      int           divisions,
      int           externalVoiceNumber);
    */
    
    SMARTP<msrVoice> createVoiceBareClone (
      S_msrStaff clonedStaff);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrVoice (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           externalVoiceNumber,
      S_msrStaff    voiceStaffUplink);
      
    virtual ~msrVoice();
  
  public:

    // set and get
    // ------------------------------------------------------

    int           getExternalVoiceNumber () const
                      { return fExternalVoiceNumber; }
                
    void          setExternalVoiceNumber (int externalVoiceNumber)
                      { fExternalVoiceNumber = externalVoiceNumber; }

    int           getStaffRelativeVoiceNumber () const
                      { return fStaffRelativeVoiceNumber; }
                
    S_msrStaff    getVoiceStaffUplink () const
                      { return fVoiceStaffUplink; }
                
    const map<int, S_msrLyrics>&
                  getVoiceLyricsMap () const
                      { return fVoiceLyricsMap; }

    string        getVoiceName () const;

    void          setNewVoicechunkForVoice (int inputLineNumber); // JMI

    S_msrVoicechunk
                  getVoiceVoicechunk () const
                      { return fVoiceVoicechunk; }

    S_msrLyrics   getVoiceLyricsmaster () const
                      { return fVoiceLyricsmaster; }
               
    int           getVoiceActualNotesCounter () const
                      { return fVoiceActualNotesCounter; }

    int           getMeasureZeroHasBeenMetInVoice () const
                      { return fMeasureZeroHasBeenMetInVoice; }

    // divisions per whole note
    void          setVoiceDivisionsPerWholeNote (
                    int divisionsPerWholeNote);
                      
    const int     getVoiceDivisionsPerWholeNote () const
                      { return fVoiceDivisionsPerWholeNote; }

    S_msrTime     getVoiceTime () const
                      { return fVoiceTime; }
               
     // measure number
    void          setVoiceMeasureNumber (
                    int inputLineNumber,
                    int measureNumber);
                      
    const int     getVoiceMeasureNumber () const
                      { return fVoiceMeasureNumber; }

    // has music been inserted in the voice?
    void          setMusicHasBeenInsertedInVoice ()
                      { fMusicHasBeenInsertedInVoice = true; }

    bool          getMusicHasBeenInsertedInVoice () const
                      { return fMusicHasBeenInsertedInVoice; }

    // services
    // ------------------------------------------------------
    /*
    void          catchupToMeasureLocation (
                    int                       inputLineNumber,
                    int                       divisionsPerWholeNote,
                    const msrMeasureLocation& measureLocation);
*/

    void          forceVoiceMeasureNumberTo (int measureNumber); // JMI
                      
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
    
    void          appendGracenotesToVoice (
                    S_msrGracenotes gracenotes);
    
    S_msrLyricschunk
                  addTextLyricschunkToVoice (
                    int       inputLineNumber,
                    int       lyricsNumber,
                    string    syllabic,
                    msrLyricschunk::msrLyricschunkKind
                              lyricschunkKind,
                    string    text,
                    bool      elision,
                    int       divisions);
    
    S_msrLyricschunk
                  addSkipLyricschunkToVoice (
                    int       inputLineNumber,
                    int       lyricsNumber,
                    int       divisions);
    
    S_msrLyricschunk
                  addTiedLyricschunkToVoice (
                    int       inputLineNumber,
                    int       lyricsNumber,
                    int       divisions);
    
    S_msrLyricschunk
                  addSlurLyricschunkToVoice (
                    int       inputLineNumber,
                    int       lyricsNumber,
                    int       divisions);
    
    S_msrLyricschunk
                  addSlurBeyondEndLyricschunkToVoice (
                    int       inputLineNumber,
                    int       lyricsNumber,
                    int       divisions);
    
    void          appendBarCheckToVoice (S_msrBarCheck bnc);
    void          appendBarnumberCheckToVoice (
                    S_msrBarnumberCheck bnc);

    void          appendBreakToVoice (S_msrBreak break_);

    void          appendRepeatToVoice (S_msrRepeat repeat);
    
    void          prependBarlineToVoice (S_msrBarline barline);
    void          appendBarlineToVoice (S_msrBarline barline);
    
    void          appendSegnoToVoice (S_msrSegno segno);
    void          appendCodaToVoice (S_msrCoda coda);
    void          appendEyeglassesToVoice (
                    S_msrEyeglasses eyeglasses);
    void          appendPedalToVoice (S_msrPedal pedal);
    
    S_xmlPartsInformationElement  removeLastElementFromVoice (
                    int inputLineNumber);

    S_msrLyrics
                  addLyricsToVoiceByItsNumber (
                    int inputLineNumber,
                    int lyricsNumber);

    void          addLyricsToVoice (S_msrLyrics lyrics);

/*
    void          bringVoiceToPosition (
                    int inputLineNumber,
                    int measurePosition);
*/
    S_msrLyrics   createLyricsInVoiceIfNeeded (
                    int inputLineNumber,
                    int lyricsNumber);
    
    void          finalizeLastMeasureOfVoice (int inputLineNumber);

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    // voice number in MusicXML may be greater than 4
    // while there can only be 4 in a staff
    // we thus have to cope with that
    int                       fExternalVoiceNumber;
    int                       fStaffRelativeVoiceNumber;

    S_msrStaff                fVoiceStaffUplink;

    bool                      fVoiceActualNotesCounter;

    int                       fVoiceDivisionsPerWholeNote;

    S_msrTime                 fVoiceTime;

    int                       fVoiceMeasureNumber;
    
    // anacrusis detection
    bool                      fMeasureZeroHasBeenMetInVoice;
    bool                      fMeasureNumberHasBeenSetInVoice;
    bool                      fMusicHasBeenInsertedInVoice;
    
    // the chunk in the voice contains the measures
    // it is created implicitly for every voice,
    S_msrVoicechunk           fVoiceVoicechunk;

    // the master lyrics, collecting skips along the way,
    // to be used as a 'prelude' by actual lyrics
    // that start at later points
    S_msrLyrics               fVoiceLyricsmaster;
    
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
class EXP msrStaff : public xmlPartsInformationElement
{
  public:

    enum msrStaffKind {
      kRegularStaff, kTablatureStaff, kPercussionStaff};

    enum msrStaffTypeKind {
      kOssiaStaffType, kCueStaffType, kEditorialStaffType,
      kRegularStaffType, kAlternateStaffType};
     
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStaff> create (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           staffNumber,
      S_msrPart     fStaffPartUplink);
    
    SMARTP<msrStaff> createStaffBareClone (
      S_msrPart clonedPart);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrStaff (
      S_msrOptions& msrOpts, 
      int           inputLineNumber,
      int           staffNumber,
      S_msrPart     fStaffPartUplink);
      
    virtual ~msrStaff();
  
  public:

    // set and get
    // ------------------------------------------------------

    msrStaffKind  getStaffKind () const
                      { return fStaffKind; }
                
    int           getStaffNumber () const
                      { return fStaffNumber; }
                
    S_msrPart     getStaffPartUplink () const
                      { return fStaffPartUplink; }

    string        getStaffName () const;

    string        getStaffInstrumentName () const
                      { return fStaffInstrumentName; }

    S_msrClef     getStaffClef () const { return fStaffClef; };
    S_msrKey      getStaffKey  () const { return fStaffKey; };
    S_msrTime     getStaffTime () const { return fStaffTime; };
    
    S_msrTranspose
                  getStaffTranspose () const { return fStaffTranspose; };
    
    void          setStaffClef (S_msrClef clef);
    void          setStaffKey  (S_msrKey  key);
    void          setStaffTime (S_msrTime time);
    
    void          setStaffTranspose (S_msrTranspose transpose);

    const map<int, S_msrVoice>&
                  getStaffVoicesCorrespondanceMap () const
                      { return fStaffVoicesCorrespondanceMap; }

    const map<int, S_msrVoice>&
                  getStaffVoicesMap () const
                      { return fStaffVoicesMap; }

    const list<S_msrStafftuning>&
                  getStafftuningsList ()
                      { return fStafftuningsList; }

    // divisions per whole note
    void          setStaffDivisionsPerWholeNote (
                    int divisionsPerWholeNote);
                      
    const int     getStaffDivisionsPerWholeNote () const
                      { return fStaffDivisionsPerWholeNote; }
          
    // measure number
    void          setStaffMeasureNumber (
                  int inputLineNumber,
                  int measureNumber);
                      
    const int     getPartMeasureNumber () const
                      { return fStaffMeasureNumber; }

    // voice master
    const S_msrVoice
                  getStaffVoiceMaster () const
                      { return fStaffVoiceMaster; }

    // services
    // ------------------------------------------------------

    S_msrVoice    addVoiceToStaffByItsRelativeNumber (
                    int inputLineNumber,
                    int voiceRelativeNumber);
  
    S_msrVoice    registerVoiceInStaffByItsExternalNumber (
                    int inputLineNumber,
                    int externalVoiceNumber);
  
    string        staffKindAsString () const;
    
    void          setAllStaffVoicesDivisionsPerWholeNote (
                    int divisions);

    void          setAllStaffVoicesMeasureNumber (
                    int inputLineNumber,
                    int measureNumber);

    S_msrVoice    addVoiceToStaffByItsNumber (
                    int inputLineNumber,
                    int externalVoiceNumber);

    void          registerVoiceInStaff (
                    int inputLineNumber,
                    S_msrVoice voice);
    
    S_msrVoice    fetchVoiceFromStaff (
                    int inputLineNumber, int externalVoiceNumber);
                              
    void          appendClefToAllStaffVoices (S_msrClef clef);
    void          appendKeyToAllStaffVoices  (S_msrKey   key);
    void          appendTimeToAllStaffVoices (S_msrTime time);
    
    void          appendTransposeToAllStaffVoices (S_msrTranspose transpose);

    void          addStafftuningToStaff (
                  S_msrStafftuning stafftuning)
                      { fStafftuningsList.push_back (stafftuning); }
                  /*
    void          bringAllStaffVoicesToPosition (
                    int inputLineNumber,
                    int measurePosition);
*/
    void          removeStaffEmptyVoices ();

    void          finalizeLastMeasureOfStaff (int inputLineNumber);
    
    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    // data
    
    msrStaffKind            fStaffKind;
    
    static int              gMaxStaffVoices;

    int                     fStaffNumber;
    S_msrPart               fStaffPartUplink;

    map<int, S_msrVoice>    fStaffVoicesCorrespondanceMap;
                              //numbered 1 to gMaxStaffVoices
    map<int, S_msrVoice>    fStaffVoicesMap;
                              // [0] is used form staff master voice

    string                  fStaffInstrumentName;

    int                     fStaffDivisionsPerWholeNote;    

    int                     fStaffMeasureNumber;
    
    S_msrVoice              fStaffVoiceMaster;

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
class EXP msrPart : public xmlPartsInformationElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrPart> create (
      S_msrOptions&  msrOpts, 
      int            inputLineNumber,
      string         partID,
      S_msrPartgroup partPartgroupUplink);
                
    SMARTP<msrPart> createPartBareClone (
      S_msrPartgroup clonedPartgroup);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrPart (
      S_msrOptions&  msrOpts, 
      int            inputLineNumber,
      string         partID,
      S_msrPartgroup partPartgroupUplink);
      
    virtual ~msrPart();
  
  public:

    // set and get
    // ------------------------------------------------------

    void          setPartAbbreviation (string partAbbreviation)
                      { fPartAbbreviation = partAbbreviation; }
                
    void          setPartInstrumentName (string partInstrumentName)
                      { fPartInstrumentName = partInstrumentName; }
                              
    void          setPartID (string partID)
                      { fPartID = partID; }
    
    void          setPartMSRName (string partMSRName);
    
    void          setPartName (string partName)
                      { fPartName = partName; }
    
    string        getPartID () const
                      { return fPartID; }

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
    void          setPartDivisionsPerWholeNote (
                    int divisionsPerWholeNote);
                      
    const int     getPartDivisionsPerWholeNote () const
                      { return fPartDivisionsPerWholeNote; }
          
    S_msrClef     getPartClef () const { return fPartClef; };
    S_msrKey      getPartKey  () const { return fPartKey; };
    S_msrTime     getPartTime () const { return fPartTime; };
    
    S_msrTranspose
                  getPartTranspose () const { return fPartTranspose; };
    
    void          setPartClef (S_msrClef clef);
    void          setPartKey  (S_msrKey  key);
    void          setPartTime (S_msrTime time);

    void          setPartTranspose (S_msrTranspose transpose);

    // measure number
    void          setPartMeasureNumber (
                    int inputLineNumber,
                    int measureNumber);
                      
    const int     getPartMeasureNumber () const
                      { return fPartMeasureNumber; }

    const bool    getMeasureZeroHasBeenMetInPart () const
                      { return fMeasureZeroHasBeenMetInPart; }

    void          setPartMeasurePositionHighTide (
                    int position)
                      { fPartMeasurePositionHighTide = position; }
                      
    void          updatePartMeasurePositionHighTide (
                    int inputLineNumber,
                    int position);
                    
    const int     getPartMeasurePositionHighTide () const
                      { return fPartMeasurePositionHighTide; }

/* JMI
    // voice master
    const S_msrVoice
                  getPartVoicemaster () const
                      { return fPartVoicemaster; }
*/

    // services
    // ------------------------------------------------------

    void          setAllPartStavesDivisionsPerWholeNote (
                    int divisions);
    
    void          setAllPartStavesMeasureNumber (
                    int inputLineNumber,
                    int measureNumber);
  /*
    void          setAllPartStavesMeasureLocation (
                    int                       inputLineNumber,
                    const msrMeasureLocation& measureLocation);
*/
    void          setAllPartStavesClef (S_msrClef clef);
              
    void          setAllPartStavesKey  (S_msrKey  key);
              
    void          setAllPartStavesTime (S_msrTime time);
              
    void          setAllPartStavesTranspose (S_msrTranspose transpose);
              
    S_msrStaff    addStaffToPartByItsNumber (
                    int inputLineNumber,
                    int staffNumber);
    
    void          addStaffToPart (S_msrStaff staff);

    S_msrStaff    fetchStaffFromPart (int staffNumber);

    void          appendHarmonyToPart (S_msrHarmony harmony);

    void          handleBackup (int divisions);
  /*  
    void          bringAllPartVoicesToPosition (
                    int inputLineNumber,
                    int measurePosition);
*/
    void          removePartEmptyVoices ();

    void          finalizeLastMeasureOfPart (int inputLineNumber);

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:
    
    string                  fPartID; // native
    string                  fPartMSRName;
                              // may be different than fPartID
                              // if renamed,
                              // coined in constructor

    string                  fPartName; // from '<part-name/>'
    string                  fPartAbbreviation;
    string                  fPartInstrumentName;

    S_msrPartgroup          fPartPartgroupUplink;

    map<int, S_msrStaff>    fPartStavesMap;

    int                     fPartDivisionsPerWholeNote;

    int                     fPartMeasureNumber;
    bool                    fMeasureZeroHasBeenMetInPart;
    int                     fPartMeasurePositionHighTide;
    
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
class EXP msrPartgroup : public xmlPartsInformationElement
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

    SMARTP<msrPartgroup> createPartgroupBareClone (
      S_msrPartgroup clonedPartgroup); // the uplink

  protected:

    // constructors/destructor
    // ------------------------------------------------------

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
                
    const list<S_xmlPartsInformationElement>&
                  getPartgroupElements () const
                      { return fPartgroupElements; }

    S_msrPartgroup
                  getPartgroupPartgroupUplink () const
                      { return fPartgroupPartgroupUplink; }

    // services
    // ------------------------------------------------------

    S_msrPart   addPartToPartgroupByItsID (
                  int    inputLineNumber,
                  string partID);
    
    void        addPartToPartgroup (S_msrPart part);
                
    void        prependSubPartgroupToPartgroup (
                  S_msrPartgroup partgroup);

    S_msrPart   fetchPartFromPartgroup (string partID);

    // visitors
    // ------------------------------------------------------

    virtual void acceptIn  (basevisitor* v);
    virtual void acceptOut (basevisitor* v);

    virtual void browseData (basevisitor* v);

    // print
    // ------------------------------------------------------

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
    list<S_xmlPartsInformationElement>      fPartgroupElements;
    
    S_msrPartgroup          fPartgroupPartgroupUplink;
};
typedef SMARTP<msrPartgroup> S_msrPartgroup;
EXP ostream& operator<< (ostream& os, const S_msrPartgroup& elt);

/*!
\brief A msr score representation.

  A score is represented by a its string contents
*/
//______________________________________________________________________________
class EXP xmlPartsInformation : public xmlPartsInformationElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<xmlPartsInformation> create (
      S_msrOptions& msrOpts, 
      int           inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xmlPartsInformation (
      S_msrOptions& msrOpts, 
      int           inputLineNumber);
      
    virtual ~xmlPartsInformation();
  
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

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    S_msrIdentification  fIdentification;

    S_msrPageGeometry    fPageGeometry;
    
    list<S_msrPartgroup> fPartgroupsList;
 };
typedef SMARTP<xmlPartsInformation> S_xmlPartsInformation;
EXP ostream& operator<< (ostream& os, const S_xmlPartsInformation& elt);


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
