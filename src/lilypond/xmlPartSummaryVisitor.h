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
    
    int         getStavesNumber () const;
    int         staffVoicesNumber (int staff) const;
    int         getStaffNotesNumber (int staffID) const;
    int         getStaffVoiceNotesNumber (int staffID, int voiceID) const;
    
    int         getVoiceNotesNumber (int voiceID) const;
    int         getPartVoiceLyricsNumber (string partID, int voiceID) const;


    list<int>   getAllStavesIDs() const; // JMI
    list<int>   getStaffVoices (int staff) const;
    vector<int> getAllVoicesIDs () const;
    vector<int> getPartVoicesIDs ( string partID) const;


    int         getPartVoicesNumber (string partID) const;
    int         getTotalVoicesNumber () { return fVoicesNumber; }
    
    list<int>   getVoiceStavesNumber (int voice) const; // JMI

    int         getTotalVoicesNumber () const;

    
    // returns the staffID of the staff that contains the max of the voice notes
    int         getVoiceMainStaffID (int voiceID) const;

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
    
    // staves and corresponding number of notes
    map<int, int>     fStavesNotesNumbersMap;
    
    // number of voices (from the staves element)
    int               fVoicesNumber;
    
    // voices and their number of notes
    map<int, int>     fVoicesNotesNumbersMap;
    
    // voices and their number of lyrics
    map<string, map<int, int> >
                      fPartVoicesLyricsNumbersMap;
                      // fPartVoicesLyricsNumbersMap ["partP1"][2] == 4
                      // means there are 4 lyrics in part P1 / voice 2
    
    // staves and corresponding voices + number of notes
    map<int, map<int, int> >
                      fStaffVoicesAndNotesNumbersMap;
                      // fStaffVoicesAndNotesNumbersMap [2][3] == 5
                      // means there are 5 notes in staff 2 / voice 3

    string            fCurrentPartID;
  
    // the current staff, in case a voice uses several
    int               fCurrentStaff;

    // the current voice
    int               fCurrentVoice;    
};

/*! @} */

}

#endif
