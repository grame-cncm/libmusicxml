/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef ___mxmlTree2MsrTranslator___
#define ___mxmlTree2MsrTranslator___

#include "msr.h"


namespace MusicXML2
{

//________________________________________________________________________
class mxmlTree2MsrTranslator : 

  // score partwise

  public visitor<S_score_partwise>,

  // parts
    
  public visitor<S_part>,
  
  // ???
  
  public visitor<S_attributes>,
  
  public visitor<S_divisions>,

  public visitor<S_staves>,
  public visitor<S_staff>,
  
  // staff details
  
  public visitor<S_staff_details>,
  public visitor<S_staff_type>,
  public visitor<S_staff_lines>,
  public visitor<S_staff_tuning>,
  public visitor<S_tuning_step>,
  public visitor<S_tuning_octave>,
  public visitor<S_tuning_alter>,
  public visitor<S_capo>,
  public visitor<S_staff_size>,

  // ???
  
  public visitor<S_voice>,

  // backup & forward
  
  public visitor<S_backup>,
  public visitor<S_forward>,
  
  // clefs
  
  public visitor<S_clef>,
  public visitor<S_sign>,
  public visitor<S_line>,
  public visitor<S_clef_octave_change>,

  // keys
  
  public visitor<S_key>,
  public visitor<S_cancel>,
  public visitor<S_fifths>,
  public visitor<S_mode>,
  public visitor<S_key_step>,
  public visitor<S_key_alter>,
  public visitor<S_key_octave>,

  // times
  
  public visitor<S_time>,
  public visitor<S_beats>,
  public visitor<S_beat_type>,
  public visitor<S_senza_misura>,
  
  public visitor<S_interchangeable>,
  public visitor<S_time_relation>,
  
  // ???
  
  public visitor<S_instruments>,

  // transpose
  
  public visitor<S_transpose>,
  public visitor<S_diatonic>,
  public visitor<S_chromatic>,
  public visitor<S_octave_change>,
  public visitor<S_double>,

  // directions
  
  public visitor<S_direction>,
  public visitor<S_direction_type>,
  public visitor<S_offset>,
  
  public visitor<S_words>,
  
  public visitor<S_octave_shift>,
  
  public visitor<S_scordatura>,
  public visitor<S_accord>,

  // tempo
  
  public visitor<S_metronome>,
  public visitor<S_beat_unit>,
  public visitor<S_beat_unit_dot>,
  public visitor<S_per_minute>,
  public visitor<S_metronome_note>,
  public visitor<S_metronome_relation>,
  public visitor<S_metronome_type>,
  public visitor<S_metronome_beam>,
  public visitor<S_metronome_dot>,
  public visitor<S_metronome_tuplet>,
  public visitor<S_normal_dot>,

  // ties, slurs, brackets & beams
  
  public visitor<S_tied>,
  public visitor<S_slur>,
  public visitor<S_bracket>,
  public visitor<S_beam>,

  // lyrics
  
  public visitor<S_lyric>,
  public visitor<S_syllabic>,
  public visitor<S_text>,
  public visitor<S_elision>,
  public visitor<S_extend>,
  
  // harmonies
  
  public visitor<S_degree>,
  public visitor<S_degree_value>,
  public visitor<S_degree_alter>,
  public visitor<S_degree_type>,

  public visitor<S_harmony>,
  public visitor<S_root_step>,
  public visitor<S_root_alter>,
  public visitor<S_kind>,
  public visitor<S_inversion>,
  public visitor<S_bass_step>,
  public visitor<S_bass_alter>,

  // frames
  // ------------------------------------------------------
    
  public visitor<S_frame>,
  public visitor<S_frame_strings>,
  public visitor<S_frame_frets>,
  public visitor<S_first_fret>,
  public visitor<S_frame_note>,
  public visitor<S_barre>,

  // figured bass
  
  public visitor<S_figured_bass>,
  public visitor<S_figure>,
  public visitor<S_prefix>,
  public visitor<S_figure_number>,
  public visitor<S_suffix>,

  // measures
  
  public visitor<S_measure>,
  public visitor<S_print>,
  public visitor<S_system_layout>,
  public visitor<S_measure_numbering>,
  
  // ???
  
  public visitor<S_barline>,
  public visitor<S_segno>,
  public visitor<S_coda>,
  public visitor<S_eyeglasses>,
  public visitor<S_pedal>,
  public visitor<S_bar_style>,
  public visitor<S_repeat>,
  public visitor<S_ending>,

  // notes
  
  public visitor<S_note>,
  public visitor<S_step>,
  public visitor<S_alter>,
  public visitor<S_octave>,
  public visitor<S_duration>,
  public visitor<S_instrument>,
  public visitor<S_dot>,

  public visitor<S_rest>,
  
  public visitor<S_unpitched>,
  public visitor<S_display_step>,
  public visitor<S_display_octave>,

  public visitor<S_stem>,

  // note heads
  
  public visitor<S_type>,
  public visitor<S_notehead>,
  public visitor<S_accidental>,
  
  // repeats
  
  public visitor<S_measure_style>,
  public visitor<S_beat_repeat>,
  public visitor<S_measure_repeat>,
  public visitor<S_multiple_rest>,
  public visitor<S_slash>,
  public visitor<S_slash_type>,
  public visitor<S_slash_dot>,
 
  // articulations
  
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
  public visitor<S_non_arpeggiate>,
  public visitor<S_doit>,
  public visitor<S_falloff>,
  public visitor<S_plop>,
  public visitor<S_scoop>,
  
  // technicals
  
  public visitor<S_technical>,
  public visitor<S_arrow>,
  public visitor<S_bend>,
  public visitor<S_bend_alter>,
  public visitor<S_double_tongue>,
  public visitor<S_down_bow>,
  public visitor<S_fingering>,
  public visitor<S_fingernails>,
  public visitor<S_fret>,
  public visitor<S_hammer_on>,
  public visitor<S_handbell>,
  public visitor<S_harmonic>,
  public visitor<S_heel>,
  public visitor<S_hole>,
  public visitor<S_open_string>,
  public visitor<S_other_technical>,
  public visitor<S_pluck>,
  public visitor<S_pull_off>,
  public visitor<S_snap_pizzicato>,
  public visitor<S_stopped>,
  public visitor<S_string>,
  public visitor<S_tap>,
  public visitor<S_thumb_position>,
  public visitor<S_toe>,
  public visitor<S_triple_tongue>,
  public visitor<S_up_bow>,

  // ornaments
  
  public visitor<S_ornaments>,
  public visitor<S_trill_mark>,
  public visitor<S_dashes>,
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

  // tremolos
  
  public visitor<S_tremolo>,

  // dynamics
  
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

  public visitor<S_mf>,
  public visitor<S_mp>,
  public visitor<S_fp>,
  public visitor<S_fz>,
  
  public visitor<S_rf>,
  public visitor<S_sf>,
  public visitor<S_rfz>,
  public visitor<S_sfz>,
  
  public visitor<S_sfp>,
  public visitor<S_sfpp>,
  public visitor<S_sffz>,
  
  public visitor<S_other_dynamics>,
  
  public visitor<S_wedge>,

  // cue notes

  public visitor<S_cue>,
  
  // grace notes
  
  public visitor<S_grace>,

  // chords
  
  public visitor<S_chord>,

  // times
  
  public visitor<S_time_modification>,
  public visitor<S_actual_notes>,
  public visitor<S_normal_notes>,
  public visitor<S_normal_type>,

  // tuplets
  
  public visitor<S_tuplet>,
  public visitor<S_tuplet_actual>,
  public visitor<S_tuplet_normal>,
  public visitor<S_tuplet_number>,
  public visitor<S_tuplet_type>,
  public visitor<S_tuplet_dot>,
  
  // glissandos

  public visitor<S_glissando>,

  // slides

  public visitor<S_slide>,

  // rehearsals
  
  public visitor<S_rehearsal>,
  
  // accordion registration
  
  public visitor<S_accordion_registration>,
  public visitor<S_accordion_high>,
  public visitor<S_accordion_middle>,
  public visitor<S_accordion_low>,

  // harp pedals tuning
  
  public visitor<S_harp_pedals>,
  public visitor<S_pedal_tuning>,
  public visitor<S_pedal_step>,
  public visitor<S_pedal_alter>,
  
  public visitor<S_damp>,
  public visitor<S_damp_all>,

  // sound
  
  public visitor<S_sound>,

  // midi
  
  public visitor<S_instrument_sound>,
  public visitor<S_virtual_instrument>,
  public visitor<S_midi_device>,
  public visitor<S_midi_instrument>
  
{
  public:
  
    // constructors/destructor
    // ------------------------------------------------------

    mxmlTree2MsrTranslator (
      S_msrScore       scoreSkeleton,
      indentedOstream& ios);

    virtual ~mxmlTree2MsrTranslator ();

    // set and get
    // ------------------------------------------------------

    S_msrScore getMsrScore () const
        { return fMsrScore; };
    
    // services
    // ------------------------------------------------------

    void populateMsrSkeletonFromMxmlTree (
        const Sxmlelement mxmlTree);

    // visitors
    // ------------------------------------------------------

    void browseMxmlTree (
        const Sxmlelement& mxmlTree);

  protected:
        
    // parts
    // ------------------------------------------------------
    
    virtual void visitStart ( S_part& elt);
    virtual void visitEnd   ( S_part& elt);
    
    // ???
    // ------------------------------------------------------

    virtual void visitStart ( S_attributes& elt);
    virtual void visitEnd   ( S_attributes& elt);
    
    virtual void visitStart ( S_divisions& elt);
  
    // staves
    // ------------------------------------------------------

    virtual void visitStart ( S_staves& elt);
    
    virtual void visitStart ( S_staff& elt);
    
    // staff details
    // ------------------------------------------------------

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
    
    // ???
    // ------------------------------------------------------

    virtual void visitStart ( S_voice& elt );
    
    // backup & forward
    // ------------------------------------------------------

    virtual void visitStart ( S_backup& elt );
    virtual void visitEnd   ( S_backup& elt );
    virtual void visitStart ( S_forward& elt );
    virtual void visitEnd   ( S_forward& elt );
    
    // clefs
    // ------------------------------------------------------

    virtual void visitStart ( S_clef& elt );
    virtual void visitEnd   ( S_clef& elt );
    virtual void visitStart ( S_sign& elt );
    virtual void visitStart ( S_line& elt );
    virtual void visitStart ( S_clef_octave_change& elt );

    // keys
    // ------------------------------------------------------

    virtual void visitStart ( S_key& elt );
    virtual void visitEnd   ( S_key& elt );
    virtual void visitStart ( S_cancel& elt );
    virtual void visitStart ( S_fifths& elt );
    virtual void visitStart ( S_mode& elt );
    virtual void visitStart ( S_key_step& elt );
    virtual void visitStart ( S_key_alter& elt );
    virtual void visitStart ( S_key_octave& elt );

    // times
    // ------------------------------------------------------

    virtual void visitStart ( S_time& elt );
    virtual void visitEnd   ( S_time& elt );
    virtual void visitStart ( S_beats& elt );
    virtual void visitStart ( S_beat_type& elt );
    virtual void visitStart ( S_senza_misura& elt );
    
    virtual void visitStart ( S_interchangeable& elt );
    virtual void visitStart ( S_time_relation& elt );

    // ???
    // ------------------------------------------------------

    virtual void visitStart ( S_instruments& elt );

    // transpose
    // ------------------------------------------------------

    virtual void visitStart ( S_transpose& elt );
    virtual void visitEnd   ( S_transpose& elt );
    virtual void visitStart ( S_diatonic& elt );
    virtual void visitStart ( S_chromatic& elt );
    virtual void visitStart ( S_octave_change& elt );
    virtual void visitStart ( S_double& elt );

    // directions
    // ------------------------------------------------------

    virtual void visitStart ( S_direction& elt );
    virtual void visitEnd   ( S_direction& elt );
    virtual void visitStart ( S_direction_type& elt );
    virtual void visitEnd   ( S_direction_type& elt );
    virtual void visitStart ( S_offset& elt );
    
    virtual void visitStart ( S_words& elt );
    
    virtual void visitStart ( S_octave_shift& elt );
    
    virtual void visitStart ( S_scordatura& elt );
    virtual void visitEnd   ( S_scordatura& elt );
    virtual void visitStart ( S_accord& elt );
    virtual void visitEnd   ( S_accord& elt );

    // accordion registration
    // ------------------------------------------------------

    virtual void visitStart ( S_accordion_registration& elt );
    virtual void visitEnd   ( S_accordion_registration& elt );
    virtual void visitStart ( S_accordion_high& elt );
    virtual void visitStart ( S_accordion_middle& elt );
    virtual void visitStart ( S_accordion_low& elt );

    // tempo
    // ------------------------------------------------------

    virtual void visitStart ( S_metronome& elt );
    virtual void visitEnd   ( S_metronome& elt );
    virtual void visitStart ( S_beat_unit& elt );
    virtual void visitStart ( S_beat_unit_dot& elt );
    virtual void visitStart ( S_per_minute& elt );
    virtual void visitStart ( S_metronome_note& elt );
    virtual void visitEnd   ( S_metronome_note& elt );
    virtual void visitStart ( S_metronome_relation& elt );
    virtual void visitStart ( S_metronome_type& elt );
    virtual void visitStart ( S_metronome_beam& elt );
    virtual void visitStart ( S_metronome_dot& elt );
    virtual void visitStart ( S_metronome_tuplet& elt );
    virtual void visitEnd   ( S_metronome_tuplet& elt );
    virtual void visitStart ( S_normal_dot& elt );

    // ties, slurs, brackets & beams
    // ------------------------------------------------------

    virtual void visitStart ( S_tied& elt);
    virtual void visitStart ( S_slur& elt);
    virtual void visitStart ( S_bracket& elt);
    virtual void visitStart ( S_beam& elt);

    // lyrics
    // ------------------------------------------------------
    
    virtual void visitStart ( S_lyric& elt);
    virtual void visitEnd   ( S_lyric& elt);
    virtual void visitStart ( S_syllabic& elt);
    virtual void visitStart ( S_text& elt );
    virtual void visitStart ( S_elision& elt );
    virtual void visitStart ( S_extend& elt);

    // ???
    // ------------------------------------------------------

    virtual void visitStart ( S_degree& elt);
    virtual void visitEnd   ( S_degree& elt);
    virtual void visitStart ( S_degree_value& elt);
    virtual void visitStart ( S_degree_alter& elt);
    virtual void visitStart ( S_degree_type& elt);

    // harmonies
    // ------------------------------------------------------
    
    virtual void visitStart ( S_harmony& elt);
    virtual void visitEnd   ( S_harmony& elt);
    virtual void visitStart ( S_root_step& elt);
    virtual void visitStart ( S_root_alter& elt);
    virtual void visitStart ( S_kind& elt);
    virtual void visitStart ( S_inversion& elt);
    virtual void visitStart ( S_bass_step& elt);
    virtual void visitStart ( S_bass_alter& elt);
    
    // frames
    // ------------------------------------------------------
    
    virtual void visitStart ( S_frame& elt);
    virtual void visitEnd   ( S_frame& elt);
    virtual void visitStart ( S_frame_strings& elt);
    virtual void visitStart ( S_frame_frets& elt);
    virtual void visitStart ( S_first_fret& elt);
    virtual void visitStart ( S_frame_note& elt);
    virtual void visitEnd   ( S_frame_note& elt);
    virtual void visitStart ( S_barre& elt);

    // figured bass
    // ------------------------------------------------------
    
    virtual void visitStart ( S_figured_bass& elt);
    virtual void visitEnd   ( S_figured_bass& elt);
    virtual void visitStart ( S_figure& elt);
    virtual void visitEnd   ( S_figure& elt);
    virtual void visitStart ( S_prefix& elt);
    virtual void visitStart ( S_figure_number& elt);
    virtual void visitStart ( S_suffix& elt);

    // measures
    // ------------------------------------------------------

    virtual void visitStart ( S_measure& elt);
    virtual void visitEnd   ( S_measure& elt);
    
    // print
    // ------------------------------------------------------

    virtual void visitStart ( S_print& elt);
    virtual void visitStart ( S_system_layout& elt);
    virtual void visitStart ( S_measure_numbering& elt);
    
    virtual void visitStart ( S_barline& elt);
    virtual void visitStart ( S_segno& elt);
    virtual void visitStart ( S_coda& elt);
    virtual void visitStart ( S_eyeglasses& elt);
    virtual void visitStart ( S_pedal& elt);
    virtual void visitStart ( S_bar_style& elt);
    virtual void visitStart ( S_repeat& elt);
    virtual void visitStart ( S_ending& elt);
    virtual void visitEnd   ( S_barline& elt);

    // notes
    // ------------------------------------------------------

    virtual void visitStart ( S_note& elt);
    virtual void visitStart ( S_step& elt);
    virtual void visitStart ( S_alter& elt);
    virtual void visitStart ( S_octave& elt);
    virtual void visitStart ( S_duration& elt);
    virtual void visitStart ( S_instrument& elt);
    virtual void visitStart ( S_dot& elt );
    
    // repeats
    // ------------------------------------------------------
    
    virtual void visitStart ( S_measure_style& elt );
    virtual void visitStart ( S_beat_repeat& elt );
    virtual void visitStart ( S_measure_repeat& elt );
    virtual void visitStart ( S_multiple_rest& elt );
    virtual void visitStart ( S_slash& elt );
    virtual void visitEnd   ( S_slash& elt );
    virtual void visitStart ( S_slash_type& elt );
    virtual void visitStart ( S_slash_dot& elt );

    // atticulations
    // ------------------------------------------------------

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
    virtual void visitStart ( S_non_arpeggiate& elt );
    virtual void visitStart ( S_doit& elt );
    virtual void visitStart ( S_falloff& elt );
    virtual void visitStart ( S_plop& elt );
    virtual void visitStart ( S_scoop& elt );
        
    // technicals
    // ------------------------------------------------------
    
    virtual void visitStart ( S_technical& elt );
    virtual void visitEnd   ( S_technical& elt );
    virtual void visitStart ( S_arrow& elt );
    virtual void visitStart ( S_bend& elt );
    virtual void visitEnd   ( S_bend& elt );
    virtual void visitStart ( S_bend_alter& elt );
    virtual void visitStart ( S_double_tongue& elt );
    virtual void visitStart ( S_down_bow& elt );
    virtual void visitStart ( S_fingering& elt );
    virtual void visitStart ( S_fingernails& elt );
    virtual void visitStart ( S_fret& elt );
    virtual void visitStart ( S_hammer_on& elt );
    virtual void visitStart ( S_handbell& elt );
    virtual void visitStart ( S_harmonic& elt );
    virtual void visitStart ( S_heel& elt );
    virtual void visitStart ( S_hole& elt );
    virtual void visitStart ( S_open_string& elt );
    virtual void visitStart ( S_other_technical& elt );
    virtual void visitStart ( S_pluck& elt );
    virtual void visitStart ( S_pull_off& elt );
    virtual void visitStart ( S_snap_pizzicato& elt );
    virtual void visitStart ( S_stopped& elt );
    virtual void visitStart ( S_string& elt );
    virtual void visitStart ( S_tap& elt );
    virtual void visitStart ( S_thumb_position& elt );
    virtual void visitStart ( S_toe& elt );
    virtual void visitStart ( S_triple_tongue& elt );
    virtual void visitStart ( S_up_bow& elt );

    // ornaments
    // ------------------------------------------------------

    virtual void visitStart ( S_ornaments& elt );
    virtual void visitEnd   ( S_ornaments& elt );
    virtual void visitStart ( S_trill_mark& elt );
    virtual void visitStart ( S_dashes& elt );
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

    // tremolos
    // ------------------------------------------------------

    virtual void visitStart ( S_tremolo& elt );

    // dynamics
    // ------------------------------------------------------
    
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
    
    virtual void visitStart ( S_mf& elt);
    virtual void visitStart ( S_mp& elt);
    virtual void visitStart ( S_fp& elt);
    virtual void visitStart ( S_fz& elt);
    
    virtual void visitStart ( S_rf& elt);
    virtual void visitStart ( S_sf& elt);
    virtual void visitStart ( S_rfz& elt);
    virtual void visitStart ( S_sfz& elt);
    
    virtual void visitStart ( S_sfp& elt);
    virtual void visitStart ( S_sfpp& elt);
    virtual void visitStart ( S_sffz& elt);
    
    virtual void visitStart ( S_other_dynamics& elt);
    
    virtual void visitStart ( S_wedge& elt);

    // harp pedals tuning
    // ------------------------------------------------------
    
    virtual void visitStart ( S_harp_pedals& elt);
    virtual void visitStart ( S_pedal_tuning& elt);
    virtual void visitEnd   ( S_pedal_tuning& elt);
    virtual void visitStart ( S_pedal_step& elt);
    virtual void visitStart ( S_pedal_alter& elt);

    virtual void visitStart ( S_damp& elt);
    virtual void visitStart ( S_damp_all& elt);

    // cue notes
    // ------------------------------------------------------
    
    virtual void visitStart ( S_cue& elt );
    
    // grace notes
    // ------------------------------------------------------
    
    virtual void visitStart ( S_grace& elt );
    
    // ???
    // ------------------------------------------------------

    virtual void visitStart ( S_type& elt);

    // note heads
    // ------------------------------------------------------
    
    virtual void visitStart ( S_notehead& elt);


    // notes
    // ------------------------------------------------------
    
    virtual void visitStart ( S_accidental& elt);
    
    virtual void visitStart ( S_stem& elt);
    
    virtual void visitEnd   ( S_note& elt);

    virtual void visitStart ( S_rest& elt);
    
    virtual void visitEnd   ( S_unpitched& elt);
    virtual void visitStart ( S_display_step& elt);
    virtual void visitStart ( S_display_octave& elt);

    // chords
    // ------------------------------------------------------

    virtual void visitStart ( S_chord& elt);

    // time
    // ------------------------------------------------------
    
    virtual void visitStart ( S_time_modification& elt);
    virtual void visitStart ( S_actual_notes& elt);
    virtual void visitStart ( S_normal_notes& elt);
    virtual void visitStart ( S_normal_type& elt);
    
    // tuplets
    // ------------------------------------------------------

    virtual void visitStart ( S_tuplet& elt);
    virtual void visitStart ( S_tuplet_actual& elt);
    virtual void visitEnd   ( S_tuplet_actual& elt);
    virtual void visitStart ( S_tuplet_normal& elt);
    virtual void visitEnd   ( S_tuplet_normal& elt);
    virtual void visitStart ( S_tuplet_number& elt);
    virtual void visitStart ( S_tuplet_type& elt);
    virtual void visitStart ( S_tuplet_dot& elt);

    // glissandos
    // ------------------------------------------------------

    virtual void visitStart ( S_glissando& elt);

    // slides
    // ------------------------------------------------------

    virtual void visitStart ( S_slide& elt);

    // rehearsals
    // ------------------------------------------------------

    virtual void visitStart ( S_rehearsal& elt);
    
    // sound & MIDI
    // ------------------------------------------------------
    
    virtual void visitStart ( S_sound& elt);
    virtual void visitEnd   ( S_sound& elt);

    virtual void visitStart ( S_instrument_sound& elt);
    virtual void visitStart ( S_virtual_instrument& elt);
    virtual void visitStart ( S_midi_device& elt);
    virtual void visitStart ( S_midi_instrument& elt);

  private:

    
    // the MSR score we are populating
    // ------------------------------------------------------
    
    S_msrScore                fMsrScore;

    // the log output stream
    // ------------------------------------------------------

    indentedOstream&          fLogOutputStream;
    
    // divisions per quarter note
    // ------------------------------------------------------

    int                       fCurrentDivisionsPerQuarterNote;
    S_msrDivisions            fCurrentDivisions;

    // print
    // ------------------------------------------------------
  
    string                    fCurrentDisplayText;             
        
    // part handling
    // ------------------------------------------------------
        
    S_msrPart                 fCurrentPart;

    // measures
    // ------------------------------------------------------
    
    // we need to count the measures for option fSeparatorLineEveryNMeasures,
    // since measure numbers are actually strings

    string                    fCurrentMeasureNumber;
    int                       fCurrentMeasureOrdinalNumber;
    
    // measure style handling
    // ------------------------------------------------------
    
    msrSlashTypeKind          fCurrentSlashTypeKind;
    msrSlashUseDotsKind       fCurrentSlashUseDotsKind;
    msrSlashUseStemsKind      fCurrentSlashUseStemsKind;

    int                       fCurrentBeatRepeatSlashes;

    msrMeasuresRepeat::msrMeasuresRepeatKind
                              fCurrentMeasuresRepeatKind;
    int                       fCurrentMeasuresRepeatMeasuresNumber;
    int                       fCurrentMeasuresRepeatSlashesNumber;
    
    msrMultipleRest::msrMultipleRestKind
                              fCurrentMultipleRestKind;
    int                       fCurrentMultipleRestMeasuresNumber;
    int                       fRemainingMultipleRestMeasuresNumber;
    bool                      fOnGoingMultipleRest;
    bool                      fCurrentMultipleRestHasBeenCreated;

    msrDurationKind           fCurrentSlashGraphicDurationKind;
    int                       fCurrentSlashDotsNumber;
    
    // staff details handling
    // ------------------------------------------------------
    
    msrStaffDetails::msrStaffTypeKind
                              fCurrentStaffTypeKind;
    msrStaffDetails::msrShowFretsKind
                              fCurrentShowFretsKind;
    msrStaffDetails::msrPrintObjectKind
                              fCurrentPrintObjectKind;
    msrStaffDetails::msrPrintSpacingKind
                              fCurrentPrintSpacingKind;
                              
    int                       fCurrentStaffDetailsCapo;

    int                       fCurrentStaffDetailsStaffSize; // JMI
    
    int     fCurrentStaffLinesNumber;

    int                       fStaffDetailsStaffNumber;
    
    int                       fCurrentStaffTuningLine;
    msrDiatonicPitchKind      fCurrentStaffTuningDiatonicPitchKind;
    msrAlterationKind         fCurrentStaffTuningAlterationKind;
    int                       fCurrentStaffTuningOctave;

   S_msrStaffDetails          fCurrentStaffDetails;

    bool                      fOnGoingStaffTuning;
    
    // staff handling
    // ------------------------------------------------------
    
    int                       fCurrentMusicXMLStaffNumber; // used throughout
    
    S_msrStaff                fetchStaffFromCurrentPart (
                                int            inputLineNumber,
                                int            staffNumber);

    // voice handling
    // ------------------------------------------------------
    
    int                       fCurrentMusicXMLVoiceNumber; // used throughout

    S_msrVoice                fetchVoiceFromPart (
                                int inputLineNumber,
                                int staffNumber,
                                int voiceNumber);
    
    // clef handling
    // ------------------------------------------------------
    
    int                       fCurrentClefStaffNumber;
    string                    fCurrentClefSign;
    int                       fCurrentClefLine;
    int                       fCurrentClefOctaveChange;

    // key handling
    // ------------------------------------------------------

    msrKey::msrKeyKind        fCurrentKeyKind;

    // traditional
    int                       fCurrentKeyStaffNumber;
    int                       fCurrentKeyFifths;
    int                       fCurrentKeyCancelFifths;
    msrKey::msrKeyModeKind    fCurrentKeyModeKind;
    
    S_msrKey                  handleTraditionalKey (
                                int inputLineNumber);

    // Humdrum-Scot
    S_msrHumdrumScotKeyItem   fCurrentHumdrumScotKeyItem;
    vector<S_msrHumdrumScotKeyItem>
                              fCurrentHumdrumScotKeyItemsVector;
    
    S_msrKey                  handleHumdrumScotKey (
                                int inputLineNumber);

    // transpose handling
    // ------------------------------------------------------
    
    int                       fCurrentTransposeNumber;
    int                       fCurrentTransposeDiatonic;
    int                       fCurrentTransposeChromatic;
    int                       fCurrentTransposeOctaveChange;
    bool                      fCurrentTransposeDouble;

    // direction handling
    // ------------------------------------------------------
    
    int                       fCurrentDirectionStaffNumber;
    msrPlacementKind          fCurrentDirectionPlacementKind;
        
    bool                      fOnGoingDirection;

    // direction-type handling
    // ------------------------------------------------------
    
    bool                      fOnGoingDirectionType;

    // rehearsal handling
    // ------------------------------------------------------
    // rehearsals remain pending until the next note:
    // in MusicXML, they precede the note and
    // may occur when no current voice exists
    list<S_msrRehearsal>      fPendingRehearsals;
    
    void                      attachPendingRehearsalsToTheVoiceOfNote (
                                S_msrNote note);

    // eyeglasses handling
    // ------------------------------------------------------
    // eyeglasses remain pending until the next note:
    // in MusicXML, they precede the note and
    // may occur when no current voice exists
    list<S_msrEyeGlasses>     fPendingEyeGlasses;
    
    void                      attachPendingEyeGlassesToNote (
                                S_msrNote note);

    // damp handling
    // ------------------------------------------------------
    // damps remain pending until the next note:
    // in MusicXML, they precede the note and
    // may occur when no current voice exists
    list<S_msrDamp>           fPendingDamps;
    
    void                      attachPendingDampsToNote (
                                S_msrNote note);

    // damp all handling
    // ------------------------------------------------------
    // damp alls remain pending until the next note:
    // in MusicXML, they precede the note and
    // may occur when no current voice exists
    list<S_msrDampAll>        fPendingDampAlls;
    
    void                      attachPendingDampAllsToNote (
                                S_msrNote note);

    // accordion-registration handling
    // ------------------------------------------------------

    int                       fCurrentAccordionHigh;
    int                       fCurrentAccordionMiddle;
    int                       fCurrentAccordionLow;
    int                       fCurrentAccordionNumbersCounter;
    
    // metronome handling
    // ------------------------------------------------------

    msrDurationKind           fCurrentMetronomeBeatUnitDurationKind;
    
    vector<msrDottedDuration> fCurrentMetronomeBeatUnitsVector;
    string                    fCurrentMetrenomePerMinute;
    msrTempo::msrTempoParenthesizedKind
                              fCurrentMetronomeParenthesedKind;
    S_msrWords                fCurrentMetronomeWords;

    int                       fCurrentMetrenomeDotsNumber;
    msrTempo::msrTempoRelationKind
                              fCurrentMetrenomeRelationKind;
    msrDurationKind           fCurrentMetronomeDurationKind;
    string                    fCurrentMetronomeBeamValue;

    bool                      fOnGoingMetronomeNote;

    list<S_msrBeam>           fPendingMetronomeBeams; // stack JMI ???
    void                      attachCurrentMetronomeBeamsToMetronomeNote (
                                S_msrTempoNote tempoNote);

    msrTempoTuplet::msrTempoTupletTypeKind
                              fCurrentTempoTupletTypeKind;
    msrTempoTuplet::msrTempoTupletBracketKind
                              fCurrentTempoTupletBracketKind;
    msrTempoTuplet::msrTempoTupletShowNumberKind
                              fCurrentTempoTupletShowNumberKind;
    
    int                       fCurrentMetrenomeNormalDotsNumber;
    int                       fCurrentMetronomeNoteActualNotes;
    int                       fCurrentMetronomeNoteNormalNotes;
    string                    fCurrentMetronomeNoteNormalType; // JMI ???

    rational                  fCurrentMetronomeNoteWholeNotesFromMetronomeType;
    
    S_msrTempoTuplet          fCurrentMetronomeTuplet;
    bool                      fOnGoingMetronomeTuplet;

    S_msrTempoRelationshipElements
                              fCurrentMetronomeRelationLeftElements;
    S_msrTempoRelationshipElements
                              fCurrentMetronomeRelationRightElements;
    
    S_msrTempo                fCurrentMetronomeTempo;
    // tempos remain pending until the next note:
    // in MusicXML, they precede the note and
    // may occur when no current voice exists
    list<S_msrTempo>          fPendingTempos;
    
    void                      attachPendingTemposToTheVoiceOfNote (
                                S_msrNote note);

    // octave shift handling
    // ------------------------------------------------------
    
    // octave shifts remain pending until the next note
    // (they precede the note in MusicXML and
    // may occur when no current voice exists)
    list<S_msrOctaveShift>    fPendingOctaveShifts;
    void                      attachPendingOctaveShiftsToNote (
                                S_msrNote note);

    // scordatura handling
    // ------------------------------------------------------

    S_msrScordatura           fCurrentScordatura;
    
    // scordaturas remain pending until the next note:
    // in MusicXML, they precede the note and
    // may occur when no current voice exists
    list<S_msrScordatura>     fPendingScordaturas;
    
    void                      attachPendingScordaturasToNote (
                                S_msrNote note);

    int                       fCurrentStringTuningNumber;
    
    msrDiatonicPitchKind      fCurrentStringTuningDiatonicPitchKind;
    msrAlterationKind         fCurrentStringTuningAlterationKind;
    int                       fCurrentStringTuningOctave;

    bool                      fOnGoingAccord;
    

    // time handling
    // ------------------------------------------------------

    msrTime::msrTimeSymbolKind
                              fCurrentTimeSymbolKind;

    int                       fCurrentTimeStaffNumber;
    string                    fCurrentTimeBeats;
    bool                      fCurrentTimeSenzaMisura;

    vector<S_msrTimeItem>
                              fCurrentTimeItemsVector;

    S_msrTime                 fCurrentTime;

    msrTime::msrTimeSymbolKind
                              fCurrentInterchangeableSymbolKind;
    msrTime::msrTimeSeparatorKind
                              fCurrentInterchangeableSeparatorKind;
    msrTime::msrTimeRelationKind
                              fCurrentInterchangeableRelationKind;
                              
    bool                      fOnGoingInterchangeable;


    // lyrics handling
    // ------------------------------------------------------
    
    bool                      fOnGoingLyric;
    string                    fCurrentSyllabic;
    msrSyllable::msrSyllableKind
                              fCurrentSyllableKind;
    msrSyllable::msrSyllableKind
                              fFirstSyllableInSlurKind;
    msrSyllable::msrSyllableKind
                              fFirstSyllableInLigatureKind;
    list<string>              fCurrentLyricTextsList;
    
    msrSyllable::msrSyllableExtendKind
                              fCurrentSyllableExtendKind;
    bool                      fOnGoingSyllableExtend;
    
    string                    fCurrentStanzaNumber;
    string                    fCurrentStanzaName;
    
    bool                      fCurrentNoteHasLyrics;
    bool                      fASkipSyllableHasBeenGeneratedForcurrentNote;

    bool                      fCurrentStanzaHasText;

    list<S_msrSyllable>       fCurrentNoteSyllables;

    bool                      fLastHandledNoteInVoiceHasLyrics;
    
    void                      handleLyricsForNote (
                                S_msrVoice currentVoice,
                                S_msrNote  newNote);

    // harmonies handling
    // ------------------------------------------------------
    
    int                       fHarmonyVoicesCounter;
    list<S_msrHarmony>        fPendingHarmoniesList;
    
    int                       fCurrentHarmonyInputLineNumber;
    
    msrDiatonicPitchKind      fCurrentHarmonyRootDiatonicPitchKind;
    msrAlterationKind         fCurrentHarmonyRootAlterationKind;
    
    msrHarmonyKind            fCurrentHarmonyKind;
    string                    fCurrentHarmonyKindText;

    int                       fCurrentHarmonyInversion;
    
    msrDiatonicPitchKind      fCurrentHarmonyBassDiatonicPitchKind;
    msrAlterationKind         fCurrentHarmonyBassAlterationKind;
    msrHarmonyDegree::msrHarmonyDegreeTypeKind
                              fCurrentHarmonyDegreeTypeKind;

    list<S_msrHarmonyDegree>  fCurrentHarmonyDegreesList;


    int                       fCurrentHarmonyDegreeValue;
    msrAlterationKind         fCurrentHarmonyDegreeAlterationKind;

    msrQuarterTonesPitchKind  fCurrentHarmonyRootQuarterTonesPitchKind;
    msrQuarterTonesPitchKind  fCurrentHarmonyBassQuarterTonesPitchKind;

    // frames handling
    // ------------------------------------------------------

    int                       fCurrentFrameStrings;
    int                       fCurrentFrameFrets;
    int                       fCurrentFrameFirstFret;
    bool                      fOnGoingFrame;

    list<S_msrFrame>          fPendingFramesList;

    int                       fCurrentFrameNoteStringNumber;
    int                       fCurrentFrameNoteFretNumber;
    int                       fCurrentFrameNoteFingering;
    
    msrFrameNote::msrBarreTypeKind
                              fCurrentFrameNoteBarreTypeKind;

    bool                      fOnGoingFrameNote;

    list<S_msrFrameNote>      fPendingFramesNotesList;
    
    // figured bass handling
    // ------------------------------------------------------
    
    bool                      fOnGoingFiguredBass;
    bool                      fPendingFiguredBass;
    
    int                       fCurrentFiguredBassInputLineNumber;

    msrFigure::msrFigurePrefixKind
                              fCurrentFigurePrefixKind;
    int                       fCurrentFigureNumber;
    msrFigure::msrFigureSuffixKind
                              fCurrentFigureSuffixKind;

    rational                  fCurrentFiguredBassSoundingWholeNotes;

    msrFiguredBass::msrFiguredBassParenthesesKind
                              fCurrentFiguredBassParenthesesKind;

    list<S_msrFigure>         fPendingFiguredBassFigures; 

    // barline handling
    // ------------------------------------------------------
    
    bool                      fOnGoingBarline;
    int                       fRepeatEndCounter;

    S_msrBarline              fCurrentEndingStartBarline;
    
    msrBarline::msrBarlineHasSegnoKind
                              fCurrentBarlineHasSegnoKind;
    msrBarline::msrBarlineHasCodaKind
                              fCurrentBarlineHasCodaKind;
    string                    fCurrentBarlineEndingNumber; // vector<string> ??? JMI
                                // may be "1, 2"

    msrBarline::msrBarlineLocationKind
                              fCurrentBarlineLocationKind;
    msrBarline::msrBarlineStyleKind
                              fCurrentBarlineStyleKind;
    msrBarline::msrBarlineEndingTypeKind
                              fCurrentBarlineEndingTypeKind;
    msrBarline::msrBarlineRepeatDirectionKind
                              fCurrentBarlineRepeatDirectionKind;
    msrBarline::msrBarlineRepeatWingedKind
                              fCurrentBarlineRepeatWingedKind;

    int                       fCurrentBarlineTimes;

    // repeats handling
    // ------------------------------------------------------
    
    bool                      fOnGoingRepeat;
    bool                      fOnGoingRepeatHasBeenCreated;
    
    void                      createAndPrependImplicitBarLine (
                                int inputLineNumber);

    void                      handleRepeatStart (
                                S_msrBarline& barline);

    void                      handleRepeatEnd (
                                S_msrBarline& barline);

    void                      handleEndingStart (
                                S_msrBarline& barline);
/* JMI
    void                      handleHookedEndingStart (
                                S_barline     elt,
                                S_msrBarline& barline);
*/
    void                      handleHookedEndingEnd (
                                S_msrBarline& barline);
    /* JMI
    void                      handleHooklessEndingStart (
                                S_barline     elt,
                                S_msrBarline& barline);
*/
    void                      handleHooklessEndingEnd (
                                S_msrBarline& barline);

    // notes/rests handling
    // ------------------------------------------------------

    // map<S_msrVoice, S_msrNote> seems buggy in g++ 4.9.x, so
    // we use a pair containing the staff and voice numbers:
    map<pair<int, int>, S_msrNote>
                              fVoicesLastMetNoteMap;

    void                      printVoicesLastMetNoteMap (
                                int    inputLineNumber);
    
    void                      checkStep (
                                int    inputLineNumber,
                                string stepValue);
                                
    // notes
    msrDiatonicPitchKind      fCurrentNoteDiatonicPitchKind;
    msrAlterationKind         fCurrentNoteAlterationKind;
    
    int                       fCurrentNoteOctave;
    
    int                       fCurrentDisplayOctave;
    msrDiatonicPitchKind      fCurrentDisplayDiatonicPitchKind;

    // note print kind
    msrNote::msrNotePrintKind fCurrentNotePrintKind;

    // note heads
    msrNote::msrNoteHeadKind  fCurrentNoteHeadKind;
    msrNote::msrNoteHeadFilledKind
                              fCurrentNoteHeadFilledKind;
    msrNote::msrNoteHeadParenthesesKind
                              fCurrentNoteHeadParenthesesKind;

    // accidentals
    msrNote::msrNoteAccidentalKind
                              fCurrentNoteAccidentalKind;
    msrNote::msrNoteEditorialAccidentalKind
                              fCurrentNoteEditorialAccidentalKind;
    msrNote::msrNoteCautionaryAccidentalKind
                              fCurrentNoteCautionaryAccidentalKind;

    // ongoing note
    bool                      fOnGoingNote;

    // glissandos
    
    // slides
    
    // note sound
    msrQuarterTonesPitchKind  fCurrentNoteQuarterTonesPitchKind;
    rational                  fCurrentNoteSoundingWholeNotesFromDuration;
    rational                  fCurrentNoteSoundingWholeNotes;

    // note display
    msrQuarterTonesPitchKind  fCurrentNoteQuarterTonesDisplayPitchKind;
    rational                  fCurrentNoteDisplayWholeNotesFromType;
    rational                  fCurrentNoteDisplayWholeNotes;

    // augmentation dots
    int                       fCurrentNoteDotsNumber;

    // graphic duration
    msrDurationKind           fCurrentNoteGraphicDurationKind;

    // rests
    bool                      fCurrentNoteIsARest;
    bool                      fCurrentRestMeasure;

    // unpitched notes
    bool                      fCurrentNoteIsUnpitched;

    // cue notes
    bool                      fCurrentNoteIsACueNote;

    // grace notes
    bool                      fCurrentNoteIsAGraceNote;
    string                    fCurrentStealTimeFollowing;
    string                    fCurrentStealTimePrevious;
    string                    fCurrentMakeTime;

    // staff
    // notes always keep their initial staff number
    int                       fPreviousNoteMusicXMLStaffNumber;

    // staff changes
    // MusicXMl contains sequences of elements on one and the same staff,
    // until a <backup/> or <forward/> markup may change the latter;
    // we keep track of the current sequence staff number
    // for the previous and current notes:
    // a staff change occurs when they are different,
    // but the note itself keeps its staff number in that case
    int                       fCurrentStaffNumberToInsertInto;

    // cross staff chords
    int                       fCurrentChordStaffNumber;
    bool                      fCurrentNoteIsCrossStaves;

    enum staffChangeKind {
        k_NoStaffChange,
        kStaffChangeChordMemberNote,
        kStaffChangeOtherNote };
        
    staffChangeKind           fCurrentStaffChangeKind;
    
    // elements attached to the note
    S_msrStem                 fCurrentStem;

    string                    fCurrentBeamValue;
    int                       fCurrentBeamNumber;

    // initialization
    void                      initializeNoteData ();

    // detailed notes handling
    void                      handleStandaloneOrDoubleTremoloNoteOrGraceNoteOrRest (
                                S_msrNote newNote);

    void                      handleNoteBelongingToAChord (
                                S_msrNote newChordNote);
    
    void                      handleNoteBelongingToATuplet (
                                S_msrNote newNote);

    void                      handleNoteBelongingToAChordInATuplet (
                                S_msrNote newChordNote);

    void                      handleNoteBelongingToAChordInAGraceNotesGroup (
                                S_msrNote newChordNote);

    // grace notes handling
    // ------------------------------------------------------
    
    bool                      fCurrentGraceIsSlashed;
    S_msrGraceNotesGroup      fPendingGraceNotesGroup;

    // articulations handling
    // ------------------------------------------------------
    
    list<S_msrArticulation>   fCurrentArticulations;

    void                      attachCurrentArticulationsToNote (
                                S_msrNote note);

                                /* JMI
    void                      attachCurrentArticulationsToChord ( // JMI
                                S_msrChord chord);
                                */
                                
    void                      copyNoteArticulationsToChord (
                                S_msrNote note, S_msrChord chord);
                                
    // dynamics, words and wedges remain pending until the next note
    // (they precede the note in MusicXML but follow it in LilyPond)
    // ------------------------------------------------------

    list<S_msrDynamics>       fPendingDynamics;
    list<S_msrOtherDynamics>  fPendingOtherDynamics;
    list<S_msrWords>          fPendingWords;
    list<S_msrSlur>           fPendingSlurs;
    list<S_msrLigature>       fPendingLigatures;
    list<S_msrWedge>          fPendingWedges;
    list<S_msrSlash>          fPendingSlashes;

    void                      attachPendingElementsToNote (
                                S_msrNote note);
                                
    void                      attachPendingDynamicsToNote (
                                S_msrNote note);
    void                      attachPendingOtherDynamicsToNote (
                                S_msrNote note);
    void                      attachPendingWordsToNote (
                                S_msrNote note);
    void                      attachPendingSlursToNote (
                                S_msrNote note);
    void                      attachPendingLigaturesToNote (
                                S_msrNote note);
    void                      attachPendingPedalsToNote (
                                S_msrNote note);
    void                      attachPendingWedgesToNote (
                                S_msrNote note);
    void                      attachPendingSlashesToNote (
                                S_msrNote note);

    void                      attachPendingGlissandosToNote (
                                S_msrNote note);
    void                      attachPendingSlidesToNote (
                                S_msrNote note);

    void                      copyNoteElementsToChord (
                                S_msrNote note, S_msrChord chord);
                                
    void                      copyNoteDynamicsToChord (
                                S_msrNote note, S_msrChord chord);
    void                      copyNoteOtherDynamicsToChord (
                                S_msrNote note, S_msrChord chord);
    void                      copyNoteWordsToChord (
                                S_msrNote note, S_msrChord chord);
    void                      copyNoteTieToChord (
                                S_msrNote note, S_msrChord chord);
    void                      copyNoteSlursToChord (
                                S_msrNote note, S_msrChord chord);
    void                      copyNoteLigaturesToChord (
                                S_msrNote note, S_msrChord chord);
    void                      copyNotePedalsToChord (
                                S_msrNote note, S_msrChord chord);
    void                      copyNoteSlashesToChord (
                                S_msrNote note, S_msrChord chord);
    void                      copyNoteWedgesToChord (
                                S_msrNote note, S_msrChord chord);
                                
    void                      copyNoteGraceNotesGroupsToChord (
                                S_msrNote note, S_msrChord chord);
       
    void                      copyNoteHarmonyToChord (
                                S_msrNote note, S_msrChord chord);
       
    // technicals handling
    // ------------------------------------------------------
    
    list<S_msrTechnical>      fCurrentTechnicalsList;
    list<S_msrTechnicalWithInteger>
                              fCurrentTechnicalWithIntegersList;
    list<S_msrTechnicalWithFloat>
                              fCurrentTechnicalWithFloatsList;
    list<S_msrTechnicalWithString>
                              fCurrentTechnicalWithStringsList;

    bool                      fOnGoingTechnical;

    float                     fBendAlterValue;
    
    void                      attachCurrentTechnicalsToNote (
                                S_msrNote note);
    void                      attachCurrentTechnicalWithIntegersToNote (
                                S_msrNote note);
    void                      attachCurrentTechnicalWithFloatsToNote (
                                S_msrNote note);
    void                      attachCurrentTechnicalWithStringsToNote (
                                S_msrNote note);

//    void                      attachCurrentTechnicalsToChord ( // JMI
 //                               S_msrChord chord);

    void                      copyNoteTechnicalsToChord (
                                S_msrNote note, S_msrChord chord);
    void                      copyNoteTechnicalWithIntegersToChord (
                                S_msrNote note, S_msrChord chord);
    void                      copyNoteTechnicalWithFloatsToChord (
                                S_msrNote note, S_msrChord chord);
    void                      copyNoteTechnicalWithStringsToChord (
                                S_msrNote note, S_msrChord chord);

    // ornaments handling
    // ------------------------------------------------------
    
    list<S_msrOrnament>       fCurrentOrnamentsList;

    void                      attachCurrentOrnamentsToNote (
                                S_msrNote note);

//    void                      attachCurrentOrnamentsToChord ( // JMI
 //                               S_msrChord chord);

    void                      copyNoteOrnamentsToChord (
                                S_msrNote note, S_msrChord chord);

    // spanners handling
    // ------------------------------------------------------

    S_msrSpanner              fCurrentWavyLineSpannerStart;
    
    list<S_msrSpanner>        fCurrentSpannersList;

    void                      attachCurrentSpannersToNote (
                                S_msrNote note);

    void                      copyNoteSpannersToChord (
                                S_msrNote note, S_msrChord chord);

    // stems handling
    // ------------------------------------------------------

    vector<S_msrStem>         fPendingStems; // stack JMI ???

    void                      copyNoteStemToChord (
                                S_msrNote note, S_msrChord chord);

    // beams handling
    // ------------------------------------------------------

    list<S_msrBeam>           fPendingBeams;

    void                      attachPendingBeamsToNote (
                                S_msrNote note);
    void                      copyNoteBeamsToChord (
                                S_msrNote note, S_msrChord chord);

    // glissandos handling
    // ------------------------------------------------------

    list<S_msrGlissando>      fPendingGlissandos;
    
    // slides handling
    // ------------------------------------------------------

    list<S_msrSlide>          fPendingSlides;

    // tremolos handling
    // ------------------------------------------------------
    
    bool                      fCurrentNoteBelongsToADoubleTremolo; // JMI ???

    msrTremoloTypeKind        fCurrentTremoloTypeKind;

    S_msrSingleTremolo        fCurrentSingleTremolo;

    S_msrDoubleTremolo        fCurrentDoubleTremolo;

    void                      attachCurrentSingleTremoloToNote (
                                S_msrNote note);

    void                      copyNoteSingleTremoloToChord (
                                S_msrNote note, S_msrChord chord);

    // chords handling
    // ------------------------------------------------------

    bool                      fCurrentNoteBelongsToAChord;

/* JMI
 //    map<S_msrVoice, S_msrChord> // seems buggy in g++ 4.9.x, so
    // we use a pair containing the staff and voice numbers:
    map<pair<int, int>, S_msrChord>
                              fVoicesCurrentChordMap;
                              */
                              
    S_msrChord                fCurrentChord;
    bool                      fOnGoingChord;
    
    S_msrChord                createChordFromItsFirstNote (
                                int                  inputLineNumber,
                                S_msrVoice           voice,
                                S_msrNote            chordFirstNote,
                                msrNote::msrNoteKind noteKind);

/* JMI
    void                      registerVoiceCurrentChordInMap (
                                int        inputLineNumber,
                                S_msrVoice voice,
                                S_msrChord chord);
                                
    void                      printVoicesCurrentChordMap ();
 */
    void                      printCurrentChord ();
                                
    // tuplets handling
    // ------------------------------------------------------
    
    bool                      fCurrentNoteHasATimeModification;
    
    int                       fCurrentNoteActualNotes;
    int                       fCurrentNoteNormalNotes;
    msrDurationKind           fCurrentNoteNormalTypeDuration;
    
    // nested tuplets are numbered 1, 2, ...
    int                       fCurrentTupletNumber;
    int                       fPreviousTupletNumber;

    bool                      fOnGoingTupletActual;
    int                       fCurrentTupletActualNumber;
    string                    fCurrentTupletActualType;
    int                       fCurrentTupletActualDotsNumber;

    bool                      fOnGoingTupletNormal;
    int                       fCurrentTupletNormalNumber;
    string                    fCurrentTupletNormalType;
    int                       fCurrentTupletNormalDotsNumber;

    msrTuplet::msrTupletTypeKind
                              fCurrentTupletTypeKind;
    msrTuplet::msrTupletLineShapeKind
                              fCurrentTupletLineShapeKind;
    msrTuplet::msrTupletBracketKind
                              fCurrentTupletBracketKind;
    msrTuplet::msrTupletShowNumberKind
                              fCurrentTupletShowNumberKind;
    msrTuplet::msrTupletShowTypeKind
                              fCurrentTupletShowTypeKind;
    int                       fCurrentTempoTupletNumber;
    
    bool                      fCurrentNoteBelongsToATuplet;

    list<S_msrTuplet>         fTupletsStack;
    void                      displayTupletsStack (
                                string context);

    bool                      fCurrentATupletStopIsPending;

    void                      createTupletWithItsFirstNoteAndPushItToTupletsStack (
                                S_msrNote note);
                                
    void                      handlePendingTupletStopIfAny (
                                int       inputLineNumber,
                                S_msrNote note);

    void                      finalizeTupletAndPopItFromTupletsStack (
                                int inputLineNumber);
                                
    void                      handleTupletsPendingOnTupletsStack (
                                int inputLineNumber);

    // map<S_msrVoice, S_msrTuplet> seems buggy in g++ 4.9.x, so
    // we use a pair containing the staff and voice numbers:
//     map<S_msrVoice, S_msrTuplet>
    map<pair<int, int>, S_msrTuplet>
                              fLastHandledTupletInVoiceMap;

     void                     displayLastHandledTupletInVoiceMap (
                                string header);
       
    // ties handling
    // ------------------------------------------------------
    
// JMI    string                    fCurrentTiedType;
    msrTie::msrTieKind        fCurrentTieKind;
    string                    fCurrentTiedOrientation; // JMI
    S_msrTie                  fCurrentTie;

    // slurs handling
    // ------------------------------------------------------
    
    string                    fCurrentSlurType;
    string                    fCurrentSlurPlacement;
    msrSlur::msrSlurTypeKind  fCurrentSlurTypeKind;
    bool                      fOnGoingSlur;
    bool                      fOnGoingSlurHasStanza;

    list<S_msrSlur>           fSlurStartsStack;
    void                      displaySlurStartsStack (
                                  string context);
    
    // ligatures handling
    // ------------------------------------------------------

    S_msrLigature             fCurrentLigatureStartAbove;
    S_msrLigature             fCurrentLigatureStartBelow;

    string                    fCurrentLigaturePlacement;
    msrLigature::msrLigatureKind
                              fCurrentLigatureKind;
    bool                      fOnGoingLigature;
    bool                      fOnGoingLigatureHasStanza;

    // piano pedals handling
    // ------------------------------------------------------

    list<S_msrPedal>          fPendingPedals;
    
/* JMI
    msrDamperPedal::damperPedalKind
                              fCurrentDamperPedalKind;
    int                       fCurrentDamperPedalIntegerValue;

    msrDamperPedal::damperPedalKind
                              fCurrentDamperPedalKind;
    int                       fCurrentDamperPedalIntegerValue;

    msrDamperPedal::damperPedalKind
                              fCurrentDamperPedalKind;
    int                       fCurrentDamperPedalIntegerValue;
*/

    // harp pedals handling
    // ------------------------------------------------------

    msrDiatonicPitchKind      fCurrentHarpPedalDiatonicPitchKind;
    msrAlterationKind         fCurrentHarpPedalAlterationKind;
    S_msrHarpPedalsTuning     fCurrentHarpPedalsTuning;

    // backup handling
    // ------------------------------------------------------

    int                       fCurrentBackupDurationDivisions;
    bool                      fOnGoingBackup;

    // forward handling
    // ------------------------------------------------------

    int                       fCurrentForwardDurationDivisions;
    int                       fCurrentForwardVoiceNumber;
    int                       fCurrentForwardStaffNumber;
    bool                      fOnGoingForward;
};


}


#endif
