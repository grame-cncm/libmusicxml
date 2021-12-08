/*
 MusicXML Library
 Copyright (C) Grame 2006-2013
 
 This Source Code Form is subject to the terms of the Mozilla Public
 License, v. 2.0. If a copy of the MPL was not distributed with this
 file, You can obtain one at http://mozilla.org/MPL/2.0/.
 
 Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
 research@grame.fr
 */

#ifndef __xmlpart2guido__
#define __xmlpart2guido__

#include <ostream>
#include <stack>
#include <queue>
#include <map>
#include <string>

#include "clefvisitor.h"
#include "exports.h"
#include "guido.h"
#include "keysignvisitor.h"
#include "metronomevisitor.h"
#include "notevisitor.h"
#include "rational.h"
#include "timesignvisitor.h"
#include "typedefs.h"
#include "visitor.h"
#include "xml.h"

namespace MusicXML2 
{

/*!
 \ingroup visitors guido
 @{
 */

/*!
 \brief A score visitor to produce a Guido representation.
 */
//______________________________________________________________________________
class EXP xmlpart2guido : 
public clefvisitor,
public timesignvisitor,
public notevisitor,
public keysignvisitor,
public visitor<S_backup>,
public visitor<S_barline>,
public visitor<S_coda>,
public visitor<S_direction>,
public visitor<S_divisions>,
public visitor<S_ending>,
public visitor<S_forward>,
public visitor<S_measure>,
public visitor<S_part>,
public visitor<S_repeat>,
public visitor<S_segno>,
public visitor<S_sound>,
public visitor<S_attributes>         // to get clef, division, staves, time and key in order!
{
    // the guido elements stack
    std::stack<Sguidoelement>	fStack;
    // structure to store delayed elements ie elements enclosed in direction with offset
    typedef struct {
        long delay;
        Sguidoelement element;
    } delayedElement;
    vector<delayedElement>	fDelayed;
    // fields to controls the guido output generation
    bool fGenerateComments, fGenerateBars, fGeneratePositions, fGenerateAutoMeasureNum, fLyricsManualSpacing;
    
    // internal parsing state
    bool	fInCue, fInGrace, fInhibitNextBar, fPendingBar, fBeamOpened, fBeamGrouping, fMeasureEmpty, fWavyTrillOpened, fSingleScopeTrill, fNonStandardNoteHead, fDoubleBar, fTremoloInProgress, fShouldStopOctava;
    
    int fCrescPending, fDiminPending;   // XML Number of Crescendo or Diminuedo that have been opened (0 if none)
    
    int fTextTagOpen;
    int fCurrentOctavaShift; // 0 if none
    int fTupletOpen;    // Number of opened Tuplets
    std::queue<int> fTiedOpen;      // Number of ongoing opened Tied
    
    std::vector< std::pair<int, int> > fSlurStack; // first int: Internal num, 2nd int: XML num
    
    Sguidoelement fLyricOpened;
    
    bool isProcessingChord;
    
    S_measure	fCurrentMeasure;
    S_part      fCurrentPart;
        
    bool	fNotesOnly;				// a flag to generate notes only (used for several voices on the same staff)
    bool	fSkipDirection;			// a flag to skip direction elements (for notes only mode or due to different staff)
    int		fCurrentStaffIndex;		// the index of the current guido staff
    int		fCurrentStaff;			// the staff we're currently generating events for (0 by default)
    int		fTargetStaff;			// the musicxml target staff (0 by default)
    int		fTargetVoice;			// the musicxml target voice (0 by default)
    
    int     fStartMeasure, fEndMeasure, fEndMeasureOffset;          // Starting and Ending measures for Partial conversion. Default=0 meaning non-partial.
    
    long	fCurrentDivision;		// the current measure division, expresses the time unit in division of the quarter note
    long	fCurrentOffset;			// the current direction offset: represents an element relative displacement in current division unit
    rational fCurrentMeasureLength;	// the current measure length (max of the current measure positions)
    rational fCurrentMeasurePosition;// the current position in the measure
    rational fCurrentVoicePosition;    // the current position within a voice
    rational fCurrentScorePosition;	// the current position within the parsing score
    rational fCurrentTimeSign;		// the current time signature
    int		fMeasNum;               // Internal Measure Number mapping
    
    int		fCurrentStemDirection;	// the current stems direction, used for stem direction changes
    int		fPendingPops;			// elements to be popped at chord exit (like fermata, articulations...)
    
    void start (Sguidoelement& elt)		{ fStack.push(elt); }
    void add  (Sguidoelement& elt)		{
        if (!checkMeasureRange()) return;
        if (fStack.size())
            fStack.top()->add(elt);
    }
    void addDelayed (Sguidoelement elt, long offset);	// adding elements to the delayed elements
    void checkDelayed (long time, bool before);						// checks the delayed elements for ready elements
    void push (Sguidoelement& elt)		{
        if (!checkMeasureRange()) return;
        add(elt); fStack.push(elt);
    }
    void pop ()	{
        if (!checkMeasureRange()) return;
        fStack.pop();
    }
    
    void moveMeasureTime (long duration, bool moveVoiceToo=false);
    void reset ();
    void stackClean	();
    
    int  checkArticulation ( const notevisitor& note );			// returns the count of articulations pushed on the stack
    void checkPostArticulation ( const notevisitor& note );      /// Articulations that should be generated in ADD mode after note creation
    
    int  checkChordOrnaments ( const notevisitor& note );			// returns the count of articulations pushed on the stack
    
    deque<notevisitor>  getChord ( const S_note& note );    // build a chord vector
    std::vector<Sxmlelement>  getChord ( const Sxmlelement& note );	// build a chord vector
    void checkStaff		 (int staff );					// check for staff change
    void checkStem		 ( const S_stem& stem );
    void checkBeamBegin	 ( const std::vector<S_beam>& beams, const S_note& elt );
    void checkBeamEnd	 ( const std::vector<S_beam>& beams );
    void checkTupletBegin( const std::vector<S_tuplet>& tuplets,
                          const notevisitor& nv,
                          const S_note& elt);
    void checkTupletEnd	 ( const std::vector<S_tuplet>& tuplets );
    void checkCue		 ( const notevisitor& nv );
    void checkGrace		 ( const notevisitor& nv );
    void checkGraceEnd(const notevisitor& nv);
    int  checkFermata	 ( const notevisitor& stem );
    void checkSlur     ( const std::vector<S_slur>& slurs );
    void checkSlurBegin	 ( const std::vector<S_slur>& slurs );
    void checkSlurEnd	 ( const std::vector<S_slur>& slurs );
    bool isSlurClosing(S_slur elt);
    void checkTiedBegin	 ( const std::vector<S_tied>& tied );
    void checkTiedEnd	 ( const std::vector<S_tied>& tied );
    void checkVoiceTime	 ( const rational& currTime, const rational& voiceTime);
    int  checkRestFormat	 ( const notevisitor& nv );
    bool checkNoteFormat	 ( const notevisitor& nv , rational posInMeasure);
    void checkWavyTrillBegin	 ( const notevisitor& nv );
    void checkWavyTrillEnd	 ( const notevisitor& nv );
    void checkTextEnd();
    void newNote		 ( const notevisitor& nv, rational posInMeasure, const std::vector<Sxmlelement>& fingerings);
    void newChord   (const deque<notevisitor>& nvs, rational posInMeasure);
    
    int checkTremolo(const notevisitor& note, const S_note& elt);
    
    std::string			noteName		( const notevisitor& nv );
    guidonoteduration	noteDuration	( const notevisitor& nv);
    
    std::vector<S_beam>::const_iterator findValue ( const std::vector<S_beam>& beams, const std::string& val ) const;
    std::vector<S_slur>::const_iterator findTypeValue ( const std::vector<S_slur>& slurs, const std::string& val ) const;
    std::vector<S_tied>::const_iterator findTypeValue ( const std::vector<S_tied>& tied, const std::string& val ) const;
    std::vector< std::pair<int, int> >::const_iterator findSlur ( const int xmlnum ) const;
    
    /// Lyrics handling by AC
    void checkLyricBegin	 ( const std::vector<S_lyric>& lyrics );
    void checkLyricEnd	 ( const std::vector<S_lyric>& lyrics );
    std::vector<S_lyric>::const_iterator findValue ( const std::vector<S_lyric>& lyrics,
                                                    const std::string& val ) const;
    std::string lyricParams;
    
    
    static std::string alter2accident ( float alter );
    
protected:
    enum { kStemUndefined, kStemUp, kStemDown, kStemNone };
    enum { kLeaveChord=-1, kNoChord, kEnterChord } chordState;
    
    virtual void visitStart( S_backup& elt);
    virtual void visitStart( S_barline& elt);
    virtual void visitStart( S_coda& elt);
    virtual void visitStart( S_direction& elt);
    virtual void visitStart( S_divisions& elt);
    virtual void visitStart( S_forward& elt);
    virtual void visitStart( S_measure& elt);
    virtual void visitStart( S_note& elt);
    virtual void visitStart( S_part& elt);
    virtual void visitStart( S_segno& elt);
    virtual void visitStart( S_attributes& elt);
    
    virtual void visitEnd  ( S_direction& elt);
    virtual void visitEnd  ( S_ending& elt);
    virtual void visitEnd  ( S_key& elt);
    virtual void visitEnd  ( S_measure& elt);
    virtual void visitEnd  ( S_note& elt);
    virtual void visitEnd  ( S_repeat& elt);
    virtual void visitEnd  ( S_sound& elt);
    virtual void visitEnd  ( S_time& elt);
    virtual void visitEnd  ( S_part& elt);

    std::string parseMetronome ( metronomevisitor &mv );
    
    void parseWedge(MusicXML2::xmlelement *elt, int staff);
    
    void parseOctaveShift(MusicXML2::xmlelement *elt, int staff);
    /// creates an Octave Shift for Guido.
    /// @param type 0 for stop, 8 for one-octave above, -8 for 1-oct below, 15 for two oct above, etc.
    void parseOctaveShift(int type);
    
    bool findNextNote(ctree<xmlelement>::iterator& elt, ctree<xmlelement>::iterator &nextnote);
    float getNoteDistanceFromStaffTop(const notevisitor& nv);
    
    rational durationInCue;
    
    std::map<int, float> fStaffDistance;
        
public:
    xmlpart2guido(bool generateComments, bool generateStem, bool generateBar = true, int startMeasure = 0, int endMeasure = 0, int endMeasureOffset = 0);
    virtual ~xmlpart2guido() {}
    
    Sguidoelement& current ()					{ return fStack.top(); }
    void	initialize (Sguidoelement seq, int staff, int guidostaff, int voice, bool notesonly, rational defaultTimeSign);
    void	generatePositions (bool state)		{ fGeneratePositions = state; }
    const rational& getTimeSign () const		{ return fCurrentTimeSign; }
    bool hasLyrics() const {return fHasLyrics;}
    
    /// Map for staffNum, measureNum, voice-Position and Clef
    std::multimap<int,  std::pair< int, std::pair< rational, string > > > staffClefMap;
    /// Containing default-x positions on a fCurrentVoicePosition (double) of measure(int)
    MusicXMLTimePositions timePositions;
    
    rational fStartPosition, fEndPosition;
    
    int lastMeasureNumber() { return fMeasNum; }
    
    std::map<double, int> measurePositionMap;
    
    double totalPartDuration() { return fCurrentScorePosition.toDouble(); }
    
    /// List of already processed Directions in other voices/staves to skip
    std::vector<int> processedDirections;
    
    // Octavas must be re-applied across voices in the same staff. This property tracks them.
    /// Map containing octavas on a staff: measureNumberString, PositionInMeasure, Type (0 for stop)
    std::map<std::string, std::map<rational, int>> octavas;

private:
    bool fHasLyrics;

    std::string getClef(int staffIndex, rational pos, int measureNum);
    
    void addPosYforNoteHead(const notevisitor& nv, Sxmlelement elt, Sguidoelement& tag, float offset);
    void addPosYforNoteHead(const notevisitor& nv, float xmlY, Sguidoelement& tag, float offset);
    
    /// If default-y exist and defiined with regards to staff, convert it to dY based on note head position and add to tag parameters. Otherwise, use Placement if exists.
    void addPositionOrPlacementToNote(const notevisitor& nv, Sxmlelement elt, Sguidoelement& tag, float offset);
    
    /// Adds `dy` to the tag : if associated note to the element is below the top staff, it uses distance to top staff (musicxml default-y), otherwise distance to notehead.
    /// Used for Bow tags.
    void addDyFromNoteOrStaff(const notevisitor& nv, Sxmlelement elt, Sguidoelement& tag);
    
    /// returns distance of the note from top-staff in HS
    float distanceFromStaffTopForNote(const notevisitor& nv);
    
    bool checkMeasureRange();
    
    void parseTime(ctree<xmlelement>::iterator &iter);
    void parseKey(ctree<xmlelement>::iterator &iter);
    
    void checkOctavaBegin();
    void checkOctavaEnd();
    
    Sguidoelement lastKey;  // Storage used for Partial Conversions
    Sguidoelement lastMeter;  // Storage used for Partial Conversions
    double fPreviousPedalYPos;  // Used for musicxml pedal change
    
    // To skip already visited Directions when looking ahead because of grace notes
    std::queue<int> fDirectionEraserStack;
    
    /// find next note in current measure from the indicated xmlelement. Returns true if found.
    bool findNextNote(MusicXML2::xmlelement *elt, MusicXML2::xmlelement *from);
};


} // namespace MusicXML2


#endif
