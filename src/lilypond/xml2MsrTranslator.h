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

#include "xml2Msr.h"

namespace MusicXML2
{

/*!
\addtogroup visitors
@{
*/

/*!
\brief Produces a summary of a MusicXML part.
*/
class EXP xml2MsrTranslator : 

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

  public visitor<S_work_number>,
  public visitor<S_work_title>,
  public visitor<S_movement_number>,
  public visitor<S_movement_title>,
  public visitor<S_creator>,
  public visitor<S_rights>,
  public visitor<S_software>,
  public visitor<S_encoding_date>,
  
  public visitor<S_millimeters>,
  public visitor<S_tenths>,
  public visitor<S_scaling>,
  
  public visitor<S_system_distance>,
  public visitor<S_top_system_distance>,
  
  public visitor<S_page_layout>,
  public visitor<S_page_height>,
  public visitor<S_page_width>,
  public visitor<S_left_margin>,
  public visitor<S_right_margin>,
  public visitor<S_top_margin>,
  public visitor<S_bottom_margin>,

// JMI  public visitor<S_instrument_name>,

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
  
  public visitor<S_staff_details>,
  public visitor<S_staff_type>,
  public visitor<S_staff_lines>,
  public visitor<S_staff_tuning>,
  public visitor<S_tuning_step>,
  public visitor<S_tuning_octave>,
  public visitor<S_tuning_alter>,
  public visitor<S_capo>,
  public visitor<S_staff_size>,

  public visitor<S_voice>,

  public visitor<S_backup>,
  public visitor<S_forward>,
  
  public visitor<S_clef>,
  public visitor<S_sign>,
  public visitor<S_line>,
  public visitor<S_clef_octave_change>,

  public visitor<S_key>,
  public visitor<S_cancel>,
  public visitor<S_fifths>,
  public visitor<S_mode>,

  public visitor<S_time>,
  public visitor<S_beats>,
  public visitor<S_beat_type>,
  public visitor<S_senza_misura>,

  public visitor<S_transpose>,
  public visitor<S_diatonic>,
  public visitor<S_chromatic>,

  public visitor<S_direction>,
  public visitor<S_direction_type>,
  public visitor<S_words>,
  public visitor<S_octave_shift>,
  
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
  
  public visitor<S_harmony>,
  public visitor<S_root_step>,
  public visitor<S_root_alter>,
  public visitor<S_kind>,
  public visitor<S_bass_step>,
  public visitor<S_bass_alter>,
  
  public visitor<S_degree_value>,
  public visitor<S_degree_alter>,
  public visitor<S_degree_type>,


  public visitor<S_measure>,
  public visitor<S_print>,
  
  public visitor<S_barline>,
  public visitor<S_segno>,
  public visitor<S_coda>,
  public visitor<S_eyeglasses>,
  public visitor<S_pedal>,
  public visitor<S_bar_style>,
  public visitor<S_repeat>,
  public visitor<S_ending>,

  public visitor<S_note>,
  public visitor<S_step>,
  public visitor<S_alter>,
  public visitor<S_octave>,
  public visitor<S_duration>,
  public visitor<S_dot>,
  
  public visitor<S_articulations>,
  public visitor<S_accent>,
  public visitor<S_breath_mark>,
  public visitor<S_caesura>,
  public visitor<S_spiccato>,
  public visitor<S_staccato>,
  public visitor<S_staccatissimo>,
  public visitor<S_stress>,
  public visitor<S_unstress>,
  public visitor<S_detached_legato>,
  public visitor<S_strong_accent>,
  public visitor<S_tenuto>,
  public visitor<S_fermata>,
  public visitor<S_arpeggiate>,
  public visitor<S_doit>,
  public visitor<S_falloff>,
  public visitor<S_plop>,
  public visitor<S_scoop>,

  public visitor<S_ornaments>,
  public visitor<S_trill_mark>,
  public visitor<S_wavy_line>,
  public visitor<S_turn>,
  public visitor<S_inverted_turn>,
  public visitor<S_delayed_turn>,
  public visitor<S_delayed_inverted_turn>,
  public visitor<S_vertical_turn>,
  public visitor<S_mordent>,
  public visitor<S_inverted_mordent>,
  public visitor<S_schleifer>,
  public visitor<S_shake>,
  public visitor<S_accidental_mark>,

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

  public visitor<S_fp>,
  public visitor<S_fz>,
  
  public visitor<S_rf>,
  public visitor<S_sf>,
  public visitor<S_rfz>,
  public visitor<S_sfz>,
  
  public visitor<S_sfp>,
  public visitor<S_sfpp>,
  public visitor<S_sffz>,

  public visitor<S_wedge>,
  
  public visitor<S_grace>,

  public visitor<S_type>,
  public visitor<S_stem>,
  
  public visitor<S_rest>,
  
  public visitor<S_unpitched>,
  public visitor<S_display_step>,
  public visitor<S_display_octave>,

  public visitor<S_chord>,

  public visitor<S_time_modification>,
  public visitor<S_actual_notes>,
  public visitor<S_normal_notes>,
  public visitor<S_normal_type>,
  public visitor<S_tuplet>,
  
  public visitor<S_rehearsal>
{
  public:
  
    xml2MsrTranslator (
      S_msrOptions& msrOpts);

    virtual ~xml2MsrTranslator ();

    S_msrScore buildMsrScoreFromXMLElementTree (
        const Sxmlelement& xmlTree);

    S_msrScore getMSRScore () const
        { return fMsrScore; };
    
  protected:
   
    virtual void visitStart ( S_work_number& elt);
    virtual void visitStart ( S_work_title& elt);
    virtual void visitStart ( S_movement_number& elt);
    virtual void visitStart ( S_movement_title& elt);
    virtual void visitStart ( S_creator& elt);
    virtual void visitStart ( S_rights& elt);
    virtual void visitStart ( S_software& elt);
    virtual void visitStart ( S_encoding_date& elt);

    virtual void visitStart ( S_millimeters& elt);
    virtual void visitStart ( S_tenths& elt);
    virtual void visitEnd   ( S_scaling& elt);

    virtual void visitStart ( S_system_distance& elt);
    virtual void visitStart ( S_top_system_distance& elt);

    virtual void visitStart ( S_page_layout& elt);
    virtual void visitEnd   ( S_page_layout& elt);
    virtual void visitStart ( S_page_height& elt);
    virtual void visitStart ( S_page_width& elt);
    virtual void visitStart ( S_left_margin& elt);
    virtual void visitStart ( S_right_margin& elt);
    virtual void visitStart ( S_top_margin& elt);
    virtual void visitStart ( S_bottom_margin& elt);

 // JMI   virtual void visitStart ( S_instrument_name& elt);
        
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
    
    virtual void visitStart ( S_staff_details& elt);
    virtual void visitEnd   ( S_staff_details& elt);
    virtual void visitStart ( S_staff_type& elt);
    virtual void visitStart ( S_staff_lines& elt);
    virtual void visitStart ( S_staff_tuning& elt);
    virtual void visitEnd   ( S_staff_tuning& elt);
    virtual void visitStart ( S_tuning_step& elt);
    virtual void visitStart ( S_tuning_octave& elt);
    virtual void visitStart ( S_tuning_alter& elt);
    virtual void visitStart ( S_capo& elt);
    virtual void visitStart ( S_staff_size& elt);

    virtual void visitStart ( S_voice& elt );
    
    virtual void visitStart ( S_backup& elt );
    virtual void visitEnd   ( S_backup& elt );
    virtual void visitStart ( S_forward& elt );
    virtual void visitEnd   ( S_forward& elt );
    
    virtual void visitStart ( S_clef& elt );
    virtual void visitEnd   ( S_clef& elt );
    virtual void visitStart ( S_sign& elt );
    virtual void visitStart ( S_line& elt );
    virtual void visitStart ( S_clef_octave_change& elt );

    virtual void visitStart ( S_key& elt );
    virtual void visitEnd   ( S_key& elt );
    virtual void visitStart ( S_cancel& elt );
    virtual void visitStart ( S_fifths& elt );
    virtual void visitStart ( S_mode& elt );

    virtual void visitStart ( S_time& elt );
    virtual void visitEnd   ( S_time& elt );
    virtual void visitStart ( S_beats& elt );
    virtual void visitStart ( S_beat_type& elt );
    virtual void visitStart ( S_senza_misura& elt );

    virtual void visitStart ( S_transpose& elt );
    virtual void visitEnd   ( S_transpose& elt );
    virtual void visitStart ( S_diatonic& elt );
    virtual void visitStart ( S_chromatic& elt );

    virtual void visitStart ( S_direction& elt );
    virtual void visitEnd   ( S_direction& elt );
    virtual void visitStart ( S_direction_type& elt );
    virtual void visitEnd   ( S_direction_type& elt );
    virtual void visitStart ( S_words& elt );
    virtual void visitStart ( S_octave_shift& elt );
    
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

    virtual void visitStart ( S_harmony& elt);
    virtual void visitEnd   ( S_harmony& elt);
    virtual void visitStart ( S_root_step& elt);
    virtual void visitStart ( S_root_alter& elt);
    virtual void visitStart ( S_kind& elt);
    virtual void visitStart ( S_bass_step& elt);
    virtual void visitStart ( S_bass_alter& elt);
    
    virtual void visitStart ( S_degree_value& elt);
    virtual void visitStart ( S_degree_alter& elt);
    virtual void visitStart ( S_degree_type& elt);

    virtual void visitStart ( S_measure& elt);
    virtual void visitEnd   ( S_measure& elt);
    
    virtual void visitStart ( S_print& elt);
    
    virtual void visitStart ( S_barline& elt);
    virtual void visitStart ( S_segno& elt);
    virtual void visitStart ( S_coda& elt);
    virtual void visitStart ( S_eyeglasses& elt);
    virtual void visitStart ( S_pedal& elt);
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
    
    virtual void visitStart ( S_articulations& elt );
    virtual void visitEnd   ( S_articulations& elt );
    virtual void visitStart ( S_accent& elt );
    virtual void visitStart ( S_breath_mark& elt );
    virtual void visitStart ( S_caesura& elt );
    virtual void visitStart ( S_spiccato& elt );
    virtual void visitStart ( S_staccato& elt );
    virtual void visitStart ( S_staccatissimo& elt );
    virtual void visitStart ( S_stress& elt );
    virtual void visitStart ( S_unstress& elt );
    virtual void visitStart ( S_detached_legato& elt );
    virtual void visitStart ( S_strong_accent& elt );
    virtual void visitStart ( S_tenuto& elt );
    virtual void visitStart ( S_fermata& elt );
    virtual void visitStart ( S_arpeggiate& elt );
    virtual void visitStart ( S_doit& elt );
    virtual void visitStart ( S_falloff& elt );
    virtual void visitStart ( S_plop& elt );
    virtual void visitStart ( S_scoop& elt );
        
    virtual void visitStart ( S_ornaments& elt );
    virtual void visitEnd   ( S_ornaments& elt );
    virtual void visitStart ( S_trill_mark& elt );
    virtual void visitStart ( S_wavy_line& elt );
    virtual void visitStart ( S_turn& elt );
    virtual void visitStart ( S_inverted_turn& elt );
    virtual void visitStart ( S_delayed_turn& elt );
    virtual void visitStart ( S_delayed_inverted_turn& elt );
    virtual void visitStart ( S_vertical_turn& elt );
    virtual void visitStart ( S_mordent& elt );
    virtual void visitStart ( S_inverted_mordent& elt );
    virtual void visitStart ( S_schleifer& elt );
    virtual void visitStart ( S_shake& elt );
    virtual void visitStart ( S_accidental_mark& elt );

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
    
    virtual void visitStart ( S_fp& elt);
    virtual void visitStart ( S_fz& elt);
    
    virtual void visitStart ( S_rf& elt);
    virtual void visitStart ( S_sf& elt);
    virtual void visitStart ( S_rfz& elt);
    virtual void visitStart ( S_sfz& elt);
    
    virtual void visitStart ( S_sfp& elt);
    virtual void visitStart ( S_sfpp& elt);
    virtual void visitStart ( S_sffz& elt);

    virtual void visitStart ( S_wedge& elt);

    virtual void visitStart ( S_grace& elt );
    
//    virtual void visitStart ( S_voice& elt);
    virtual void visitStart ( S_type& elt);
    virtual void visitStart ( S_stem& elt);
    virtual void visitEnd   ( S_note& elt);

    virtual void visitStart ( S_rest& elt);
    
    virtual void visitEnd   ( S_unpitched& elt);
    virtual void visitStart ( S_display_step& elt);
    virtual void visitStart ( S_display_octave& elt);

    virtual void visitStart ( S_chord& elt);
    
    virtual void visitStart ( S_time_modification& elt);
    virtual void visitStart ( S_actual_notes& elt);
    virtual void visitStart ( S_normal_notes& elt);
    virtual void visitStart ( S_normal_type& elt);
    virtual void visitStart ( S_tuplet& elt);
    
    virtual void visitStart ( S_rehearsal& elt);

  private:
                     
    S_msrOptions              fMsrOptions;

    // dividing quater notes in MusicXML
    // ------------------------------------------------------
    int                       fCurrentDivisionsPerQuarterNote;

    // measure locations
    // ------------------------------------------------------
// JMI    msrMeasureLocation        fCurrentMeasureLocation;
    
    // description of the current MusicXML note
    msrNoteData               fNoteData;

    // the MSR score we're building
    // ------------------------------------------------------
    S_msrScore                fMsrScore;

    // geometry handling
    // ------------------------------------------------------
    int                       fMillimeters;
    int                       fTenths;
    bool                      fOnGoingPageLayout;
    
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
                                int partgroupNumber);

    // MusicXML allows part groups to overlap,
    // we use a list in which part groups are orderd by
    // increasing part group <default-x>
    // (all of them are negative)    
    // the current part group is either null or the front of the list
    
    list<S_msrPartgroup>      fPartgroupsList;
    
    void                      showPartgroupsData (string context);
    
    void                      handlePartgroupStart (
                                int     inputLineNumber,
                                msrPartgroup::msrPartgroupSymbolKind
                                        partgroupSymbol,
                                bool    partgroupBarline);
                                
    void                      handlePartgroupStop (
                                int inputLineNumber);

    // part handling
    // ------------------------------------------------------
    string                    fCurrentPartID; // used throughout
    string                    fCurrentPartName;
    string                    fCurrentPartAbbreviation;
    string                    fCurrentPartInstrumentName;

    map<string, S_msrPart>    fPartsMap;
    
    S_msrPart                 fCurrentPart; // used throughout

    S_msrPart                 createDownToPartIfNeeded (
                                int            inputLineNumber,
                                S_msrPartgroup partgroup,
                                string         partID);

    // staff handling
    // ------------------------------------------------------
    int                       fCurrentStaffNumber; // used throughout
    msrStaff::msrStaffTypeKind
                              fCurrentStaffTypeKind;
    
    S_msrStaff                createStaffInCurrentPartIfNeeded (
                                int            inputLineNumber,
                                int            staffNumber);

    int                       fStaffDetailsStaffNumber;
    int                       fCurrentStaffTuningLinesNumber;
    
    int                       fCurrentStaffTuningLine;
    char                      fCurrentStaffTuningStep;
    int                       fCurrentStaffTuningOctave;
    float                     fCurrentStaffTuningAlter;
    
    int                       fCurrentStaffDetailsCapo;
    int                       fCurrentStaffDetailsStaffSize;
  
    // voice handling
    // ------------------------------------------------------
    int                       fCurrentVoiceNumber; // used throughout

    S_msrVoice                registerVoiceInStaffInCurrentPartIfNeeded (
                                int            inputLineNumber,
                                int            staffNumber,
                                int            voiceNumber);
    
    // clef handling
    // ------------------------------------------------------
    int                       fCurrentClefStaffNumber;
    string                    fCurrentClefSign;
    int                       fCurrentClefLine;
    int                       fCurrentClefOctaveChange;

    // key handling
    // ------------------------------------------------------
    int                       fCurrentKeyStaffNumber;
    int                       fCurrentFifths;
    int                       fCurrentCancel;
    string                    fCurrentMode;

    // transpose handling
    // ------------------------------------------------------
    int                       fCurrentTransposeNumber;
    int                       fCurrentTransposeDiatonic;
    int                       fCurrentTransposeChromatic;

    // direction handling
    // ------------------------------------------------------
    int                       fCurrentDirectionStaffNumber;
    string                    fCurrentDirectionPlacement;
    string                    fCurrentWordsContents;
    string                    fCurrentFontStyle;
    string                    fCurrentFontSize;
    string                    fCurrentFontWeight;
    string                    fCurrentFontXMLLang;

    msrWords::msrWordsPlacementKind
                              fCurrentWordsPlacementKind; // JMI
    bool                      fOnGoingDirection;

    // direction-type handling
    // ------------------------------------------------------
    bool                      fOnGoingDirectionType;
  
    // metronome handling
    // ------------------------------------------------------
    vector<msrBeatData>       fBeatsData; // JMI
    int                       fPerMinute;
    msrBeatData               fCurrentBeat;
    bool                      fParentheses;
    S_msrWords                fCurrentWords;
    S_msrTempo                fCurrentTempo;

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
    msrLyricschunk::msrLyricschunkKind
                              fCurrentLyricschunkKind;
    msrLyricschunk::msrLyricschunkKind
                              fFirstLyricschunkInSlurKind;
    // the last lyrics fragment met
    string                    fCurrentText;
    bool                      fCurrentElision;
    
    int                       fCurrentLyricsNumber;
    bool                      fCurrentNoteHasLyrics;
    bool                      fCurrentLyricsHasText;

    list<S_msrLyricschunk>    fCurrentNoteLyricchunks;

    void                      handleLyrics (S_msrNote newNote);

    // harmony handling
    // ------------------------------------------------------
    char                      fCurrentHarmonyRootStep;
    float                     fCurrentHarmonyRootAlter;
    msrHarmony::msrHarmonyKind
                              fCurrentHarmonyKind;
    string                    fCurrentHarmonyKindText;
    char                      fCurrentHarmonyBassStep;
    float                     fCurrentHarmonyBassAlter;

    int                       fCurrentHarmonyDegreeValue;
    float                     fCurrentHarmonyDegreeAlter;
    

    // barline handling
    // ------------------------------------------------------
    bool                      fOnGoingBarline;
    bool                      fCurrentBarlineHasSegno;
    bool                      fCurrentBarlineHasCoda;
    string                    fCurrentLocation;
    string                    fCurrentStyle;
    string                    fCurrentEndingtype;
    string                    fCurrentEndingNumber;
    string                    fCurrentRepeatDirection;
    string                    fCurrentRepeatWinged;

    msrBarline::msrBarlineLocation
                              fCurrentBarlineLocation;
    msrBarline::msrBarlineStyle
                              fCurrentBarlineStyle;
    msrBarline::msrBarlineEndingType
                              fCurrentBarlineEndingType;
    string                    fCurrentBarlineEndingNumber;
                                // may be "1, 2"
    msrBarline::msrBarlineRepeatDirection
                              fCurrentBarlineRepeatDirection;
    msrBarline::msrBarlineRepeatWinged
                              fCurrentBarlineRepeatWinged;

    // repeat handling
    // ------------------------------------------------------
    S_msrRepeat               fCurrentRepeat;
    stack<S_msrBarline>       fPendingBarlines;
    bool                      fOnGoingRepeat;
    
    void                      handleRepeatStart (
                                S_barline     elt,
                                S_msrBarline& barline);
    void                      handleHooklessEndingEnd (
                                S_barline     elt,
                                S_msrBarline& barline);
    void                      handleHookedEndingEnd (
                                S_barline     elt,
                                S_msrBarline& barline);
    void                      handleEndingStart (
                                S_barline     elt,
                                S_msrBarline& barline);
    void                      handleEndingEnd (
                                S_barline     elt,
                                S_msrBarline& barline);
    
    // unpitched notes handling
    // ------------------------------------------------------
    char                      fDisplayStep;
    int                       fDisplayOctave;

    // stem handling
    // ------------------------------------------------------    
    S_msrStem                 fCurrentStem;

    // beam handling
    // ------------------------------------------------------
    string                    fCurrentBeamValue;
    int                       fCurrentBeamNumber;
    S_msrBeam                 fCurrentBeam;

    // beam handling
    // ------------------------------------------------------
    bool                      fCurrentGraceIsSlashed;
    S_msrGracenotes           fCurrentGracenotes;

    // note/rest handling
    // ------------------------------------------------------
    int                       fCurrentNoteStaffNumber;
    int                       fCurrentNoteVoiceNumber;
    void                      handleStandaloneOrGraceNoteOrRest (
                                S_msrNote newNote);

    // ------------------------------------------------------
    // chord handling
    S_msrChord                fCurrentChord;
    bool                      fOnGoingChord;
    
    S_msrChord                createChordFromItsFirstNote (
                                S_msrVoice voice,
                                S_msrNote firstNote);
    void                      handleNoteBelongingToAChord (
                                S_msrNote newNote);
    
    // ------------------------------------------------------
    // articulations handling
    list<S_msrArticulation>   fCurrentArticulations;

    void                      attachCurrentArticulationsToNote (
                                S_msrNote note);
                                
    void                      attachCurrentArticulationsToChord (
                                S_msrChord chord);
                                
    void                      copyNoteArticulationsToChord (
                                S_msrNote note, S_msrChord chord);
                                
    // dynamics, words and wedges remain pending until the next note
    // (they precede the note in MusicXML but follow it in LilyPond)
    list<S_msrDynamics>       fPendingDynamics;
    list<S_msrWords>          fPendingWords;
    list<S_msrSlur>           fPendingSlurs;
    list<S_msrWedge>          fPendingWedges;

    void                      attachPendingElementsToNote (
                                S_msrNote note);
                                
    void                      attachPendingDynamicsToNote (
                                S_msrNote note);
    void                      attachPendingWordsToNote (
                                S_msrNote note);
    void                      attachPendingSlursToNote (
                                S_msrNote note);
    void                      attachPendingWedgesToNote (
                                S_msrNote note);

    void                      copyNoteElementsToChord (
                                S_msrNote note, S_msrChord chord);
                                
    void                      copyNoteDynamicsToChord (
                                S_msrNote note, S_msrChord chord);
    void                      copyNoteWordsToChord (
                                S_msrNote note, S_msrChord chord);
    void                      copyNoteSlursToChord (
                                S_msrNote note, S_msrChord chord);
    void                      copyNoteWedgesToChord (
                                S_msrNote note, S_msrChord chord);
       
    // ------------------------------------------------------
    // ornaments handling
    S_msrOrnament             fCurrentOrnament;
    list<S_msrOrnament>       fCurrentOrnamentsList;

    string                    fCurrentAccidentalMark;
    msrOrnament::msrOrnamentPlacementKind
                              fCurrentOrnamentPlacementKind;

    void                      attachCurrentOrnamentsToNote (
                                S_msrNote note);

    void                      attachCurrentOrnamentsToChord (
                                S_msrChord chord);

    void                      copyNoteOrnamentsToChord (
                                S_msrNote note, S_msrChord chord);

    // description of the current MSR note
    // ------------------------------------------------------
    string                    fCurrentNoteType;
    bool                      fOnGoingNote;
     // needed for chords
    map<S_msrVoice, S_msrNote>  fLastHandledNoteInVoice;
    void                      displayLastHandledNoteInVoice (
                                string header);


    // tuplet handling
     // ------------------------------------------------------
    int                       fCurrentActualNotes;
    int                       fCurrentNormalNotes;
    string                    fCurrentNormalNoteType;
    // embedded tuplets are numbered 1, 2, ...
    int                       fCurrentTupletNumber;
    msrTuplet::msrTupletKind  fCurrentTupletKind;
    stack<S_msrTuplet>        fTupletsStack;

    void                      createTupletWithItsFirstNote (
                                S_msrNote note);
    void                      finalizeTuplet (
                                S_msrNote note);
    void                      handleNoteBelongingToATuplet (
                                S_msrNote newNote);
    void                      handleTupletsPendingOnTupletStack ();
         
    // ties handling
    // ------------------------------------------------------
// JMI    string                    fCurrentTiedType;
    msrTie::msrTieKind        fCurrentTieKind;
    string                    fCurrentTiedOrientation; // JMI
    S_msrTie                  fCurrentTie;

    // slurs handling
    // ------------------------------------------------------
    int                       fCurrentSlurNumber;
    string                    fCurrentSlurType;
    string                    fCurrentSlurPlacement;
    msrSlur::msrSlurKind      fCurrentSlurKind;
    bool                      fOnGoingSlur;
    bool                      fOnGoingSlurHasLyrics;

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
