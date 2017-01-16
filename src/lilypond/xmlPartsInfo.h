/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __xmlPartsInfo__
#define __xmlPartsInfo__

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

class xmlVoice;
typedef SMARTP<xmlVoice> S_xmlVoice;

class xmlStaff;
typedef SMARTP<xmlStaff> S_xmlStaff;

class xmlPart;
typedef SMARTP<xmlPart> S_xmlPart;


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
class EXP xmlPartsInfoOptions : public smartable
{
  public:

    static SMARTP<xmlPartsInfoOptions> create ();
    
  protected:
  
    // constructors/destructor
    // ------------------------------------------------------

    xmlPartsInfoOptions();
  
    virtual ~xmlPartsInfoOptions();
 
  public:

    // advanced options
    bool                            fCreateStaffRelativeVoiceNumbers;
    bool                            fDelayRestsDynamics;

    // parts info display
    bool                            fDisplayPartsInfo;

    // lyrics display
    bool                            fDontDisplayMSRLyrics;
    
    // MSR score summary
    bool                            fDisplaySummary;

};
typedef SMARTP<xmlPartsInfoOptions> S_xmlPartsInfoOptions;

//______________________________________________________________________________
class EXP xmlPartsInfoElement : public smartable
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

  protected:

     xmlPartsInfoElement (
      S_xmlPartsInfoOptions& msrOpts, 
      int           inputLineNumber);

    virtual ~xmlPartsInfoElement();

  public:

    // set and get
    // ------------------------------------------------------

    int getInputLineNumber ()
      { return fInputLineNumber; }

    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);
    
  protected:

    S_xmlPartsInfoOptions   fXmlPartsInfoOptions;
     
    int            fInputLineNumber;
};
typedef SMARTP<xmlPartsInfoElement> S_xmlPartsInfoElement;
EXP ostream& operator<< (ostream& os, const S_xmlPartsInfoElement& elt);

/*!
\brief A MSR page geometry representation.

  A page geometry is represented by variable/value pairs
*/
//______________________________________________________________________________
class EXP xmlPageGeometry : public xmlPartsInfoElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<xmlPageGeometry> create (
      S_xmlPartsInfoOptions& msrOpts, 
      int                    inputLineNumber);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xmlPageGeometry (
      S_xmlPartsInfoOptions& msrOpts, 
      int                    inputLineNumber);
      
    virtual ~xmlPageGeometry();
  
  public:

    // set and get
    // ------------------------------------------------------

    void    setMillimeters        (float val) { fMillimeters = val; }
    void    setTenths             (int val)   { fTenths = val; }

    int     getMillimeters        () const    { return fMillimeters; }
    float   getTenths             () const    { return fTenths; }
    
    // services
    // ------------------------------------------------------

    float   globalStaffSize       () const;

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);


  private:

    int               fMillimeters;
    int               fTenths;
};
typedef SMARTP<xmlPageGeometry> S_xmlPageGeometry;
EXP ostream& operator<< (ostream& os, const S_xmlPageGeometry& elt);

/*!
\brief A msr lyrics representation.

  A lyrics is represented by a list of lyrics chunks,
*/
//______________________________________________________________________________
class EXP msrLyrics : public xmlPartsInfoElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrLyricsMasterStatus {
       kMasterLyrics, kRegularLyrics };

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrLyrics> create (
      S_xmlPartsInfoOptions&         msrOpts, 
      int                   inputLineNumber,
      int                   lyricsNumber,
      msrLyricsMasterStatus lyricsMasterStatus,
      S_xmlVoice            lyricsVoiceUplink);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrLyrics (
      S_xmlPartsInfoOptions&         msrOpts, 
      int                   inputLineNumber,
      int                   lyricsNumber,
      msrLyricsMasterStatus lyricsMasterStatus,
      S_xmlVoice            lyricsVoiceUplink);

    virtual ~msrLyrics();
  
  public:

    // set and get
    // ------------------------------------------------------

    int     getLyricsNumber () const
                { return fLyricsNumber; }
                
    string  getLyricsName () const;
                
    S_xmlVoice
            getLyricsVoiceUplink () const
                { return fLyricsVoiceUplink; }
                
    void    setLyricsTextPresent ()
                { fLyricsTextPresent = true; }

    bool    getLyricsTextPresent () const
                { return fLyricsTextPresent; }


    // services
    // ------------------------------------------------------
                
    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    int                       fLyricsNumber;
    msrLyricsMasterStatus     fLyricsMasterStatus;

    bool                      fLyricsTextPresent;

    S_xmlVoice                fLyricsVoiceUplink;

};
typedef SMARTP<msrLyrics> S_msrLyrics;
EXP ostream& operator<< (ostream& os, const S_msrLyrics& elt);

/*!
\brief A msr harmony representation.

  A harmony is represented by a list of lyrics chunks,
*/
//______________________________________________________________________________
class EXP msrHarmony : public xmlPartsInfoElement
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
      S_xmlPartsInfoOptions&         msrOpts, 
      int                   inputLineNumber,
      char                  harmonyRootStep,
      float                 harmonyRootAlter,
      msrHarmonyKind        harmonyKind,
      string                harmonyKindText,
      char                  harmonyBassStep,
      float                 harmonyBassAlter,
      S_xmlPart             harmonyPartUplink);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrHarmony (
      S_xmlPartsInfoOptions&         msrOpts, 
      int                   inputLineNumber,
      char                  harmonyRootStep,
      float                 harmonyRootAlter,
      msrHarmonyKind        harmonyKind,
      string                harmonyKindText,
      char                  harmonyBassStep,
      float                 harmonyBassAlter,
      S_xmlPart             harmonyPartUplink);

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
                
    S_xmlPart  getHarmonyVoiceUplink () const
                  { return fHarmonyPartUplink; }
                

    // services
    // ------------------------------------------------------

    string    harmonyKindAsString () const;
    
    string    harmonyAsString () const;
   
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
    
    S_xmlPart                 fHarmonyPartUplink;
};
typedef SMARTP<msrHarmony> S_msrHarmony;
EXP ostream& operator<< (ostream& os, const S_msrHarmony& elt);

/*!
\brief A msr barline representation.

  A barline is represented by the number of the next bar
*/
//______________________________________________________________________________
class EXP msrBarline : public xmlPartsInfoElement
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
      S_xmlPartsInfoOptions&             msrOpts, 
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
      S_xmlPartsInfoOptions&             msrOpts, 
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
class EXP xmlVoice : public xmlPartsInfoElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<xmlVoice> create (
      S_xmlPartsInfoOptions& msrOpts, 
      int           inputLineNumber,
      int           externalVoiceNumber,
      S_xmlStaff    voiceStaffUplink);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xmlVoice (
      S_xmlPartsInfoOptions& msrOpts, 
      int           inputLineNumber,
      int           externalVoiceNumber,
      S_xmlStaff    voiceStaffUplink);
      
    virtual ~xmlVoice();
  
  public:

    // set and get
    // ------------------------------------------------------

    int           getExternalVoiceNumber () const
                      { return fExternalVoiceNumber; }
                
    void          setExternalVoiceNumber (int externalVoiceNumber)
                      { fExternalVoiceNumber = externalVoiceNumber; }

    int           getStaffRelativeVoiceNumber () const
                      { return fStaffRelativeVoiceNumber; }
                
    S_xmlStaff    getVoiceStaffUplink () const
                      { return fVoiceStaffUplink; }
                
    const map<int, S_msrLyrics>&
                  getVoiceLyricsMap () const
                      { return fVoiceLyricsMap; }

    string        getVoiceName () const;

    void          setNewVoicechunkForVoice (int inputLineNumber); // JMI

    int           getVoiceActualNotesCounter () const
                      { return fVoiceActualNotesCounter; }

    int           getMeasureZeroHasBeenMetInVoice () const
                      { return fMeasureZeroHasBeenMetInVoice; }

    // divisions per whole note
    void          setVoiceDivisionsPerWholeNote (
                    int divisionsPerWholeNote);
                      
    const int     getVoiceDivisionsPerWholeNote () const
                      { return fVoiceDivisionsPerWholeNote; }

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

    S_msrLyrics
                  addLyricsToVoiceByItsNumber (
                    int inputLineNumber,
                    int lyricsNumber);

    void          addLyricsToVoice (S_msrLyrics lyrics);

    S_msrLyrics   createLyricsInVoiceIfNeeded (
                    int inputLineNumber,
                    int lyricsNumber);
    
    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    // voice number in MusicXML may be greater than 4
    // while there can only be 4 in a staff
    // we thus have to cope with that
    int                       fExternalVoiceNumber;
    int                       fStaffRelativeVoiceNumber;

    S_xmlStaff                fVoiceStaffUplink;

    bool                      fVoiceActualNotesCounter;

    int                       fVoiceDivisionsPerWholeNote;

    int                       fVoiceMeasureNumber;
    
    // anacrusis detection
    bool                      fMeasureZeroHasBeenMetInVoice;
    bool                      fMeasureNumberHasBeenSetInVoice;
    bool                      fMusicHasBeenInsertedInVoice;
        
    // the lyrics map
    map<int, S_msrLyrics>     fVoiceLyricsMap;
};
EXP ostream& operator<< (ostream& os, const S_xmlVoice& elt);

/*!
\brief A msr staff representation.

  A staff is represented by a its string contents
*/

/*
Staff assignment is only needed for music notated on multiple staves. Used by both notes and directions. Staff values are numbers, with 1 referring to the top-most staff in a part.
*/
//______________________________________________________________________________
class EXP xmlStaff : public xmlPartsInfoElement
{
  public:

    enum xmlStaffKind {
      kRegularStaff, kTablatureStaff, kPercussionStaff};

    enum xmlStaffTypeKind {
      kOssiaStaffType, kCueStaffType, kEditorialStaffType,
      kRegularStaffType, kAlternateStaffType};
     
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<xmlStaff> create (
      S_xmlPartsInfoOptions& msrOpts, 
      int           inputLineNumber,
      int           staffNumber,
      S_xmlPart     fStaffPartUplink);
    
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xmlStaff (
      S_xmlPartsInfoOptions& msrOpts, 
      int           inputLineNumber,
      int           staffNumber,
      S_xmlPart     fStaffPartUplink);
      
    virtual ~xmlStaff();
  
  public:

    // set and get
    // ------------------------------------------------------

    xmlStaffKind  getStaffKind () const
                      { return fStaffKind; }
                
    int           getStaffNumber () const
                      { return fStaffNumber; }
                
    S_xmlPart     getStaffPartUplink () const
                      { return fStaffPartUplink; }

    string        getStaffName () const;

    string        getStaffInstrumentName () const
                      { return fStaffInstrumentName; }

    const map<int, S_xmlVoice>&
                  getStaffVoicesCorrespondanceMap () const
                      { return fStaffVoicesCorrespondanceMap; }

    const map<int, S_xmlVoice>&
                  getStaffVoicesMap () const
                      { return fStaffVoicesMap; }

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

    // services
    // ------------------------------------------------------

    S_xmlVoice    addVoiceToStaffByItsRelativeNumber (
                    int inputLineNumber,
                    int voiceRelativeNumber);
  
    S_xmlVoice    registerVoiceInStaffByItsExternalNumber (
                    int inputLineNumber,
                    int externalVoiceNumber);
  
    string        staffKindAsString () const;
    
    void          setAllStaffVoicesDivisionsPerWholeNote (
                    int divisions);

    void          setAllStaffVoicesMeasureNumber (
                    int inputLineNumber,
                    int measureNumber);

    S_xmlVoice    addVoiceToStaffByItsNumber (
                    int inputLineNumber,
                    int externalVoiceNumber);

    void          registerVoiceInStaff (
                    int inputLineNumber,
                    S_xmlVoice voice);
    
    S_xmlVoice    fetchVoiceFromStaff (
                    int inputLineNumber, int externalVoiceNumber);
                              
    void          removeStaffEmptyVoices ();
    
    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    // data
    
    xmlStaffKind            fStaffKind;
    
    static int              gMaxStaffVoices;

    int                     fStaffNumber;
    S_xmlPart               fStaffPartUplink;

    map<int, S_xmlVoice>    fStaffVoicesCorrespondanceMap;
                              //numbered 1 to gMaxStaffVoices
    map<int, S_xmlVoice>    fStaffVoicesMap;
                              // [0] is used form staff master voice

    string                  fStaffInstrumentName;

    int                     fStaffDivisionsPerWholeNote;    

    int                     fStaffMeasureNumber;
    
    int                     fRegisteredVoicesCounter;
};
typedef SMARTP<xmlStaff> S_xmlStaff;
EXP ostream& operator<< (ostream& os, const S_xmlStaff& elt);

/*!
\brief A msr part representation.

  A part is represented by a its string contents
*/
//______________________________________________________________________________
class EXP xmlPart : public xmlPartsInfoElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<xmlPart> create (
      S_xmlPartsInfoOptions&  msrOpts, 
      int            inputLineNumber,
      string         partID);
                
  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xmlPart (
      S_xmlPartsInfoOptions&  msrOpts, 
      int            inputLineNumber,
      string         partID);
      
    virtual ~xmlPart();
  
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
                
    const map<int, S_xmlStaff>&
                  getPartStavesMap ()
                      { return fPartStavesMap; }

    string        getPartCombinedName () const;

    // divisions per whole note
    void          setPartDivisionsPerWholeNote (
                    int divisionsPerWholeNote);
                      
    const int     getPartDivisionsPerWholeNote () const
                      { return fPartDivisionsPerWholeNote; }
          
    // measure number
    void          setPartMeasureNumber (
                    int inputLineNumber,
                    int measureNumber);
                      
    const int     getPartMeasureNumber () const
                      { return fPartMeasureNumber; }

    const bool    getMeasureZeroHasBeenMetInPart () const
                      { return fMeasureZeroHasBeenMetInPart; }

    // services
    // ------------------------------------------------------

    void          setAllPartStavesDivisionsPerWholeNote (
                    int divisions);
    
    void          setAllPartStavesMeasureNumber (
                    int inputLineNumber,
                    int measureNumber);

    S_xmlStaff    addStaffToPartByItsNumber (
                    int inputLineNumber,
                    int staffNumber);
    
    void          addStaffToPart (S_xmlStaff staff);

    S_xmlStaff    fetchStaffFromPart (int staffNumber);

    void          appendHarmonyToPart (S_msrHarmony harmony);

    void          removePartEmptyVoices ();

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

    map<int, S_xmlStaff>    fPartStavesMap;

    int                     fPartDivisionsPerWholeNote;

    int                     fPartMeasureNumber;
    bool                    fMeasureZeroHasBeenMetInPart;
};
typedef SMARTP<xmlPart> S_xmlPart;
EXP ostream& operator<< (ostream& os, const S_xmlPart& elt);

/*!
\brief A msr score representation.

  A score is represented by a its string contents
*/
//______________________________________________________________________________
class EXP xmlPartsInfo : public xmlPartsInfoElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<xmlPartsInfo> create (
      S_xmlPartsInfoOptions& msrOpts, 
      int           inputLineNumber);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    xmlPartsInfo (
      S_xmlPartsInfoOptions& msrOpts, 
      int           inputLineNumber);
      
    virtual ~xmlPartsInfo();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_xmlPageGeometry
                getPageGeometry () const
                    { return fPageGeometry; }
    
    // services
    // ------------------------------------------------------

    // print
    // ------------------------------------------------------

    virtual void print (ostream& os);

  private:

    S_xmlPageGeometry    fPageGeometry;
};
typedef SMARTP<xmlPartsInfo> S_xmlPartsInfo;
EXP ostream& operator<< (ostream& os, const S_xmlPartsInfo& elt);


/*! @} */

}


#endif
