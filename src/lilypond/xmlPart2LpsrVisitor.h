/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __xmlPart2LpsrVisitor__
#define __xmlPart2LpsrVisitor__

#include <ostream>
#include <stack>
#include <map>
#include <string>
#include <list>

#include "exports.h"
#include "rational.h"
#include "typedefs.h"
#include "visitor.h"
#include "xml.h"

#include "lpsr.h"

namespace MusicXML2 
{

/*!
\ingroup visitors lpsr
@{
*/

/*!
\brief A part visitor to produce a LilyPond representation.
*/
//______________________________________________________________________________
class EXP xmlPart2LpsrVisitor : 
  
  public visitor<S_divisions>,

  public visitor<S_part>,

  public visitor<S_barline>,

  public visitor<S_note>,
  public visitor<S_step>,
  public visitor<S_alter>,
  public visitor<S_octave>,
  public visitor<S_duration>,
  public visitor<S_dot>,
  public visitor<S_voice>,
  public visitor<S_type>,
  public visitor<S_stem>,
  
  public visitor<S_rest>,

  public visitor<S_beam>,

  public visitor<S_chord>,

  public visitor<S_actual_notes>,
  public visitor<S_normal_notes>,
  public visitor<S_tuplet>,
  
  public visitor<S_staff>,

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

  public visitor<S_backup>,
  public visitor<S_print>,
  public visitor<S_coda>,
  public visitor<S_direction>,
  public visitor<S_dynamics>,
  public visitor<S_ending>,
  public visitor<S_forward>,
  public visitor<S_measure>,
  public visitor<S_octave_shift>,
  public visitor<S_repeat>,
  public visitor<S_segno>,
  public visitor<S_trill_mark>,
  public visitor<S_sound>,

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

  public visitor<S_wedge>
{
  public:
  
    enum type { kUndefinedType, kPitched, kUnpitched, kRest };

    xmlPart2LpsrVisitor( S_translationSwitches& ts, SlpsrPart part);
    virtual ~xmlPart2LpsrVisitor() {}
    
    void  initialize (
      SlpsrElement seq, 
      int staff,
      int lpsrstaff,
      int voice, 
      bool notesonly,
      rational defaultTimeSign);
      
    SlpsrPart&     getLilypondpart () { return fLpsrpart; };
    
    void           addElementToPartSequence (SlpsrElement& elt);
    SlpsrElement   getLastElementOfPartSequence();
    void           removeLastElementOfPartSequence ();
    
  protected:
  
    enum { kStemUndefined, kStemUp, kStemDown, kStemNone };

    virtual void visitStart ( S_backup& elt);
    virtual void visitStart ( S_barline& elt);
    virtual void visitStart ( S_print& elt);
    virtual void visitStart ( S_coda& elt);
    virtual void visitStart ( S_direction& elt);
    virtual void visitStart ( S_divisions& elt);
    virtual void visitStart ( S_dynamics& elt);
    virtual void visitStart ( S_forward& elt);
    virtual void visitStart ( S_measure& elt);
    
    virtual void visitStart ( S_note& elt);
    virtual void visitStart ( S_step& elt);
    virtual void visitStart ( S_alter& elt);
    virtual void visitStart ( S_octave& elt);
    virtual void visitStart ( S_duration& elt);
    virtual void visitStart ( S_dot& elt );
    virtual void visitStart ( S_voice& elt);
    virtual void visitStart ( S_type& elt);
    virtual void visitStart ( S_stem& elt);
    virtual void visitStart ( S_staff& elt);
    virtual void visitEnd   ( S_note& elt);

    virtual void visitStart ( S_rest& elt);

    virtual void visitStart ( S_beam& elt);

    virtual void visitStart ( S_chord& elt);
    
    virtual void visitStart ( S_actual_notes& elt);
    virtual void visitStart ( S_normal_notes& elt);
    virtual void visitStart ( S_tuplet& elt);


    virtual void visitStart ( S_time& elt );
    virtual void visitStart ( S_beats& elt );
    virtual void visitStart ( S_beat_type& elt );
    virtual void visitStart ( S_senza_misura& elt );

    virtual void visitStart ( S_key& elt );
    virtual void visitStart ( S_cancel& elt );
    virtual void visitStart ( S_fifths& elt );
    virtual void visitStart ( S_mode& elt );

    virtual void visitStart ( S_clef& elt );
    virtual void visitStart ( S_sign& elt );
    virtual void visitStart ( S_line& elt );
    virtual void visitStart ( S_clef_octave_change& elt );

    virtual void visitStart ( S_metronome& elt );
    virtual void visitEnd   ( S_metronome& elt );
    virtual void visitStart ( S_beat_unit& elt );
    virtual void visitStart ( S_beat_unit_dot& elt );
    virtual void visitStart ( S_per_minute& elt );

    virtual void visitStart ( S_octave_shift& elt);
    virtual void visitStart ( S_part& elt);
    virtual void visitStart ( S_segno& elt);

    virtual void visitStart ( S_trill_mark& elt);

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

    virtual void visitEnd   ( S_clef& elt);
    virtual void visitEnd   ( S_direction& elt);
    virtual void visitEnd   ( S_ending& elt);
    virtual void visitEnd   ( S_key& elt);
    virtual void visitEnd   ( S_measure& elt);
    
    virtual void visitEnd   ( S_repeat& elt);
    virtual void visitEnd   ( S_sound& elt);
    virtual void visitEnd   ( S_time& elt);

  public:
  
    enum { kStandardLine, kTrebleStdLine=2, kBassStdLine=4, kCStdLine=3, kTabStdLine=5 };
    enum { kNoNumber = -1 };

  private:
  
    // fields to controls the lpsr output generation
    S_translationSwitches   fTranslationSwitches;
  
    // the current measure divisions, expresses the time unit in division of the quarter note
    enum { kNoStaffNumber = -1 };

    void resetCurrentTime();

    int                     fCurrentDivisions;
    
    // description of the current MusicXML note
    musicXMLNoteData        fMusicXMLNoteData;

    // description of the current chord
    SlpsrChord              fCurrentChord;
    bool                    fAChordIsBeingBuilt = false;
    int                     fCurrentBeats;
    int                     fCurrentBeatType;
     
    /*! gives the time sign as a rational
      \param index an index into fTimeSignatures vector
      \return the time signature as a rational or rational(0,1)
    */
  // JMI  rational            timeSignatureFromIndex(int index);
    
 //   std::vector<std::pair<std::string,std::string> >
//                        fTimeSignatures;
  
    // staff handling
    int                     fCurrentStaff;    
    // the staff we're currently generating events for (0 by default)
    int                     fStaffNumber;
    int                     fCurrentStaffIndex;   // the index of the current lpsr staff

    int                     fTargetStaff;     // the musicxml target staff (0 by default)
    int                     fTargetVoice;     // the musicxml target voice (0 by default)
  
    // time handling
    std::string             fSymbol;
    bool                    fSenzaMisura;

    // the part containing the generated code for the part
    SlpsrPart               fLpsrpart; 

    // description of the current LPSR note
    SlpsrNote               fCurrentNote;
    

    SlpsrBeam               fCurrentBeam;


    // description of current tuplet
    int                     fCurrentActualNotes;
    int                     fCurrentNormalNotes;
    // embedded tuplets are numbered 1, 2, ...
    int                     fCurrentTupletNumber;
    lpsrTuplet::TupletKind  fCurrentTupletKind;
    // remains true until a S_tuplet of type "stop" is met
    bool                    fCurrentNoteBelongsToATuplet = false;

    SlpsrTuplet             fCurrentTuplet;
    std::stack<SlpsrTuplet> fCurrentTupletsStack;
         
//    std::string         fCurrentType;
//    std::string         fCurrentStem;
   
    // LPSR informations
    lpsrNote::MusicXMLDiatonicPitch
                            fMusicXMLDiatonicPitch;
    
    // chord handling
    void                    createChord (SlpsrDuration noteDuration);
    
    // tuplet handling
    void                    createTuplet   (SlpsrNote note);
    void                    finalizeTuplet (SlpsrNote note);
    
    // key handling
    int                     fCurrentFifths;
    int                     fCurrentCancel;
    std::string             fCurrentMode;

    std::string             fSign;
    int                     fLine;
    int                     fOctaveChange;
    int                     fNumber;

    // metronome handling
    typedef struct {
      std::string           fUnit;
      int                   fDots;
    } beat;

    std::vector<beat>       fBeats;
    int                     fPerMinute;
    beat                    fCurrentBeat;

    void                    resetMetronome();
    virtual void            resetMetronome(beat& b);
    
    // another name for fCurrentNote, fCurrentChord, fCurrentTuplet and the like
    SlpsrElement            fCurrentElement;
    
    // structure to store delayed elements ie elements enclosed in direction with offset
    typedef struct {
      int                   fDelay;
      SlpsrElement          fElement;
    } delayedElement;
    vector<delayedElement>  fDelayed;
    
    // dynamics and wedges remain pending until the next note
    // (they precede the note in MusicXML but follow it in LilyPond)
    std::list<SlpsrDynamics> fPendingDynamics;
    std::list<SlpsrWedge>    fPendingWedges;
       
    int                      fMeasureNumber;
    S_measure                fCurrentMeasure;  
  
  
    int       fCurrentOffset;     // the current direction offset: represents an element relative displacement in current division unit

    rational  fCurrentMeasureLength; // the current measure length (max of the current measure positions)
    rational  fCurrentMeasurePosition;// the current position in the measure
    rational  fCurrentVoicePosition; // the current position within a voice

    void addDelayed (SlpsrElement elt, int offset);  // adding elements to the delayed elements
    void checkDelayed (int time);            // checks the delayed elements for ready elements 
};


} // namespace MusicXML2


#endif
