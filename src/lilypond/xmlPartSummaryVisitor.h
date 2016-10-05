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
class EXP xmlPartSummaryVisitor : 

  public visitor<S_divisions>,

  public visitor<S_part>,
  public visitor<S_staves>,
  public visitor<S_staff>,
  public visitor<S_voice>,
  
//  public visitor<S_note>,
//  public visitor<S_rest>,
//  public visitor<S_duration>,

  public visitor<S_lyric>
{
  public:
  
    xmlPartSummaryVisitor (S_translationSettings& ts);
    virtual ~xmlPartSummaryVisitor ();
    
    // returns the number of staves for the part
    int getStavesNumber () const;
    
    // returns all the staffIDs list
    list<int> getAllStaves() const;
    
    // returns the number of notes on a staff
    int getStaffNotesNumber (int staffID) const;
    
    // returns the voicesIDs list for one staff
    list<int> getStaffVoices (int staff) const;

        
    // returns the number of voices on a staff 
    int staffVoicesNumber (int staff) const;

    // returns the staffIDs list for one voice
    list<int> getVoiceStaves (int voice) const;
    
    // returns the staffID of the staff that contains the max of the voice notes
    int getVoiceMainStaffID (int voiceID) const;

    // returns the number of notes in a voice
    int getVoiceNotesNumber (int voiceID) const;

    // returns the number of lyrics in a voice
    int getVoiceLyricsNumber (int voiceID) const;

    // returns the number of notes on a voice of a staff
    int getStaffVoiceNotesNumber (int staffID, int voiceID) const;

    // returns the total number of voices 
    int getTotalVoicesNumber () const;
    
    // returns the voicesIDs list
    vector<int> getVoicesIDsList () const;
    
  protected:
  
    virtual void visitStart ( S_part& elt);
    
    virtual void visitStart ( S_staves& elt);
    virtual void visitStart ( S_staff& elt);
    
    virtual void visitStart ( S_voice& elt );
    
    virtual void visitEnd   ( S_note& elt);

    virtual void visitStart ( S_lyric& elt);

  private:
  
    S_translationSettings fTranslationSettings;

    // number of staves (from the staves element)
    int               fStavesNumber; // JMI UNUSED
    
    // number of voices (from the staves element)
 //   int                   fVoicesNumber;
    
    // staves and corresponding number of notes
    map<int, int>     fStavesNotesNumbersMap;
    
    // voices and their number of notes
    // one entry per voice, hence size is the number of voices
    map<int, int>     fVoicesNotesNumbersMap;
    
    // voices and their number of lyrics
    map<int, int>     fVoicesLyricsNumbersMap;
    
    // staves and corresponding voices + number of notes
    map<int, map<int, int> >
                      fStaffVoicesAndNotesNumbersMap;

    // the current staff, in case a voice uses several
    int               fCurrentStaff;

    // the current voice
    int               fCurrentVoice;    
};

/*! @} */

}

#endif
