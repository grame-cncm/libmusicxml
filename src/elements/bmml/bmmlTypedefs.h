/*
  MusicXML Library
  Copyright (C) Grame 2006-2020

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

//======================================================================
// DON'T MODIFY THIS FILE!
// It is automatically generated from the MusicXML schema.
// (see the schema folder) 
//======================================================================

#ifndef __typedefs__
#define __typedefs__

#include "elements.h"
#include "types.h"

namespace MusicXML2 
{

typedef SMARTP<musicxml<kComment> >					S_comment;
typedef SMARTP<musicxml<kProcessingInstruction> >	S_processing_instruction;

typedef SMARTP<musicxml<k_abbr_name> >		S_abbr_name;
typedef SMARTP<musicxml<k_accidental> >		S_accidental;
typedef SMARTP<musicxml<k_accordion_register> >		S_accordion_register;
typedef SMARTP<musicxml<k_accordion_row> >		S_accordion_row;
typedef SMARTP<musicxml<k_alteration> >		S_alteration;
typedef SMARTP<musicxml<k_alternation> >		S_alternation;
typedef SMARTP<musicxml<k_alternation_ref> >		S_alternation_ref;
typedef SMARTP<musicxml<k_appoggiatura_ref> >		S_appoggiatura_ref;
typedef SMARTP<musicxml<k_barline> >		S_barline;
typedef SMARTP<musicxml<k_barline_type> >		S_barline_type;
typedef SMARTP<musicxml<k_barre> >		S_barre;
typedef SMARTP<musicxml<k_bow> >		S_bow;
typedef SMARTP<musicxml<k_breath> >		S_breath;
typedef SMARTP<musicxml<k_chord> >		S_chord;
typedef SMARTP<musicxml<k_chord_data> >		S_chord_data;
typedef SMARTP<musicxml<k_chord_prefix> >		S_chord_prefix;
typedef SMARTP<musicxml<k_chord_type> >		S_chord_type;
typedef SMARTP<musicxml<k_clef> >		S_clef;
typedef SMARTP<musicxml<k_coda> >		S_coda;
typedef SMARTP<musicxml<k_dot> >		S_dot;
typedef SMARTP<musicxml<k_duration> >		S_duration;
typedef SMARTP<musicxml<k_dynamic> >		S_dynamic;
typedef SMARTP<musicxml<k_editorial_mark> >		S_editorial_mark;
typedef SMARTP<musicxml<k_ending> >		S_ending;
typedef SMARTP<musicxml<k_family> >		S_family;
typedef SMARTP<musicxml<k_fermata> >		S_fermata;
typedef SMARTP<musicxml<k_fingering> >		S_fingering;
typedef SMARTP<musicxml<k_foot_crossing> >		S_foot_crossing;
typedef SMARTP<musicxml<k_generic_text> >		S_generic_text;
typedef SMARTP<musicxml<k_hand> >		S_hand;
typedef SMARTP<musicxml<k_harmonic> >		S_harmonic;
typedef SMARTP<musicxml<k_inaccord> >		S_inaccord;
typedef SMARTP<musicxml<k_interval> >		S_interval;
typedef SMARTP<musicxml<k_interval_data> >		S_interval_data;
typedef SMARTP<musicxml<k_interval_ref> >		S_interval_ref;
typedef SMARTP<musicxml<k_interval_type> >		S_interval_type;
typedef SMARTP<musicxml<k_intervals> >		S_intervals;
typedef SMARTP<musicxml<k_key_signature> >		S_key_signature;
typedef SMARTP<musicxml<k_line_of_continuation> >		S_line_of_continuation;
typedef SMARTP<musicxml<k_lyric> >		S_lyric;
typedef SMARTP<musicxml<k_lyric_prefix> >		S_lyric_prefix;
typedef SMARTP<musicxml<k_lyric_repeat> >		S_lyric_repeat;
typedef SMARTP<musicxml<k_lyric_repetition> >		S_lyric_repetition;
typedef SMARTP<musicxml<k_lyrics> >		S_lyrics;
typedef SMARTP<musicxml<k_merged_text> >		S_merged_text;
typedef SMARTP<musicxml<k_meta_data> >		S_meta_data;
typedef SMARTP<musicxml<k_metronome> >		S_metronome;
typedef SMARTP<musicxml<k_metronome_equal> >		S_metronome_equal;
typedef SMARTP<musicxml<k_metronome_note_type> >		S_metronome_note_type;
typedef SMARTP<musicxml<k_metronome_value> >		S_metronome_value;
typedef SMARTP<musicxml<k_midi_instrument> >		S_midi_instrument;
typedef SMARTP<musicxml<k_midi_metronome> >		S_midi_metronome;
typedef SMARTP<musicxml<k_multimeasure> >		S_multimeasure;
typedef SMARTP<musicxml<k_music_hyphen> >		S_music_hyphen;
typedef SMARTP<musicxml<k_name> >		S_name;
typedef SMARTP<musicxml<k_newline> >		S_newline;
typedef SMARTP<musicxml<k_note> >		S_note;
typedef SMARTP<musicxml<k_note_data> >		S_note_data;
typedef SMARTP<musicxml<k_note_ref> >		S_note_ref;
typedef SMARTP<musicxml<k_note_type> >		S_note_type;
typedef SMARTP<musicxml<k_nuance> >		S_nuance;
typedef SMARTP<musicxml<k_nuance_ref> >		S_nuance_ref;
typedef SMARTP<musicxml<k_nuances> >		S_nuances;
typedef SMARTP<musicxml<k_number> >		S_number;
typedef SMARTP<musicxml<k_octave> >		S_octave;
typedef SMARTP<musicxml<k_organ_pedal> >		S_organ_pedal;
typedef SMARTP<musicxml<k_ornament> >		S_ornament;
typedef SMARTP<musicxml<k_ornament_type> >		S_ornament_type;
typedef SMARTP<musicxml<k_part> >		S_part;
typedef SMARTP<musicxml<k_part_data> >		S_part_data;
typedef SMARTP<musicxml<k_part_list> >		S_part_list;
typedef SMARTP<musicxml<k_part_name> >		S_part_name;
typedef SMARTP<musicxml<k_pedal> >		S_pedal;
typedef SMARTP<musicxml<k_pitch> >		S_pitch;
typedef SMARTP<musicxml<k_pizzicato> >		S_pizzicato;
typedef SMARTP<musicxml<k_rasgueado> >		S_rasgueado;
typedef SMARTP<musicxml<k_repeat> >		S_repeat;
typedef SMARTP<musicxml<k_repeat_data> >		S_repeat_data;
typedef SMARTP<musicxml<k_repeat_ref> >		S_repeat_ref;
typedef SMARTP<musicxml<k_repeats> >		S_repeats;
typedef SMARTP<musicxml<k_repetition> >		S_repetition;
typedef SMARTP<musicxml<k_rest> >		S_rest;
typedef SMARTP<musicxml<k_rest_data> >		S_rest_data;
typedef SMARTP<musicxml<k_rest_type> >		S_rest_type;
typedef SMARTP<musicxml<k_rhythmic_group> >		S_rhythmic_group;
typedef SMARTP<musicxml<k_right_string_fingering> >		S_right_string_fingering;
typedef SMARTP<musicxml<k_score> >		S_score;
typedef SMARTP<musicxml<k_score_data> >		S_score_data;
typedef SMARTP<musicxml<k_score_header> >		S_score_header;
typedef SMARTP<musicxml<k_segno> >		S_segno;
typedef SMARTP<musicxml<k_separator> >		S_separator;
typedef SMARTP<musicxml<k_shift_line> >		S_shift_line;
typedef SMARTP<musicxml<k_slur> >		S_slur;
typedef SMARTP<musicxml<k_slur_ref> >		S_slur_ref;
typedef SMARTP<musicxml<k_slurs> >		S_slurs;
typedef SMARTP<musicxml<k_space> >		S_space;
typedef SMARTP<musicxml<k_stem> >		S_stem;
typedef SMARTP<musicxml<k_stem_data> >		S_stem_data;
typedef SMARTP<musicxml<k_stem_type> >		S_stem_type;
typedef SMARTP<musicxml<k_string> >		S_string;
typedef SMARTP<musicxml<k_string_fingering> >		S_string_fingering;
typedef SMARTP<musicxml<k_string_position> >		S_string_position;
typedef SMARTP<musicxml<k_stroke> >		S_stroke;
typedef SMARTP<musicxml<k_syllabic_mute> >		S_syllabic_mute;
typedef SMARTP<musicxml<k_syllabic_parenthesis> >		S_syllabic_parenthesis;
typedef SMARTP<musicxml<k_syllabic_slur> >		S_syllabic_slur;
typedef SMARTP<musicxml<k_syllabic_text> >		S_syllabic_text;
typedef SMARTP<musicxml<k_syllable> >		S_syllable;
typedef SMARTP<musicxml<k_syllable_mute> >		S_syllable_mute;
typedef SMARTP<musicxml<k_syllable_ref> >		S_syllable_ref;
typedef SMARTP<musicxml<k_tie> >		S_tie;
typedef SMARTP<musicxml<k_tie_ref> >		S_tie_ref;
typedef SMARTP<musicxml<k_ties> >		S_ties;
typedef SMARTP<musicxml<k_time_signature> >		S_time_signature;
typedef SMARTP<musicxml<k_tremolo> >		S_tremolo;
typedef SMARTP<musicxml<k_tremolo_ref> >		S_tremolo_ref;
typedef SMARTP<musicxml<k_tuplet> >		S_tuplet;
typedef SMARTP<musicxml<k_tuplet_ref> >		S_tuplet_ref;
typedef SMARTP<musicxml<k_tuplets> >		S_tuplets;
typedef SMARTP<musicxml<k_unknown> >		S_unknown;
typedef SMARTP<musicxml<k_value_prefix> >		S_value_prefix;
}
#endif
