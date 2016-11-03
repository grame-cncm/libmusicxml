/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __msr2SummaryVisitor__
#define __msr2SummaryVisitor__

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
class EXP msr2SummaryVisitor : 

  public visitor<S_msrScore>

{
  public:
  
    msr2SummaryVisitor (
        S_translationSettings& ts);
        
    virtual ~msr2SummaryVisitor ();

    void printSummaryFromMsrElementTree (
        const S_msrElement& msrTree);

//    S_msrScore getMSRScore () const
//        { return fMsrScore; };
    
  protected:
      
    virtual void visitStart ( S_msrScore& elt);
    virtual void visitEnd   ( S_msrScore& elt);

  private:
                     
    S_translationSettings   fTranslationSettings;

/*
    // the score we're building
    // ------------------------------------------------------
    S_msrScore              fMsrScore;

    // part group handling
    // ------------------------------------------------------
    int                     fCurrentPartgroupNumber;
    string                  fCurrentPartgroupType;
    string                  fCurrentPartgroupName;
    string                  fCurrentPartgroupAbbreviation;
    string                  fCurrentPartgroupSymbol;
    int                     fCurrentPartgroupSymbolDefaultX;
    string                  fCurrentPartgroupBarline;

    // an implicit part group has to be created
    // if none is specified in the MusicXML data,
    // in which case a part group "stop" has to be forced later
    S_msrPartgroup          fImplicitPartgroup;
    S_msrPartgroup          createImplicitMSRPartgroup (
                              int inputLineNumber);
    
    // part groups numbers can be re-used, they're no identifier
    // we use a map to access them by part group number
    msrPartgroupsMap        fPartgroupsMap;
    S_msrPartgroup          fetchPartgroupInThisVisitor (
                              int partGroupNumber);

    // MusicXML allows part groups to overlap,
    // we use a list in which part groups are orderd by
    // increasing part group <default-x>
    // (all of them they are negative)    
    // the current part group is the front of the list
    
    msrPartgroupsList       fPartgroupsList;
    void                    showPartgroupsData (string context);
    void                    handlePartgroupStart (
                              int                               inputLineNumber,
                              msrPartgroup::PartgroupSymbolKind partGroupSymbol,
                              bool                              partGroupBarline);
    void                    handlePartgroupStop (
                              int inputLineNumber);

    // staff handling
    // ------------------------------------------------------
    int                     fCurrentStaffNumber;
    S_msrStaff              fCurrentStaff;

    // part handling
    // ------------------------------------------------------
    string                  fCurrentPartMusicXMLName;
    string                  fCurrentPartName;
    string                  fCurrentPartAbbreviation;
    string                  fCurrentPartInstrumentName;

    msrPartsMap             fPartsMap;
    S_msrPart               fCurrentPart;

    // voice handling
    // ------------------------------------------------------
    int                     fCurrentVoiceNumber;
    S_msrVoice              fCurrentVoice;
    
    // key handling
    // ------------------------------------------------------
    int                     fCurrentKeyStaffNumber;
    int                     fCurrentFifths;
    int                     fCurrentCancel;
    string                  fCurrentMode;

    // clef handling
    // ------------------------------------------------------
    string                  fCurrentClefSign;
    int                     fCurrentClefLine;
    int                     fCurrentClefOctaveChange; // JMI
    int                     fCurrentClefStaffNumber;

    // direction handling
    // ------------------------------------------------------
    string fCurrentDirectionPlacement;
    string fCurrentDirectionWords;
    bool   fOnGoingDirection;
  
    // metronome handling
    // ------------------------------------------------------
    vector<musicXMLBeatData>
                            fBeatsData;
    int                     fPerMinute;
    musicXMLBeatData        fCurrentBeat;
    bool                    fParentheses;

    // time handling
    // ------------------------------------------------------
    int                     fCurrentTimeStaffNumber;
    string                  fCurrentTimeSymbol;
    int                     fCurrentTimeBeats;
    int                     fCurrentTimeBeatType;
    bool                    fCurrentTimeSenzaMisura;

    // lyrics handling
    // ------------------------------------------------------
    // the last sysllabic spec met (single, begin, middle or end)
    string                  fCurrentSyllabic;
    msrLyricsChunk::LyricsChunkType
                            fCurrentLyricsChunkType;
    // the last lyrics fragment met
    string                  fCurrentText;
    bool                    fCurrentElision;
    
    int                     fCurrentLyricsNumber;
    bool                    fCurrentNoteHasLyrics;
    S_msrLyrics             fCurrentLyrics;
    bool                    fCurrentLyricsHasText;
    void                    handleLyricsText (
                              int inputLineNumber);

    // repeat handling
    // ------------------------------------------------------
    string                  fCurrentBarlineLocation;
    string                  fCurrentBarStyle;
    string                  fCurrentRepeatDirection;
    string                  fCurrentEndingType;
    int                     fCurrentEndingNumber;

    // dividing quater notes in MusicXML
    // ------------------------------------------------------
    int                     fCurrentMusicXMLDivisions;

    // description of the current MusicXML note
    musicXMLNoteData        fMusicXMLNoteData;

    // unpitched notes handling
    // ------------------------------------------------------
    char                    fDisplayStep;
    int                     fDisplayOctave;

    // stem handling
    // ------------------------------------------------------
    enum StemDirection { kStemNeutral, kStemUp, kStemDown };
    
    string                  fCurrentStem;
    StemDirection           fCurrentStemDirection;

    // beam handling
    // ------------------------------------------------------
    string                  fCurrentBeam;
    int                     fCurrentBeamNumber; 

    // note/rest handling
    // ------------------------------------------------------
    void                    handleStandaloneNoteOrRest (
                              S_msrNote newNote);

    // ------------------------------------------------------
    // chord handling
    S_msrChord              fCurrentChord; // cannot be local to a method? JMI
    bool                    fOnGoingChord;
    S_msrChord              createChordFromCurrentNote ();
    void                    handleNoteBelongingToAChord (
                              S_msrNote newNote);
    
    // ------------------------------------------------------
    // articulations handling
    list<S_msrArticulation> fCurrentArticulations;
    
    // dynamics and wedges remain pending until the next note
    // (they precede the note in MusicXML but follow it in LilyPond)
    msrDynamicssList        fPendingDynamics;
    msrWedgesList           fPendingWedges;
    void                    attachPendingDynamicsAndWedgesToNote (
                              S_msrNote note);
       
    // description of the current MSR note
    // ------------------------------------------------------
    string                  fCurrentNoteType;
    S_msrNote               fCurrentNote;
    bool                    fOnGoingNote;

    // tuplet handling
     // ------------------------------------------------------
    int                     fCurrentActualNotes;
    int                     fCurrentNormalNotes;
    string                  fCurrentNormalNoteType;
    // embedded tuplets are numbered 1, 2, ...
    int                     fCurrentTupletNumber;
    msrTuplet::TupletKind   fCurrentTupletKind;
    // remains true until a S_tuplet of type "stop" is met
    bool                    fOnGoingTuplet;
    msrTupletsStack         fCurrentTupletsStack;

    void                    createTupletFromItsecondNote (
                              S_msrNote note);
    void                    finalizeTuplet (
                              S_msrNote note);
    void                    handleNoteBelongingToATuplet (
                              S_msrNote newNote);
         
    // another name for fCurrentNote, fCurrentChord, fCurrentTuplet
    // and the like
// JMI    S_msrElement           fCurrentElement;

    // ties handling
    // ------------------------------------------------------
    string                  fCurrentTiedType;
    string                  fCurrentTiedOrientation;

    // slurs handling
    // ------------------------------------------------------
    int                     fCurrentSlurNumber;
    string                  fCurrentSlurType;
    string                  fCurrentSlurPlacement;
    msrSlur::SlurKind       fCurrentSlurKind;
    bool                    fOnGoingSlur;

    // backup handling
    // ------------------------------------------------------
    int                     fCurrentBackupDuration;
    bool                    fOnGoingBackup;

    // forward handling
    // ------------------------------------------------------
    int                     fCurrentForwardDuration;
    int                     fCurrentForwardVoiceNumber;
    int                     fCurrentForwardStaffNumber;
    bool                    fOnGoingForward;
    */
};

/*! @} */

}

#endif
