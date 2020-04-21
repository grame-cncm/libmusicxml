/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___msr2MxmltreeTranslator___
#define ___msr2MxmltreeTranslator___

#include <map>
#include <vector>

#include "msr.h"


namespace MusicXML2
{

//________________________________________________________________________
class msr2MxmltreeTranslator :

  public visitor<S_msrScore>,

  // rights

  public visitor<S_msrIdentification>,

  public visitor<S_msrCredit>,
  public visitor<S_msrCreditWords>,

  // scaling

  public visitor<S_msrScaling>,

  // layout

  public visitor<S_msrPageLayout>,
  public visitor<S_msrSystemLayout>,
  public visitor<S_msrStaffLayout>,
  public visitor<S_msrMeasureLayout>,

  // appearance

  public visitor<S_msrAppearance>,

  // parts & part groups

  public visitor<S_msrPartGroup>,

  public visitor<S_msrPart>,

  // staff details

  public visitor<S_msrStaffTuning>,
  public visitor<S_msrStaffDetails>,

  public visitor<S_msrStaff>,

  public visitor<S_msrVoice>,

  public visitor<S_msrVoiceStaffChange>,

  // lyrics

  public visitor<S_msrStanza>,
  public visitor<S_msrSyllable>,

  // clefs

  public visitor<S_msrClef>,

  // keys

  public visitor<S_msrKey>,

  // times

  public visitor<S_msrTime>,

  // transpose

  public visitor<S_msrTranspose>,

  // print

  public visitor<S_msrPartNameDisplay>,
  public visitor<S_msrPartAbbreviationDisplay>,

  // words

  public visitor<S_msrWords>,

  // tempos

  public visitor<S_msrTempo>,

  // segments

  public visitor<S_msrSegment>,

  // chords

  public visitor<S_msrChord>,

  // tuplets

  public visitor<S_msrTuplet>,

  // rehearsal

  public visitor<S_msrRehearsal>,

  // harmonies

  public visitor<S_msrHarmony>,
  public visitor<S_msrHarmonyDegree>,

  // frames

  public visitor<S_msrFrame>,

  // figured bass

  public visitor<S_msrFiguredBass>,
  public visitor<S_msrFigure>,

  // measures

  public visitor<S_msrMeasure>,

  // print layouts

  public visitor<S_msrPrintLayout>,

  // articulations

  public visitor<S_msrArticulation>,

  public visitor<S_msrFermata>,

  public visitor<S_msrArpeggiato>,
  public visitor<S_msrNonArpeggiato>,

  // technicals

  public visitor<S_msrTechnical>,
  public visitor<S_msrTechnicalWithInteger>,
  public visitor<S_msrTechnicalWithFloat>,
  public visitor<S_msrTechnicalWithString>,

  // ornaments

  public visitor<S_msrOrnament>,

  // spanners

//  public visitor<S_msrSpanner>,

  // glissandos

  public visitor<S_msrGlissando>,

  // slides

  public visitor<S_msrSlide>,

  // tremolos

  public visitor<S_msrSingleTremolo>,

  public visitor<S_msrDoubleTremolo>,

  // dynamics

  public visitor<S_msrDynamics>,

  public visitor<S_msrOtherDynamics>,

  public visitor<S_msrSlash>,

  public visitor<S_msrSlur>,
  public visitor<S_msrLigature>,
  public visitor<S_msrWedge>,

  // grace notes

  public visitor<S_msrGraceNotesGroup>,

  // notes

  public visitor<S_msrNote>,
  public visitor<S_msrOctaveShift>,

  // accordion registration

  public visitor<S_msrAccordionRegistration>,

  // harp pedals tuning

  public visitor<S_msrHarpPedalsTuning>,

  public visitor<S_msrStem>,

  // ties, slurs, brackets & beams

  public visitor<S_msrTie>,

  public visitor<S_msrBeam>,

  // ?

  public visitor<S_msrBarline>,

  public visitor<S_msrSegno>,
  public visitor<S_msrDalSegno>,
  public visitor<S_msrCoda>,

  public visitor<S_msrEyeGlasses>,
  public visitor<S_msrPedal>,
  public visitor<S_msrDamp>,
  public visitor<S_msrDampAll>,
  public visitor<S_msrScordatura>,

  public visitor<S_msrBarCheck>,
  public visitor<S_msrBarNumberCheck>,

  public visitor<S_msrLineBreak>,

  public visitor<S_msrPageBreak>,

  // repeats

  public visitor<S_msrRepeat>,
  public visitor<S_msrRepeatCommonPart>,
  public visitor<S_msrRepeatEnding>,

  public visitor<S_msrMeasuresRepeat>,
  public visitor<S_msrMeasuresRepeatPattern>,
  public visitor<S_msrMeasuresRepeatReplicas>,

  public visitor<S_msrRestMeasures>,
  public visitor<S_msrRestMeasuresContents>,

  // midi

  public visitor<S_msrMidiTempo>

{
  public:

    msr2MxmltreeTranslator (
      indentedOstream& ios,
      S_msrScore       mScore);

    virtual ~msr2MxmltreeTranslator ();

    void buildMxmltreeFromMsrScore ();

    Sxmlelement getScorePartWiseElement () const
        { return fScorePartWiseElement; }

  protected:

    virtual void visitStart (S_msrScore& elt);
    virtual void visitEnd   (S_msrScore& elt);

    // identification

    virtual void visitStart (S_msrIdentification& elt);
    virtual void visitEnd   (S_msrIdentification& elt);

    // credits

    virtual void visitStart (S_msrCredit& elt);
    virtual void visitEnd   (S_msrCredit& elt);
    virtual void visitStart (S_msrCreditWords& elt);
    virtual void visitEnd   (S_msrCreditWords& elt);

    // parts & part groups

    virtual void visitStart (S_msrPartGroup& elt);
    virtual void visitEnd   (S_msrPartGroup& elt);
    virtual void visitStart (S_msrPart& elt);
    virtual void visitEnd   (S_msrPart& elt);

/*
    virtual void visitStart (S_msrStaffTuning& elt);
    virtual void visitStart (S_msrStaffDetails& elt);
    virtual void visitEnd   (S_msrStaffDetails& elt);

    virtual void visitStart (S_msrStaff& elt);
    virtual void visitEnd   (S_msrStaff& elt);

    virtual void visitStart (S_msrVoice& elt);
    virtual void visitEnd   (S_msrVoice& elt);

    virtual void visitStart (S_msrVoiceStaffChange& elt);

    virtual void visitStart (S_msrStanza& elt);
    virtual void visitEnd   (S_msrStanza& elt);

    virtual void visitStart (S_msrSyllable& elt);
    virtual void visitEnd   (S_msrSyllable& elt);
*/

    virtual void visitStart (S_msrClef& elt);
    virtual void visitEnd   (S_msrClef& elt);
    virtual void visitStart (S_msrKey& elt);
    virtual void visitEnd   (S_msrKey& elt);
    virtual void visitStart (S_msrTime& elt);
    virtual void visitEnd   (S_msrTime& elt);
/*

    virtual void visitStart (S_msrTranspose& elt);
    virtual void visitEnd   (S_msrTranspose& elt);

    virtual void visitStart (S_msrPartNameDisplay& elt);
    virtual void visitStart (S_msrPartAbbreviationDisplay& elt);

    virtual void visitStart (S_msrWords& elt);
    virtual void visitEnd   (S_msrWords& elt);
*/
    // tempos
    virtual void visitStart (S_msrTempo& elt);
    virtual void visitEnd   (S_msrTempo& elt);
/*
    virtual void visitStart (S_msrRehearsal& elt);
    virtual void visitEnd   (S_msrRehearsal& elt);
*/
    // segments
    virtual void visitStart (S_msrSegment& elt);
    virtual void visitEnd   (S_msrSegment& elt);

    // measures
    virtual void visitStart (S_msrMeasure& elt);
    virtual void visitEnd   (S_msrMeasure& elt);

    // print layouts
    virtual void visitStart (S_msrPrintLayout& elt);
    virtual void visitEnd   (S_msrPrintLayout& elt);

    // chords
    virtual void visitStart (S_msrChord& elt);
    virtual void visitEnd   (S_msrChord& elt);

    // tuplets
    virtual void visitStart (S_msrTuplet& elt);
    virtual void visitEnd   (S_msrTuplet& elt);

/*
    virtual void visitStart (S_msrHarmony& elt);
    virtual void visitEnd   (S_msrHarmony& elt);
    virtual void visitStart (S_msrHarmonyDegree& elt);

    virtual void visitStart (S_msrFrame& elt);

    virtual void visitStart (S_msrFiguredBass& elt);
    virtual void visitEnd   (S_msrFiguredBass& elt);
    virtual void visitStart (S_msrFigure& elt);
*/

/*
    virtual void visitStart (S_msrArticulation& elt);
    virtual void visitEnd   (S_msrArticulation& elt);

    virtual void visitStart (S_msrFermata& elt);

    virtual void visitStart (S_msrArpeggiato& elt);
    virtual void visitStart (S_msrNonArpeggiato& elt);

    virtual void visitStart (S_msrTechnical& elt);
    virtual void visitEnd   (S_msrTechnical& elt);

    virtual void visitStart (S_msrTechnicalWithInteger& elt);
    virtual void visitEnd   (S_msrTechnicalWithInteger& elt);

    virtual void visitStart (S_msrTechnicalWithFloat& elt);
    virtual void visitEnd   (S_msrTechnicalWithFloat& elt);

    virtual void visitStart (S_msrTechnicalWithString& elt);
    virtual void visitEnd   (S_msrTechnicalWithString& elt);

    virtual void visitStart (S_msrOrnament& elt);
    virtual void visitEnd   (S_msrOrnament& elt);

    virtual void visitStart (S_msrSpanner& elt);
    virtual void visitEnd   (S_msrSpanner& elt);

    virtual void visitStart (S_msrGlissando& elt);
    virtual void visitEnd   (S_msrGlissando& elt);

    virtual void visitStart (S_msrSlide& elt);
    virtual void visitEnd   (S_msrSlide& elt);

    virtual void visitStart (S_msrSingleTremolo& elt);
    virtual void visitEnd   (S_msrSingleTremolo& elt);

    virtual void visitStart (S_msrDoubleTremolo& elt);
    virtual void visitEnd   (S_msrDoubleTremolo& elt);

    virtual void visitStart (S_msrDynamics& elt);
    virtual void visitEnd   (S_msrDynamics& elt);

    virtual void visitStart (S_msrOtherDynamics& elt);
    virtual void visitEnd   (S_msrOtherDynamics& elt);

    virtual void visitStart (S_msrSlash& elt);

    virtual void visitStart (S_msrWedge& elt);
    virtual void visitEnd   (S_msrWedge& elt);
*/

    virtual void visitStart (S_msrGraceNotesGroup& elt);
    virtual void visitEnd   (S_msrGraceNotesGroup& elt);

    virtual void visitStart (S_msrNote& elt);
    virtual void visitEnd   (S_msrNote& elt);
/*
    virtual void visitStart (S_msrOctaveShift& elt);
    virtual void visitEnd   (S_msrOctaveShift& elt);

    virtual void visitStart (S_msrAccordionRegistration& elt);

    virtual void visitStart (S_msrHarpPedalsTuning& elt);

    virtual void visitStart (S_msrStem& elt);
    virtual void visitEnd   (S_msrStem& elt);

    virtual void visitStart (S_msrBeam& elt);
    virtual void visitEnd   (S_msrBeam& elt);

    virtual void visitStart (S_msrTie& elt);
    virtual void visitEnd   (S_msrTie& elt);

    virtual void visitStart (S_msrSlur& elt);
    virtual void visitEnd   (S_msrSlur& elt);

    virtual void visitStart (S_msrLigature& elt);
    virtual void visitEnd   (S_msrLigature& elt);
*/
    virtual void visitStart (S_msrBarline& elt);
    virtual void visitEnd   (S_msrBarline& elt);
/*
    virtual void visitStart (S_msrSegno& elt);
    virtual void visitStart (S_msrDalSegno& elt);
    virtual void visitStart (S_msrCoda& elt);

    virtual void visitStart (S_msrEyeGlasses& elt);
    virtual void visitStart (S_msrPedal& elt);
    virtual void visitStart (S_msrDamp& elt);
    virtual void visitStart (S_msrDampAll& elt);
    virtual void visitStart (S_msrScordatura& elt);

    virtual void visitStart (S_msrBarCheck& elt);
    virtual void visitEnd   (S_msrBarCheck& elt);
    virtual void visitStart (S_msrBarNumberCheck& elt);
    virtual void visitEnd   (S_msrBarNumberCheck& elt);

    virtual void visitStart (S_msrLineBreak& elt);
    virtual void visitEnd   (S_msrLineBreak& elt);

    virtual void visitStart (S_msrPageBreak& elt);
    virtual void visitEnd   (S_msrPageBreak& elt);

    virtual void visitStart (S_msrRepeat& elt);
    virtual void visitEnd   (S_msrRepeat& elt);
    virtual void visitStart (S_msrRepeatCommonPart& elt);
    virtual void visitEnd   (S_msrRepeatCommonPart& elt);
    virtual void visitStart (S_msrRepeatEnding& elt);
    virtual void visitEnd   (S_msrRepeatEnding& elt);

    virtual void visitStart (S_msrMeasuresRepeat& elt);
    virtual void visitEnd   (S_msrMeasuresRepeat& elt);
    virtual void visitStart (S_msrMeasuresRepeatPattern& elt);
    virtual void visitEnd   (S_msrMeasuresRepeatPattern& elt);
    virtual void visitStart (S_msrMeasuresRepeatReplicas& elt);
    virtual void visitEnd   (S_msrMeasuresRepeatReplicas& elt);

    virtual void visitStart (S_msrRestMeasures& elt);
    virtual void visitEnd   (S_msrRestMeasures& elt);
    virtual void visitStart (S_msrRestMeasuresContents& elt);
    virtual void visitEnd   (S_msrRestMeasuresContents& elt);
*/

    // scaling

    virtual void visitStart (S_msrScaling& elt);
    virtual void visitEnd   (S_msrScaling& elt);

    // layout

    virtual void visitStart (S_msrPageLayout& elt);
    virtual void visitEnd   (S_msrPageLayout& elt);

    virtual void visitStart (S_msrSystemLayout& elt);
    virtual void visitEnd   (S_msrSystemLayout& elt);

    virtual void visitStart (S_msrStaffLayout& elt);
    virtual void visitEnd   (S_msrStaffLayout& elt);

    virtual void visitStart (S_msrMeasureLayout& elt);
    virtual void visitEnd   (S_msrMeasureLayout& elt);

    // appearance

    virtual void visitStart (S_msrAppearance& elt);
    virtual void visitEnd   (S_msrAppearance& elt);

/*
    virtual void visitStart (S_msrMidiTempo& elt);
    virtual void visitEnd   (S_msrMidiTempo& elt);
*/

  private:

    indentedOstream&          fLogOutputStream;


    // the MSR score we're visiting
    // ------------------------------------------------------
    S_msrScore                fVisitedMsrScore;


    // the mxmltree we're building
    // ------------------------------------------------------
    Sxmlelement               fScorePartWiseElement;


    // scaling
    // ------------------------------------------------------
    float                     fMillimeters;
    float                     fTenths;

    Sxmlelement               fScoreDefaultsScalingElement;


    // work
    // ------------------------------------------------------

    Sxmlelement               fScoreWorkElement;

    void                      appendSubElementToScoreWork (
                                Sxmlelement elem);


    // movement number
    // ------------------------------------------------------

    Sxmlelement               fScoreMovementNumberElement;


    // movement title
    // ------------------------------------------------------

    Sxmlelement               fScoreMovementTitleElement;


    // identification
    // ------------------------------------------------------

    Sxmlelement               fScoreIdentificationElement;

/*
<!ELEMENT identification (creator*, rights*, encoding?,
	source?, relation*, miscellaneous?)>
*/

    // identification creator
    list<Sxmlelement>         fComposersElementsList;
    list<Sxmlelement>         fLyricistsElementsList;
    list<Sxmlelement>         fArrangersElementsList;

    list<Sxmlelement>         fPoetsElementsList;
    list<Sxmlelement>         fTranslatorsElementsList;
    list<Sxmlelement>         fArtistsElementsList;

    list<Sxmlelement>         fSoftwaresElementsList;

    // identification rights
    list<Sxmlelement>         fRightsElementsList;


    // identification encoding
    Sxmlelement               fScoreIdentificationEncodingElement;

/* JMI to be handled
<!ELEMENT encoding ((encoding-date | encoder |
	encoding-description | supports)*)>
<!ELEMENT encoding-date %yyyy-mm-dd;>
<!ELEMENT encoder (#PCDATA)>
<!ATTLIST encoder
    type CDATA #IMPLIED
>
<!ELEMENT encoding-description (#PCDATA)>
<!ELEMENT supports EMPTY>
<!ATTLIST supports
    type %yes-no; #REQUIRED
    element CDATA #REQUIRED
    attribute CDATA #IMPLIED
    value CDATA #IMPLIED
*/

    // identification source
    Sxmlelement               fScoreIdentificationSourceElement;

    // identification relation
    Sxmlelement               fScoreIdentificationRelationElement;

    // identification miscellaneous
    Sxmlelement               fIdentificationMiscellaneousElement;

    void                      appendSubElementToScoreIdentification (
                                Sxmlelement elem);

    void                      appendSubElementToScoreIdentificationEncoding (
                                Sxmlelement elem);

    // defaults
    // ------------------------------------------------------

    Sxmlelement               fScoreDefaultsElement;

    // layout
    // ------------------------------------------------------

    // page layout
    Sxmlelement               fScoreDefaultsPageLayoutElement;

    void                      appendSubElementToScoreDefaults (
                                Sxmlelement elem);

    void                      appendSubElementToScoreDefaultsPageLayout (
                                Sxmlelement elem);

    void                      appendPageMarginsElementToScoreDefaultsPageLayout (
                                S_msrMarginsGroup marginsGroup);

    void                      populatePageMarginsElement (
                                Sxmlelement       elem,
                                S_msrMarginsGroup marginsGroup);

    // system layout
    Sxmlelement               fScoreDefaultsSystemLayoutElement;

    void                      appendSystemMarginsElementToScoreDefaultsSystemLayout (
                                S_msrSystemLayout systemLayout);

    void                      populateSystemMarginsElement (
                                Sxmlelement       elem,
                                S_msrSystemLayout systemLayout);

    // staff layouts
    list<Sxmlelement>         fScoreDefaultsStaffLayoutElementsList;

    // measure print layout
    Sxmlelement               fCurrentMeasurePrintLayoutElement;

    // appearance
    // ------------------------------------------------------

    Sxmlelement               fScoreDefaultsAppearanceElement;

  void                        populateAppearanceLineWidths (
                                Sxmlelement                 appearanceElement,
                                const list<S_msrLineWidth>& lineWidthsList);

  void                        populateAppearanceNoteSizes (
                                Sxmlelement                appearanceElement,
                                const list<S_msrNoteSize>& noteSizesList);

    // credits
    // ------------------------------------------------------

    Sxmlelement               fCurrentScoreCreditElement;

    list<Sxmlelement>         fPendingScoreCreditElementsList;

    // part list
    // ------------------------------------------------------

    Sxmlelement               fScorePartListElement;


    // the part direction element
    // ------------------------------------------------------
// JMI    Sxmlelement               fCurrentPartDirection;

    void                      appendSubElementToMeasureDirection (
                                Sxmlelement      elem,
                                msrPlacementKind placementKind);

    // divisions
    // ------------------------------------------------------
    rational                  fPartShortestNoteDuration;
    msrTupletFactor           fPartShortestNoteTupletFactor;

    int                       fDivisionsPerQuarterNote;
    int                       fDivisionsMultiplyingFactor;

    bool                      fPartDivisionsElementHasToBeAppended;

    // its header
    // ------------------------------------------------------


    // score
    // ------------------------------------------------------


    // header
    // ------------------------------------------------------


    // part groups
    // ------------------------------------------------------

    // partGroup's can be nested, hence this stack
    // the current partGroup is the top of the stack
    stack<Sxmlelement>        fPartGroupElementsStack;

    // current part
    // ------------------------------------------------------

    Sxmlelement               fCurrentPartElement;

    list<Sxmlelement>         fPendingPartElementsList;

/*
    // staff details
    // ------------------------------------------------------

    // staves
    // ------------------------------------------------------

    // voices
    // ------------------------------------------------------

    // dal segnos
    // ------------------------------------------------------

    // JMI ???

    // harmonies
    // ------------------------------------------------------

    // figured bass
    // ------------------------------------------------------

    // segments
    // ------------------------------------------------------
*/

    // measures
    // ------------------------------------------------------
    Sxmlelement               fCurrentMeasureElement;

    void                      appendNoteSubElementToMesureIfRelevant (
                                S_msrNote note);

    void                      appendNoteSubElementToMeasure (
                                S_msrNote   note,
                                Sxmlelement elem);

    void                      appendOtherSubElementToMeasure (
                                Sxmlelement elem);

    // the measure attributes element
    // ------------------------------------------------------
    Sxmlelement               fCurrentMeasureAttributesElement;

    void                      appendSubElementToMeasureAttributes (
                                Sxmlelement elem);

/*
    // full measure rests compression
    S_msrMeasure              fCurrentRestMeasure;
    S_msrRestMeasures         fCurrentRestMeasures;

    // bar checks
    // ------------------------------------------------------
*/

    // notes
    // ------------------------------------------------------
    /*
*/

    // print layouts
    // ------------------------------------------------------

    Sxmlelement               fCurrentPrintElement;

    bool                      fOnGoingPrintElement;

    // directions
    // ------------------------------------------------------
    void                      appendSubElementsToNoteDirections (S_msrNote note);
    void                      appendNoteWedges (S_msrNote note);
    void                      appendNoteDynamics (S_msrNote note);

    // spanners
    // ------------------------------------------------------
    void                      appendNoteSpannersBeforeNoteElement (
                                S_msrNote note);
    void                      appendNoteSpannersAfterNoteElement (
                                S_msrNote note);

    void                      appendBasicSubElementsToNote (
                                S_msrNote note);

    // voice
    // ------------------------------------------------------
    void                      appendVoiceSubElementToNoteIfRelevant (
                                S_msrNote note);

    // duration
    // ------------------------------------------------------
    void                      appendDurationSubElementToNoteIfRelevant (
                                S_msrNote note);

    // tuplets
    // ------------------------------------------------------
    void                      appendTimeModificationSubElementToNoteIfRelevant (
                                S_msrNote note);

    void                      appendNoteTupletIfRelevant (S_msrNote note);

    // notes
    // ------------------------------------------------------
    Sxmlelement               fCurrentNoteElement;

    void                      appendNoteSubElementToMesure (S_msrNote note);

    bool                      fCurrentNoteElementAwaitsGraceNotes;
    S_msrNote                 fPendingNoteAwaitingGraceNotes;
    Sxmlelement               fPendingNoteElement;

    // stems
    // ------------------------------------------------------
    void                      appendStemToNote (S_msrNote note);

    // beams
    // ------------------------------------------------------
    void                      appendBeamsToNote (S_msrNote note);

    // note notations
    // ------------------------------------------------------
    Sxmlelement               fCurrentNoteNotationsElement;

    void                      appendNoteNotationsToNote (S_msrNote note);

    void                      appendSubElementToNoteNotations (
                                Sxmlelement      elem,
                                msrPlacementKind placementKind);

    // lyrics
    // ------------------------------------------------------
    void                      appendNoteLyricsToNote (S_msrNote note);

    // note notations ornaments
    // ------------------------------------------------------
    Sxmlelement               fCurrentNoteNotationsOrnamentsElement;

    void                      appendNoteOrnaments (S_msrNote note);

    void                      appendSubElementToNoteNotationsOrnaments (
                                Sxmlelement      elem,
                                msrPlacementKind placementKind);

    // note notations articulations
    // ------------------------------------------------------
    Sxmlelement               fCurrentNoteNotationsArticulationsElement;

    void                      appendNoteArticulations (S_msrNote note);

    void                      appendSubElementToNoteNotationsArticulations (
                                Sxmlelement      elem,
                                msrPlacementKind placementKind);

    // note notations technicals
    // ------------------------------------------------------

    void                      appendNoteTechnicals (S_msrNote note);
    void                      appendNoteTechnicalWithIntegers (S_msrNote note);
    void                      appendNoteTechnicalWithFloats (S_msrNote note);
    void                      appendNoteTechnicalWithStrings (S_msrNote note);

    Sxmlelement               fCurrentNoteNotationsTechnicalsElement;

    void                      appendSubElementToNoteNotationsTechnicals (
                                Sxmlelement      elem,
                                msrPlacementKind placementKind);

    // note ties
    // ------------------------------------------------------

    void                      appendNoteTieIfAny (S_msrNote note);

    //note slurs
    // ------------------------------------------------------

    void                      appendNoteSlursIfAny (S_msrNote note);

/*
    // glissandos
    // ------------------------------------------------------


    // slides
    // ------------------------------------------------------


    // double tremolos
    // ------------------------------------------------------


    // stems
    // ------------------------------------------------------


    // grace notes
    // ------------------------------------------------------

    // afterGraceNotes optimisation
    S_msrAfterGraceNotesGroup fPendingAfterGraceNotesGroup;
    S_msrElement              fCurrentAfterGraceNotesGroupElement;
*/

    // chords
    // ------------------------------------------------------
    Sxmlelement               fPendingChordStartComment;

/*
    // stanzas
    // ------------------------------------------------------


    // syllables
    // ------------------------------------------------------


    // current ongoing values display
    // ------------------------------------------------------
    void                      displayCurrentOnGoingValues ();
*/

    string                    msrLengthAsTenths (
                                msrLength length);
    string                    S_msrLengthAsTenths (
                                S_msrLength length);

    string                    msrPlacementKindAsMusicXMLString (
                                msrPlacementKind placementKind);

    string                    msrSpannerTypeKindAsMusicXMLString (
                                msrSpannerTypeKind spannerTypeKind);
};


}


#endif
