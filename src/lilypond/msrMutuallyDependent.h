/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msrMutuallyDependent___
#define ___msrMutuallyDependent___

#include "typedefs.h"
#include "tree_browser.h"
#include "exports.h"

#include "utilities.h"
#include "messagesHandling.h"

#include "msrElements.h"

#include "msrVarValAssocs.h"

#include "msrIdentification.h"
#include "msrPageGeometry.h"
#include "msrLayout.h"

#include "msrCredits.h"

#include "msrDivisions.h"

#include "msrPartNamesDisplays.h"

#include "msrStavesDetails.h"

#include "msrClefs.h"
#include "msrKeys.h"
#include "msrTimes.h"

#include "msrTempos.h"

#include "msrRehearsals.h"

#include "msrWords.h"

#include "msrTranspositions.h"

#include "msrBreaks.h"

#include "msrStems.h"
#include "msrBeams.h"

#include "msrTechnicals.h"

#include "msrBars.h"

#include "msrDynamics.h"
#include "msrArticulations.h"

#include "msrTablatures.h"
#include "msrInstruments.h"
#include "msrOtherElements.h"

#include "msrTies.h"
#include "msrSlurs.h"
#include "msrLigatures.h"

#include "msrTremolos.h"

#include "msrSlashes.h"

#include "msrScores.h"
#include "msrPartGroups.h"
// JMI #include "msrParts.h"

#include "msrMidi.h"

#include "generalOptions.h"


namespace MusicXML2 
{

/*
  This file declares classes that cannot be placed in 'individual' headers
  due to mutual dependancies
*/
 
//______________________________________________________________________________
// PRE-declarations for class mutual dependencies

class msrNote;
typedef SMARTP<msrNote> S_msrNote;

class msrChord;
typedef SMARTP<msrChord> S_msrChord;

class msrTuplet;
typedef SMARTP<msrTuplet> S_msrTuplet;

class msrGraceNotesGroup;
typedef SMARTP<msrGraceNotesGroup> S_msrGraceNotesGroup;

class msrAfterGraceNotesGroup;
typedef SMARTP<msrAfterGraceNotesGroup> S_msrAfterGraceNotesGroup;

class msrHarmony;
typedef SMARTP<msrHarmony> S_msrHarmony;

class msrFiguredBass;
typedef SMARTP<msrFiguredBass> S_msrFiguredBass;

class msrMultipleRest;
typedef SMARTP<msrMultipleRest> S_msrMultipleRest;

class msrMeasuresRepeat;
typedef SMARTP<msrMeasuresRepeat> S_msrMeasuresRepeat;

class msrRepeat;
typedef SMARTP<msrRepeat> S_msrRepeat;

class msrMeasure;
typedef SMARTP<msrMeasure> S_msrMeasure;

class msrSegment;
typedef SMARTP<msrSegment> S_msrSegment;

class msrVoice;
typedef SMARTP<msrVoice> S_msrVoice;

class msrVoiceStaffChange;
typedef SMARTP<msrVoiceStaffChange> S_msrVoiceStaffChange;

class msrPart;
typedef SMARTP<msrPart> S_msrPart;


//______________________________________________________________________________
class msrOrnament : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrOrnamentKind {
        kOrnamentTrill, kOrnamentDashes,
        kOrnamentTurn, kOrnamentInvertedTurn, kOrnamentDelayedTurn,
        kOrnamentDelayedInvertedTurn, kOrnamentVerticalTurn,
        kOrnamentMordent, kOrnamentInvertedMordent,
        kOrnamentSchleifer, kOrnamentShake,
        kOrnamentAccidentalMark };

    static string ornamentKindAsString (
      msrOrnamentKind ornamentKind);
                  
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrOrnament> create (
      int              inputLineNumber,
      msrOrnamentKind  ornamentKind,
      msrPlacementKind ornamentPlacementKind);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrOrnament (
      int              inputLineNumber,
      msrOrnamentKind  ornamentKind,
      msrPlacementKind ornamentPlacementKind);
      
    virtual ~msrOrnament ();
  
  public:

    // set and get
    // ------------------------------------------------------

    void                  setOrnamentNoteUplink (S_msrNote note)
                              { fOrnamentNoteUplink = note; }

    S_msrNote             getOrnamentNoteUplink () const
                              { return fOrnamentNoteUplink; }

    msrOrnamentKind       getOrnamentKind () const
                              { return fOrnamentKind; }
        
    void                  setOrnamentPlacementKind (
                            msrPlacementKind
                              ornamentPlacementKind)
                              {
                                fOrnamentPlacementKind =
                                  ornamentPlacementKind;
                              }
        
    msrPlacementKind      getOrnamentPlacementKind () const
                              { return fOrnamentPlacementKind; }
        
    void                  setOrnamentAccidentalMarkKind (
                            msrAlterationKind
                              ornamentAccidentalMark)
                              {
                                fOrnamentAccidentalMark =
                                  ornamentAccidentalMark;
                              }
        
    msrAlterationKind     getOrnamentAccidentalMark () const
                              { return fOrnamentAccidentalMark; }
        
    // services
    // ------------------------------------------------------

    string                ornamentKindAsString () const;

    string                ornamentPlacementKindAsString () const;

    string                ornamentAccidentalMarkAsString () const;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    S_msrNote             fOrnamentNoteUplink;

    msrOrnamentKind       fOrnamentKind;

    msrPlacementKind      fOrnamentPlacementKind;
    
    msrAlterationKind     fOrnamentAccidentalMark;
};
typedef SMARTP<msrOrnament> S_msrOrnament;
EXP ostream& operator<< (ostream& os, const S_msrOrnament& elt);

//______________________________________________________________________________
class msrDoubleTremolo : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrDoubleTremoloKind {
      kNotesDoubleTremolo, kChordsDoubleTremolo};

    static string msrDoubleTremoloKindAsString (
      msrDoubleTremoloKind doubleTremolotKind);
                        
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrDoubleTremolo> create (
      int                  inputLineNumber,
      msrDoubleTremoloKind doubleTremoloKind,
      msrTremoloTypeKind   doubleTremoloTypeKind,
      int                  doubleTremoloMarksNumber,
      msrPlacementKind     doubleTremoloPlacementKind);


    SMARTP<msrDoubleTremolo> createDoubleTremoloNewbornClone (
      S_msrElement doubleTremoloFirstElement,
      S_msrElement doubleTremoloSecondElement);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrDoubleTremolo (
      int                  inputLineNumber,
      msrDoubleTremoloKind doubleTremoloKind,
      msrTremoloTypeKind   doubleTremoloTypeKind,
      int                  doubleTremoloMarksNumber,
      msrPlacementKind     doubleTremoloPlacementKind);
      
    virtual ~msrDoubleTremolo ();
  
  public:

    // set and get
    // ------------------------------------------------------

    // double tremolo kind
    
    void                  setDoubleTremoloKind (
                            msrDoubleTremoloKind doubleTremoloKind)
                              { fDoubleTremoloKind = doubleTremoloKind; }

    msrDoubleTremoloKind  getDoubleTremoloKind () const
                              { return fDoubleTremoloKind; }

    // double tremolo type kind
    
    void                  setDoubleTremoloTypeKind (
                            msrTremoloTypeKind doubleTremoloTypeKind)
                              { fDoubleTremoloTypeKind = doubleTremoloTypeKind; }

    msrTremoloTypeKind    getDoubleTremoloTypeKind () const
                              { return fDoubleTremoloTypeKind; }

    // double tremolo placement
    
    void                  setDoubleTremoloPlacementKind (
                            msrPlacementKind
                              doubleTremoloPlacementKind)
                              {
                                fDoubleTremoloPlacementKind =
                                  doubleTremoloPlacementKind;
                              }
        
    msrPlacementKind      getDoubleTremoloPlacementKind () const
                              { return fDoubleTremoloPlacementKind; }
        
    // double tremolo marks number

    int                   getDoubleTremoloMarksNumber () const
                              { return fDoubleTremoloMarksNumber; }
                
    // double tremolo elements duration

    rational              getDoubleTremoloElementsDuration () const
                              { return fDoubleTremoloElementsDuration; }
                
    // double tremolo number of repeats

    void                  setDoubleTremoloNumberOfRepeats (
                            int doubleTremoloNumberOfRepeats)
                              {
                                fDoubleTremoloNumberOfRepeats =
                                  doubleTremoloNumberOfRepeats;
                              }
                              
    int                   getDoubleTremoloNumberOfRepeats () const
                              {
                                return
                                  fDoubleTremoloNumberOfRepeats;
                              }
                

    // double tremolo placement

    void                  setDoubleTremoloNoteFirstElement (
                            S_msrNote note);
    
    void                  setDoubleTremoloChordFirstElement (
                            S_msrChord chord);

    // double tremolo first element

    S_msrElement          getDoubleTremoloFirstElement () const
                              { return fDoubleTremoloFirstElement; }
        
    // double tremolo second element

    void                  setDoubleTremoloNoteSecondElement (
                            S_msrNote note);
                            
    void                  setDoubleTremoloChordSecondElement (
                            S_msrChord chord);

    S_msrElement          getDoubleTremoloSecondElement () const
                              { return fDoubleTremoloSecondElement; }
                              
    // sounding whole notes
     
    void                  setDoubleTremoloSoundingWholeNotes (
                            rational wholeNotes)
                              {
                                fDoubleTremoloSoundingWholeNotes =
                                  wholeNotes;
                              }
            
    rational              getDoubleTremoloSoundingWholeNotes () const
                              {
                                return
                                  fDoubleTremoloSoundingWholeNotes;
                              }
            
    // measure number
    
    void                  setDoubleTremoloMeasureNumber (
                            string measureNumber)
                              {
                                fDoubleTremoloMeasureNumber =
                                  measureNumber;
                              }
    
    string                getDoubleTremoloMeasureNumber () const
                              { return fDoubleTremoloMeasureNumber; }
 
    // position in measure
    
    void                  setDoubleTremoloPositionInMeasure (
                            rational positionInMeasure)
                              {
                                fDoubleTremoloPositionInMeasure =
                                  positionInMeasure;
                              }

    rational              getDoubleTremoloPositionInMeasure () const
                              {
                                return
                                  fDoubleTremoloPositionInMeasure;
                              }

    // services
    // ------------------------------------------------------

    // tremolo first note
    
    void                  setDoubleTremoloFirstNotePositionInMeasure (
                            rational positionInMeasure);
                    
    void                  setDoubleTremoloFirstNoteMeasureNumber (
                            string measureNumber);
                    
  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                doubleTremoloPlacementKindAsString () const;
        
    string                asString () const;

    string                asShortString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // sounding divisions
    // the same as the displayed divisions of both members
    rational              fDoubleTremoloSoundingWholeNotes;

    msrDoubleTremoloKind  fDoubleTremoloKind;
    
    msrTremoloTypeKind    fDoubleTremoloTypeKind;

    int                   fDoubleTremoloMarksNumber;

    rational              fDoubleTremoloElementsDuration;
    
    int                   fDoubleTremoloNumberOfRepeats;

    msrPlacementKind      fDoubleTremoloPlacementKind;

    // the two elements of a double tremole are notes or chords
    S_msrElement          fDoubleTremoloFirstElement;
    S_msrElement          fDoubleTremoloSecondElement;


    string                fDoubleTremoloMeasureNumber;
    rational              fDoubleTremoloPositionInMeasure;
};
typedef SMARTP<msrDoubleTremolo> S_msrDoubleTremolo;
EXP ostream& operator<< (ostream& os, const S_msrDoubleTremolo& elt);

//______________________________________________________________________________
// pre-declaration for two-way sidelinks
class msrSpanner;
typedef SMARTP<msrSpanner> S_msrSpanner;

class msrSpanner : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrSpannerKind {
        kSpannerDashes, kSpannerWavyLine };

    static string spannerKindAsString (
      msrSpannerKind spannerKind);
            
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSpanner> create (
      int                inputLineNumber,
      int                spannerNumber,
      msrSpannerKind     spannerKind,
      msrSpannerTypeKind spannerTypeKind,
      msrPlacementKind   spannerPlacementKind,
      S_msrNote          spannerNoteUplink);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSpanner (
      int                inputLineNumber,
      int                spannerNumber,
      msrSpannerKind     spannerKind,
      msrSpannerTypeKind spannerTypeKind,
      msrPlacementKind   spannerPlacementKind,
      S_msrNote          spannerNoteUplink);
      
    virtual ~msrSpanner ();
  
  public:

    // set and get
    // ------------------------------------------------------

    // uplinks
    void                  setSpannerNoteUplink (
                            S_msrNote spannerNoteUplink)
                              { fSpannerNoteUplink = spannerNoteUplink; }

    S_msrNote             getSpannerNoteUplink () const
                              { return fSpannerNoteUplink; }

    // sidelinks
    void                  setSpannerOtherEndSidelink (
                            S_msrSpanner otherEndSideLink);
      
    S_msrSpanner          getSpannerOtherEndSidelink () const
                              { return fSpannerOtherEndSidelink; }

    int                   getSpannerNumber () const
                              { return fSpannerNumber; }
        
    msrSpannerKind        getSpannerKind () const
                              { return fSpannerKind; }
        
    msrSpannerTypeKind    getSpannerTypeKind () const
                              { return fSpannerTypeKind; }
        
    msrPlacementKind      getSpannerPlacementKind () const
                              { return fSpannerPlacementKind; }

    // services
    // ------------------------------------------------------

    string                spannerKindAsString () const;
    
    string                spannerTypeKindAsString () const;

    string                spannerPlacementKindAsString () const;

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  protected:

    // uplinks
    // ------------------------------------------------------
    S_msrNote             fSpannerNoteUplink;

    // sidelinks
    // ------------------------------------------------------
    S_msrSpanner          fSpannerOtherEndSidelink; // two-way

    // fields
    // ------------------------------------------------------

    int                   fSpannerNumber;
    
    msrSpannerKind        fSpannerKind;

    msrSpannerTypeKind    fSpannerTypeKind;

    msrPlacementKind      fSpannerPlacementKind;
};
typedef SMARTP<msrSpanner> S_msrSpanner;
EXP ostream& operator<< (ostream& os, const S_msrSpanner& elt);

//______________________________________________________________________________
class msrMeasure : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrMeasureKind {
        kUnknownMeasureKind,
        kFullMeasureKind,
        kUpbeatMeasureKind,
        kUnderfullMeasureKind,
        kOverfullMeasureKind,
        kSenzaMisuraMeasureKind,
        kEmptyMeasureKind}; // for <measure ... /> without nested contents
    
    static string measureKindAsString (
      msrMeasureKind measureKind);

    enum msrMeasureImplicitKind {
        kMeasureImplicitYes,
        kMeasureImplicitNo };
      
    static string measureImplicitKindAsString (
      msrMeasureImplicitKind measureImplicitKind);

    enum msrMeasureFirstInSegmentKind {
        kMeasureFirstInSegmentUnknown,
        kMeasureFirstInSegmentYes,
        kMeasureFirstInSegmentNo };
      
    static string measureFirstInSegmentKindAsString (
      msrMeasureFirstInSegmentKind measureFirstInSegmentKind);

    enum msrMeasureCreatedForARepeatKind {
        kMeasureCreatedForARepeatNo,
        kMeasureCreatedForARepeatBefore,
        kMeasureCreatedForARepeatAfter,
        kMeasureCreatedForARepeatPadded };
      
    static string measureCreatedForARepeatKindAsString (
      msrMeasureCreatedForARepeatKind measureCreatedForARepeatKind);

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasure> create (
      int           inputLineNumber,
      string        measureNumber,
      S_msrSegment  measureSegmentUplink);
    
    SMARTP<msrMeasure> createMeasureNewbornClone (
      S_msrSegment containingSegment);

    SMARTP<msrMeasure> createMeasureDeepCopy (
      S_msrSegment containingSegment);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrMeasure (
      int           inputLineNumber,
      string        measureNumber,
      S_msrSegment  measureSegmentUplink);
      
    virtual ~msrMeasure ();
  
  private:

    // initialization
    // ------------------------------------------------------

    void                  initializeMeasure ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplinks
        
    S_msrSegment          getMeasureSegmentUplink () const
                              { return fMeasureSegmentUplink; }
                      
    // measure numbers
    
    void                  setMeasureNumber (string measureNumber)
                              { fMeasureNumber = measureNumber; }

    string                getMeasureNumber () const
                              { return fMeasureNumber; }

    void                  setMeasureOrdinalNumber (
                            int measureOrdinalNumber)
                              {
                                fMeasureOrdinalNumber =
                                  measureOrdinalNumber;
                              }

    int                   getMeasureOrdinalNumber () const
                              { return fMeasureOrdinalNumber; }


    void                  setNextMeasureNumber (string nextMeasureNumber);

    string                getNextMeasureNumber () const
                              { return fNextMeasureNumber; }

    // measure longest note
    
    S_msrNote             getMeasureLongestNote () const
                              { return fMeasureLongestNote; }
                              
    // measure lengthes
    
    void                  setMeasureFullLength (
                            rational measureFullLength)
                              {
                                fMeasureFullLength =
                                  measureFullLength;
                              }

    rational              getMeasureFullLength () const
                              {
                                return
                                  fMeasureFullLength;
                              }

    void                  setMeasureLength (
                            int      inputLineNumber,
                            rational measureLength);

    rational              getMeasureLength () const
                              { return fMeasureLength; }
                      
    // measure kind
    
 // JMI   void                  setMeasureKind (msrMeasureKind measureKind)
   //                           { fMeasureKind = measureKind; }

    msrMeasureKind        getMeasureKind () const
                              { return fMeasureKind; }

    // measure implicit kind
    
    void                  setMeasureImplicitKind (
                            msrMeasureImplicitKind measureImplicitKind)
                              {
                                fMeasureImplicitKind =
                                measureImplicitKind;
                              }

    msrMeasureImplicitKind
                          getMeasureImplicitKind () const
                              { return fMeasureImplicitKind; }

   
    // measure 'first in segment' kind
    
    void                  setMeasureFirstInSegmentKind (
                            msrMeasureFirstInSegmentKind
                              measureFirstInSegmentKind)
                              {
                                fMeasureFirstInSegmentKind =
                                  measureFirstInSegmentKind;
                              }

    msrMeasureFirstInSegmentKind
                          getMeasureFirstInSegmentKind () const
                              { return fMeasureFirstInSegmentKind; }

    // measure 'created for a repeat' kind
    
    void                  setMeasureCreatedForARepeatKind (
                            msrMeasureCreatedForARepeatKind
                              measureCreatedForARepeatKind);

    msrMeasureCreatedForARepeatKind
                          getMeasureCreatedForARepeatKind () const
                              { return fMeasureCreatedForARepeatKind; }

    // single-measure rest?


    void                  setMeasureIsASingleMeasureRest ()
                              { fMeasureIsASingleMeasureRest = true; }

    bool                  getMeasureIsASingleMeasureRest ()
                             { return fMeasureIsASingleMeasureRest; }

    // chords handling
    
    S_msrNote              getMeasureLastHandledNote () const
                              { return fMeasureLastHandledNote; }

    // elements list
    
    const list<S_msrElement>&
                          getMeasureElementsList () const
                              { return fMeasureElementsList; }

    bool                  getMeasureContainsMusic () const
                              { return fMeasureContainsMusic; }
                              
    // services
    // ------------------------------------------------------

    // uplinks
    
    S_msrPart             fetchMeasurePartUplink () const;
    
    S_msrVoice            fetchMeasureVoiceUplink () const;

    // lengthes

    string                measureFullLengthAsMSRString ();

    string                measureLengthAsMSRString ();

    S_msrNote             createPaddingNoteForVoice (
                            int        inputLineNumber,
                            rational   duration,
                            S_msrVoice voice);
  
    void                  padUpToMeasureLengthInMeasure (
                            int      inputLineNumber,
                            rational measureLength);

    void                  appendPaddingNoteToMeasure (
                            int inputLineNumber,
                            int divisions,
                            int divisionsPerQuarterNote);
  
    // clefs
    
    void                  appendClefToMeasure (S_msrClef clef);

    // keys
    
    void                  appendKeyToMeasure (S_msrKey key);

    // times

    void                  appendTimeToMeasure (S_msrTime time);

    void                  setMeasureFullLengthFromTime (
                            S_msrTime time);
  
    void                  appendTimeToMeasureClone (S_msrTime time);

    // transpose

    void                  appendTransposeToMeasure (
                            S_msrTranspose transpose);

    // part name display

    void                  appendPartNameDisplayToMeasure (
                            S_msrPartNameDisplay partNameDisplay);

    // part abbreviation display

    void                  appendPartAbbreviationDisplayToMeasure (
                            S_msrPartAbbreviationDisplay partAbbreviationDisplay);

    // staff details

    void                  appendStaffDetailsToMeasure (
                            S_msrStaffDetails staffDetails);

    // bar number checks
    
    void                  appendBarNumberCheckToMeasure (
                            S_msrBarNumberCheck barNumberCheck);

    // breaks

    void                  appendLineBreakToMeasure (S_msrLineBreak lineBreak);
    void                  appendPageBreakToMeasure (S_msrPageBreak pageBreak);

    // tempo
    
    void                  appendTempoToMeasure (S_msrTempo tempo);

    // rehearsals
    
    void                  appendRehearsalToMeasure (
                            S_msrRehearsal rehearsal);

    // octave shifts
    
    void                  appendOctaveShiftToMeasure (
                            S_msrOctaveShift octaveShift);

    // scordaturas
    
    void                  appendScordaturaToMeasure (
                            S_msrScordatura scordatura);

    // accordion registration

    void                  appendAccordionRegistrationToMeasure (
                            S_msrAccordionRegistration
                              accordionRegistration);

    // harp pedals tuning
    
    void                  appendHarpPedalsTuningToMeasure (
                            S_msrHarpPedalsTuning
                              harpPedalsTuning);

    // bar lines
    
    void                  appendBarlineToMeasure (S_msrBarline barline);
    void                  prependBarlineToMeasure (S_msrBarline barline);

    // bar checks
    
    void                  appendBarCheckToMeasure (S_msrBarCheck barCheck);

    // staff change
    
    void                  appendVoiceStaffChangeToMeasure (
                            S_msrVoiceStaffChange voiceStaffChange);

    // notes
    
    void                  appendNoteToMeasure      (S_msrNote note);
    void                  appendNoteToMeasureClone (S_msrNote note);

    // tremolos
    
    void                  appendDoubleTremoloToMeasure (
                            S_msrDoubleTremolo doubleTremolo);

    // repeats
    
    void                  appendMeasuresRepeatToMeasure (
                            S_msrMeasuresRepeat measuresRepeat);

    void                  appendMultipleRestToMeasure (
                            S_msrMultipleRest multipleRest);

    // chords
    
    void                  appendChordToMeasure (
                            S_msrChord chord);

    // tuplets
    
    void                  appendTupletToMeasure (
                            S_msrTuplet tuplet);

    // harmonies
    
    void                  appendHarmonyToMeasure (
                            S_msrHarmony harmony);
    
    void                  appendHarmonyToMeasureClone (
                            S_msrHarmony harmony);

    // frames
    
    void                  appendFrameToMeasure (
                            S_msrFrame frame);
    
    void                  appendFrameToMeasureClone (
                            S_msrFrame frame);

    // figured bass

    void                  appendFiguredBassToMeasure (
                            S_msrFiguredBass figuredBass);
    
    void                  appendFiguredBassToMeasureClone (
                            S_msrFiguredBass figuredBass);

    // grace notes

    /* JMI
    void                  addGraceNotesGroupAheadOfMeasure (
                            S_msrGraceNotesGroup graceNotesGroup);
    
    void                  appendGraceNotesToMeasure (
                            S_msrGraceNotes graceNotes);

    void                  appendAfterGraceNotesToMeasure (
                            S_msrAfterGraceNotes afterGraceNotes);

    void                  prependAfterGraceNotesToMeasure (
                            S_msrAfterGraceNotes afterGraceNotes);
                            */
                            
    // segno
    
    void                  appendSegnoToMeasure (S_msrSegno segno);

    // coda
    
    void                  appendCodaToMeasure (S_msrCoda coda);

    // eyeglasses
    
    void                  appendEyeGlassesToMeasure (
                            S_msrEyeGlasses eyeGlasses);

    // pedal
    
    void                  appendPedalToMeasure (S_msrPedal pedal);

    // damp
    
    void                  appendDampToMeasure (S_msrDamp damp);

    // damp all
    
    void                  appendDampAllToMeasure (S_msrDampAll dampAll);

    // other elements
    
    void                  prependOtherElementToMeasure (S_msrElement elem);
    void                  appendOtherElementToMeasure (S_msrElement elem);

    // last element of measure
    
    S_msrElement          getLastElementOfMeasure () const
                              { return fMeasureElementsList.back (); }

     /* JMI                 
    S_msrElement          removeLastElementFromMeasure (
                            int inputLineNumber);
*/

    // removing elements from measure

    void                  removeNoteFromMeasure (
                            int       inputLineNumber,
                            S_msrNote note);

    void                  removeElementFromMeasure (
                            int          inputLineNumber,
                            S_msrElement elem);

    // finalization

    void                  determineMeasureKind (
                            int inputLineNumber);

    void                  padUpToPartMeasureLengthHighTide (
                            int inputLineNumber);

    void                  finalizeMeasure (
                            int inputLineNumber);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------
    
    string                measureKindAsString () const;
    
    string                asShortString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    
    // uplinks
    
    S_msrSegment          fMeasureSegmentUplink;

    // measure lengthes
    
    rational              fMeasureFullLength;
                            // meaningfull only
                            // when there is a time signature,
                            // but not for cadenzas
    
    rational              fMeasureLength;

    // measure numbers
    
    string                fMeasureNumber;
    string                fNextMeasureNumber;
    int                   fMeasureOrdinalNumber;

    // measure longest note
    
    S_msrNote             fMeasureLongestNote;

    // measure kind

    msrMeasureKind        fMeasureKind;
    
    // measure implicit

    msrMeasureImplicitKind
                          fMeasureImplicitKind;
    
    // measure 'first in segment' kind

    msrMeasureFirstInSegmentKind
                          fMeasureFirstInSegmentKind;
                        
    // measure 'created for a repeat' kind

    msrMeasureCreatedForARepeatKind
                          fMeasureCreatedForARepeatKind;

    // single-measure rest?

    bool                  fMeasureIsASingleMeasureRest;
    
    // chords handling
    
    S_msrNote             fMeasureLastHandledNote;

    // elements

    list<S_msrElement>    fMeasureElementsList;
    
    bool                  fMeasureContainsMusic;
};
typedef SMARTP<msrMeasure> S_msrMeasure;
EXP ostream& operator<< (ostream& os, const S_msrMeasure& elt);

//______________________________________________________________________________
class msrSegment : public msrElement
{
  public:
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSegment> create (
      int        inputLineNumber,
      S_msrVoice segmentVoicekUplink);

    SMARTP<msrSegment> createSegmentNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrSegment> createSegmentDeepCopy (
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSegment (
      int        inputLineNumber,
      S_msrVoice segmentVoicekUplink);
      
    virtual ~msrSegment ();
    
  private:

    // initialization
    // ------------------------------------------------------

    void                  initializeSegment ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplinks

    S_msrVoice            getSegmentVoiceUplink () const
                              { return fSegmentVoiceUplink; }
                      
    // number
    
    int                   getSegmentAbsoluteNumber () const
                              { return fSegmentAbsoluteNumber; }
                      
    // measures
    
    const list<S_msrMeasure>&
                          getSegmentMeasuresList () const
                              { return fSegmentMeasuresList; }
                                            
    list<S_msrMeasure>&   getSegmentMeasuresListToModify ()
                              { return fSegmentMeasuresList; }
                                            
    const string          getSegmentMeasureNumber () const
                              { return fSegmentMeasureNumber; }

    // services
    // ------------------------------------------------------

    // uplinks
    
    S_msrPart             fetchSegmentPartUplink () const;

    // divisions ??? JMI
    
    void                  padUpToMeasureLengthInSegment (
                            int      inputLineNumber,
                            rational measureLength);
  
    void                  appendPaddingNoteToSegment (
                            int inputLineNumber,
                            int divisions,
                            int divisionsPerQuarterNote);
  
    // measures

    void                  createMeasureAndAppendItToSegment (
                            int    inputLineNumber,
                            string measureNumber,
                            int    measureOrdinalNumber,
                            msrMeasure::msrMeasureImplicitKind
                                   measureImplicitKind);
                      
    void                  setNextMeasureNumberInSegment (
                            int    inputLineNumber,
                            string nextMeasureNumber);

    void                  appendMeasureToSegment (
                            S_msrMeasure measure);

    void                  prependMeasureToSegment (
                            S_msrMeasure measure);

    void                  appendMeasureToSegmentIfNotYetDone (  // JMI
                            int    inputLineNumber,
                            string measureNumber);

    // clef, key, time
    
    void                  appendClefToSegment  (S_msrClef clef);
    void                  prependClefToSegment (S_msrClef clef);
    
    void                  appendKeyToSegment (S_msrKey key);
    
    void                  appendTimeToSegment (S_msrTime time);
    void                  appendTimeToSegmentClone (S_msrTime time);

    // transpose

    void                  appendTransposeToSegment (
                            S_msrTranspose transpose);

    // part name display

    void                  appendPartNameDisplayToSegment (
                            S_msrPartNameDisplay partNameDisplay);

    // part abbreviation display

    void                  appendPartAbbreviationDisplayToSegment (
                            S_msrPartAbbreviationDisplay partAbbreviationDisplay);

    // staff details

    void                  appendStaffDetailsToSegment (
                            S_msrStaffDetails staffDetails);

    // tempo
    
    void                  appendTempoToSegment (S_msrTempo tempo);

    // rehearsals
    
    void                  appendRehearsalToSegment (
                            S_msrRehearsal rehearsal);

    // octave shifts
    
    void                  appendOctaveShiftToSegment (
                            S_msrOctaveShift octaveShift);

    // scordaturas
    
    void                  appendScordaturaToSegment (
                            S_msrScordatura scordatura);

    // accordion registration

    void                  appendAccordionRegistrationToSegment (
                            S_msrAccordionRegistration
                              accordionRegistration);

    // harp pedals tuning
    
    void                  appendHarpPedalsTuningToSegment (
                            S_msrHarpPedalsTuning
                              harpPedalsTuning);

    // harmonies
    
    void                  appendHarmonyToSegment (
                            S_msrHarmony harmony);
    
    void                  appendHarmonyToSegmentClone (
                            S_msrHarmony harmony);

    // frames
    
    void                  appendFrameToSegment (
                            S_msrFrame frame);
    
    void                  appendFrameToSegmentClone (
                            S_msrFrame frame);

    // figured bass

    void                  appendFiguredBassToSegment (
                            S_msrFiguredBass figuredBass);
    
    void                  appendFiguredBassToSegmentClone (
                            S_msrFiguredBass figuredBass);

    // staff change
    
    void                  appendVoiceStaffChangeToSegment (
                            S_msrVoiceStaffChange voiceStaffChange);

    // notes
    
    void                  appendNoteToSegment      (S_msrNote note);
    void                  appendNoteToSegmentClone (S_msrNote note);

    // tremolos
    
    void                  appendDoubleTremoloToSegment (
                            S_msrDoubleTremolo doubleTremolo);

    void                  appendMeasuresRepeatToSegment (
                            S_msrMeasuresRepeat measuresRepeat);

    // repeats
    
    void                  appendMultipleRestToSegment (
                            S_msrMultipleRest multipleRest);

    // chords
    
    void                  appendChordToSegment (S_msrChord chord);

    // tuplets
    
    void                  appendTupletToSegment (S_msrTuplet tuplet);

    // bar lines
    
    void                  appendBarlineToSegment (
                            S_msrBarline barline);

    void                  prependBarlineToSegment (
                            S_msrBarline barline);

    // bar checks
    
    void                  appendBarCheckToSegment (
                            S_msrBarCheck barCheck);

    // bar number checks
    
    void                  appendBarNumberCheckToSegment (
                            S_msrBarNumberCheck barNumberCheck);

    // breaks

    void                  appendLineBreakToSegment (S_msrLineBreak lineBreak);
    void                  appendPageBreakToSegment (S_msrPageBreak pageBreak);

    // segno
    
    void                  appendSegnoToSegment (S_msrSegno segno);

    // coda
    
    void                  appendCodaToSegment (S_msrCoda coda);

    // eyeglasses
    
    void                  appendEyeGlassesToSegment (
                            S_msrEyeGlasses eyeGlasses);

    // pedal
    
    void                  appendPedalToSegment (S_msrPedal pedal);

    // damp
    
    void                  appendDampToSegment (S_msrDamp damp);

    // damp all
    
    void                  appendDampAllToSegment (S_msrDampAll dampAll);

    // grace notes

    /* JMI
    void                  addGraceNotesGroupAheadOfSegmentIfNeeded (
                            S_msrGraceNotesGroup graceNotesGroup);
    
    void                  appendGraceNotesToSegment (
                            S_msrGraceNotes graceNotes);

    void                  appendAfterGraceNotesToSegment (
                            S_msrAfterGraceNotes afterGraceNotes);

    void                  prependAfterGraceNotesToSegment (
                            S_msrAfterGraceNotes afterGraceNotes);
                            */

    // other elements
    
    void                  prependOtherElementToSegment (S_msrElement elem);
    void                  appendOtherElementToSegment (S_msrElement elem);

    // removing elements
    
    void                  removeNoteFromSegment (
                            int       inputLineNumber,
                            S_msrNote note);

    void                  removeElementFromSegment (
                            int          inputLineNumber,
                            S_msrElement element);

    S_msrMeasure          removeLastMeasureFromSegment (
                            int inputLineNumber);

    // finalization

    void                  finalizeCurrentMeasureInSegment (
                            int inputLineNumber);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------
  
    string                asString () const;

    string                asShortString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // uplinks
    S_msrVoice            fSegmentVoiceUplink;

    // counter
    static int            gSegmentsCounter;

    // absolute number
    int                   fSegmentAbsoluteNumber;
        
    // number
    string                fSegmentMeasureNumber;
    bool                  fMeasureNumberHasBeenSetInSegment; // JMI

    // the measures in the segment contain the mmusic
    list<S_msrMeasure>    fSegmentMeasuresList;
};
typedef SMARTP<msrSegment> S_msrSegment;
EXP ostream& operator<< (ostream& os, const S_msrSegment& elt);

//______________________________________________________________________________
class msrGraceNotesGroup : public msrElement
{
  public:
        
    // data types
    // ------------------------------------------------------

    enum msrGraceNotesGroupKind {
      kGraceNotesGroupBefore,
      kGraceNotesGroupAfter };

    static string graceNotesGroupKindAsString (
      msrGraceNotesGroupKind graceNotesGroupKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrGraceNotesGroup> create (
      int                    inputLineNumber,
      msrGraceNotesGroupKind graceNotesGroupKind,
      bool                   graceNotesGroupIsSlashed,
      S_msrVoice             graceNotesGroupVoiceUplink);
    
    SMARTP<msrGraceNotesGroup> createGraceNotesGroupNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrGraceNotesGroup> createSkipGraceNotesGroupClone (
      S_msrVoice containingVoice);

    SMARTP<msrGraceNotesGroup> createGraceNotesGroupDeepCopy (
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrGraceNotesGroup (
      int                    inputLineNumber,
      msrGraceNotesGroupKind graceNotesGroupKind,
      bool                   graceNotesGroupIsSlashed,
      S_msrVoice             graceNotesGroupVoiceUplink);
      
    virtual ~msrGraceNotesGroup ();
  
  public:

    // set and get
    // ------------------------------------------------------
                              
    S_msrVoice            getGraceNotesGroupVoiceUplink () const
                              { return fGraceNotesGroupVoiceUplink; }
                              
    void                  setGraceNotesGroupNoteUplink (
                            S_msrNote note)
                              { fGraceNotesGroupNoteUplink = note; }

    S_msrNote             getGraceNotesGroupNoteUplink () const
                              { return fGraceNotesGroupNoteUplink; }

    msrGraceNotesGroupKind
                          getGraceNotesGroupKind () const
                              { return fGraceNotesGroupKind; }

    void                  setGraceNotesGroupKind (
                            msrGraceNotesGroupKind graceNotesGroupKind)
                              { fGraceNotesGroupKind = graceNotesGroupKind; }

    list<S_msrElement>&   getGraceNotesGroupElementsList ()
                              { return fGraceNotesGroupElementsList; }

    bool                  getGraceNotesGroupIsSlashed () const
                              { return fGraceNotesGroupIsSlashed; }

    bool                  getGraceNotesGroupIsTied () const
                              { return fGraceNotesGroupIsTied; }

    void                  setGraceNotesGroupIsFollowedByNotes (bool value)
                              { fGraceNotesGroupIsFollowedByNotes = value; }

    bool                  getGraceNotesGroupIsFollowedByNotes () const
                              { return fGraceNotesGroupIsFollowedByNotes; }

    void                  setGraceNotesGroupMeasureNumber (
                            string graceNotesGroupMeasureNumber)
                              {
                                fGraceNotesGroupMeasureNumber =
                                  graceNotesGroupMeasureNumber;
                              }
                              
    string                getGraceNotesGroupMeasureNumber () const
                              { return fGraceNotesGroupMeasureNumber; }

    // services
    // ------------------------------------------------------

    S_msrPart             fetchGraceNotesGroupPartUplink () const;

    void                  appendNoteToGraceNotesGroup (S_msrNote note);
    void                  appendChordToGraceNotesGroup (S_msrChord chord);

    S_msrNote             removeLastNoteFromGraceNotesGroup (
                            int inputLineNumber);


  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortString () const;
    string                asString () const;
    
    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // uplinks
    S_msrVoice            fGraceNotesGroupVoiceUplink;
    S_msrNote             fGraceNotesGroupNoteUplink;

    msrGraceNotesGroupKind
                          fGraceNotesGroupKind;
                          
    list<S_msrElement>    fGraceNotesGroupElementsList;

    bool                  fGraceNotesGroupIsSlashed;
    bool                  fGraceNotesGroupIsTied;

    bool                  fGraceNotesGroupIsFollowedByNotes;

    // LilyPond issue 34 may lead to add skip grace notes to voices
    // other than the one containing these grace notes:
    // the measure number is needed to create the first measure
    // in case the grace notes are at the beginning of the voice
    string                fGraceNotesGroupMeasureNumber; // JMI ???
};
typedef SMARTP<msrGraceNotesGroup> S_msrGraceNotesGroup;
EXP ostream& operator<< (ostream& os, const S_msrGraceNotesGroup& elt);

//______________________________________________________________________________
class msrAfterGraceNotesGroupContents : public msrElement
{
  public:
        
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrAfterGraceNotesGroupContents> create (
      int        inputLineNumber,
      S_msrVoice afterGraceNotesGroupContentsVoiceUplink);
    
    SMARTP<msrAfterGraceNotesGroupContents> createAfterGraceNotesGroupContentsNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrVoice> createAfterGraceNotesGroupContentsDeepCopy (
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrAfterGraceNotesGroupContents (
      int        inputLineNumber,
      S_msrVoice afterGraceNotesGroupContentsVoiceUplink);
      
    virtual ~msrAfterGraceNotesGroupContents ();
  
  public:

    // set and get
    // ------------------------------------------------------
                              
    const list<S_msrNote>&
                          getAfterGraceNotesGroupContentsNotesList ()
                              {
                                return
                                  fAfterGraceNotesGroupContentsNotesList;
                              }

    // services
    // ------------------------------------------------------

    // uplinks
    S_msrPart             fetchAfterGraceNotesGroupContentsPartUplink () const;

    // notes
    void                  appendNoteToAfterGraceNotesGroupContents (
                            S_msrNote note);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortString () const;
    
    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // uplinks
    S_msrVoice            fAfterGraceNotesGroupContentsVoiceUplink;

    // the notes list
    list<S_msrNote>       fAfterGraceNotesGroupContentsNotesList;
};
typedef SMARTP<msrAfterGraceNotesGroupContents> S_msrAfterGraceNotesGroupContents;
EXP ostream& operator<< (ostream& os, const S_msrAfterGraceNotesGroupContents& elt);

//______________________________________________________________________________
class msrAfterGraceNotesGroup : public msrElement
{
  public:
        
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrAfterGraceNotesGroup> create (
      int          inputLineNumber,
      S_msrElement afterGraceNotesGroupElement,
      bool         afterGraceNotesGroupIsSlashed,
      S_msrVoice   afterGraceNotesGroupVoiceUplink);
    
    SMARTP<msrAfterGraceNotesGroup> createAfterGraceNotesGroupNewbornClone (
      S_msrNote  noteClone,
      S_msrVoice containingVoice);

    SMARTP<msrVoice> createAfterGraceNotesGroupDeepCopy (
      S_msrNote  noteClone,
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrAfterGraceNotesGroup (
      int          inputLineNumber,
      S_msrElement afterGraceNotesGroupElement,
      bool         afterGraceNotesGroupIsSlashed,
      S_msrVoice   afterGraceNotesGroupVoiceUplink);
      
    virtual ~msrAfterGraceNotesGroup ();
  
  public:

    // set and get
    // ------------------------------------------------------
                              
    S_msrElement          getAfterGraceNotesGroupElement () const
                              { return fAfterGraceNotesGroupElement; }

    S_msrAfterGraceNotesGroupContents
                          getAfterGraceNotesGroupContents () const
                              { return fAfterGraceNotesGroupContents; }

    // services
    // ------------------------------------------------------

    // uplinks
    S_msrPart             fetchAfterGraceNotesGroupPartUplink () const;

    // notes
    void                  appendNoteToAfterGraceNotesGroupContents (
                            S_msrNote note);
    
  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asShortString () const;
    
    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // uplinks
    S_msrVoice            fAfterGraceNotesGroupVoiceUplink;

    S_msrElement          fAfterGraceNotesGroupElement;

    bool                  fAfterGraceNotesGroupIsSlashed;

    S_msrAfterGraceNotesGroupContents
                          fAfterGraceNotesGroupContents;
};
typedef SMARTP<msrAfterGraceNotesGroup> S_msrAfterGraceNotesGroup;
EXP ostream& operator<< (ostream& os, const S_msrAfterGraceNotesGroup& elt);

//______________________________________________________________________________
class msrStanza;
typedef SMARTP<msrStanza> S_msrStanza;

//______________________________________________________________________________
class msrSyllable : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    // we want to end the line in the LilyPond code at a break
    enum msrSyllableKind {
      kSyllableNone,
      kSyllableSingle,
      kSyllableBegin, kSyllableMiddle, kSyllableEnd,
      kSyllableSkip,
      kSyllableMeasureEnd,
      kSyllableLineBreak, kSyllablePageBreak};

    static string syllableKindAsString (
      msrSyllableKind syllableKind);
      
    enum msrSyllableExtendKind {
      kSyllableExtendNone,
      kSyllableExtendSingle,
      kSyllableExtendStart, kSyllableExtendContinue, kSyllableExtendStop };

    static string syllableExtendKindAsString (
      msrSyllableExtendKind syllableExtendKind);


    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSyllable> create (
      int                   inputLineNumber,
      msrSyllableKind       syllableKind,
      msrSyllableExtendKind syllableExtendKind,
      rational              syllableWholeNotes,
      msrTupletFactor       syllableTupletFactor,
      S_msrStanza           syllableStanzaUplink);

    SMARTP<msrSyllable> createSyllableNewbornClone (
      S_msrPart containingPart); // JMI

    SMARTP<msrSyllable> createSyllableDeepCopy (
      S_msrPart containingPart);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSyllable (
      int                   inputLineNumber,
      msrSyllableKind       syllableKind,
      msrSyllableExtendKind syllableExtendKind,
      rational              syllableWholeNotes,
      msrTupletFactor       syllableTupletFactor,
      S_msrStanza           syllableStanzaUplink);
        
    virtual ~msrSyllable ();

  public:

    // set and get
    // ------------------------------------------------------
                              
    // uplinks
    S_msrNote             getSyllableNoteUplink () const
                              { return fSyllableNoteUplink; }

    S_msrStanza           getSyllableStanzaUplink () const
                              { return fSyllableStanzaUplink; }

    // syllable kind and contents
    msrSyllableKind       getSyllableKind () const
                              { return fSyllableKind; }

    const list<string>&   getSyllableTextsList ()
                              { return fSyllableTextsList; }

    msrSyllableExtendKind getSyllableExtendKind () const
                              { return fSyllableExtendKind; }

    // syllable whole notes
    rational              getSyllableWholeNotes () const
                              { return fSyllableWholeNotes; }
                              
    // syllable tuplet factor
    msrTupletFactor       getSyllableTupletFactor () const
                              { return fSyllableTupletFactor; }

    // services
    // ------------------------------------------------------

    void                  appendSyllableToNoteAndSetItsNoteUplink (
                            S_msrNote note);

    void                  appendLyricTextToSyllable (string text);
                                  
  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                syllableNoteUplinkAsString () const;

    string                syllableWholeNotesAsMsrString () const;
    
    string                syllableKindAsString () const;
    
    string                syllableExtendKindAsString () const;
    
    string                syllableTextsListAsString () const;

    static void           writeTextsList (
                            const list<string>& textsList,
                            ostream&            os);

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // uplinks
    S_msrNote             fSyllableNoteUplink;
    S_msrStanza           fSyllableStanzaUplink;
  
    // syllable kind and contents
    msrSyllableKind       fSyllableKind;
    list<string>          fSyllableTextsList;
    msrSyllableExtendKind fSyllableExtendKind;

    // syllable whole notes
    rational              fSyllableWholeNotes;

    // syllable tuplet factor
    msrTupletFactor       fSyllableTupletFactor;
};
typedef SMARTP<msrSyllable> S_msrSyllable;
EXP ostream& operator<< (ostream& os, const S_msrSyllable& elt);

//______________________________________________________________________________
class msrHarmonyDegree : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

/*
Degree elements
  can then add, subtract, or alter from these
  starting points.

      <harmony>
        <root>
          <root-step>B</root-step>
        </root>
        <kind>dominant</kind>
        <degree>
          <degree-value>5</degree-value>
          <degree-alter>1</degree-alter>
          <degree-type>alter</degree-type>
        </degree>
        <degree>
          <degree-value>9</degree-value>
          <degree-alter>1</degree-alter>
          <degree-type>add</degree-type>
        </degree>
      </harmony>
*/

    enum msrHarmonyDegreeTypeKind {
      kHarmonyDegreeTypeAdd,
      kHarmonyDegreeTypeAlter,
      kHarmonyDegreeTypeSubstract };

    static string harmonyDegreeTypeKindAsString (
      msrHarmonyDegreeTypeKind harmonyDegreeTypeKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrHarmonyDegree> create (
      int                      inputLineNumber,
      int                      harmonyDegreeValue,
      msrAlterationKind        harmonyDegreeAlterationKind,
      msrHarmonyDegreeTypeKind harmonyDegreeTypeKind);

    /* JMI
    SMARTP<msrHarmonyDegree> createHarmonyDegreeNewbornClone (
      S_msrPart containingPart);

    SMARTP<msrHarmonyDegree> createHarmonyDegreeDeepCopy ( // JMI ???
      S_msrPart containingPart);
      */

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrHarmonyDegree (
      int                      inputLineNumber,
      int                      harmonyDegreeValue,
      msrAlterationKind        harmonyDegreeAlterationKind,
      msrHarmonyDegreeTypeKind harmonyDegreeTypeKind);

    virtual ~msrHarmonyDegree ();
  
  public:

    // set and get
    // ------------------------------------------------------

    void                  setHarmonyDegreeHarmonyUplink (
                            S_msrHarmony harmonyUplink);

    S_msrHarmony          getHarmonyDegreeHarmonyUplink () const
                              { return fHarmonyDegreeHarmonyUplink; }

    int                   getHarmonyDegreeValue () const
                              { return fHarmonyDegreeValue; }
                              
    msrAlterationKind     getHarmonyDegreeAlterationKind () const
                              { return fHarmonyDegreeAlterationKind; }
                              
    msrHarmonyDegreeTypeKind
                          getHarmonyDegreeTypeKind () const
                              { return fHarmonyDegreeTypeKind; }

    // services
    // ------------------------------------------------------

    int                   harmonyDegreeAsSemitones () const;
    
  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                harmonyDegreeKindAsString () const;
    string                harmonyDegreeKindAsShortString () const;
    
    string                asString () const;
   
    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // uplinks
    S_msrHarmony          fHarmonyDegreeHarmonyUplink;

    int                   fHarmonyDegreeValue;
    msrAlterationKind     fHarmonyDegreeAlterationKind;
    msrHarmonyDegreeTypeKind
                          fHarmonyDegreeTypeKind;
};
typedef SMARTP<msrHarmonyDegree> S_msrHarmonyDegree;
EXP ostream& operator<< (ostream& os, const S_msrHarmonyDegree& elt);

//______________________________________________________________________________
class msrHarmony : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------
      
    static SMARTP<msrHarmony> create (
      int                      inputLineNumber,
      // no harmonyVoiceUplink yet
      msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
      msrHarmonyKind           harmonyKind,
      string                   harmonyKindText,
      int                      harmonyInversion,
      msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
      rational                 harmonySoundingWholeNotes);
    
    SMARTP<msrHarmony> createHarmonyNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrHarmony> createHarmonyDeepCopy ( // JMI ???
      S_msrVoice containingVoice);

    // applications API
    // ------------------------------------------------------

    static SMARTP<msrHarmony> create (
      int                      inputLineNumber,
      S_msrVoice               harmonyVoiceUplink,
      msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
      msrHarmonyKind           harmonyKind,
      string                   harmonyKindText,
      int                      harmonyInversion,
      msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
      rational                 harmonySoundingWholeNotes);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrHarmony (
      int                      inputLineNumber,
      S_msrVoice               harmonyVoiceUplink,
      msrQuarterTonesPitchKind harmonyRootQuarterTonesPitchKind,
      msrHarmonyKind           harmonyKind,
      string                   harmonyKindText,
      int                      harmonyInversion,
      msrQuarterTonesPitchKind harmonyBassQuarterTonesPitchKind,
      rational                 harmonySoundingWholeNotes);

    virtual ~msrHarmony ();
  
  public:

    // set and get
    // ------------------------------------------------------

    void                  setHarmonyNoteUplink (
                            S_msrNote note)
                              { fHarmonyNoteUplink = note; }

    S_msrNote             getHarmonyNoteUplink () const
                             { return fHarmonyNoteUplink; }

    void                  setHarmonyVoiceUplink (
                            S_msrVoice voice)
                              { fHarmonyVoiceUplink = voice; }

    S_msrVoice            getHarmonyVoiceUplink () const
                             { return fHarmonyVoiceUplink; }

    rational              getHarmonySoundingWholeNotes () const
                              { return fHarmonySoundingWholeNotes; }

    void                  setHarmonySoundingWholeNotes (
                            rational wholeNotes)
                              { fHarmonySoundingWholeNotes = wholeNotes; }

    msrQuarterTonesPitchKind
                          getHarmonyRootQuarterTonesPitchKind () const
                              { return fHarmonyRootQuarterTonesPitchKind; }
                                
    msrHarmonyKind        getHarmonyKind () const
                              { return fHarmonyKind; }
                
    string                getHarmonyKindText () const
                              { return fHarmonyKindText; }
                
    const list<S_msrHarmonyDegree>&
                          getHarmonyDegreesList ()
                              { return fHarmonyDegreesList; }
                
    int                   getHarmonyInversion () const
                              { return fHarmonyInversion; }
                
    msrQuarterTonesPitchKind
                          getHarmonyBassQuarterTonesPitchKind () const
                              { return fHarmonyBassQuarterTonesPitchKind; }
                              
    // services
    // ------------------------------------------------------

    void                  appendHarmonyDegreeToHarmony (
                            S_msrHarmonyDegree harmonyDegree)
                              {
                                fHarmonyDegreesList.push_back (
                                  harmonyDegree);
                              }
    
  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;
   
    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // uplinks
    S_msrNote             fHarmonyNoteUplink;
    S_msrVoice            fHarmonyVoiceUplink;

    rational              fHarmonySoundingWholeNotes;
    
    msrQuarterTonesPitchKind
                          fHarmonyRootQuarterTonesPitchKind;

    msrHarmonyKind        fHarmonyKind;
    string                fHarmonyKindText;

    int                   fHarmonyInversion;
    
    msrQuarterTonesPitchKind
                          fHarmonyBassQuarterTonesPitchKind;

    list<S_msrHarmonyDegree>
                          fHarmonyDegreesList;
};
typedef SMARTP<msrHarmony> S_msrHarmony;
EXP ostream& operator<< (ostream& os, const S_msrHarmony& elt);

//______________________________________________________________________________
class msrFigure : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrFigurePrefixKind {
      k_NoFigurePrefix,
      kDoubleFlatPrefix, kFlatPrefix, kFlatFlatPrefix,
      kNaturalPrefix,
      kSharpSharpPrefix, kSharpPrefix, kDoubleSharpPrefix };

    static string figurePrefixKindAsString (
      msrFigurePrefixKind figurePrefixKind);
      
    enum msrFigureSuffixKind {
      k_NoFigureSuffix,
      kDoubleFlatSuffix, kFlatSuffix, kFlatFlatSuffix,
      kNaturalSuffix,
      kSharpSharpSuffix, kSharpSuffix, kDoubleSharpSuffix,
      kSlashSuffix };

    static string figureSuffixKindAsString (
      msrFigureSuffixKind figureSuffixKind);
            
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrFigure> create (
      int                 inputLineNumber,
      S_msrPart           figurePartUplink,
      msrFigurePrefixKind figurePrefixKind,
      int                 figureNumber,
      msrFigureSuffixKind figureSuffixKind);
    
    SMARTP<msrFigure> createFigureNewbornClone (
      S_msrPart containingPart);

    SMARTP<msrFigure> createFigureDeepCopy ( // JMI ???
      S_msrPart containingPart);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrFigure (
      int                 inputLineNumber,
      S_msrPart           figurePartUplink,
      msrFigurePrefixKind figurePrefixKind,
      int                 figureNumber,
      msrFigureSuffixKind figureSuffixKind);

    virtual ~msrFigure ();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrPart             getFigurePartUplink () const
                              { return fFigurePartUplink; }

    msrFigurePrefixKind
                          getFigurePrefixKind () const
                              { return fFigurePrefixKind; }

    int                   getFigureNumber () const
                              { return fFigureNumber; }

    msrFigureSuffixKind
                          getFigureSuffixKind () const
                              { return fFigureSuffixKind; }
                                                                              
    // services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                figurePrefixKindAsString () const;
    string                figureSuffixKindAsShortString () const;
    
    string                asString () const;
   
    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // uplinks
    S_msrPart             fFigurePartUplink;

    msrFigurePrefixKind
                          fFigurePrefixKind;
    int                   fFigureNumber;
    msrFigureSuffixKind
                          fFigureSuffixKind;    
};
typedef SMARTP<msrFigure> S_msrFigure;
EXP ostream& operator<< (ostream& os, const S_msrFigure& elt);

//______________________________________________________________________________
class msrFiguredBass : public msrElement
{
  public:
            
    // data types
    // ------------------------------------------------------

    enum msrFiguredBassParenthesesKind {
      kFiguredBassParenthesesYes, kFiguredBassParenthesesNo };

    static string figuredBassParenthesesKindAsString (
      msrFiguredBassParenthesesKind figuredBassParenthesesKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrFiguredBass> create (
      int       inputLineNumber,
      S_msrPart figuredBassPartUplink); // set by part ??? JMI
    
    static SMARTP<msrFiguredBass> create (
      int       inputLineNumber,
      S_msrPart figuredBassPartUplink,
      rational  figuredBassSoundingWholeNotes,
      msrFiguredBassParenthesesKind
                figuredBassParenthesesKind);
    
    SMARTP<msrFiguredBass> createFiguredBassNewbornClone (
      S_msrPart containingPart);

    SMARTP<msrFiguredBass> createFiguredBassDeepCopy ( // JMI ???
      S_msrPart containingPart);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrFiguredBass (
      int       inputLineNumber,
      S_msrPart figuredBassPartUplink,
      rational  figuredBassSoundingWholeNotes,
      msrFiguredBassParenthesesKind
                figuredBassParenthesesKind);

    virtual ~msrFiguredBass ();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrPart             getFiguredBassPartUplink () const
                              { return fFiguredBassPartUplink; }

    void                  setFiguredBassSoundingWholeNotes ( // JMI
                            rational figuredBassSoundingWholeNotes)
                              {
                                fFiguredBassSoundingWholeNotes =
                                  figuredBassSoundingWholeNotes;
                              }

    rational              getFiguredBassSoundingWholeNotes () const
                              { return fFiguredBassSoundingWholeNotes; }

    msrFiguredBassParenthesesKind
                          getFiguredBassParenthesesKind () const
                              { return fFiguredBassParenthesesKind; }

    const list<S_msrFigure>&
                          getFiguredBassFiguresList ()
                              { return fFiguredBassFiguresList;  }
                                                                              
    // services
    // ------------------------------------------------------

    void                  appendFiguredFigureToFiguredBass (
                            S_msrFigure figure);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;
   
    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // uplinks
    S_msrPart             fFiguredBassPartUplink;

    rational              fFiguredBassSoundingWholeNotes;

    msrFiguredBassParenthesesKind
                          fFiguredBassParenthesesKind;

    list<S_msrFigure>     fFiguredBassFiguresList; 
};
typedef SMARTP<msrFiguredBass> S_msrFiguredBass;
EXP ostream& operator<< (ostream& os, const S_msrFiguredBass& elt);

//______________________________________________________________________________
class msrGlissando : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrGlissandoTypeKind {
      kGlissandoTypeNone,
      kGlissandoTypeStart, kGlissandoTypeStop };

    static string glissandoTypeKindAsString (
      msrGlissandoTypeKind glissandoTypeKind);
                  
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrGlissando> create (
      int                  inputLineNumber,
      int                  glissandoNumber,
      msrGlissandoTypeKind glissandoTypeKind,
      msrLineTypeKind      glissandoLineTypeKind,
      string               glissandoTextValue);

    SMARTP<msrGlissando> createGlissandoNewbornClone ();

    SMARTP<msrGlissando> createGlissandoDeepCopy ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrGlissando (
      int                  inputLineNumber,
      int                  glissandoNumber,
      msrGlissandoTypeKind glissandoTypeKind,
      msrLineTypeKind      glissandoLineTypeKind,
      string               glissandoTextValue);
      
    virtual ~msrGlissando ();
  
  public:

    // set and get
    // ------------------------------------------------------
    
    int                   getGlissandoNumber () const
                              { return fGlissandoNumber; }

    msrGlissandoTypeKind  getGlissandoTypeKind () const
                              { return fGlissandoTypeKind; }
            
    msrLineTypeKind       getGlissandoLineTypeKind () const
                              { return fGlissandoLineTypeKind; }
            
    string                getGlissandoTextValue () const
                              { return fGlissandoTextValue; }
            
    // measure uplink
    void                  setGlissandoMeasureUplink (
                            const S_msrMeasure& measure)
                              { fGlissandoMeasureUplink = measure; }
                      
    S_msrMeasure          getGlissandoMeasureUplink () const
                              { return fGlissandoMeasureUplink; }
                      
    // services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // uplink
    S_msrMeasure          fGlissandoMeasureUplink;
    
    int                   fGlissandoNumber;
                  
    msrGlissandoTypeKind  fGlissandoTypeKind;
                          
    msrLineTypeKind       fGlissandoLineTypeKind;
    
    string                fGlissandoTextValue;
};
typedef SMARTP<msrGlissando> S_msrGlissando;
EXP ostream& operator<< (ostream& os, const S_msrGlissando& elt);

//______________________________________________________________________________
class msrSlide : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrSlideTypeKind {
      kSlideTypeNone,
      kSlideTypeStart, kSlideTypeStop };

    static string slideTypeKindAsString (
      msrSlideTypeKind slideTypeKind);
                  
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrSlide> create (
      int              inputLineNumber,
      int              slideNumber,
      msrSlideTypeKind slideTypeKind,
      msrLineTypeKind  slideLineTypeKind,
      string           slideTextValue);

    SMARTP<msrSlide> createSlideNewbornClone ();

    SMARTP<msrSlide> createSlideDeepCopy ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrSlide (
      int              inputLineNumber,
      int              slideNumber,
      msrSlideTypeKind slideTypeKind,
      msrLineTypeKind  slideLineTypeKind,
      string           slideTextValue);
      
    virtual ~msrSlide ();
  
  public:

    // set and get
    // ------------------------------------------------------
    
    int                   getSlideNumber () const
                              { return fSlideNumber; }

    msrSlideTypeKind      getSlideTypeKind () const
                              { return fSlideTypeKind; }
            
    msrLineTypeKind       getSlideLineTypeKind () const
                              { return fSlideLineTypeKind; }
            
    string                getSlideTextValue () const
                              { return fSlideTextValue; }
            
    // measure uplink
    void                  setSlideMeasureUplink (
                            const S_msrMeasure& measure)
                              { fSlideMeasureUplink = measure; }
                      
    S_msrMeasure          getSlideMeasureUplink () const
                              { return fSlideMeasureUplink; }
                      
    // services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // uplink
    S_msrMeasure          fSlideMeasureUplink;
    
    int                   fSlideNumber;
                  
    msrSlideTypeKind      fSlideTypeKind;
                          
    msrLineTypeKind       fSlideLineTypeKind;

    string                fSlideTextValue;
};
typedef SMARTP<msrSlide> S_msrSlide;
EXP ostream& operator<< (ostream& os, const S_msrSlide& elt);

//______________________________________________________________________________
class msrNote : public msrElement
{
  public:

    // constants
    // ------------------------------------------------------

    #define K_NO_OCTAVE -1
    
    #define K_NO_MEASURE_NUMBER "unknown"
  
    #define K_NO_POSITION_MEASURE_NUMBER rational(-222, 1)

    // data types
    // ------------------------------------------------------

    enum msrNoteKind {
      k_NoNoteKind,
      kRestNote, kSkipNote,
      kUnpitchedNote,
      kStandaloneNote,
      kDoubleTremoloMemberNote,
      kGraceNote,
      kChordMemberNote,
      kGraceChordMemberNote,
      kTupletMemberNote,
      kGraceTupletMemberNote,
      kTupletMemberUnpitchedNote };
      
    static string noteKindAsString (
      msrNoteKind noteKind);
      
    enum msrNoteAccidentalKind {
      kNoteAccidentalNone,
      
      kNoteAccidentalSharp, kNoteAccidentalNatural, kNoteAccidentalFlat, kNoteAccidentaldoubleSharp, kNoteAccidentalSharpSharp,
      kNoteAccidentalFlatFlat, kNoteAccidentalNaturalSharp,
      kNoteAccidentalNaturalFlat, kNoteAccidentalQuarterFlat, kNoteAccidentalQuarterSharp,kNoteAccidentalThreeQuartersFlat, kNoteAccidentalThreeQuartersSharp,
      
      kNoteAccidentalSharpDown, kNoteAccidentalSharpUp,
      kNoteAccidentalNaturalDown, kNoteAccidentalNaturalUp,
      kNoteAccidentalFlatDown, kNoteAccidentalFlatUp,kNoteAccidentalTripleSharp, kNoteAccidentalTripleFlat,
      kNoteAccidentalSlashQuarterSharp, kNoteAccidentalSlashSharp, kNoteAccidentalSlashFlat,kNoteAccidentaldoubleSlashFlat,
      kNoteAccidentalSharp_1, kNoteAccidentalSharp_2, kNoteAccidentalSharp_3, kNoteAccidentalSharp_5, kNoteAccidentalFlat_1, kNoteAccidentalFlat_2, kNoteAccidentalFlat_3, kNoteAccidentalFlat_4, kNoteAccidentalSori, kNoteAccidentalKoron };
  
    string noteAccidentalKindAsString (
      msrNoteAccidentalKind noteAccidentalKind);
  
    enum msrNoteEditorialAccidentalKind {
      kNoteEditorialAccidentalYes, kNoteEditorialAccidentalNo };

    static string noteEditorialAccidentalKindAsString (
      msrNoteEditorialAccidentalKind noteEditorialAccidentalKind);
      
    enum msrNoteCautionaryAccidentalKind {
      kNoteCautionaryAccidentalYes, kNoteCautionaryAccidentalNo };

    static string noteCautionaryAccidentalKindAsString (
      msrNoteCautionaryAccidentalKind noteCautionaryAccidentalKind);

    enum msrNotePrintKind {
      kNotePrintYes, kNotePrintNo };
      
    static string notePrintKindAsString (
      msrNotePrintKind notePrintKind);
      
    enum msrNoteHeadKind {
      kNoteHeadSlash,
      kNoteHeadTriangle, kNoteHeadDiamond, kNoteHeadSquare,
      kNoteHeadCross,kNoteHeadX,
      kNoteHeadCircleX, kNoteHeadInvertedTriangle,
      kNoteHeadArrowDown, kNoteHeadArrowUp,
      kNoteHeadSlashed, kNoteHeadBackSlashed,
      kNoteHeadNormal, kNoteHeadCluster,
      kNoteHeadCircleDot,
      kNoteHeadLeftTriangle,
      kNoteHeadRectangle,
      kNoteHeadNone,
      kNoteHeadDo, kNoteHeadRe, kNoteHeadMi, kNoteHeadFa, kNoteHeadFaUp,
      kNoteHeadSo, kNoteHeadLa, kNoteHeadTi };
      
    static string noteHeadKindAsString (
      msrNoteHeadKind noteHeadKind);
      
    enum msrNoteHeadFilledKind {
      kNoteHeadFilledYes, kNoteHeadFilledNo };
      
    static string noteHeadFilledKindAsString (
      msrNoteHeadFilledKind noteHeadFilledKind);
      
    enum msrNoteHeadParenthesesKind {
      kNoteHeadParenthesesYes, kNoteHeadParenthesesNo };
      
    static string noteHeadParenthesesKindAsString (
      msrNoteHeadParenthesesKind noteHeadParenthesesKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrNote> create (
      int                        inputLineNumber,
      string                     noteMeasureNumber,
      
      msrNoteKind                noteKind,
    
      msrQuarterTonesPitchKind   noteQuarterTonesPitchKind,
      
      rational                   noteSoundingWholeNotes,
      rational                   noteDisplayWholeNotes,
      
      int                        noteDotsNumber,
      
      msrDurationKind            noteGraphicDurationKind,
      
      int                        noteOctave,
      
      msrQuarterTonesPitchKind   noteQuarterTonesDisplayPitchKind,
      int                        noteDisplayOctave,
      
      bool                       noteIsARest,
      bool                       noteIsUnpitched,
    
      bool                       noteIsACueNote,
      bool                       noteIsAGraceNote,

      msrNotePrintKind           notePrintKind,
      
      msrNoteHeadKind            noteHeadKind,
      msrNoteHeadFilledKind      noteHeadFilledKind,
      msrNoteHeadParenthesesKind noteHeadParenthesesKind);
    
    SMARTP<msrNote> createNoteNewbornClone (
      S_msrPart containingPart);
    
    SMARTP<msrNote> createNoteDeepCopy (
      S_msrVoice containingVoice);

    // creation from xml2Msr
    // ------------------------------------------------------

    static SMARTP<msrNote> createRestNote (
      int       inputLineNumber,
      string    noteMeasureNumber,
      rational  soundingWholeNotes,
      rational  displayWholeNotes,
      int       dotsNumber,
      int       staffNumber,
      int       voiceNumber);
    
    static SMARTP<msrNote> createSkipNote (
      int       inputLineNumber,
      string    noteMeasureNumber,
      rational  soundingWholeNotes,
      rational  displayWholeNotes,
      int       dotsNumber,
      int       staffNumber,
      int       voiceNumber);
    
  protected:
 
    // constructors/destructor
    // ------------------------------------------------------

    msrNote (
      int                        inputLineNumber,
      string                     noteMeasureNumber,
      
      msrNoteKind                noteKind,
    
      msrQuarterTonesPitchKind   noteQuarterTonesPitchKind,
      
      rational                   noteSoundingWholeNotes,
      rational                   noteDisplayWholeNotes,
      
      int                        noteDotsNumber,
      
      msrDurationKind            noteGraphicDurationKind,
      
      int                        noteOctave,
      
      msrQuarterTonesPitchKind   noteQuarterTonesDisplayPitchKind,
      int                        noteDisplayOctave,
      
      bool                       noteIsARest,
      bool                       noteIsUnpitched,
    
      bool                       noteIsACueNote,
      bool                       noteIsAGraceNote,

      msrNotePrintKind           notePrintKind,
      
      msrNoteHeadKind            noteHeadKind,
      msrNoteHeadFilledKind      noteHeadFilledKind,
      msrNoteHeadParenthesesKind noteHeadParenthesesKind);
        
    virtual ~msrNote ();
    
  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeNote ();
  
  public:

    // set and get
    // ------------------------------------------------------

    // measure uplink
    void                  setNoteMeasureUplink (
                            const S_msrMeasure& measure)
                              { fNoteMeasureUplink = measure; }
                      
    S_msrMeasure          getNoteMeasureUplink () const
                              { return fNoteMeasureUplink; }

    // chord uplink
    void                  setNoteChordUplink (
                            const S_msrChord& chord)
                              { fNoteChordUplink = chord; }
                      
    S_msrChord            getNoteChordUplink () const
                              { return fNoteChordUplink; }

    // grace notes group uplink
    void                  setNoteGraceNotesGroupUplink (
                            const S_msrGraceNotesGroup& graceNotesGroup)
                              { fNoteGraceNotesGroupUplink = graceNotesGroup; }
                      
    S_msrGraceNotesGroup  getNoteGraceNotesGroupUplink () const
                              { return fNoteGraceNotesGroupUplink; }

    // tuplet uplink
    void                  setNoteTupletUplink (
                            const S_msrTuplet& tuplet)
                              { fNoteTupletUplink = tuplet; }
                      
    S_msrTuplet           getNoteTupletUplink () const
                              { return fNoteTupletUplink; }

    // note kind
    
    void                  setNoteKind (msrNoteKind noteKind);

    msrNoteKind           getNoteKind () const
                              { return fNoteKind; }

    // note pitch

    msrQuarterTonesPitchKind
                          getNoteQuarterTonesPitchKind () const
                              { return fNoteQuarterTonesPitchKind; }

    // note print kind

    void                  setNotePrintKind (
                            msrNotePrintKind notePrintKind)
                              { fNotePrintKind = notePrintKind; }
    msrNotePrintKind      getNotePrintKind () const
                              { return fNotePrintKind; }

    // note head

    void                  setNoteHeadKind (
                            msrNoteHeadKind noteHeadKind)
                              { fNoteHeadKind = noteHeadKind; }

    msrNoteHeadKind       getNoteHeadKind () const
                              { return fNoteHeadKind; }

    void                  setNoteHeadFilledKind (
                            msrNoteHeadFilledKind
                              noteHeadFilledKind)
                              {
                                fNoteHeadFilledKind =
                                  noteHeadFilledKind;
                              }

    msrNoteHeadFilledKind getNoteHeadFilledKind () const
                              { return fNoteHeadFilledKind; }

    void                  setNoteHeadParenthesesKind (
                            msrNoteHeadParenthesesKind
                              noteHeadParenthesesKind)
                              {
                                fNoteHeadParenthesesKind =
                                  noteHeadParenthesesKind;
                              }

    msrNoteHeadParenthesesKind
                          getNoteHeadParenthesesKind () const
                              { return fNoteHeadParenthesesKind; }

    // accidentals

    void                  setNoteAccidentalKind (
                            msrNoteAccidentalKind
                              noteAccidentalKind)
                              {
                                fNoteAccidentalKind =
                                  noteAccidentalKind;
                              }
                              
    msrNoteAccidentalKind
                          getNoteAccidentalKind ()
                              {
                                return
                                  fNoteAccidentalKind;
                              }

    void                  setNoteEditorialAccidentalKind (
                            msrNoteEditorialAccidentalKind
                              noteEditorialAccidentalKind)
                              {
                                fNoteEditorialAccidentalKind =
                                  noteEditorialAccidentalKind;
                              }
                              
    msrNoteEditorialAccidentalKind
                          getNoteEditorialAccidentalKind ()
                              {
                                return
                                  fNoteEditorialAccidentalKind;
                              }

    void                  setNoteCautionaryAccidentalKind (
                            msrNoteCautionaryAccidentalKind
                              noteCautionaryAccidentalKind)
                              {
                                fNoteCautionaryAccidentalKind =
                                  noteCautionaryAccidentalKind;
                              }
                              
    msrNoteCautionaryAccidentalKind
                          getNoteCautionaryAccidentalKind ()
                              {
                                return
                                  fNoteCautionaryAccidentalKind;
                              }

    // note whole notes
    
    void                  setNoteSoundingWholeNotes (
                            rational wholeNotes);

    rational              getNoteSoundingWholeNotes ()
                              {
                                return
                                  fNoteSoundingWholeNotes;
                              }

    // note display
    
    msrQuarterTonesPitchKind
                          getNoteQuarterTonesDisplayPitchKind () const
                              {
                                return
                                  fNoteQuarterTonesDisplayPitchKind;
                              }
                              
    void                  setNoteDisplayWholeNotes (
                            rational wholeNotes);

    rational              getNoteDisplayWholeNotes ()
                              { return fNoteDisplayWholeNotes; }

    // dots
    
    int                   getNoteDotsNumber () const
                              { return fNoteDotsNumber; }

    msrDurationKind       getNoteGraphicDurationKind () const
                              { return fNoteGraphicDurationKind; }

    int                   getNoteOctave () const
                              { return fNoteOctave; }

    int                   getNoteDisplayOctave () const
                              { return fNoteDisplayOctave; }

    bool                  getNoteIsARest () const
                            // useful for rest tuplet members
                            { return fNoteIsARest; }
                      
    bool                  getNoteIsUnpitched () const
                              { return fNoteIsUnpitched; }
                      
    bool                  getNoteIsACueNote () const
                              { return fNoteIsACueNote; }
                      
    // octave shifts
    void                  setNoteOctaveShift (
                             S_msrOctaveShift octaveShift)
                                { fNoteOctaveShift = octaveShift; }

    S_msrOctaveShift      getNoteOctaveShift () const
                              { return fNoteOctaveShift; }

    // note context
    // -------------------------------

    // chord members
    void                  setNoteBelongsToAChord ();

    bool                  getNoteBelongsToAChord () const
                              { return fNoteBelongsToAChord; }

    // grace notes
    bool                  getNoteIsAGraceNote () const
                              { return fNoteIsAGraceNote; }

    // harmony
    void                  setNoteHarmony (
                            S_msrHarmony harmony);
                              
    const S_msrHarmony&   getNoteHarmony () const
                              { return fNoteHarmony; };

    // frame
    void                  setNoteFrame (
                            S_msrFrame frame);
                              
    const S_msrFrame&     getNoteFrame () const
                              { return fNoteFrame; };

    // figured bass
    void                  setNoteFiguredBass (
                            S_msrFiguredBass figuredBass);
                              
    const S_msrFiguredBass&
                          getNoteFiguredBass () const
                              { return fNoteFiguredBass; };

    // note lyrics
    // -------------------------------

    list<S_msrSyllable>   getNoteSyllables () const
                              { return fNoteSyllables; }              

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

    // spanners
    const list<S_msrSpanner>&
                          getNoteSpanners () const
                              { return fNoteSpanners; }
                      
    list<S_msrSpanner>&
                          getNoteSpannersToModify ()
                              { return fNoteSpanners; }

    // technicals
    const list<S_msrTechnical>&
                          getNoteTechnicals () const
                              { return fNoteTechnicals; }
                              
    const list<S_msrTechnicalWithInteger>&
                          getNoteTechnicalWithIntegers () const
                              { return fNoteTechnicalWithIntegers; }
                              
    const list<S_msrTechnicalWithFloat>&
                          getNoteTechnicalWithFloats () const
                              { return fNoteTechnicalWithFloats; }
                              
    const list<S_msrTechnicalWithString>&
                          getNoteTechnicalWithStrings () const
                              { return fNoteTechnicalWithStrings; }
                              
    // ornaments
    const list<S_msrOrnament>&
                          getNoteOrnaments () const
                              { return fNoteOrnaments; }
        
    // glissandos
    const list<S_msrGlissando>&
                          getNoteGlissandos () const
                              { return fNoteGlissandos; }
        
    // slides
    const list<S_msrSlide>&
                          getNoteSlides () const
                              { return fNoteSlides; }
        
    // grace notes
    void                  setNoteGraceNotesGroupBefore (
                            S_msrGraceNotesGroup graceNotesGroupBefore);

    S_msrGraceNotesGroup  getNoteGraceNotesGroupBefore () const
                              { return fNoteGraceNotesGroupBefore; }

    void                  setNoteGraceNotesGroupAfter (
                            S_msrGraceNotesGroup graceNotesGroupAfter);

    S_msrGraceNotesGroup  getNoteGraceNotesGroupAfter () const
                              { return fNoteGraceNotesGroupAfter; }

    // after grace notes
    /* JMI
    void                  setNoteAfterGraceNotesGroup (
                            S_msrAfterGraceNotesGroup afterGraceNotesGroup);

    S_msrAfterGraceNotesGroup
                          getNoteAfterGraceNotesGroup () const
                              { return fNoteAfterGraceNotesGroup; }
                            */

    // singleTremolo
    void                  setNoteSingleTremolo (
                            S_msrSingleTremolo trem);
    
    S_msrSingleTremolo    getNoteSingleTremolo () const
                              { return fNoteSingleTremolo; }

    // ties
    void                  setNoteTie (S_msrTie tie)
                              { fNoteTie = tie; }
    S_msrTie              getNoteTie () const
                              { return fNoteTie; }

    // dynamics
    const list<S_msrDynamics>&
                          getNoteDynamics () const
                              { return fNoteDynamics; }
    const list<S_msrOtherDynamics>&
                          getNoteOtherDynamics () const
                              { return fNoteOtherDynamics; }
        
    // words
    const list<S_msrWords>&
                          getNoteWords () const
                              { return fNoteWords; }
                      
    list<S_msrWords>&     getNoteWordsToModify ()
                              { return fNoteWords; }
                      
    // slashes
    const list<S_msrSlash>&
                          getNoteSlashes () const
                              { return fNoteSlashes; }

    // wedges
    const list<S_msrWedge>&
                          getNoteWedges () const
                              { return fNoteWedges; }

    list<S_msrWedge>&
                          getNoteWedgesToModify ()
                              { return fNoteWedges; }

    // eyeglasses
    const list<S_msrEyeGlasses>&
                          getNoteEyeGlasses () const
                              { return fNoteEyeGlasses; }
    
    // damps
    const list<S_msrDamp>&
                          getNoteDamps () const
                              { return fNoteDamps; };
    
    // damp alls
    const list<S_msrDampAll>&
                          getNoteDampAlls () const
                              { return fNoteDampAlls; }
    
    // scordaturas
    const list<S_msrScordatura>&
                          getNoteScordaturas () const
                              { return fNoteScordaturas; }

    // slurs
    const list<S_msrSlur>&
                          getNoteSlurs () const
                              { return fNoteSlurs; }

    // ligatures
    const list<S_msrLigature>&
                          getNoteLigatures () const
                              { return fNoteLigatures; }

    // pedals
    const list<S_msrPedal>&
                          getNotePedals () const
                              { return fNotePedals; }

    // note measure information
    // -------------------------------

    // measure number
    void                  setNoteMeasureNumber (
                            string measureNumber)
                              { fNoteMeasureNumber = measureNumber; }

    string                getNoteMeasureNumber () const
                              { return fNoteMeasureNumber; }
 
    // position in measure
    void                  setNotePositionInMeasure (
                            rational positionInMeasure)
                              {
                                fNotePositionInMeasure =
                                  positionInMeasure;
                              }
                      
    rational              getNotePositionInMeasure () const
                              { return fNotePositionInMeasure; }

    void                  setNoteOccupiesAFullMeasure ();
                      
    bool                  getNoteOccupiesAFullMeasure () const
                              { return fNoteOccupiesAFullMeasure; }

    // note redundant information (for speed)
    bool                  getNoteIsStemless () const
                              { return fNoteIsStemless; }

    void                  setNoteIsAChordsFirstMemberNote ()
                              {
                                fNoteIsAChordsFirstMemberNote = true;
                              }

    bool                  getNoteIsAChordsFirstMemberNote () const
                              { return fNoteIsAChordsFirstMemberNote; }

    void                  setNoteIsFirstNoteInADoubleTremolo ()
                              { fNoteIsFirstNoteInADoubleTremolo = true; }

    bool                  getNoteIsFirstNoteInADoubleTremolo () const
                              { return fNoteIsFirstNoteInADoubleTremolo; }
                  
    void                  setNoteIsSecondNoteInADoubleTremolo ()
                              { fNoteIsSecondNoteInADoubleTremolo = true; }

    bool                  getNoteIsSecondNoteInADoubleTremolo () const
                              { return fNoteIsSecondNoteInADoubleTremolo; }
                  
    S_msrOrnament         getNoteTrillOrnament () const
                              { return fNoteTrillOrnament; }
                  
    S_msrOrnament         getNoteDashesOrnament () const
                              { return fNoteDashesOrnament; }
                  
    S_msrOrnament         getNoteDelayedTurnOrnament () const
                              { return fNoteDelayedTurnOrnament; }
                              
    S_msrOrnament         getNoteDelayedInvertedTurnOrnament () const
                              { return fNoteDelayedInvertedTurnOrnament; }
                              
    S_msrSpanner          getNoteWavyLineSpannerStart () const
                              { return fNoteWavyLineSpannerStart; }
                  
    S_msrSpanner          getNoteWavyLineSpannerStop () const
                              { return fNoteWavyLineSpannerStop; }
                  
    void                  setNoteIsFollowedByGraceNotesGroup ()
                              { fNoteIsFollowedByGraceNotesGroup = true; }

    bool                  getNoteIsFollowedByGraceNotesGroup () const
                              { return fNoteIsFollowedByGraceNotesGroup; }
                  
    // services
    // ------------------------------------------------------

    bool                  noteIsAPitchedRest () const;
    
    string                asShortStringWithRawWholeNotes () const;
    
    // diatonic pitch
    msrDiatonicPitchKind  noteDiatonicPitchKind (
                            int inputLineNumber) const;
                            
    // tuplet members
    void                  determineTupletMemberSoundingFromDisplayWholeNotes (
                            int actualNotes, int normalNotes);
                    
    // lyrics
    void                  appendSyllableToNote (S_msrSyllable syllable);

    // beams
    void                  appendBeamToNote (S_msrBeam beam);
    
    // articulations
    void                  appendArticulationToNote (S_msrArticulation art);
    
    // spanners
    void                  appendSpannerToNote (S_msrSpanner span);
    
    // technicals
    void                  appendTechnicalToNote (S_msrTechnical tech);
    
    void                  appendTechnicalWithIntegerToNote (
                            S_msrTechnicalWithInteger tech);
    
    void                  appendTechnicalWithFloatToNote (
                            S_msrTechnicalWithFloat tech);
    
    void                  appendTechnicalWithStringToNote (
                            S_msrTechnicalWithString tech);
    
    // ornaments
    void                  appendOrnamentToNote (S_msrOrnament orn);
    
    // glissandos
    void                  appendGlissandoToNote (S_msrGlissando glissando);
    
    // slides
    void                  appendSlideToNote (S_msrSlide slide);
    
    // dynamics
    void                  appendDynamicsToNote (
                            S_msrDynamics dynamics);
    void                  appendOtherDynamicsToNote (
                            S_msrOtherDynamics otherDynamics);

    S_msrDynamics         removeFirstDynamics (); // ???

    // words
    void                  appendWordsToNote (S_msrWords words);
    
    // slurs
    void                  appendSlurToNote (S_msrSlur slur);
    
    // ligatures
    void                  appendLigatureToNote (S_msrLigature ligature);
    
    // pedals
    void                  appendPedalToNote (S_msrPedal pedal);
    
    //  slashes
    void                  appendSlashToNote (S_msrSlash slash);

    //  wedges
    void                  appendWedgeToNote (S_msrWedge wedge);

    S_msrWedge            removeFirstWedge (); // JMI

    // eyeglasses
    void                  appendEyeGlassesToNote (S_msrEyeGlasses eyeGlasses);
    
    // damps
    void                  appendDampToNote (S_msrDamp damp);
    
    // damp alls
    void                  appendDampAllToNote (S_msrDampAll dampAll);
    
    // scordaturas
    void                  appendScordaturaToNote (S_msrScordatura scordatura);
    

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    // note kind
    string                noteKindAsString () const;
    
    string                notePitchAsString () const;
    string                noteDisplayPitchKindAsString () const;

    string                noteDisplayOctaveAsString () const;
    
    // note print kind
    string                notePrintKindAsString () const;
    
    // note head
    string                noteHeadKindAsString () const;
    
    string                noteHeadFilledKindAsString () const;
    string                noteHeadParenthesesKindAsString () const;

    string                noteDiatonicPitchKindAsString (
                            int inputLineNumber) const;

    // whole notes
    string                noteSoundingWholeNotesAsMsrString () const;
    string                noteDisplayWholeNotesAsMsrString () const;
    
    // graphic duration
    string                noteGraphicDurationAsMsrString () const;
    string                tupletNoteGraphicDurationAsMsrString ( // JMI
                            int actualNotes, int normalNotes) const;

    // note as string
    string                asString () const;

    string                asShortString () const;
      
    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------


    // uplinks
    // ------------------------------------------------------

    S_msrChord            fNoteChordUplink;

    S_msrGraceNotesGroup  fNoteGraceNotesGroupUplink;
    
    S_msrTuplet           fNoteTupletUplink;

    S_msrMeasure          fNoteMeasureUplink;

    // basic note description
    // ------------------------------------------------------

    // note kind
    msrNoteKind           fNoteKind;

    // note quarter tones pitch
    msrQuarterTonesPitchKind
                          fNoteQuarterTonesPitchKind;

    // whole notes
    rational              fNoteSoundingWholeNotes;
    rational              fNoteDisplayWholeNotes;
    
    int                   fNoteDotsNumber;
    
    msrDurationKind       fNoteGraphicDurationKind;
    
    int                   fNoteOctave;
    
    msrQuarterTonesPitchKind
                          fNoteQuarterTonesDisplayPitchKind;
    int                   fNoteDisplayOctave;
                                // for unpitched notes
                                // and pitched rests

    // short cuts for efficiency
    bool                  fNoteIsARest;
    bool                  fNoteIsUnpitched;
  
    bool                  fNoteIsACueNote;
    bool                  fNoteIsAGraceNote;
    
   // note octave shift
    S_msrOctaveShift      fNoteOctaveShift; // JMI ???

    // note print kind
    msrNotePrintKind      fNotePrintKind;

    // note head
    msrNoteHeadKind       fNoteHeadKind;
    msrNoteHeadFilledKind fNoteHeadFilledKind;
    msrNoteHeadParenthesesKind
                          fNoteHeadParenthesesKind;

    // accidentals
    // ------------------------------------------------------

    msrNoteAccidentalKind fNoteAccidentalKind;
    
    msrNoteEditorialAccidentalKind
                          fNoteEditorialAccidentalKind;     
    msrNoteCautionaryAccidentalKind
                          fNoteCautionaryAccidentalKind;

    // staff and voice context
    // ------------------------------------------------------

    int                   fNoteStaffNumber;
    int                   fNoteVoiceNumber;

    // chord member?
    // ------------------------------------------------------

    bool                  fNoteBelongsToAChord;
    
    // tuplet member?
    // ------------------------------------------------------

    bool                  fNoteBelongsToATuplet;

    // multiple rest member?
    // ------------------------------------------------------

    bool                  fNoteBelongsToAMultipleRest; // JMI
    int                   fNoteMultipleRestSequenceNumber; // JMI

    // note lyrics
    // ------------------------------------------------------

    list<S_msrSyllable>   fNoteSyllables;
    
    // stem
    // ------------------------------------------------------

    S_msrStem             fNoteStem;

    // beams
    // ------------------------------------------------------

    list<S_msrBeam>       fNoteBeams;
                                      
    // articulations
    // ------------------------------------------------------

    list<S_msrArticulation>
                          fNoteArticulations;

    // spanners
    // ------------------------------------------------------

    list<S_msrSpanner>    fNoteSpanners;

    // technicals
    // ------------------------------------------------------

    list<S_msrTechnical>  fNoteTechnicals;
    
    list<S_msrTechnicalWithInteger>
                          fNoteTechnicalWithIntegers;
                          
    list<S_msrTechnicalWithFloat>
                          fNoteTechnicalWithFloats;
                          
    list<S_msrTechnicalWithString>
                          fNoteTechnicalWithStrings;

    // ornaments
    // ------------------------------------------------------

    list<S_msrOrnament>   fNoteOrnaments;
    
    // glissandos
    // ------------------------------------------------------

    list<S_msrGlissando>  fNoteGlissandos;
    
    // slides
    // ------------------------------------------------------

    list<S_msrSlide>      fNoteSlides;
    
    // grace notes
    // ------------------------------------------------------

    S_msrGraceNotesGroup  fNoteGraceNotesGroupBefore;
  // JMI  S_msrAfterGraceNotesGroup
    S_msrGraceNotesGroup
                          fNoteGraceNotesGroupAfter;

    // single tremolo
    // ------------------------------------------------------

    S_msrSingleTremolo    fNoteSingleTremolo;

    // tie
    // ------------------------------------------------------

    S_msrTie              fNoteTie;
    
    // dynamics
    // ------------------------------------------------------

    list<S_msrDynamics>   fNoteDynamics;
    list<S_msrOtherDynamics>
                          fNoteOtherDynamics;
                          
    // slashes
    // ------------------------------------------------------

    list<S_msrSlash>      fNoteSlashes;

    // wedges
    // ------------------------------------------------------

    list<S_msrWedge>      fNoteWedges;

    // eyeglasses
    // ------------------------------------------------------

    list<S_msrEyeGlasses> fNoteEyeGlasses;
    
    // damps
    // ------------------------------------------------------

    list<S_msrDamp>       fNoteDamps;
    
    // damp alls
    // ------------------------------------------------------

    list<S_msrDampAll>    fNoteDampAlls;
    
    // scordaturas
    // ------------------------------------------------------

    list<S_msrScordatura> fNoteScordaturas;

    // words
    // ------------------------------------------------------

    list<S_msrWords>      fNoteWords;

    // slurs
    // ------------------------------------------------------

    list<S_msrSlur>       fNoteSlurs;

    // ligatures
    // ------------------------------------------------------

    list<S_msrLigature>   fNoteLigatures;

    // pedals
    // ------------------------------------------------------

    list<S_msrPedal>      fNotePedals;

    // harmony
    // ------------------------------------------------------

    S_msrHarmony          fNoteHarmony;

    // frame
    // ------------------------------------------------------

    S_msrFrame            fNoteFrame;

    // figured bass
    // ------------------------------------------------------

    S_msrFiguredBass      fNoteFiguredBass;

    // note measure information
    // ------------------------------------------------------

    string                fNoteMeasureNumber;
    rational              fNotePositionInMeasure;
    
    bool                  fNoteOccupiesAFullMeasure;
    
    // note redundant information (for speed)
    // ------------------------------------------------------

    // this is needed to handle stemless notes,
    // because the <stem> is visited after 'visitorStart ( S_msrNote )' 
    bool                  fNoteIsStemless;

    // this is to distinguish the first chord member note
    // from the other ones, since only it should get the slur if any
    bool                  fNoteIsAChordsFirstMemberNote;

    // this is needed to handle double tremolos
    bool                  fNoteIsFirstNoteInADoubleTremolo;
    bool                  fNoteIsSecondNoteInADoubleTremolo;

    // this is useful
    // as well as to produce a nice \aftergrace in LilyPond 
    S_msrOrnament         fNoteTrillOrnament;
    bool                  fNoteIsFollowedByGraceNotesGroup;

    // this is useful to produce a text spanner in LilyPond
    S_msrOrnament         fNoteDashesOrnament;

    // this is needed to produce a delayed turn/inverted-turn in LilyPond 
    S_msrOrnament         fNoteDelayedTurnOrnament;
    S_msrOrnament         fNoteDelayedInvertedTurnOrnament;

    // this is useful to  to combine a trill and a wavy line in LilyPond
    S_msrSpanner          fNoteWavyLineSpannerStart;
    S_msrSpanner          fNoteWavyLineSpannerStop;
};
typedef SMARTP<msrNote> S_msrNote;
EXP ostream& operator<< (ostream& os, const S_msrNote& elt);

//______________________________________________________________________________
class msrChord : public msrElement
{
  public:

    static SMARTP<msrChord> create (
      int             inputLineNumber,
      rational        chordSoundingWholeNotes,
      rational        chordDisplayWholeNotes,
      msrDurationKind chordGraphicDurationKind);

    // creation from MusicXML
    // ------------------------------------------------------

    SMARTP<msrChord> createChordNewbornClone (
      S_msrPart containingPart);

    SMARTP<msrChord> createChordDeepCopy (
      S_msrPart containingPart);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrChord (
      int             inputLineNumber,
      rational        chordSoundingWholeNotes,
      rational        chordDisplayWholeNotes,
      msrDurationKind chordGraphicDurationKind);
      
    virtual ~msrChord ();
  
  public:

    // set and get
    // ------------------------------------------------------
                              
     // whole notes
    void                  setChordSoundingWholeNotes (
                            rational divisions);
            
    rational              getChordSoundingWholeNotes () const
                              { return fChordSoundingWholeNotes; }
                        
    void                  setChordDisplayWholeNotes (
                            rational divisions);
            
    rational              getChordDisplayWholeNotes () const
                              { return fChordDisplayWholeNotes; }
                        
    // graphic duration
    msrDurationKind       getChordGraphicDurationKind () const
                              { return fChordGraphicDurationKind; }
            
    // chord notes
    const vector<S_msrNote>&
                          getChordNotesVector () const
                              { return fChordNotesVector; }

    // stems
    const list<S_msrStem>&
                          getChordStems () const
                              { return fChordStems; }

    // beams
    const list<S_msrBeam>&
                          getChordBeams () const
                              { return fChordBeams; }

    // articulations
    const list<S_msrArticulation>&
                          getChordArticulations () const
                              { return fChordArticulations; }

    // spanners
    const list<S_msrSpanner>&
                          getChordSpanners () const
                              { return fChordSpanners; }

    // technicals
    const list<S_msrTechnical>&
                          getChordTechnicals () const
                              { return fChordTechnicals; }
                              
    const list<S_msrTechnicalWithInteger>&
                          getChordTechnicalWithIntegers () const
                              { return fChordTechnicalWithIntegers; }
    
    const list<S_msrTechnicalWithFloat>&
                          getChordTechnicalWithFloats () const
                              { return fChordTechnicalWithFloats; }
    
    const list<S_msrTechnicalWithString>&
                          getChordTechnicalWithStrings () const
                              { return fChordTechnicalWithStrings; }
    
    // ornaments
    const list<S_msrOrnament>&
                          getChordOrnaments () const
                              { return fChordOrnaments; }
    
    // glissandos
    const list<S_msrGlissando>&
                          getChordGlissandos () const
                              { return fChordGlissandos; }
    
    // slides
    const list<S_msrSlide>&
                          getChordSlides () const
                              { return fChordSlides; }
    
    // singleTremolo
    void                  setChordSingleTremolo (
                            S_msrSingleTremolo trem);
    
    S_msrSingleTremolo    getChordSingleTremolo () const
                              { return fChordSingleTremolo; }

    // dynamics
    const list<S_msrDynamics>&
                          getChordDynamics () const
                              { return fChordDynamics; }
    const list<S_msrOtherDynamics>&
                          getChordOtherDynamics () const
                              { return fChordOtherDynamics; }
                      
    const list<S_msrWedge>&
                          getChordWedges () const
                              { return fChordWedges; }

    // words
    const list<S_msrWords>&
                          getChordWords () const
                              { return fChordWords; }
                      
    // ties
    const list<S_msrTie>&
                          getChordTies () const
                              { return fChordTies; }
                      
    // slurs
    const list<S_msrSlur>&
                          getChordSlurs () const
                              { return fChordSlurs; }
                      
    // ligatures
    const list<S_msrLigature>&
                          getChordLigatures () const
                              { return fChordLigatures; }
                      
    // pedals
    const list<S_msrPedal>&
                          getChordPedals () const
                              { return fChordPedals; }
                      
    // double tremolo
    void                  setChordIsFirstChordInADoubleTremolo ()
                              { fChordIsFirstChordInADoubleTremolo = true; }

    bool                  getChordIsFirstChordInADoubleTremolo () const
                              { return fChordIsFirstChordInADoubleTremolo; }
                  
    void                  setChordIsSecondChordInADoubleTremolo ()
                              { fChordIsSecondChordInADoubleTremolo = true; }

    bool                  getChordIsSecondChordInADoubleTremolo () const
                              { return fChordIsSecondChordInADoubleTremolo; }
                  
    // tie
    void                  setChordTie (
                            const S_msrTie tie)
                              { fChordTie = tie; }

    S_msrTie              getChordTie () const
                              { return fChordTie; }

    // grace notes
    void                  setChordGraceNotesGroupBefore (
                            S_msrGraceNotesGroup graceNotesGroupBefore)
                              {
                                fChordGraceNotesGroupBefore =
                                  graceNotesGroupBefore;
                              }

    S_msrGraceNotesGroup  getChordGraceNotesGroupBefore () const
                              { return fChordGraceNotesGroupBefore; }

    void                  setChordGraceNotesGroupAfter (
                            S_msrGraceNotesGroup graceNotesGroupAfter)
                              {
                                fChordGraceNotesGroupAfter =
                                  graceNotesGroupAfter;
                              }

    S_msrGraceNotesGroup  getChordGraceNotesGroupAfter () const
                              { return fChordGraceNotesGroupAfter; }


    // harmony
    void                  setChordHarmony (
                            S_msrHarmony harmony)
                              { fChordHarmony = harmony; }
                      
    // figured bass
    void                  setChordFiguredBass (
                            S_msrFiguredBass figuredBass)
                              { fChordFiguredBass = figuredBass; }
                      
    const S_msrFiguredBass&
                          getChordFiguredBass () const
                              { return fChordFiguredBass; };
                      
    // measure uplink
    void                  setChordMeasureUplink (
                            const S_msrMeasure& measure)
                              { fChordMeasureUplink = measure; }
                      
    S_msrMeasure          getChordMeasureUplink () const
                            { return fChordMeasureUplink; }

    // measure number
    void                  setChordMeasureNumber (
                            string measureNumber)
                              { fChordMeasureNumber = measureNumber; }
    
    string                getChordMeasureNumber () const
                              { return fChordMeasureNumber; }
 
    // position in measure
    void                  setChordPositionInMeasure (
                            rational position)
                              {
                                fChordPositionInMeasure =
                                  position;
                              }

    rational              getChordPositionInMeasure () const
                              { return fChordPositionInMeasure; }
                         
    // services
    // ------------------------------------------------------

    // notes
    void                  addFirstNoteToChord (
                            S_msrNote  note,
                            S_msrVoice voice);
                            
    void                  addAnotherNoteToChord (
                            S_msrNote note,
                            S_msrVoice voice);

    S_msrNote             fetchChordFirstNonGraceNote () const;
    
    // articulations
    void                  appendArticulationToChord (S_msrArticulation art);
     
    // spanners
    void                  appendSpannerToChord (S_msrSpanner span);
     
    // technicals
    void                  appendTechnicalToChord (S_msrTechnical tech);
    
    void                  appendTechnicalWithIntegerToChord (
                            S_msrTechnicalWithInteger tech);
    
    void                  appendTechnicalWithFloatToChord (
                            S_msrTechnicalWithFloat tech);
    
    void                  appendTechnicalWithStringToChord (
                            S_msrTechnicalWithString tech);
    
    // ornaments
    void                  appendOrnamentToChord (S_msrOrnament orn);
     
    // glissandos
    void                  appendGlissandoToChord (S_msrGlissando gliss);
     
    // slides
    void                  appendSlideToChord (S_msrSlide slide);
     
    // dynamics
    void                  appendDynamicsToChord (S_msrDynamics dynamic)
                              {
                                fChordDynamics.push_back (dynamic);
                              }
    void                  appendOtherDynamicsToChord (
                            S_msrOtherDynamics otherDynamic)
                              {
                                fChordOtherDynamics.push_back (otherDynamic);
                              }

    // slashes
    void                  appendSlashToChord (S_msrSlash slash)
                              {
                                fChordSlashes.push_back (slash);
                              }

    // wedges
    void                  appendWedgeToChord (S_msrWedge wedge)
                              {
                                fChordWedges.push_back (wedge);
                              }
                                        
    // words
    void                  appendWordsToChord (S_msrWords dynamic)
                              {
                                fChordWords.push_back (dynamic);
                              }
                    
    // ties
    void                  appendTieToChord (S_msrTie tie)
                              {
                                fChordTies.push_back (tie);
                              }
                      
    // slurs
    void                  appendSlurToChord (S_msrSlur slur)
                              {
                                fChordSlurs.push_back (slur);
                              }
                      
    // stems
    void                  appendStemToChord (S_msrStem stem)
                              {
                                fChordStems.push_back (stem);
                              }

    // beams
    void                  appendBeamToChord (S_msrBeam beam);

    // ligatures
    void                  appendLigatureToChord (S_msrLigature ligature)
                              {
                                fChordLigatures.push_back (ligature);
                              }
                      
    // pedals
    void                  appendPedalToChord (S_msrPedal pedal)
                              {
                                fChordPedals.push_back (pedal);
                              }
                      
    // tuplet members
    /* JMI
    void                  applyTupletMemberDisplayedFactorToChordMembers (
                            int actualNotes, int normalNotes);
*/

    // position in measure
    void                  setChordFirstNotePositionInMeasure (
                            rational position);
                    
  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    // as MSR string
    string                chordSoundingWholeNotesAsMsrString ();
    string                chordDisplayWholeNotesAsMsrString ();
 // JMI   string                chordGraphicDurationAsMsrString () const;

    string                asStringwithRawDivisions () const;

    string                asString () const;
    
    string                asShortString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // sounding divisions
    rational              fChordSoundingWholeNotes;
    
    // display divisions
    rational              fChordDisplayWholeNotes;
                                  
    // graphic duration is needed for grace notes,
    // since they don't have any note (sounding) duration
    msrDurationKind       fChordGraphicDurationKind;
    
    vector<S_msrNote>     fChordNotesVector;

    // position in measure
    S_msrMeasure          fChordMeasureUplink;

    string                fChordMeasureNumber;
    rational              fChordPositionInMeasure;

    // stems
    list<S_msrStem>       fChordStems;
    
    // beams
    list<S_msrBeam>       fChordBeams;
    
    // articulations
    list<S_msrArticulation>
                          fChordArticulations;

    // spanners
    list<S_msrSpanner>    fChordSpanners;

    // single tremolo
    S_msrSingleTremolo    fChordSingleTremolo;
    
    // double tremolo
    bool                  fChordIsFirstChordInADoubleTremolo;
    bool                  fChordIsSecondChordInADoubleTremolo;

    // technicals
    list<S_msrTechnical>  fChordTechnicals;
    
    list<S_msrTechnicalWithInteger>
                          fChordTechnicalWithIntegers;
    
    list<S_msrTechnicalWithFloat>
                          fChordTechnicalWithFloats;
    
    list<S_msrTechnicalWithString>
                          fChordTechnicalWithStrings;
    
    // ornaments
    list<S_msrOrnament>   fChordOrnaments;

    // glissandos
    list<S_msrGlissando>  fChordGlissandos;

    // slides
    list<S_msrSlide>      fChordSlides;
    
    // ties
    S_msrTie              fChordTie;
    
    // dynamics
    list<S_msrDynamics>   fChordDynamics;
    list<S_msrOtherDynamics>
                          fChordOtherDynamics;

    // slashes
    list<S_msrSlash>      fChordSlashes;
    
    // wedges
    list<S_msrWedge>      fChordWedges;
    
    // words
    list<S_msrWords>      fChordWords;
    
    // ties
    list<S_msrTie>        fChordTies;

    // slurs
    list<S_msrSlur>       fChordSlurs;

    // ligatures
    list<S_msrLigature>   fChordLigatures;

    // pedals
    list<S_msrPedal>      fChordPedals;

    // grace notes
    S_msrGraceNotesGroup  fChordGraceNotesGroupBefore;
    S_msrGraceNotesGroup  fChordGraceNotesGroupAfter;

    // harmony
    S_msrHarmony          fChordHarmony;

    // figured bass
    S_msrFiguredBass      fChordFiguredBass;
};
typedef SMARTP<msrChord> S_msrChord;
EXP ostream& operator<< (ostream& os, const S_msrChord& elt);

//______________________________________________________________________________
class msrTuplet : public msrElement
{
  public:
    
    // data types
    // ------------------------------------------------------

    enum msrTupletTypeKind {
      kTupletTypeNone,
      kTupletTypeStart, kTupletTypeContinue, kTupletTypeStop,
      kTupletTypeStartAndStopInARow };

    static string tupletTypeKindAsString (
      msrTupletTypeKind tupletTypeKind);
      
    enum msrTupletBracketKind {
      kTupletBracketYes, kTupletBracketNo};
      
    static string tupletBracketKindAsString (
      msrTupletBracketKind tupletBracketKind);
      
    enum msrTupletLineShapeKind {
      kTupletLineShapeStraight, kTupletLineShapeCurved};
      
    static string tupletLineShapeKindAsString (
      msrTupletLineShapeKind tupletLineShapeKind);
      
    enum msrTupletShowNumberKind {
      kTupletShowNumberActual, kTupletShowNumberBoth, kTupletShowNumberNone };

    static string tupletShowNumberKindAsString (
      msrTupletShowNumberKind tupletShowNumberKind);
      
    enum msrTupletShowTypeKind {
      kTupletShowTypeActual, kTupletShowTypeBoth, kTupletShowTypeNone };

    static string tupletShowTypeKindAsString (
      msrTupletShowTypeKind tupletShowTypeKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrTuplet> create (
      int                     inputLineNumber,
      string                  tupletMeasureNumber,
      int                     tupletNumber,
      msrTupletBracketKind    tupletBracketKind,
      msrTupletLineShapeKind  tupletLineShapeKind,
      msrTupletShowNumberKind tupletShowNumberKind,
      msrTupletShowTypeKind   tupletShowTypeKind,
      msrTupletFactor         tupletFactor,
      /*
      int                     tupletActualNotes,
      int                     tupletNormalNotes,
      */
      rational                memberNotesSoundingWholeNotes,
      rational                memberNotesDisplayWholeNotes,
      rational                notePositionInMeasure); // JMI

    SMARTP<msrTuplet> createTupletNewbornClone ();

    SMARTP<msrTuplet> createTupletDeepCopy ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrTuplet (
      int                     inputLineNumber,
      string                  tupletMeasureNumber,
      int                     tupletNumber,
      msrTupletBracketKind    tupletBracketKind,
      msrTupletLineShapeKind  tupletLineShapeKind,
      msrTupletShowNumberKind tupletShowNumberKind,
      msrTupletShowTypeKind   tupletShowTypeKind,
      msrTupletFactor         tupletFactor,
      /*
      int                     tupletActualNotes,
      int                     tupletNormalNotes,
      */
      rational                memberNotesSoundingWholeNotes,
      rational                memberNotesDisplayWholeNotes,
      rational                notePositionInMeasure);
      
    virtual ~msrTuplet ();
  
  public:

    // set and get
    // ------------------------------------------------------
    
    int                   getTupletNumber () const
                              { return fTupletNumber; }

    msrTupletBracketKind  getTupletBracketKind () const
                              { return fTupletBracketKind; }

    msrTupletLineShapeKind
                          getTupletLineShapeKind () const
                              { return fTupletLineShapeKind; }

    msrTupletShowNumberKind
                          getTupletShowNumberKind () const
                              { return fTupletShowNumberKind; }

    msrTupletShowTypeKind getTupletShowTypeKind () const
                              { return fTupletShowTypeKind; }

    const msrTupletFactor&
                          getTupletFactor ()
                              { return fTupletFactor; }

/* JMI
    int                   getTupletActualNotes () const
                              { return fTupletActualNotes; }
    int                   getTupletNormalNotes () const
                              { return fTupletNormalNotes; }
    */
    
    rational              getMemberNotesSoundingWholeNotes () const
                              { return fMemberNotesSoundingWholeNotes; }
    rational              getMemberNotesDisplayWholeNotes () const
                              { return fMemberNotesDisplayWholeNotes; }

    const list<S_msrElement>&
                          getTupletElementsList () const
                              { return fTupletElementsList; }

    rational              getTupletSoundingWholeNotes () const
                              { return fTupletSoundingWholeNotes; }
                              
    rational              getTupletDisplayWholeNotes () const
                              { return fTupletDisplayWholeNotes; }
            
    // measure uplink
    void                  setTupletMeasureUplink (
                            const S_msrMeasure& measure)
                              { fTupletMeasureUplink = measure; }
                      
    S_msrMeasure          getTupletMeasureUplink () const
                              { return fTupletMeasureUplink; }

    // measure number
    void                  setTupletMeasureNumber (string measureNumber);

    string                getTupletMeasureNumber () const
                              { return fTupletMeasureNumber; }
 
    // position in measure
    rational              setTupletPositionInMeasure (
                            rational position);
                              // returns the position after the tuplet

    rational              getTupletPositionInMeasure () const
                              { return fTupletPositionInMeasure; }
                      
    // services
    // ------------------------------------------------------

    void                  addNoteToTuplet (
                            S_msrNote  note,
                            S_msrVoice voice);
                          
    void                  addChordToTuplet (S_msrChord chord);
    
    void                  addTupletToTuplet (S_msrTuplet tuplet);
    
    void                  addTupletToTupletClone (S_msrTuplet tuplet);
    
    S_msrNote             fetchTupletFirstNonGraceNote () const;

    S_msrNote             removeFirstNoteFromTuplet ( // JMI
                            int       inputLineNumber);
    S_msrNote             removeLastNoteFromTuplet (
                            int       inputLineNumber);

 // JMI   void                  applyDisplayFactorToTupletMembers ();
    
    void                  unapplySoundingFactorToTupletMembers (
                            const msrTupletFactor& containingTupletFactor);
                            /*
                            int containingTupletActualNotes,
                            int containingTupletNormalNotes);
                            */

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

    virtual void          printShort (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // uplink
    S_msrMeasure          fTupletMeasureUplink;
    
    int                   fTupletNumber;
    
    msrTupletBracketKind  fTupletBracketKind;
              
    msrTupletLineShapeKind
                          fTupletLineShapeKind;
              
    msrTupletShowNumberKind
                          fTupletShowNumberKind;
              
    msrTupletShowTypeKind fTupletShowTypeKind;

    msrTupletFactor       fTupletFactor;
//    int                   fTupletActualNotes;
 //   int                   fTupletNormalNotes;

    rational              fMemberNotesSoundingWholeNotes;
    rational              fMemberNotesDisplayWholeNotes;

    rational              fTupletSoundingWholeNotes;
    rational              fTupletDisplayWholeNotes;

    string                fTupletMeasureNumber;
    rational              fTupletPositionInMeasure;
    
    list<S_msrElement>    fTupletElementsList;
};
typedef SMARTP<msrTuplet> S_msrTuplet;
EXP ostream& operator<< (ostream& os, const S_msrTuplet& elt);

//______________________________________________________________________________
class msrStanza : public msrElement
{
  public:

    // constants
    // ------------------------------------------------------
    
    #define K_NO_STANZA_NUMBER "-1"
    #define K_NO_STANZA_NAME   "Unknown stanza"
  
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStanza> create (
      int           inputLineNumber,
      string        stanzaNumber,
      S_msrVoice    stanzaVoiceUplink);
    
    SMARTP<msrStanza> createStanzaNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrStanza> createStanzaDeepCopy (
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrStanza (
      int           inputLineNumber,
      string        stanzaNumber,
      S_msrVoice    stanzaVoiceUplink);

  public:
  
    virtual ~msrStanza ();
  
  private:

    // initialization
    // ------------------------------------------------------

    void                  initializeStanza ();

  public:

    // set and get
    // ------------------------------------------------------
                              
    // number
    string                getStanzaNumber () const
                              { return fStanzaNumber; }
                
    // name
    string                getStanzaName () const
                              { return fStanzaName; }
                         
    // contents
    const vector<S_msrSyllable>&
                          getSyllables () const
                              { return fSyllables; }

    void                  setStanzaTextPresent ()
                              { fStanzaTextPresent = true; }

    bool                  getStanzaTextPresent () const
                              { return fStanzaTextPresent; }

    // uplinks
    S_msrVoice            getStanzaVoiceUplink () const
                              { return fStanzaVoiceUplink; }

    // services
    // ------------------------------------------------------

    void                  appendSyllableToStanza (
                            S_msrSyllable syllable);
      
    S_msrSyllable         appendRestSyllableToStanza (
                            int      inputLineNumber,
                            rational wholeNotes);

    S_msrSyllable         appendSkipSyllableToStanza (
                            int      inputLineNumber,
                            rational wholeNotes);

    S_msrSyllable         appendMeasureEndSyllableToStanza (
                            int inputLineNumber);

    S_msrSyllable         appendMelismaSyllableToStanza (
                            int             inputLineNumber,
                            msrSyllable::msrSyllableKind
                                            syllableKind,
                            rational        wholeNote);

    S_msrSyllable         appendLineBreakSyllableToStanza (
                            int inputLineNumber);

/* JMI
    S_msrSyllable         appendTiedSyllableToStanza (
                            int      inputLineNumber,
                            rational wholeNotes);

    S_msrSyllable         appendSlurSyllableToStanza (
                            int      inputLineNumber,
                            rational wholeNotes);

    S_msrSyllable         appendSlurBeyondEndSyllableToStanza (
                            int      inputLineNumber,
                            rational wholeNotes);

    S_msrSyllable         appendLigatureSyllableToStanza (
                            int      inputLineNumber,
                            rational wholeNotes);

    S_msrSyllable         appendLigatureBeyondEndSyllableToStanza (
                            int      inputLineNumber,
                            rational wholeNotes);

    S_msrSyllable         appendBarNumberCheckSyllableToStanza (
                            int    inputLineNumber,
                            string nextMeasureNumber);

    S_msrSyllable         appendBarcheckSyllableToStanza (
                            int    inputLineNumber,
                            string nextMeasureNumber);
*/

    S_msrSyllable         appendLineBreakSyllableToStanza ( // JMI ???
                            int    inputLineNumber,
                            string nextMeasureNumber);
                
    S_msrSyllable         appendPageBreakSyllableToStanza ( // JMI ???
                            int    inputLineNumber,
                            string nextMeasureNumber);
                
    void                  padUpToMeasureLengthInStanza ( // JMI
                            int      inputLineNumber,
                            rational measureLength);
  
    void                  appendPaddingNoteToStanza (
                            int inputLineNumber,
                            int divisions,
                            int divisionsPerQuarterNote);
  
  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // uplinks
    S_msrVoice            fStanzaVoiceUplink;

    // number
    // The lyric number indicates multiple lines,
    // though a name can be used as well (as in Finale's verse/chorus/section specification)
    string                fStanzaNumber;

    // name
    string                fStanzaName;

    // contents
    vector<S_msrSyllable> fSyllables;

    bool                  fStanzaTextPresent;

    // current measure length
    rational              fStanzaCurrentMeasureLength;
};
typedef SMARTP<msrStanza> S_msrStanza;
EXP ostream& operator<< (ostream& os, const S_msrStanza& elt);

//______________________________________________________________________________
class msrRepeatCommonPart : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeatCommonPart> create (
      int          inputLineNumber,
      S_msrRepeat  repeatUplink);

    /* JMI
    SMARTP<msrRepeatCommonPart> createRepeatCommonPartNewbornClone (
      S_msrRepeat containingRepeat);

    SMARTP<msrRepeatCommonPart> createRepeatCommonPartDeepCopy (
      S_msrRepeat containingRepeat);
      */

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRepeatCommonPart (
      int          inputLineNumber,
      S_msrRepeat  repeatUplink);
      
    virtual ~msrRepeatCommonPart ();
  
  public:

    // set and get
    // ------------------------------------------------------

    // uplinks
    S_msrRepeat           getRepeatCommonPartRepeatUplink () const
                              { return fRepeatCommonPartRepeatUplink; }

    // elements
    const list<S_msrElement>&
                          getRepeatCommonPartElementsList ()
                              { return fRepeatCommonPartElementsList; }
                
    // services
    // ------------------------------------------------------
  
    void                  appendElementToRepeatCommonPart (
                            S_msrElement elem);

    S_msrNote             fetchRepeatCommonPartFirstNonGraceNote () const;

    void                  collectRepeatCommonPartMeasuresIntoFlatList (
                            int inputLineNumber);
    
  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;
                    
    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // uplinks
    S_msrRepeat           fRepeatCommonPartRepeatUplink;

    // elements list
    list<S_msrElement>    fRepeatCommonPartElementsList;
};
typedef SMARTP<msrRepeatCommonPart> S_msrRepeatCommonPart;
EXP ostream& operator<< (ostream& os, const S_msrRepeatCommonPart& elt);

//______________________________________________________________________________
class msrRepeatEnding : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrRepeatEndingKind {
      kHookedEnding,
      kHooklessEnding};

    static string repeatEndingKindAsString (
      msrRepeatEndingKind repeatEndingKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeatEnding> create (
      int                 inputLineNumber,
      string              repeatEndingNumber, // may be "1, 2"
      msrRepeatEndingKind repeatEndingKind,
      S_msrSegment        segment,
      S_msrRepeat         repeatUplink);
    
    /* JMI
    SMARTP<msrRepeatEnding> createRepeatEndingNewbornClone (
      S_msrRepeat containingRepeat);

    SMARTP<msrRepeatEnding> createRepeatEndingDeepCopy (
      S_msrRepeat containingRepeat);
      */

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRepeatEnding (
      int                 inputLineNumber,
      string              repeatEndingNumber, // may be "1, 2"
      msrRepeatEndingKind repeatEndingKind,
      S_msrSegment        segment,
      S_msrRepeat         repeatUplink);
      
    virtual ~msrRepeatEnding ();
  
  public:

    // set and get
    // ------------------------------------------------------

    // numbers
    string                getRepeatEndingNumber () const
                              { return fRepeatEndingNumber; }
                
    void                  setRepeatEndingNumber (int repeatEndingNumber)
                              { fRepeatEndingNumber = repeatEndingNumber; }

    void                  setRepeatEndingInternalNumber (
                            int repeatEndingInternalNumber)
                              {
                                fRepeatEndingInternalNumber =
                                  repeatEndingInternalNumber;
                              }
                      
    int                   getRepeatEndingInternalNumber () const
                              { return fRepeatEndingInternalNumber; }
                
    // kind
    msrRepeatEndingKind   getRepeatEndingKind () const
                              { return fRepeatEndingKind; }
                
    // segment
    S_msrSegment          getRepeatEndingSegment () const
                              { return fRepeatEndingSegment; }
                
    // uplinks
    S_msrRepeat           getRepeatEndingRepeatUplink () const
                              { return fRepeatEndingRepeatUplink; }

    // services
    // ------------------------------------------------------
  
 //  JMI void                  appendElementToRepeatEnding (S_msrElement elem);
                    
    void                  collectRepeatEndingMeasuresIntoFlatList (
                            int inputLineNumber);
    
  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // uplinks
    S_msrRepeat           fRepeatEndingRepeatUplink;

    // numbers
    string                fRepeatEndingNumber; // may be "1, 2"
    int                   fRepeatEndingInternalNumber; // internally assigned

    // kind
    msrRepeatEndingKind   fRepeatEndingKind;

    // segment
    S_msrSegment          fRepeatEndingSegment;
};
typedef SMARTP<msrRepeatEnding> S_msrRepeatEnding;
EXP ostream& operator<< (ostream& os, const S_msrRepeatEnding& elt);

//______________________________________________________________________________
class msrRepeat : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeat> create (
      int        inputLineNumber,
      int        repeatTimes,
      S_msrVoice voiceUplink);

    /* JMI
    SMARTP<msrRepeat> createRepeatNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrRepeat> createRepeatDeepCopy (
      S_msrVoice containingVoice);
      */

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRepeat (
      int        inputLineNumber,
      int        repeatTimes,
      S_msrVoice voiceUplink);
      
    virtual ~msrRepeat ();
  
  public:

    // set and get
    // ------------------------------------------------------

    // times

    
    int                   getRepeatTimes () const
                              { return fRepeatTimes; }

    // common part
    void                  setRepeatCommonPart (
                            S_msrRepeatCommonPart repeatCommonPart);
                  
    S_msrRepeatCommonPart getRepeatCommonPart () const
                              { return fRepeatCommonPart; }
/*
    // common segment
    void                  setRepeatCommonSegment (
                            S_msrSegment repeatCommonSegment);
                  
    S_msrSegment          getRepeatCommonSegment () const
                              { return fRepeatCommonSegment; }
*/
    // endings
    const vector<S_msrRepeatEnding>&
                          getRepeatEndings () const
                              { return fRepeatEndings; }

    S_msrVoice            getRepeatVoiceUplink () const
                              { return fRepeatVoiceUplink; }

    // services
    // ------------------------------------------------------

    void                  addRepeatEnding (
                            S_msrRepeatEnding repeatEnding);

    S_msrNote             fetchRepeatFirstNonGraceNote () const;

    void                  collectRepeatMeasuresIntoFlatList (
                            int inputLineNumber);
    
  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;
    
    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    int                   fRepeatTimes;
    // common part
    S_msrRepeatCommonPart fRepeatCommonPart;

    // repeat endings
    vector<S_msrRepeatEnding>
                          fRepeatEndings;
    int                   fRepeatEndingsInternalCounter;

    // uplinks
    S_msrVoice            fRepeatVoiceUplink;
};
typedef SMARTP<msrRepeat> S_msrRepeat;
EXP ostream& operator<< (ostream& os, const S_msrRepeat& elt);

//______________________________________________________________________________
class msrMeasuresRepeatPattern : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasuresRepeatPattern> create (
      int                 inputLineNumber,
      S_msrMeasuresRepeat measuresRepeatUplink);
    
    /* JMI
    SMARTP<msrMeasuresRepeatPattern> createMeasuresRepeatPatternNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrMeasuresRepeatPattern> createMeasuresRepeatPatternDeepCopy (
      S_msrVoice containingVoice);
*/

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrMeasuresRepeatPattern (
      int                 inputLineNumber,
      S_msrMeasuresRepeat measuresRepeatUplink);
      
    virtual ~msrMeasuresRepeatPattern ();
  
  public:

    // set and get
    // ------------------------------------------------------

    // segment
    void                  setMeasuresRepeatPatternSegment (
                            S_msrSegment measuresRepeatPatternSegment);
                              
    S_msrSegment          getMeasuresRepeatPatternSegment () const
                              { return fMeasuresRepeatPatternSegment; }

    // uplinks
    S_msrMeasuresRepeat   getMeasuresRepeatUplink () const
                            { return fMeasuresRepeatUplink; }

    // services
    // ------------------------------------------------------

    int                   measuresRepeatPatternMeasuresNumber () const;
        
    void                  collectRepeatPatternMeasuresIntoFlatList (
                            int inputLineNumber);
    
  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------
    
    // uplinks
    S_msrMeasuresRepeat   fMeasuresRepeatUplink;

    // segment
    S_msrSegment          fMeasuresRepeatPatternSegment;
};
typedef SMARTP<msrMeasuresRepeatPattern> S_msrMeasuresRepeatPattern;
EXP ostream& operator<< (ostream& os, const S_msrMeasuresRepeatPattern& elt);

//______________________________________________________________________________
class msrMeasuresRepeatReplicas : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasuresRepeatReplicas> create (
      int                 inputLineNumber,
      S_msrMeasuresRepeat measuresRepeatUplink);
    
    /* JMI
    SMARTP<msrMeasuresRepeatReplicas> createMeasuresRepeatReplicasNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrMeasuresRepeatReplicas> createMeasuresRepeatReplicasDeepCopy (
      S_msrVoice containingVoice);
      */

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrMeasuresRepeatReplicas (
      int                 inputLineNumber,
      S_msrMeasuresRepeat measuresRepeatUplink);
      
    virtual ~msrMeasuresRepeatReplicas ();
  
  public:

    // set and get
    // ------------------------------------------------------

    // uplinks
    S_msrMeasuresRepeat   getMeasuresRepeatUplink () const
                            { return fMeasuresRepeatUplink; }
    // segment
    void                  setMeasuresRepeatReplicasSegment (
                            S_msrSegment measuresRepeatReplicasSegment);

    S_msrSegment          getMeasuresRepeatReplicasSegment () const
                              { return fMeasuresRepeatReplicasSegment; }

    // services
    // ------------------------------------------------------

    int                   measuresRepeatReplicasMeasuresNumber () const;
    
    int                   measuresRepeatReplicasNumber () const;

    void                  collectRepeatReplicasMeasuresIntoFlatList (
                            int inputLineNumber);
    
  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

  private:
      
    // fields
    // ------------------------------------------------------

    // uplinks
    S_msrMeasuresRepeat   fMeasuresRepeatUplink;

    // segment
    S_msrSegment          fMeasuresRepeatReplicasSegment;
};
typedef SMARTP<msrMeasuresRepeatReplicas> S_msrMeasuresRepeatReplicas;
EXP ostream& operator<< (ostream& os, const S_msrMeasuresRepeatReplicas& elt);

//______________________________________________________________________________
class msrMeasuresRepeat : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrMeasuresRepeatKind {
      k_NoMeasuresRepeat,
      kStartMeasuresRepeat, kStopMeasuresRepeat };

    static string measuresRepeatKindAsString (
      msrMeasuresRepeatKind measuresRepeatKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMeasuresRepeat> create (
      int        inputLineNumber,
      int        measuresRepeatMeasuresNumber,
      int        measuresRepeatSlashesNumber,
      S_msrVoice voiceUplink);
    
    SMARTP<msrMeasuresRepeat> createMeasuresRepeatNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrMeasuresRepeat> createMeasuresRepeatDeepCopy (
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrMeasuresRepeat (
      int        inputLineNumber,
      int        measuresRepeatMeasuresNumber,
      int        measuresRepeatSlashesNumber,
      S_msrVoice voiceUplink);
      
    virtual ~msrMeasuresRepeat ();
  
  public:

    // set and get
    // ------------------------------------------------------

    // numbers
    int                   getMeasuresRepeatMeasuresNumber () const
                              { return fMeasuresRepeatMeasuresNumber; }
                              
    int                   getMeasuresRepeatSlashesNumber () const
                              { return fMeasuresRepeatSlashesNumber; }
                              
    // repeat pattern
    void                  setMeasuresRepeatPattern (
                            S_msrMeasuresRepeatPattern
                              measuresRepeatPattern);
                  
    S_msrMeasuresRepeatPattern
                          getMeasuresRepeatPattern () const
                              { return fMeasuresRepeatPattern; }

    // repeat replicas
    void                  setMeasuresRepeatReplicas (
                            S_msrMeasuresRepeatReplicas
                              measuresRepeatReplicas);
                  
    S_msrMeasuresRepeatReplicas
                          getMeasuresRepeatReplicas () const
                              { return fMeasuresRepeatReplicas; }

    // uplinks
    S_msrVoice            getMeasuresRepeatVoiceUplink () const
                            { return fMeasuresRepeatVoiceUplink; }

    // services
    // ------------------------------------------------------

    int                   measuresRepeatPatternMeasuresNumber () const
                            {
                              return
                                fMeasuresRepeatPattern->
                                  measuresRepeatPatternMeasuresNumber ();
                            }
    
    int                   measuresRepeatReplicasMeasuresNumber () const
                            {
                              return
                                fMeasuresRepeatReplicas->
                                  measuresRepeatReplicasMeasuresNumber ();
                            }
    
    int                   measuresRepeatReplicasNumber () const;

    void                  collectMeasuresRepeatMeasuresIntoFlatList (
                            int inputLineNumber);
    
  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // numbers
    int                   fMeasuresRepeatMeasuresNumber;
    int                   fMeasuresRepeatSlashesNumber;

    // repeat pattern
    S_msrMeasuresRepeatPattern
                          fMeasuresRepeatPattern;
    
    // repeat replicas
    S_msrMeasuresRepeatReplicas
                          fMeasuresRepeatReplicas;

    // uplinks
    S_msrVoice            fMeasuresRepeatVoiceUplink;
};
typedef SMARTP<msrMeasuresRepeat> S_msrMeasuresRepeat;
EXP ostream& operator<< (ostream& os, const S_msrMeasuresRepeat& elt);

//______________________________________________________________________________
class msrMultipleRestContents : public msrElement
{
  public:
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMultipleRestContents> create (
      int        inputLineNumber,
      S_msrVoice voiceUplink);
    
    SMARTP<msrMultipleRestContents> createMultipleRestContentsNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrMultipleRestContents> createMultipleRestContentsDeepCopy (
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrMultipleRestContents (
      int        inputLineNumber,
      S_msrVoice voiceUplink);
      
    virtual ~msrMultipleRestContents ();
  
  public:

    // set and get
    // ------------------------------------------------------

    // uplink
    S_msrVoice            getMultipleRestContentsVoiceUplink () const
                            { return fMultipleRestContentsVoiceUplink; }

    void                  setMultipleRestContentsSegment (
                            S_msrSegment multipleRestContentsSegment);

    S_msrSegment          getMultipleRestContentsSegment () const
                              { return fMultipleRestContentsSegment; }

    // services
    // ------------------------------------------------------

    int                   multipleRestContentsMeasuresNumber () const;

    void                  collectMultipleRestContentsMeasuresIntoFlatList (
                            int inputLineNumber);
    
  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;
    
    virtual void          print (ostream& os);

  private:
    
    // fields
    // ------------------------------------------------------

    // uplink
    S_msrVoice            fMultipleRestContentsVoiceUplink;
    
    S_msrSegment          fMultipleRestContentsSegment;
};
typedef SMARTP<msrMultipleRestContents> S_msrMultipleRestContents;
EXP ostream& operator<< (ostream& os, const S_msrMultipleRestContents& elt);

//______________________________________________________________________________
class msrMultipleRest : public msrElement
{
  public:

    // data types
    // ------------------------------------------------------

    enum msrMultipleRestKind {
      kStartMultipleRest, kStopMultipleRest, 
      k_NoMultipleRest };

    static string multipleRestKindAsString (
      msrMultipleRestKind multipleRestKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrMultipleRest> create (
      int        inputLineNumber,
      rational   multipleRestMeasureSoundingNotes,
      int        multipleRestMeasuresNumber,
      S_msrVoice voiceUplink);
    
    SMARTP<msrMultipleRest> createMultipleRestNewbornClone (
      S_msrVoice containingVoice);

    SMARTP<msrMultipleRest> createMultipleRestDeepCopy (
      S_msrVoice containingVoice);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrMultipleRest (
      int        inputLineNumber,
      rational   multipleRestMeasureSoundingNotes,
      int        multipleRestMeasuresNumber,
      S_msrVoice voiceUplink);
      
    virtual ~msrMultipleRest ();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrVoice            getMultipleRestVoiceUplink () const
                            { return fMultipleRestVoiceUplink; }

    int                   getMultipleRestMeasuresNumber () const
                              { return fMultipleRestMeasuresNumber; }
                              
    void                  setMultipleRestContents (
                            S_msrMultipleRestContents multipleRestContents);
                  
    S_msrMultipleRestContents
                          getMultipleRestContents () const
                              { return fMultipleRestContents; }

    void                  setMultipleRestNextMeasureNumber (
                            string measureNumber);

    string                getMultipleRestNextMeasureNumber () const
                              { return fMultipleRestNextMeasureNumber; }

    rational              getMultipleRestMeasureSoundingNotes () const
                              { return fMultipleRestMeasureSoundingNotes; }

    // services
    // ------------------------------------------------------

    int                   multipleRestContentsMeasuresNumber () const
                            {
                              return
                                fMultipleRestContents->
                                  multipleRestContentsMeasuresNumber ();
                            }

    void                  collectMultipleRestMeasuresIntoFlatList (
                            int inputLineNumber);
    
  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;
    
    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    S_msrVoice            fMultipleRestVoiceUplink;

    int                   fMultipleRestMeasuresNumber;
    
    S_msrMultipleRestContents
                          fMultipleRestContents;

    string                fMultipleRestNextMeasureNumber;

    // shortcut for efficiency
    rational              fMultipleRestMeasureSoundingNotes;
};
typedef SMARTP<msrMultipleRest> S_msrMultipleRest;
EXP ostream& operator<< (ostream& os, const S_msrMultipleRest& elt);

//______________________________________________________________________________
class msrRepeatCoda : public msrElement
{
  public:

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrRepeatCoda> create (
      int          inputLineNumber,
      S_msrSegment segment,
      S_msrRepeat  repeatUplink);
    
    SMARTP<msrRepeatCoda> createRepeatCodaNewbornClone (
      S_msrRepeat containingRepeat);

    SMARTP<msrRepeatCoda> createRepeatCodaDeepCopy (
      S_msrRepeat containingRepeat);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrRepeatCoda (
      int          inputLineNumber,
      S_msrSegment segment,
      S_msrRepeat  repeatUplink);
      
    virtual ~msrRepeatCoda ();
  
  public:

    // set and get
    // ------------------------------------------------------

    // segment
    S_msrSegment          getRepeatCodaSegment () const
                              { return fRepeatCodaSegment; }
                
    // uplinks
    S_msrRepeat           getRepeatCodaRepeatUplink () const
                              { return fRepeatCodaRepeatUplink; }

    // services
    // ------------------------------------------------------
  
 //  JMI void                  appendElementToRepeatCoda (S_msrElement elem);
                    
  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;

    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // segment
    S_msrSegment          fRepeatCodaSegment;

    // uplinks
    S_msrRepeat           fRepeatCodaRepeatUplink;
};
typedef SMARTP<msrRepeatCoda> S_msrRepeatCoda;
EXP ostream& operator<< (ostream& os, const S_msrRepeatCoda& elt);

//______________________________________________________________________________
class msrStaff;
typedef SMARTP<msrStaff> S_msrStaff;

//______________________________________________________________________________
class msrVoice : public msrElement
{
  public:
    
    // constants
    // ------------------------------------------------------

    #define K_NO_VOICE_NUMBER                 -39
    #define K_VOICE_HARMONY_VOICE_BASE_NUMBER 20

    // data types
    // ------------------------------------------------------

    enum msrVoiceKind {
      kRegularVoice,
      kHarmonyVoice,       // for MusicXML <harmony/>, LilyPond ChordNames
      kFiguredBassVoice }; // for MusicXML <figured-bass/>, LilyPond ChordNames
          
    static string voiceKindAsString (
      msrVoiceKind voiceKind);
      
    enum msrVoiceFinalizationStatusKind { // JMI ???
      kKeepVoice,
      kEraseVoice };
          
    static string voiceFinalizationStatusKindAsString (
      msrVoiceFinalizationStatusKind voiceFinalizationStatusKind);

    enum msrVoiceCreateInitialLastSegmentKind {
      kCreateInitialLastSegmentYes, kCreateInitialLastSegmentNo };
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrVoice> create (
      int          inputLineNumber,
      msrVoiceKind voiceKind,
      int          voiceNumber,
      msrVoiceCreateInitialLastSegmentKind
                   voiceCreateInitialLastSegmentKind,
      S_msrStaff   voiceStaffUplink);
    
    SMARTP<msrVoice> createVoiceNewbornClone (
      S_msrStaff containingStaff);

    SMARTP<msrVoice> createVoiceDeepCopy (
      int          inputLineNumber,
      msrVoiceKind voiceKind,
      int          voiceNumber,
      S_msrStaff   containingStaff);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    // for regular voices
    msrVoice (
      int          inputLineNumber,
      msrVoiceKind voiceKind,
      int          voiceNumber,
      msrVoiceCreateInitialLastSegmentKind
                   voiceCreateInitialLastSegmentKind,
      S_msrStaff   voiceStaffUplink);

    // destructor
    virtual ~msrVoice ();

  private:

    // initialisation
    // ------------------------------------------------------

    void                  initializeVoice (
                            msrVoiceCreateInitialLastSegmentKind
                             voiceCreateInitialLastSegmentKind);

  public:

    // set and get
    // ------------------------------------------------------

    // uplinks
    
    S_msrStaff            getVoiceStaffUplink () const
                              { return fVoiceStaffUplink; }

    // voice kind

    void                  setVoiceKind (msrVoiceKind voiceKind)
                            { fVoiceKind = voiceKind; }
                            
    msrVoiceKind          getVoiceKind () const
                              { return fVoiceKind; }

    // voice numbers
    
    void                  setVoiceNumber (int voiceNumber)
                              { fVoiceNumber = voiceNumber; }

    int                   getVoiceNumber () const
                              { return fVoiceNumber; }
                
    void                  setRegularVoiceStaffSequentialNumber (
                            int regularVoiceStaffSequentialNumber)
                              {
                                fRegularVoiceStaffSequentialNumber =
                                  regularVoiceStaffSequentialNumber;
                              }

    int                   getRegularVoiceStaffSequentialNumber () const
                              { return fRegularVoiceStaffSequentialNumber; }

    // voice name
    
    void                  setVoiceNameFromNumber (
                            int inputLineNumber,
                            int voiceNumber);

    string                getVoiceName () const
                              { return fVoiceName; }

    // harmonies

    S_msrVoice            getHarmonyVoiceForRegularVoice () const
                              { return fHarmonyVoiceForRegularVoice; }

    S_msrVoice            getRegularVoiceForHarmonyVoice () const
                              { return fRegularVoiceForHarmonyVoice; }

    // stanzas
                   
    const map<string, S_msrStanza>&
                          getVoiceStanzasMap () const
                              { return fVoiceStanzasMap; }

    // voice last segment
    
    void                  setVoiceCloneLastSegment (
                            S_msrSegment segment);

    S_msrSegment          getVoiceLastSegment () const
                              { return fVoiceLastSegment; }

    // voice last appended note

    S_msrNote             getVoiceLastAppendedNote () const
                              { return fVoiceLastAppendedNote; }

    // voice shortest note
    
    rational              getVoiceShortestNoteDuration () const
                              { return fVoiceShortestNoteDuration; }

    rational              getVoiceShortestNoteTupletFactor () const
                              { return fVoiceShortestNoteTupletFactor; }

    // counters
    
    int                   getVoiceActualNotesCounter () const
                              { return fVoiceActualNotesCounter; }

    int                   getVoiceRestsCounter () const
                              { return fVoiceRestsCounter; }

    int                   getVoiceSkipsCounter () const
                              { return fVoiceSkipsCounter; }

    int                   getVoiceActualHarmoniesCounter () const
                              { return fVoiceActualHarmoniesCounter; }

    int                   getVoiceActualFiguredBassCounter () const
                              { return fVoiceActualFiguredBassCounter; }

     // measures
     
    const string          getVoiceCurrentMeasureNumber () const
                              { return fVoiceCurrentMeasureNumber; }

    // has music been inserted in the voice?
    
    void                  setMusicHasBeenInsertedInVoice ()
                              { fMusicHasBeenInsertedInVoice = true; }

    bool                  getMusicHasBeenInsertedInVoice () const
                              { return fMusicHasBeenInsertedInVoice; }

    // multiple rests
    
    void                  setVoiceContainsMultipleRests (
                            int inputLineNumber);

    bool                  getVoiceContainsMultipleRests () const
                              { return fVoiceContainsMultipleRests; }

    // measures flat list
    const list<S_msrMeasure>&
                          getVoiceMeasuresFlatList () const
                              { return fVoiceMeasuresFlatList; }

    // services
    // ------------------------------------------------------

    // uplinks

    S_msrPart             fetchVoicePartUplink () const;

    // identity

    void                  changeVoiceIdentity ( // after a deep copy
                            int voiceNumber);

    // measures
     
    void                  createMeasureAndAppendItToVoice (
                            int    inputLineNumber,
                            string measureNumber,
                            int    measureOrdinalNumber,
                            msrMeasure::msrMeasureImplicitKind
                                   measureImplicitKind);

    void                  setNextMeasureNumberInVoice (
                            int    inputLineNumber,
                            string nextMeasureNumber);

    void                  appendAFirstMeasureToVoiceIfNotYetDone ( // JMI ???
                             int inputLineNumber);
                                                    
    void                  padUpToMeasureLengthInVoice (
                            int      inputLineNumber,
                            rational measureLength);
  
    void                  appendPaddingNoteToVoice ( // for forward
                            int inputLineNumber,
                            int divisions,
                            int divisionsPerQuarterNote);
  
    // clef, key, time
    
    void                  appendClefToVoice (S_msrClef clef);
    
    void                  appendKeyToVoice  (S_msrKey  key);
    
    void                  appendTimeToVoice (S_msrTime time);
    void                  appendTimeToVoiceClone (S_msrTime time);

    // notes
    
    S_msrNote             fetchVoiceFirstNonGraceNote () const;

    void                  registerShortestNoteIfRelevant (S_msrNote note);

    void                  registerNoteAsVoiceLastAppendedNote (S_msrNote note);

    // harmonies

    S_msrVoice            createHarmonyVoiceForRegularVoice (
                            int    inputLineNumber,
                            string currentMeasureNumber);

    // transpose

    void                  appendTransposeToVoice (
                            S_msrTranspose transpose);

    // part name display

    void                  appendPartNameDisplayToVoice (
                            S_msrPartNameDisplay partNameDisplay);

    // part abbreviation display

    void                  appendPartAbbreviationDisplayToVoice (
                            S_msrPartAbbreviationDisplay partAbbreviationDisplay);

    // staff details

    void                  appendStaffDetailsToVoice (
                            S_msrStaffDetails staffDetails);

    // tempo
    
    void                  appendTempoToVoice (S_msrTempo tempo);

    // rehearsals
    
    void                  appendRehearsalToVoice (
                            S_msrRehearsal rehearsal);

    // octave shifts
    
    void                  appendOctaveShiftToVoice (
                            S_msrOctaveShift octaveShift);

    // scordaturas
    
    void                  appendScordaturaToVoice (
                            S_msrScordatura scordatura);

    // accordion registration

    void                  appendAccordionRegistrationToVoice (
                            S_msrAccordionRegistration
                              accordionRegistration);

    // harp pedals tuning
    
    void                  appendHarpPedalsTuningToVoice (
                            S_msrHarpPedalsTuning
                              harpPedalsTuning);

    // staff change
    
    void                  appendVoiceStaffChangeToVoice (
                            S_msrVoiceStaffChange voiceStaffChange);

    // notes
    
    void                  appendNoteToVoice      (S_msrNote note);
    void                  appendNoteToVoiceClone (S_msrNote note);

    // tremolos
    
    void                  appendDoubleTremoloToVoice (
                            S_msrDoubleTremolo doubleTremolo);

    // chords
    
    void                  appendChordToVoice  (S_msrChord chord);

    // tuplets
    
    void                  appendTupletToVoice (S_msrTuplet tuplet);

    // harmonies

    void                  appendHarmonyToVoice (
                            S_msrHarmony harmony);
    
    void                  appendHarmonyToVoiceClone (
                            S_msrHarmony harmony);

    // frames

    void                  appendFrameToVoice (
                            S_msrFrame frame);
    
    void                  appendFrameToVoiceClone (
                            S_msrFrame frame);

    // figured bass

    void                  appendFiguredBassToVoice (
                            S_msrFiguredBass figuredBass);
    
    void                  appendFiguredBassToVoiceClone (
                            S_msrFiguredBass figuredBass);

    // grace notes

    void                  addGraceNotesGroupBeforeAheadOfVoiceIfNeeded (
                            S_msrGraceNotesGroup graceNotesGroup);

    /*
    void                  appendGraceNotesToVoice (
                            S_msrGraceNotes graceNotes);

    void                  appendAfterGraceNotesToVoice (
                            S_msrAfterGraceNotes afterGraceNotes);

    void                  prependAfterGraceNotesToVoice (
                            S_msrAfterGraceNotes afterGraceNotes);
                            */

    // lyrics
    
    void                  appendSyllableToVoice ( // JMI
                            int           inputLineNumber,
                            string        stanzaNumber,
                            string        stanzaName,
                            S_msrSyllable syllable);

    // bar checks
    
    void                  appendBarCheckToVoice (
                            S_msrBarCheck barCheck);

    // bar number checks
    
    void                  appendBarNumberCheckToVoice (
                            S_msrBarNumberCheck barNumberCheck);

    // breaks
    
    void                  appendLineBreakToVoice (S_msrLineBreak lineBreak);
    void                  appendPageBreakToVoice (S_msrPageBreak pageBreak);

    // bar lines
    
    void                  prependBarlineToVoice (S_msrBarline barline);
    void                  appendBarlineToVoice (S_msrBarline barline);

    // segno
    
    void                  appendSegnoToVoice (S_msrSegno segno);

    // coda
    
    void                  appendCodaToVoice (S_msrCoda coda);

    // eyeglasses
    
    void                  appendEyeGlassesToVoice (
                            S_msrEyeGlasses eyeGlasses);

    // pedal
    
    void                  appendPedalToVoice (S_msrPedal pedal);

    // damp
    
    void                  appendDampToVoice (S_msrDamp damp);

    // damp all
    
    void                  appendDampAllToVoice (S_msrDampAll dampAll);

    // other elements
    
    void                  prependOtherElementToVoice (S_msrElement elem);
    void                  appendOtherElementToVoice (S_msrElement elem);
                            // for other types of elements not known
                            // in this header file, such as LPSR elements

    // last measure in voice
    
    S_msrMeasure          fetchVoiceLastMeasure (
                            int inputLineNumber) const;
    
    // last element in voice

    S_msrElement          fetchVoiceLastElement (
                            int inputLineNumber) const;
    
    // removing elements from voice
    
    void                  removeNoteFromVoice (
                            int       inputLineNumber,
                            S_msrNote note);

    void                  removeElementFromVoice (
                            int          inputLineNumber,
                            S_msrElement element);

    S_msrMeasure          removeLastMeasureFromVoice (
                            int inputLineNumber);

    // repeats
    
    void                  prepareForRepeatInVoice (
                            int inputLineNumber);
                            
    void                  prepareForRepeatInVoiceClone (
                            int inputLineNumber,
                            int repeatTimes);
  
    void                  nestContentsIntoNewRepeatInVoice (
                            int inputLineNumber);
  
    void                  createRepeatUponItsEndAndAppendItToVoice (
                            int    inputLineNumber,
                            string measureNumber,
                            int    repeatTimes);
  
    void                  createRepeatUponItsEndAndAppendItToVoiceClone (
                            int    inputLineNumber,
                            int    repeatTimes);
  
    void                  createRegularRepeatUponItsFirstEndingInVoice (
                            int inputLineNumber,
                            int repeatTimes);
  
    void                  createEnclosingRepeatUponItsFirstEndingInVoice (
                            int inputLineNumber,
                            int repeatTimes);
  
    void                  appendRepeatCloneToVoice (
                            int         inputLineNumber,
                            S_msrRepeat repeatCLone);
    
    void                  appendRepeatEndingToVoice (
                            int       inputLineNumber,
                            string    repeatEndingNumber, // may be "1, 2"
                            msrRepeatEnding::msrRepeatEndingKind
                                      repeatEndingKind);

    void                  appendRepeatEndingCloneToVoice (
                            S_msrRepeatEnding repeatEndingClone);

    // multiple rests

    void                  createMultipleRestInVoice (
                            int inputLineNumber,
                            int multipleRestMeasuresNumber);

    void                  appendPendingMultipleRestToVoice (
                            int inputLineNumber);
                            
    void                  appendMeasuresRepeatReplicaToVoice (
                            int       inputLineNumber);

    void                  prepareForMultipleRestInVoiceClone (
                            int inputLineNumber);
  
    void                  appendMultipleRestCloneToVoice (
                            int               inputLineNumber,
                            S_msrMultipleRest multipleRestClone);
  
    // measures repeats
    
    void                  createMeasuresRepeatFromItsFirstMeasuresInVoice (
                            int inputLineNumber,
                            int measuresRepeatMeasuresNumber,
                            int measuresRepeatSlashes);

    void                  appendPendingMeasuresRepeatToVoice (
                            int inputLineNumber);
                            
    void                  createMeasuresRepeatAndAppendItToVoiceClone (
                            int inputLineNumber,
                            int measuresRepeatMeasuresNumber,
                            int measuresRepeatSlashesNumber);

    // segments
    
    void                  createNewLastSegmentForVoice (
                            int inputLineNumber);
    
    void                  createNewLastSegmentFromFirstMeasureForVoice (
                            int          inputLineNumber,
                            S_msrMeasure firstMeasure);

    void                  createNewLastSegmentAndANewMeasureBeforeARepeat (
                            int inputLineNumber,
                            int measureFullLength);
    void                  createNewLastSegmentAndANewMeasureAfterARepeat (
                            int inputLineNumber,
                            int measureFullLength);
                            
    // stanzas

    S_msrStanza           addStanzaToVoiceByItsNumber (
                            int    inputLineNumber,
                            string stanzaNumber);

    void                  addStanzaToVoiceWithoutCatchUp ( // JMI
                            S_msrStanza stanza);

    void                  addStanzaToVoice (
                            S_msrStanza stanza);

    S_msrStanza           createStanzaInVoiceIfNotYetDone (
                            int    inputLineNumber,
                            string stanzaNumber,
                            string stanzaName);
    
    S_msrStanza           fetchStanzaInVoice (
                            int    inputLineNumber,
                            string stanzaNumber,
                            string stanzaName);

    // strings
    
    // finalization
    
    void                  finalizeCurrentMeasureInVoice (
                            int inputLineNumber);

    void                  finalizeVoice (
                            int inputLineNumber);
    
    void                  collectVoiceMeasuresIntoFlatList (
                            int inputLineNumber);
    
  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                voiceKindAsString () const;
    
    string                voiceNumberAsString () const;

    string                regularVoiceStaffSequentialNumberAsString () const;

    string                asShortString () const;
    
    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // uplinks
    
    S_msrStaff            fVoiceStaffUplink;

    // voice kind

    msrVoiceKind          fVoiceKind;

    // voice numbers

    // voice numbers in MusicXML may be greater than 4
    // and there can be holes
    int                   fVoiceNumber;

    // there can only be 4 regular voices in a staff
    // (those that can contain beamed notes)
    // and we need a number for the orientation of beams
    int                   fRegularVoiceStaffSequentialNumber;

    // voice name

    string                fVoiceName;

    // harmonies

    // two-way links
    S_msrVoice            fHarmonyVoiceForRegularVoice;
    S_msrVoice            fRegularVoiceForHarmonyVoice;

    // counters
    
    static int            gVoicesCounter;

    int                   fVoiceActualNotesCounter;
    int                   fVoiceRestsCounter;
    int                   fVoiceSkipsCounter;
    int                   fVoiceActualHarmoniesCounter;
    int                   fVoiceActualFiguredBassCounter;

    // measures
    
    string                fVoiceCurrentMeasureNumber;
    
    // musically empty voices
    
    bool                  fMusicHasBeenInsertedInVoice;

    // voice internal handling
    
    list<S_msrElement>    fVoiceInitialElementsList;

    // fVoiceLastSegment contains the music
    // not yet stored in fVoiceInitialElementsList,
    // it is thus logically the end of the latter,
    // and is created implicitly for every voice.
    // Is is needed 'outside' of the 'list<S_msrElement>'
    // because it is not a mere S_msrElement, but a S_msrSegment
    S_msrSegment          fVoiceLastSegment;
    // it should be saved sometimes
    S_msrSegment          fSaveVoiceLastSegment;

    // fVoiceLastAppendedNote is used to build chords upon their second note
    S_msrNote             fVoiceLastAppendedNote;

    // fVoiceShortestNoteDuration and fVoiceShortestNoteTupletFactor
    // are used to compute a number of divisions per quarter note
    // if needed, such as when generating MusicXML from MSR
    rational              fVoiceShortestNoteDuration;
    rational              fVoiceShortestNoteTupletFactor;
    
    // fVoiceFirstSegment is used to work around LilyPond issue 34
    S_msrSegment          fVoiceFirstSegment;

    // fVoiceCurrentRepeat is null or
    // the last msrRepeat in fVoiceInitialElementsList
    S_msrRepeat           fVoiceCurrentRepeat;
    S_msrRepeatCommonPart fVoiceCurrentRepeatCommonPart;

    // fVoiceCurrentMeasuresRepeat is null
    // or the last msrMeasuresRepeat created with its repeated measure,
    // but not yet appended to the voice
    S_msrMeasuresRepeat   fVoiceCurrentMeasuresRepeat;

    // fVoicePendingMultipleRest is null
    // or the last msrMultipleRest created,
    // but not yet appended to the voice
    S_msrMultipleRest     fVoicePendingMultipleRest;
    
    // fVoiceMultipleRestWaitingForItsNextMeasureNumber is null
    // or the last msrMultipleRest created and appended to the voice,
    // but with its next measure number not yet set
    S_msrMultipleRest     fVoiceMultipleRestWaitingForItsNextMeasureNumber;
    int                   fVoiceRemainingRestMeasures;
    
    bool                  fVoiceContainsMultipleRests;
 
    // stanzas
        
    map<string, S_msrStanza>
                          fVoiceStanzasMap;

    // measures flat list
    // i.e. without segments nor repeats,
    // extracted from fVoiceInitialElementsList and fSaveVoiceLastSegment
    // by finalizeMeasure()
    list<S_msrMeasure>    fVoiceMeasuresFlatList;
};
EXP ostream& operator<< (ostream& os, const S_msrVoice& elt);

//______________________________________________________________________________
class msrStaff : public msrElement
{
  public:

    // constants
    // ------------------------------------------------------

    #define K_NO_STAFF_NUMBER                 -79

    // data types
    // ------------------------------------------------------

    enum msrStaffKind {
      kRegularStaff,
      kTablatureStaff,
      kHarmonyStaff,
      kFiguredBassStaff,
      kDrumStaff,
      kRythmicStaff };

    static string staffKindAsString (
      msrStaffKind staffKind);
      
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrStaff> create (
      int          inputLineNumber,
      msrStaffKind staffKind,
      int          staffNumber,
      S_msrPart    staffPartUplink);
    
    SMARTP<msrStaff> createStaffNewbornClone (
      S_msrPart containingPart);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrStaff (
      int          inputLineNumber,
      msrStaffKind staffKind,
      int          staffNumber,
      S_msrPart    staffPartUplink);
      
    virtual ~msrStaff ();
  
  private:

    // initialization
    // ------------------------------------------------------

    void                  initializeStaff ();

  public:

    // set and get
    // ------------------------------------------------------

    // uplinks

    S_msrPart             getStaffPartUplink () const
                              { return fStaffPartUplink; }

    // staff kind
    
    msrStaffKind          getStaffKind () const
                              { return fStaffKind; }

    // staff number and name
    
    int                   getStaffNumber () const
                              { return fStaffNumber; }
                
    string                getStaffName () const
                              { return fStaffName; }

    string                getStaffInstrumentName () const
                              { return fStaffInstrumentName; }

    string                getStaffInstrumentAbbreviation () const
                              { return fStaffInstrumentAbbreviation; }

    // voices

    int                   getStaffRegularVoicesCounter () const
                              { return fStaffRegularVoicesCounter; }

    // clef, key, time
    
    void                  setStaffCurrentClef (S_msrClef clef);
                              
    S_msrClef             getStaffCurrentClef () const
                              { return fStaffCurrentClef; };
                              
    void                  setStaffCurrentKey (S_msrKey key);
                              
    S_msrKey              getStaffCurrentKey  () const
                              { return fStaffCurrentKey; };
                              
    S_msrTime             getStaffCurrentTime () const
                              { return fStaffCurrentTime; };

    // transpose
    
    S_msrTranspose        getStaffCurrentTranspose () const
                              { return fStaffCurrentTranspose; };
    
    // staff details

    S_msrStaffDetails     getStaffStaffDetails () const
                            { return fStaffStaffDetails; }

    // staff voices
    
    const map<int, S_msrVoice>&
                          getStaffAllVoicesMap () const
                              { return fStaffAllVoicesMap; }

    const map<int, S_msrVoice>&
                          getStaffRegularVoicesMap () const
                              {
                                return
                                  fStaffRegularVoicesMap;
                              }

    const list<S_msrVoice>&
                          getStaffAllVoicesList () const
                              {
                                return
                                  fStaffAllVoicesList;
                              }

    // services
    // ------------------------------------------------------

    // staff details
    
    void                  appendStaffDetailsToStaff (
                            S_msrStaffDetails staffDetails);

    // measures

    void                  padUpToMeasureLengthInStaff (
                            int      inputLineNumber,
                            rational measureLength);
  
    // clef, key, time

    void                  appendClefToStaff (S_msrClef clef);
    
    void                  appendKeyToStaff (S_msrKey  key);
    
    void                  appendTimeToStaff (S_msrTime time);
    void                  appendTimeToStaffClone (S_msrTime time);
    
    // transpose

    void                  appendTransposeToStaff (
                            S_msrTranspose transpose);

    // part name display

    void                  appendPartNameDisplayToStaff (
                            S_msrPartNameDisplay partNameDisplay);

    // part abbreviation display

    void                  appendPartAbbreviationDisplayToStaff (
                            S_msrPartAbbreviationDisplay partAbbreviationDisplay);

    // voices
      
    S_msrVoice            createVoiceInStaffByItsNumber (
                            int                    inputLineNumber,
                            msrVoice::msrVoiceKind voiceKind,
                            int                    voiceNumber,
                            string                 currentMeasureNumber);

    void                  registerVoiceInStaff (
                            int        inputLineNumber,
                            S_msrVoice voice);

    void                  registerVoiceByItsNumber (
                            int        inputLineNumber,
                            int        voiceNumber,
                            S_msrVoice voice);
  
    void                  registerVoiceInRegularVoicesMap (
                            int        voiceNumber,
                            S_msrVoice voice);
  
    void                  registerVoiceInAllVoicesList (
                            int        voiceNumber,
                            S_msrVoice voice);
  
    S_msrVoice            fetchVoiceFromStaffByItsNumber (
                            int inputLineNumber,
                            int voiceNumber);

    void                  addAVoiceToStaffIfItHasNone (
                            int inputLineNumber);
  
    // measures
    
    void                  createMeasureAndAppendItToStaff (
                            int    inputLineNumber,
                            string measureNumber,
                            int    measureOrdinalNumber,
                            msrMeasure::msrMeasureImplicitKind
                                   measureImplicitKind);

    void                  setNextMeasureNumberInStaff (
                            int    inputLineNumber,
                            string nextMeasureNumber);

    // repeats
    
    void                  prepareForRepeatInStaff (
                            int inputLineNumber);
    
    void                  nestContentsIntoNewRepeatInStaff (
                            int inputLineNumber);
    
    void                  createRepeatUponItsEndAndAppendItToStaff (
                            int    inputLineNumber,
                            string measureNumber,
                            int    repeatTimes);
    
    void                  createRegularRepeatUponItsFirstEndingInStaff (
                            int inputLineNumber,
                            int repeatTimes);
    
    void                  createEnclosingRepeatUponItsFirstEndingInStaff (
                            int inputLineNumber,
                            int repeatTimes);
    
    void                  createMeasuresRepeatFromItsFirstMeasuresInStaff (
                            int inputLineNumber,
                            int measuresRepeatMeasuresNumber,
                            int measuresRepeatSlashes);
    
    void                  appendPendingMeasuresRepeatToStaff (
                            int inputLineNumber);
                            
    void                  createMultipleRestInStaff (
                            int inputLineNumber,
                            int multipleRestMeasuresNumber);
    
    void                  appendPendingMultipleRestToStaff (
                            int inputLineNumber);
                            
    void                  appendMultipleRestCloneToStaff (
                            int               inputLineNumber,
                            S_msrMultipleRest multipleRest);
  
    void                  appendRepeatCloneToStaff (
                            int         inputLineNumber,
                            S_msrRepeat repeatCLone);

    void                  appendRepeatEndingToStaff (
                            int       inputLineNumber,
                            string    repeatEndingNumber, // may be "1, 2"
                            msrRepeatEnding::msrRepeatEndingKind
                                      repeatEndingKind);

    void                  appendRepeatEndingCloneToStaff (
                            S_msrRepeatEnding repeatEndingClone);

/* JMI
    void                  createRepeatAndAppendItToAllStaffVoices (
                            int inputLineNumber,
                            int repeatTimes);
    */
    
    // barlines
    
    void                  appendBarlineToStaff (S_msrBarline barline);
    
    // transpose
    
    void                  appendTransposeToAllStaffVoices ( // JMI
                            S_msrTranspose transpose);
  
    // part name display
    
    void                  appendPartNameDisplayToAllStaffVoices ( // JMI
                            S_msrPartNameDisplay partNameDisplay);
  
    // part abbreviation display
    
    void                  appendPartAbbreviationDisplayToAllStaffVoices ( // JMI
                            S_msrPartAbbreviationDisplay partAbbreviationDisplay);
  
    // scordaturas
    
    void                  appendScordaturaToStaff (
                            S_msrScordatura scordatura);

    // accordion registration

    void                  appendAccordionRegistrationToStaff (
                            S_msrAccordionRegistration
                              accordionRegistration);

    // harp pedals tuning
    
    void                  appendHarpPedalsTuningToStaff (
                            S_msrHarpPedalsTuning
                              harpPedalsTuning);

    // finalization

    void                  finalizeCurrentMeasureInStaff (
                            int inputLineNumber);

    void                  finalizeStaff (
                            int inputLineNumber);

    // voices ordering in staves
    
    static bool           compareVoicesToHaveHarmoniesAboveCorrespondingVoice (
                            const S_msrVoice& first,
                            const S_msrVoice& second);

    // strings
    
  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    // staff number
    
    string                staffNumberAsString ();

    string                staffKindAsString () const;
    
    virtual void          print (ostream& os);

    virtual void          printSummary (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // uplinks

    S_msrPart             fStaffPartUplink;

    // staff name

    string                fStaffName;
    
    // staff kind

    msrStaffKind          fStaffKind;

    // staff number

    int                   fStaffNumber;

    // staff instrument name
    
    string                fStaffInstrumentName;
    string                fStaffInstrumentAbbreviation;

    // staff voices
    
    static int            gStaffMaxRegularVoices;

    // the dictionary of all the voices in the staff
    map<int, S_msrVoice>  fStaffAllVoicesMap;

    // we need to handle the regular voice specifically
    // to assign them sequencing numbers from 1 to gMaxStaffVoices,
    // needed to set the beams orientation (up or down)
    int                   fStaffRegularVoicesCounter;
    map<int, S_msrVoice>  fStaffRegularVoicesMap;

    // we need to sort the voices by increasing voice numbers,
    // but with harmony voices right before the corresponding regular voices
    list<S_msrVoice>      fStaffAllVoicesList;
                              
    // clef, key, time
    
    S_msrClef             fStaffCurrentClef;
    
    S_msrKey              fStaffCurrentKey;
    
    S_msrTime             fStaffCurrentTime;

    // transpose
    
    S_msrTranspose        fStaffCurrentTranspose;

    // staff details

    S_msrStaffDetails     fStaffStaffDetails;
};
typedef SMARTP<msrStaff> S_msrStaff;
EXP ostream& operator<< (ostream& os, const S_msrStaff& elt);

//______________________________________________________________________________
class msrVoiceStaffChange : public msrElement
{
  public:
    
    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrVoiceStaffChange> create (
      int        inputLineNumber,
      S_msrStaff staffToChangeTo);

    SMARTP<msrVoiceStaffChange> createStaffChangeNewbornClone ();

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrVoiceStaffChange (
      int        inputLineNumber,
      S_msrStaff staffToChangeTo);
      
    virtual ~msrVoiceStaffChange ();
  
  public:

    // set and get
    // ------------------------------------------------------

    S_msrStaff            getStaffToChangeTo () const
                              { return fStaffToChangeTo; }
                        
    // services
    // ------------------------------------------------------

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    string                asString () const;
         
    virtual void          print (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    S_msrStaff            fStaffToChangeTo;
};
typedef SMARTP<msrVoiceStaffChange> S_msrVoiceStaffChange;
EXP ostream& operator<< (ostream& os, const S_msrVoiceStaffChange& elt);

//______________________________________________________________________________
class msrPartGroup;
typedef SMARTP<msrPartGroup> S_msrPartGroup;

class msrScore;
typedef SMARTP<msrScore> S_msrScore;

//______________________________________________________________________________
class msrPart : public msrElement
{
  public:

    // constants
    // ------------------------------------------------------

    #define K_PART_FIGURED_BASS_STAFF_NUMBER -219  
    #define K_PART_FIGURED_BASS_VOICE_NUMBER -227

    // creation from MusicXML
    // ------------------------------------------------------

    static SMARTP<msrPart> create (
      int            inputLineNumber,
      string         partID,
      S_msrPartGroup partPartGroupUplink);
                
    SMARTP<msrPart> createPartNewbornClone (
      S_msrPartGroup partGroupClone);

  protected:

    // constructors/destructor
    // ------------------------------------------------------

    msrPart (
      int            inputLineNumber,
      string         partID,
      S_msrPartGroup partPartGroupUplink);
      
    virtual ~msrPart ();
  
  private:

    // initialization
    // ------------------------------------------------------

    void                  initializePart ();

  public:

    // set and get
    // ------------------------------------------------------
    
    // uplinks
    
    void                  setPartPartGroupUplink (
                            S_msrPartGroup partGroup)
                              { fPartPartGroupUplink = partGroup; };

    S_msrPartGroup        getPartPartGroupUplink () const
                              { return fPartPartGroupUplink; }
              
    // part absolute number
    
    int                   getPartAbsoluteNumber () const
                              { return fPartAbsoluteNumber; }

    // ID and name
    
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

    void                  setPartNameDisplayText (
                            string partNameDisplayText)
                              {
                                fPartNameDisplayText =
                                  partNameDisplayText;
                              }
                
    string                getPartNameDisplayText () const
                              { return fPartNameDisplayText; }

    void                  setPartAbbreviation (
                            string partAbbreviation)
                              {
                                fPartAbbreviation =
                                  partAbbreviation;
                              }
                
    string                getPartAbbreviation () const
                              { return fPartAbbreviation; }

    void                  setPartAbbreviationDisplayText (
                            string partAbbreviationDisplayText)
                              {
                                fPartAbbreviationDisplayText =
                                  partAbbreviationDisplayText;
                              }
                
    string                getPartAbbreviationDisplayText () const
                              { return fPartAbbreviationDisplayText; }

    string                getPartCombinedName () const;

    void                  setPartInstrumentNamesMaxLengthes ();

    // measures
    
    void                  setPartMeasureLengthHighTide (
                            int      inputLineNumber,
                            rational measureLength);
                      
    void                  updatePartMeasureLengthHighTide (
                            int      inputLineNumber,
                            rational measureLength);
                    
    rational              getPartMeasureLengthHighTide () const
                              {
                                return
                                  fPartMeasureLengthHighTide;
                              }

    void                  setPartCurrentMeasureNumber (
                            string measureNumber)
                              {
                                fPartCurrentMeasureNumber =
                                  measureNumber;
                              }

    const string          getPartCurrentMeasureNumber () const
                              { return fPartCurrentMeasureNumber; }

    void                  setPartNumberOfMeasures (
                            int partNumberOfMeasures)
                              {
                                fPartNumberOfMeasures =
                                  partNumberOfMeasures;
                              }

    const int             getPartNumberOfMeasures () const
                              { return fPartNumberOfMeasures; }

    // staff details

    S_msrStaffDetails     getCurrentPartStaffDetails () const
                              { return fCurrentPartStaffDetails; }

    // instrument name

    void                  setPartInstrumentName (
                            string partInstrumentName)
                              {
                                fPartInstrumentName =
                                  partInstrumentName;
                              }
                              
    string                getPartInstrumentName () const
                              { return fPartInstrumentName; }
                
    void                  setPartInstrumentAbbreviation (
                            string partInstrumentAbbreviation)
                              {
                                fPartInstrumentAbbreviation =
                                  partInstrumentAbbreviation;
                              }
                              
    string                getPartInstrumentAbbreviation () const
                              { return fPartInstrumentAbbreviation; }

    // figured bass staff and voice
    
    void                  setPartFiguredBassStaff (
                            S_msrStaff figuredBassStaff)
                              {
                                fPartFiguredBassStaff =
                                  figuredBassStaff;
                              }
                  
    S_msrStaff            getPartFiguredBassStaff () const
                              { return fPartFiguredBassStaff; }
                 
    void                  setPartFiguredBassVoice (
                            S_msrVoice figuredBassVoice)
                              {
                                fPartFiguredBassVoice =
                                  figuredBassVoice;
                              }

    S_msrVoice            getPartFiguredBassVoice () const
                              { return fPartFiguredBassVoice; }
                  
    // clef, key, time
    
    S_msrClef             getPartCurrentClef () const
                              { return fPartCurrentClef; };

    S_msrKey              getPartCurrentKey  () const
                              { return fPartCurrentKey; };

    void                  setPartCurrentTime (S_msrTime time)
                              { fPartCurrentTime = time; };

    S_msrTime             getPartCurrentTime () const
                              { return fPartCurrentTime; };

    // transpose

    S_msrTranspose        getPartCurrentTranspose () const
                              { return fPartCurrentTranspose; };

    // staves map
    
    const map<int, S_msrStaff>&
                          getPartStavesMap ()
                              { return fPartStavesMap; }

    // services
    // ------------------------------------------------------

    void                  addAVoiceToStavesThatHaveNone (
                            int inputLineNumber);
  
    // measures

    void                  padUpToMeasureLengthInPart (
                            int      inputLineNumber,
                            rational measureLength);
  
    // part name display

    void                  appendPartNameDisplayToPart (
                            S_msrPartNameDisplay partNameDisplay);

    // part abbreviation display

    void                  appendPartAbbreviationDisplayToPart (
                            S_msrPartAbbreviationDisplay partAbbreviationDisplay);

    // figured bass staff and voice
    
    void                  createPartFiguredBassStaffAndVoiceIfNotYetDone (
                            int inputLineNumber);
        
    // measures
    
    void                  createMeasureAndAppendItToPart (
                            int    inputLineNumber,
                            string measureNumber,
                            int    measureOrdinalNumber,
                            msrMeasure::msrMeasureImplicitKind
                                   measureImplicitKind);

    void                  setNextMeasureNumberInPart (
                            int    inputLineNumber,
                            string nextMeasureNumber);

    // clef, key, time
    
    void                  appendClefToPart (S_msrClef clef);
    
    void                  appendKeyToPart (S_msrKey  key);
    
    void                  appendTimeToPart (S_msrTime time);
    void                  appendTimeToPartClone (S_msrTime time);

    // transpose

    void                  appendTransposeToPart (
                            S_msrTranspose transpose);

    // staff details

    void                  appendStaffDetailsToPart (
                            S_msrStaffDetails staffDetails);

    // tuplets JMI ???
    
    // barlines
    
    void                  appendBarlineToPart (S_msrBarline barline);
              
    // repeats
    
    void                  prepareForRepeatInPart (
                            int inputLineNumber);
    
    void                  nestContentsIntoNewRepeatInPart (
                            int inputLineNumber);
    
    void                  createRepeatUponItsEndAndAppendItToPart (
                            int    inputLineNumber,
                            string measureNumber,
                            int    repeatTimes);
    
    void                  createRegularRepeatUponItsFirstEndingInPart (
                            int inputLineNumber,
                            int repeatTimes);
    
    void                  createEnclosingRepeatUponItsFirstEndingInPart (
                            int inputLineNumber,
                            int repeatTimes);
    
    void                  appendRepeatCloneToPart (
                            int         inputLineNumber,
                            S_msrRepeat repeatCLone);
    
    void                  appendRepeatEndingToPart (
                            int       inputLineNumber,
                            string    repeatEndingNumber, // may be "1, 2"
                            msrRepeatEnding::msrRepeatEndingKind
                                      repeatEndingKind);
    
    void                  appendRepeatEndingCloneToPart (
                            S_msrRepeatEnding repeatEndingCLone);

    void                  createMeasuresRepeatFromItsFirstMeasuresInPart (
                            int inputLineNumber,
                            int measuresRepeatMeasuresNumber,
                            int measuresRepeatSlashes);

    void                  appendPendingMeasuresRepeatToPart (
                            int inputLineNumber);
                            
    void                  appendMeasuresRepeatCloneToPart (
                            int               inputLineNumber,
                            S_msrMultipleRest multipleRest);
  
    void                  createMultipleRestInPart (
                            int inputLineNumber,
                            int multipleRestMeasuresNumber);

    void                  appendPendingMultipleRestToPart (
                            int inputLineNumber);
                            
    void                  appendMultipleRestCloneToPart (
                            int               inputLineNumber,
                            S_msrMultipleRest multipleRest);

    // staves
    
    S_msrStaff            addStaffToPartByItsNumber (
                            int                    inputLineNumber,
                            msrStaff::msrStaffKind staffKind,
                            int                    staffNumber);

    void                  addStaffToPartCloneByItsNumber (
                            S_msrStaff staff);

    S_msrStaff            fetchStaffFromPart (int staffNumber);

    // voices

    void                  removePartEmptyVoices ();
    
    // harmonies
    
    // JMI ???
    
    // frames
    
    // JMI ???
    
    // figured bass

    void                  appendFiguredBassToPart (
                            S_msrVoice      figuredBassSupplierVoice,
                           S_msrFiguredBass figuredBass);
    
    void                  appendFiguredBassToPartClone (
                            S_msrVoice       figuredBassSupplierVoice,
                            S_msrFiguredBass figuredBass);
  
    // scordaturas
    
    void                  appendScordaturaToPart (
                            S_msrScordatura scordatura);

    // accordion registration

    void                  appendAccordionRegistrationToPart (
                            S_msrAccordionRegistration
                              accordionRegistration);

    // harp pedals tuning
    
    void                  appendHarpPedalsTuningToPart (
                            S_msrHarpPedalsTuning
                              harpPedalsTuning);

    // backup
    
    void                  handleBackup (
                            int inputLineNumber,
                            int divisions,
                            int divisionsPerQuarterNote);

    // LilyPond issue 34

    void                  addSkipGraceNotesGroupBeforeAheadOfVoicesClonesIfNeeded (
                            S_msrVoice           graceNotesGroupOriginVoice,
                            S_msrGraceNotesGroup skipGraceNotesGroup);

/*
    void                  appendSkipGraceNotesToVoicesClones ( // JMI ???
                            S_msrVoice      graceNotesOriginVoice,
                            S_msrGraceNotes skipGraceNotes);
                            
                            */
                            
    // finalization

    void                  finalizeCurrentMeasureInPart (
                            int inputLineNumber);

    void                  finalizePart (
                            int inputLineNumber);

    void                  finalizePartClone (
                            int inputLineNumber);

  public:

    // visitors
    // ------------------------------------------------------

    virtual void          acceptIn  (basevisitor* v);
    virtual void          acceptOut (basevisitor* v);

    virtual void          browseData (basevisitor* v);

  public:

    // print
    // ------------------------------------------------------

    virtual string        asString () const;

    virtual void          print (ostream& os);

    virtual void          printSummary (ostream& os);

  private:

    // fields
    // ------------------------------------------------------

    // uplinks
    
    S_msrPartGroup        fPartPartGroupUplink;

    // part ID and name
    
    string                fPartID; // native
    
    string                fPartMsrName;
                            // may be different than fPartID
                            // if renamed,
                            // coined in constructor

    string                fPartName; // from '<part-name/>'
    string                fPartNameDisplayText;
    
    string                fPartAbbreviation;
    string                fPartAbbreviationDisplayText;

    // part absolute number
    
    int                   fPartAbsoluteNumber;

    // part instrument names
    
    string                fPartInstrumentName;
    string                fPartInstrumentAbbreviation;

    // measures

    string                fPartCurrentMeasureNumber;

    int                   fPartNumberOfMeasures;

    rational              fPartMeasureLengthHighTide;

    // clef, key, time
    
    S_msrClef             fPartCurrentClef;
    
    S_msrKey              fPartCurrentKey;
    
    S_msrTime             fPartCurrentTime;

    // transpose
    
    S_msrTranspose        fPartCurrentTranspose;

    // staff details

    S_msrStaffDetails     fCurrentPartStaffDetails;
    
    // figured bass

    S_msrStaff            fPartFiguredBassStaff;
    S_msrVoice            fPartFiguredBassVoice;

    // the registered staves map
    
    map<int, S_msrStaff>  fPartStavesMap;

    // counter
    
    static int            gPartsCounter;
};
typedef SMARTP<msrPart> S_msrPart;
EXP ostream& operator<< (ostream& os, const S_msrPart& elt);


}

#endif
