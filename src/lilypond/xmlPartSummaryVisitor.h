/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifndef __xmlPartSummaryVisitor__
#define __xmlPartSummaryVisitor__

#include <map>
#include <vector>

#include "smartlist.h"

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
  - a count of staves
  - a map that associates each stave with the corresponding number of notes
  - a map that associates each stave with the corresponding voices and notes
  - a map that associates each stave with the correcponding stanzas
*/
class EXP xmlPartSummaryVisitor : 

  public visitor<S_divisions>,

  public visitor<S_part>,
  public visitor<S_staves>,
  public visitor<S_staff>,
  public visitor<S_voice>,
  
  public visitor<S_note>,
  public visitor<S_rest>,
  public visitor<S_duration>,

  public visitor<S_lyric>,
  public visitor<S_syllabic>,
  public visitor<S_text>
{
  public:
  
    xmlPartSummaryVisitor (S_translationSettings& ts);
    virtual ~xmlPartSummaryVisitor ();
    
    // returns the number of staves for the part
    int getStavesNumber () const;
    
    // returns the total number of voices 
    int getTotalVoicesNumber () const;
    
    // returns the number of voices on a staff 
    int staffVoicesNumber (int staff) const;

    // returns the staff ids list
    smartlist<int>::ptr getStaves() const;
    
    // returns the staff ids list for one voice
    smartlist<int>::ptr getStaves (int voice) const;
    
    // returns the count of notes on a staff
    int getStaffNotesNumber (int staffID) const;

    // returns the voices ids list
    smartlist<int>::ptr getVoicesIDsList () const;
    
    // returns the voices ids list for one staff
    smartlist<int>::ptr getVoices (int staff) const;
    
    // returns the id of the staff that contains the more of the voice notes
    int getVoiceMainStaffID (int voiceID) const;
    
    // returns the number of notes in a voice
    int getVoiceNotesNumber (int voiceID) const;
    
    // returns the number of notes on a voice and a staff
    int getStaffVoiceNotesNumber (int staffID, int voiceID) const;

    S_lpsrLyrics getCurrentLyrics () const { return fCurrentLyrics; }
                                          
  protected:
  
    virtual void visitStart ( S_divisions& elt);

    virtual void visitStart ( S_part& elt);
    
    virtual void visitStart ( S_staves& elt);
    virtual void visitStart ( S_staff& elt);
    
    virtual void visitStart ( S_voice& elt );
    
    virtual void visitEnd   ( S_note& elt);
    virtual void visitStart ( S_rest& elt);
    virtual void visitStart ( S_duration& elt);

    virtual void visitStart ( S_lyric& elt);
    virtual void visitEnd   ( S_lyric& elt);
    virtual void visitStart ( S_syllabic& elt);
    virtual void visitEnd   ( S_text& elt );
    
  private:
  
    S_translationSettings fTranslationSettings;

    // quarter note divisions
    int                   fCurrentDivisions;
    
    // number of staves (from the staves element)
    int                   fStavesNumber; // JMI UNUSED
    
    // number of voices (from the staves element)
 //   int                   fVoicesNumber;
    
    // staves and corresponding count of notes
    std::map<int, int>    fStavesNotesCount;
    
    // voices and their number of notes
    // one entry per voice, hence size is the number of voices
    std::map<int, int>    fVoicesNotesCount;
    
    // staves and corresponding voices + count of notes
    std::map<int, std::map<int, int> >
                          fStaffVoicesAndNotesNumber;

    // the current staff, in case a voice uses several
    int                   fCurrentStaff;

    // the current voice
    int                   fCurrentVoice;

    bool                  fCurrentStepIsARest;
 // JMI   S_lpsrDuration       fCurrentLpsrDuration;
    
    // the last lyric number, i.e. stanza number
    S_lyric               fLastLyric;
    // the last sysllabic spec met (single, begin, middle or end)
    S_syllabic            fLastSyllabic;

    // the current lyrics and stanza
    S_lpsrLyrics          fCurrentLyrics;
    S_lpsrStanza          fCurrentLyricsStanza;
    
    // allow for the creation of skips in lyrics
    bool                  fOnGoingLyrics;
    };

/*! @} */

}

#endif
