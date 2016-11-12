/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __xml2MsrScoreVisitor__
#define __xml2MsrScoreVisitor__

#include <map>
#include <vector>

#include "msr.h"

#include "musicxml2msr.h"

namespace MusicXML2
{

/*!
\addtogroup visitors
@{
*/

/*!
\brief Produces a summary of a MusicXML part.

  A part summary consists in 
  - a number of staves
  - a map that associates each stave with the corresponding number of notes
  - a map that associates each stave with the corresponding voices and notes
  - a map that associates each stave with the correcponding lyricss
*/
class EXP xml2MsrScoreVisitor : 

/*
  <part-list>
  
    <part-group number="1" type="start">
      <group-symbol default-x="-7">bracket</group-symbol>
      <group-barline>yes</group-barline>
    </part-group>
    
    <score-part id="P1">
      <part-name>Piccolo</part-name>
      <part-abbreviation>Picc.</part-abbreviation>
      <score-instrument id="P1-I18">
        <instrument-name>Picc.</instrument-name>
      </score-instrument>
      <midi-instrument id="P1-I18">
        <midi-channel>1</midi-channel>
        <midi-program>73</midi-program>
      </midi-instrument>
    </score-part>
    
    <part-group number="2" type="start">
      <group-name>1
2</group-name>
      <group-barline>yes</group-barline>
    </part-group>
*/

  public visitor<S_part_list>,
  
  public visitor<S_part_group>,
  public visitor<S_group_name>,
  public visitor<S_group_abbreviation>,
  public visitor<S_group_symbol>,
  public visitor<S_group_barline>,
  
  public visitor<S_score_part>,
  public visitor<S_part_name>,
  public visitor<S_part_abbreviation>,
  public visitor<S_instrument_name>,
  
  public visitor<S_part>,
  public visitor<S_divisions>,

  public visitor<S_staves>,
  public visitor<S_staff>,

  public visitor<S_voice>,

  public visitor<S_backup>,
  public visitor<S_forward>,
  
  public visitor<S_time>,
  public visitor<S_beats>,
  public visitor<S_beat_type>,
  public visitor<S_senza_misura>,

  public visitor<S_key>,
  public visitor<S_cancel>,
  public visitor<S_fifths>,
  public visitor<S_mode>,

  public visitor<S_clef>,
  public visitor<S_sign>,
  public visitor<S_line>,
  public visitor<S_clef_octave_change>,

  public visitor<S_direction>,
  public visitor<S_direction_type>,
  public visitor<S_words>,
  
  public visitor<S_metronome>,
  public visitor<S_beat_unit>,
  public visitor<S_beat_unit_dot>,
  public visitor<S_per_minute>,

  public visitor<S_tied>,
  public visitor<S_slur>,
  public visitor<S_beam>,

  public visitor<S_lyric>,
  public visitor<S_syllabic>,
  public visitor<S_text>,
  public visitor<S_elision>,

  public visitor<S_measure>,
  public visitor<S_print>,
  
  public visitor<S_barline>,
  public visitor<S_bar_style>,
  public visitor<S_repeat>,
  public visitor<S_ending>,

  public visitor<S_note>,
  public visitor<S_step>,
  public visitor<S_alter>,
  public visitor<S_octave>,
  public visitor<S_duration>,
  public visitor<S_dot>,
  
  public visitor<S_staccato>,
  public visitor<S_staccatissimo>,
  public visitor<S_fermata>,

  public visitor<S_f>,
  public visitor<S_ff>,
  public visitor<S_fff>,
  public visitor<S_ffff>,
  public visitor<S_fffff>,
  public visitor<S_ffffff>,

  public visitor<S_p>,
  public visitor<S_pp>,
  public visitor<S_ppp>,
  public visitor<S_pppp>,
  public visitor<S_ppppp>,
  public visitor<S_pppppp>,

  public visitor<S_wedge>,
  
  public visitor<S_grace>,

  public visitor<S_type>,
  public visitor<S_stem>,
  
  public visitor<S_rest>,
  
  public visitor<S_unpitched>,
  public visitor<S_display_step>,
  public visitor<S_display_octave>,

  public visitor<S_chord>,

  public visitor<S_actual_notes>,
  public visitor<S_normal_notes>,
  public visitor<S_normal_type>,
  public visitor<S_tuplet>
{
  public:
  
    xml2MsrScoreVisitor (
        S_msrOptions& msrOpts);
    virtual ~xml2MsrScoreVisitor ();

    S_msrScore buildMsrScoreFromXMLElementTree (
        const Sxmlelement& xmlTree);

    S_msrScore getMSRScore () const
        { return fMsrScore; };
    
  protected:
      
    virtual void visitStart ( S_part_list& elt);
    virtual void visitEnd   ( S_part_list& elt);
    
    virtual void visitStart ( S_part_group& elt);
    virtual void visitEnd   ( S_part_group& elt);
    virtual void visitStart ( S_group_name& elt);
    virtual void visitStart ( S_group_abbreviation& elt);
    virtual void visitStart ( S_group_symbol& elt);
    virtual void visitStart ( S_group_barline& elt);
    
    virtual void visitStart ( S_score_part& elt);
    virtual void visitEnd   ( S_score_part& elt);
    virtual void visitStart ( S_part_name& elt);
    virtual void visitStart ( S_part_abbreviation& elt);
    virtual void visitStart ( S_instrument_name& elt);
    
    virtual void visitStart ( S_part& elt);
    virtual void visitEnd   ( S_part& elt);
    virtual void visitStart ( S_divisions& elt);
  
    virtual void visitStart ( S_staves& elt);
    
    virtual void visitStart ( S_staff& elt);
    
    virtual void visitStart ( S_voice& elt );
    
    virtual void visitStart ( S_backup& elt );
    virtual void visitEnd   ( S_backup& elt );
    virtual void visitStart ( S_forward& elt );
    virtual void visitEnd   ( S_forward& elt );
    
    virtual void visitStart ( S_time& elt );
    virtual void visitEnd   ( S_time& elt );
    virtual void visitStart ( S_beats& elt );
    virtual void visitStart ( S_beat_type& elt );
    virtual void visitStart ( S_senza_misura& elt );

    virtual void visitStart ( S_key& elt );
    virtual void visitEnd   ( S_key& elt );
    virtual void visitStart ( S_cancel& elt );
    virtual void visitStart ( S_fifths& elt );
    virtual void visitStart ( S_mode& elt );

    virtual void visitStart ( S_clef& elt );
    virtual void visitEnd   ( S_clef& elt );
    virtual void visitStart ( S_sign& elt );
    virtual void visitStart ( S_line& elt );
    virtual void visitStart ( S_clef_octave_change& elt );

    virtual void visitStart ( S_direction& elt );
    virtual void visitEnd   ( S_direction& elt );
    virtual void visitStart ( S_direction_type& elt );
    virtual void visitStart ( S_words& elt );
    
    virtual void visitStart ( S_metronome& elt );
    virtual void visitEnd   ( S_metronome& elt );
    virtual void visitStart ( S_beat_unit& elt );
    virtual void visitStart ( S_beat_unit_dot& elt );
    virtual void visitStart ( S_per_minute& elt );

    virtual void visitStart ( S_tied& elt);
    virtual void visitStart ( S_slur& elt);
    virtual void visitStart ( S_beam& elt);

    virtual void visitStart ( S_lyric& elt);
    virtual void visitEnd   ( S_lyric& elt);
    virtual void visitStart ( S_syllabic& elt);
    virtual void visitEnd   ( S_text& elt );
    virtual void visitEnd   ( S_elision& elt );
    
    virtual void visitStart ( S_measure& elt);
    virtual void visitEnd   ( S_measure& elt);
    
    virtual void visitStart ( S_print& elt);
    
    virtual void visitStart ( S_barline& elt);
    virtual void visitStart ( S_bar_style& elt);
    virtual void visitStart ( S_repeat& elt);
    virtual void visitStart ( S_ending& elt);
    virtual void visitEnd   ( S_barline& elt);

    virtual void visitStart ( S_note& elt);
    virtual void visitStart ( S_step& elt);
    virtual void visitStart ( S_alter& elt);
    virtual void visitStart ( S_octave& elt);
    virtual void visitStart ( S_duration& elt);
    virtual void visitStart ( S_dot& elt );
    
    virtual void visitStart ( S_staccato& elt );
    virtual void visitStart ( S_staccatissimo& elt );
    virtual void visitStart ( S_fermata& elt );
    
    virtual void visitStart ( S_f& elt);
    virtual void visitStart ( S_ff& elt);
    virtual void visitStart ( S_fff& elt);
    virtual void visitStart ( S_ffff& elt);
    virtual void visitStart ( S_fffff& elt);
    virtual void visitStart ( S_ffffff& elt);

    virtual void visitStart ( S_p& elt);
    virtual void visitStart ( S_pp& elt);
    virtual void visitStart ( S_ppp& elt);
    virtual void visitStart ( S_pppp& elt);
    virtual void visitStart ( S_ppppp& elt);
    virtual void visitStart ( S_pppppp& elt);

    virtual void visitStart ( S_wedge& elt);

    virtual void visitStart ( S_grace& elt );
    
//    virtual void visitStart ( S_voice& elt);
    virtual void visitStart ( S_type& elt);
    virtual void visitStart ( S_stem& elt);
//    virtual void visitStart ( S_staff& elt);
    virtual void visitEnd   ( S_note& elt);

    virtual void visitStart ( S_rest& elt);
    
    virtual void visitEnd   ( S_unpitched& elt);
    virtual void visitStart ( S_display_step& elt);
    virtual void visitStart ( S_display_octave& elt);

    virtual void visitStart ( S_chord& elt);
    
    virtual void visitStart ( S_actual_notes& elt);
    virtual void visitStart ( S_normal_notes& elt);
    virtual void visitStart ( S_normal_type& elt);
    virtual void visitStart ( S_tuplet& elt);

  private:
                     
    S_msrOptions   fMsrOptions;

    // the MSR score we're building
    // ------------------------------------------------------
    S_msrScore              fMsrScore;

    // part group handling
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

    // part group handling
    // ------------------------------------------------------
    int                       fCurrentPartgroupNumber;
    string                    fCurrentPartgroupType;
    string                    fCurrentPartgroupName;
    string                    fCurrentPartgroupAbbreviation;
    string                    fCurrentPartgroupSymbol;
    int                       fCurrentPartgroupSymbolDefaultX;
    string                    fCurrentPartgroupBarline;

    // an implicit part group has to be created
    // if none is specified in the MusicXML data,
    // in which case a part group "stop" has to be forced later
    S_msrPartgroup            fImplicitPartgroup;
    S_msrPartgroup            createImplicitMSRPartgroup (
                                int inputLineNumber);
    
    // part groups numbers can be re-used, they're no identifier
    // we use a map to access them by part group number
    map<int, S_msrPartgroup>  fPartgroupsMap;
    S_msrPartgroup            fetchPartgroupInThisVisitor (
                                int partGroupNumber);

    // MusicXML allows part groups to overlap,
    // we use a list in which part groups are orderd by
    // increasing part group <default-x>
    // (all of them they are negative)    
    // the current part group is the front of the list
    
    list<S_msrPartgroup>      fPartgroupsList;
    void                      showPartgroupsData (string context);
    void                      handlePartgroupStart (
                                int
                                    inputLineNumber,
                                msrPartgroup::msrPartgroupSymbolKind
                                    partGroupSymbol,
                                bool
                                    partGroupBarline);
    void                      handlePartgroupStop (
                                int inputLineNumber);

    // staff handling
    // ------------------------------------------------------
    int                       fCurrentStaffNumber;
    S_msrStaff                fCurrentStaff;

    // part handling
    // ------------------------------------------------------
    string                    fCurrentPartMusicXMLName;
    string                    fCurrentPartName;
    string                    fCurrentPartAbbreviation;
    string                    fCurrentPartInstrumentName;

    map<string, S_msrPart>    fPartsMap;
    S_msrPart                 fCurrentPart;

    // voice handling
    // ------------------------------------------------------
    int                       fCurrentVoiceNumber;
    S_msrVoice                fCurrentVoice;
    
    // key handling
    // ------------------------------------------------------
    int                       fCurrentKeyStaffNumber;
    int                       fCurrentFifths;
    int                       fCurrentCancel;
    string                    fCurrentMode;

    // clef handling
    // ------------------------------------------------------
    string                    fCurrentClefSign;
    int                       fCurrentClefLine;
    int                       fCurrentClefOctaveChange; // JMI
    int                       fCurrentClefStaffNumber;

    // direction handling
    // ------------------------------------------------------
    string fCurrentDirectionPlacement;
    string fCurrentDirectionWords;
    bool   fOnGoingDirection;
  
    // metronome handling
    // ------------------------------------------------------
    vector<musicXMLBeatData>
                              fBeatsData; // JMI
    int                       fPerMinute;
    musicXMLBeatData          fCurrentBeat;
    bool                      fParentheses;

    // time handling
    // ------------------------------------------------------
    int                       fCurrentTimeStaffNumber;
    string                    fCurrentTimeSymbol;
    int                       fCurrentTimeBeats;
    int                       fCurrentTimeBeatType;
    bool                      fCurrentTimeSenzaMisura;

    // lyrics handling
    // ------------------------------------------------------
    // the last sysllabic spec met (single, begin, middle or end)
    string                    fCurrentSyllabic;
    msrLyricschunk::msrLyricschunkType
                              fCurrentLyricschunkType;
    // the last lyrics fragment met
    string                    fCurrentText;
    bool                      fCurrentElision;
    
    int                       fCurrentLyricsNumber;
    bool                      fCurrentNoteHasLyrics;
    S_msrLyrics               fCurrentLyrics;
    bool                      fCurrentLyricsHasText;
    void                      handleLyricsText (
                                int inputLineNumber);

    // repeat handling
    // ------------------------------------------------------
    string                    fCurrentBarlineLocation;
    string                    fCurrentBarStyle;
    string                    fCurrentRepeatDirection;
    string                    fCurrentEndingType;
    int                       fCurrentEndingNumber;

    // dividing quater notes in MusicXML
    // ------------------------------------------------------
    int                       fCurrentMusicXMLDivisions;

    // description of the current MusicXML note
    musicXMLNoteData          fMusicXMLNoteData;

    // unpitched notes handling
    // ------------------------------------------------------
    char                      fDisplayStep;
    int                       fDisplayOctave;

    // stem handling
    // ------------------------------------------------------
    enum StemDirection { kStemNeutral, kStemUp, kStemDown };
    
    string                    fCurrentStem;
    StemDirection             fCurrentStemDirection;

    // beam handling
    // ------------------------------------------------------
    string                    fCurrentBeam;
    int                       fCurrentBeamNumber; 

    // note/rest handling
    // ------------------------------------------------------
    void                      handleStandaloneNoteOrRest (
                                S_msrNote newNote);

    // ------------------------------------------------------
    // chord handling
    S_msrChord                fCurrentChord; // cannot be local to a method? JMI
    bool                      fOnGoingChord;
    S_msrChord                createChordFromCurrentNote ();
    void                      handleNoteBelongingToAChord (
                                S_msrNote newNote);
    
    // ------------------------------------------------------
    // articulations handling
    list<S_msrArticulation>   fCurrentArticulations;
    
    // dynamics and wedges remain pending until the next note
    // (they precede the note in MusicXML but follow it in LilyPond)
    list<S_msrDynamics>       fPendingDynamics;
    list<S_msrWedge>          fPendingWedges;
    void                      attachPendingDynamicsAndWedgesToNote (
                                S_msrNote note);
       
    // description of the current MSR note
    // ------------------------------------------------------
    string                    fCurrentNoteType;
    S_msrNote                 fCurrentNote;
    bool                      fOnGoingNote;

    // tuplet handling
     // ------------------------------------------------------
    int                       fCurrentActualNotes;
    int                       fCurrentNormalNotes;
    string                    fCurrentNormalNoteType;
    // embedded tuplets are numbered 1, 2, ...
    int                       fCurrentTupletNumber;
    msrTuplet::TupletKind     fCurrentTupletKind;
    // remains true until a S_tuplet of type "stop" is met
    bool                      fOnGoingTuplet;
    stack<S_msrTuplet>        fCurrentTupletsStack;

    void                      createTupletFromItsecondNote (
                                S_msrNote note);
    void                      finalizeTuplet (
                                S_msrNote note);
    void                      handleNoteBelongingToATuplet (
                                S_msrNote newNote);
         
    // another name for fCurrentNote, fCurrentChord, fCurrentTuplet
    // and the like
// JMI    S_msrElement           fCurrentElement;

    // ties handling
    // ------------------------------------------------------
    string                    fCurrentTiedType;
    string                    fCurrentTiedOrientation;

    // slurs handling
    // ------------------------------------------------------
    int                       fCurrentSlurNumber;
    string                    fCurrentSlurType;
    string                    fCurrentSlurPlacement;
    msrSlur::msrSlurKind      fCurrentSlurKind;
    bool                      fOnGoingSlur;

    // backup handling
    // ------------------------------------------------------
    int                       fCurrentBackupDuration;
    bool                      fOnGoingBackup;

    // forward handling
    // ------------------------------------------------------
    int                       fCurrentForwardDuration;
    int                       fCurrentForwardVoiceNumber;
    int                       fCurrentForwardStaffNumber;
    bool                      fOnGoingForward;

};

/*
    The part-group element indicates groupings of parts in the
    score, usually indicated by braces and brackets. Braces
    that are used for multi-staff parts should be defined in
    the attributes element for that part. The part-group start
    element appears before the first score-part in the group.
    The part-group stop element appears after the last
    score-part in the group.
    
    The number attribute is used to distinguish overlapping
    and nested part-groups, not the sequence of groups. As
    with parts, groups can have a name and abbreviation.
    Formatting attributes for group-name and group-abbreviation
    are deprecated in Version 2.0 in favor of the new
    group-name-display and group-abbreviation-display elements.
    Formatting specified in the group-name-display and
    group-abbreviation-display elements overrides formatting
    specified in the group-name and group-abbreviation
    elements, respectively.

    The group-symbol element indicates how the symbol for
    a group is indicated in the score. Values include none,
    brace, line, bracket, and square; the default is none.
    The group-barline element indicates if the group should
    have common barlines. Values can be yes, no, or
    Mensurstrich. The group-time element indicates that the
    displayed time signatures should stretch across all parts
    and staves in the group. Values for the child elements
    are ignored at the stop of a group.

    A part-group element is not needed for a single multi-staff
    part. By default, multi-staff parts include a brace symbol
    and (if appropriate given the bar-style) common barlines.
    The symbol formatting for a multi-staff part can be more
    fully specified using the part-symbol element, defined in
    the attributes.mod file.
*/

/*! @} */

}

#endif
