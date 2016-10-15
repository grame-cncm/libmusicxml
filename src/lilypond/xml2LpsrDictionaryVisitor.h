/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __xml2LpsrDictionaryVisitor__
#define __xml2LpsrDictionaryVisitor__

#include <map>
#include <vector>

#include "lpsr.h"
#include "musicxml2lpsr.h"

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
class EXP xml2LpsrDictionaryVisitor : 

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

  public visitor<S_chord>,

  public visitor<S_actual_notes>,
  public visitor<S_normal_notes>,
  public visitor<S_normal_type>,
  public visitor<S_tuplet>
{
  public:
  
    xml2LpsrDictionaryVisitor (
        S_translationSettings& ts);
    virtual ~xml2LpsrDictionaryVisitor ();

    S_lpsrDictionary buildDictionaryFromXMLElementTree (
        const Sxmlelement& xmlTree);

    S_lpsrDictionary getDictionary () const
        { return fDictionary; };
    
  protected:
      
    virtual void visitStart ( S_part_list& elt);
    
    virtual void visitStart ( S_part_group& elt);
    virtual void visitStart ( S_group_symbol& elt);
    virtual void visitStart ( S_group_barline& elt);
    
    virtual void visitStart ( S_score_part& elt);
    virtual void visitStart ( S_part_name& elt);
    virtual void visitStart ( S_part_abbreviation& elt);
    virtual void visitStart ( S_instrument_name& elt);
    
    virtual void visitStart ( S_part& elt);
    virtual void visitStart ( S_divisions& elt);
  
    virtual void visitStart ( S_staves& elt);
    virtual void visitStart ( S_staff& elt);
    
    virtual void visitStart ( S_voice& elt );
    
    virtual void visitStart ( S_backup& elt );
    virtual void visitEnd   ( S_backup& elt );
    virtual void visitStart ( S_forward& elt );
    
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

    virtual void visitStart ( S_chord& elt);
    
    virtual void visitStart ( S_actual_notes& elt);
    virtual void visitStart ( S_normal_notes& elt);
    virtual void visitStart ( S_normal_type& elt);
    virtual void visitStart ( S_tuplet& elt);

  private:

    void         internalError (
                    string message);
                     
    void         resetCurrentTime();

    S_translationSettings   fTranslationSettings;

    S_lpsrDictionary        fDictionary;

    // the current part group, if any
    S_lpsrPartGroup         fCurrentPartGroup;
        
    int                     fCurrentStaffNumber;
    S_lpsrStaff             fCurrentStaff;

    S_lpsrPart              fCurrentPart;

    S_lpsrVoice             fCurrentVoice;
    
    // key handling
    int                     fCurrentFifths;
    int                     fCurrentCancel;
    string                  fCurrentMode;

    string                  fCurrentClefSign;
    int                     fCurrentClefLine;
    int                     fCurrentClefOctaveChange; // JMI
    int                     fCurrentClefStaffNumber;

    // metronome handling
    vector<musicXMLBeatData>
                            fBeatsData;
    int                     fPerMinute;
    musicXMLBeatData        fCurrentBeat;
    bool                    fParentheses;

    // time handling
    int                     fCurrentTimeStaffNumber; // ??? JMI
    string                  fCurrentTimeSymbol;
    bool                    fCurrentSenzaMisura;

    // lyrics handling
    // the last sysllabic spec met (single, begin, middle or end)
    string                  fCurrentSyllabic;
    // the last lyrics fragment met
    string                  fCurrentText;
    bool                    fCurrentElision;

    // the current lyrics map and chunk
//    map<int, S_lpsrLyrics>
//                            fCurrentVoiceLyricsMap;
//    S_lpsrLyricsChunk       fCurrentChunk;
    
    void                    initiateLyrics ();

    string                  fCurrentLyricsName;
    string                  fCurrentVoiceName;
    bool                    fCurrentNoteHasLyrics;
    S_lpsrLyrics            fCurrentLyrics;
    bool                    fCurrentLyricsHasText;

    int                     fCurrentMeasureNumber;

    string                  fCurrentBarlineLocation;
    string                  fCurrentBarStyle;
    string                  fCurrentRepeatDirection;
    string                  fCurrentEndingType;
    int                     fCurrentEndingNumber;

    // dividing quater notes in MusicXML
    int                     fCurrentMusicXMLDivisions;

    // description of the current MusicXML note
    musicXMLNoteData        fMusicXMLNoteData;

    // stem handling
    enum StemDirection { kStemNeutral, kStemUp, kStemDown };
    
    string                  fCurrentStem;
    StemDirection           fCurrentStemDirection;

    // beam handling
    string                  fCurrentBeam;
    int                     fCurrentBeamNumber; 

    // description of the current chord
    S_lpsrChord             fCurrentChord;
    bool                    fAChordIsBeingBuilt;
    int                     fCurrentBeats;
    int                     fCurrentBeatType;

    // articulations
    list<S_lpsrArticulation> fCurrentArticulations;
    
    // dynamics and wedges remain pending until the next note
    // (they precede the note in MusicXML but follow it in LilyPond)
    list<S_lpsrDynamics>    fPendingDynamics;
    list<S_lpsrWedge>       fPendingWedges;
       
    // grace notes
    bool                    fCurrentNoteIsAGraceNote;

    // description of the current LPSR note
    S_lpsrNote              fCurrentNote;

    // description of current tuplet
    string                  fCurrentType;
    int                     fCurrentActualNotes;
    int                     fCurrentNormalNotes;
    string                  fCurrentNormalType;
    // embedded tuplets are numbered 1, 2, ...
    int                     fCurrentTupletNumber;
    lpsrTuplet::TupletKind  fCurrentTupletKind;
    // remains true until a S_tuplet of type "stop" is met
    bool                    fATupletIsBeingBuilt;

    S_lpsrTuplet            fCurrentTuplet;
    stack<S_lpsrTuplet>     fCurrentTupletsStack;
         
    // chord handling
    S_lpsrChord             createChordFromCurrentNote ();
    
    // tuplet handling
    void                    createTuplet   (S_lpsrNote note);
    void                    finalizeTuplet (S_lpsrNote note);
    
    // another name for fCurrentNote, fCurrentChord, fCurrentTuplet
    // and the like
    S_lpsrElement           fCurrentElement;

    // ties handling
    string                  fCurrentTiedType;
    string                  fCurrentTiedOrientation;

    // slurs handling
    string                  fCurrentSlurNumber; // JMI
    string                  fCurrentSlurType; // JMI
    string                  fCurrentSlurPlacement; // JMI
    lpsrSlur::SlurKind      fCurrentSlurKind;

    // backup and forward handling
    bool                    fOnGoingBackup;  // S_duration disambiguation
    int                     fCurrentBackupDuration;

};


/*! @} */

}

#endif
