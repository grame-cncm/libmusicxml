/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <sstream>
#include <stdlib.h>     /* abort, NULL */

#include "lpsrUtilities.h"

#include "lpsr.h"
#include "xmlPartSummaryVisitor.h"

using namespace std;

namespace MusicXML2
{
//________________________________________________________________________
xmlPartSummaryVisitor::xmlPartSummaryVisitor (S_translationSettings& ts)
{
  fTranslationSettings = ts;
  
  fStavesNumber = 1;
}

xmlPartSummaryVisitor::~xmlPartSummaryVisitor () {}

//________________________________________________________________________
int xmlPartSummaryVisitor::getStavesNumber () const
  { return fStavesNumber; }

//________________________________________________________________________
int xmlPartSummaryVisitor::getTotalVoicesNumber () const
  { return fVoicesNotesCount.size(); }

//________________________________________________________________________
smartlist<int>::ptr xmlPartSummaryVisitor::getStaves() const
{
  smartlist<int>::ptr sl = smartlist<int>::create();
  for (
    map<int, int>::const_iterator i =
      fStavesNotesCount.begin();
      i != fStavesNotesCount.end();
      i++) {
    sl->push_back (i->first);
  } // for
  return sl;
}

//________________________________________________________________________
smartlist<int>::ptr xmlPartSummaryVisitor::getStaves (int voice) const
{
  smartlist<int>::ptr sl = smartlist<int>::create();
  for (
      map<int, map<int, int> >::const_iterator i =
        fStaffVoicesAndNotesNumber.begin();
      i != fStaffVoicesAndNotesNumber.end();
      i++) {
    map<int, int>::const_iterator l = i->second.find( voice );
    if (l != i->second.end())
      sl->push_back (i->first);
  } // for
  return sl;
}

//________________________________________________________________________
smartlist<int>::ptr xmlPartSummaryVisitor::getVoicesIDsList () const
{
  smartlist<int>::ptr sl = smartlist<int>::create();

//  cout << "--> getVoices () : " << std::endl;
  for (
      map<int, int>::const_iterator i =
        fVoicesNotesCount.begin();
      i != fVoicesNotesCount.end();
      i++) {
    /*
    cout <<
      "i->first = " << i->first <<
      ", i->second = " << i->second <<
      std::endl;
    */
    sl->push_back (i->first);
  } // for
  
  return sl;
}

//________________________________________________________________________
smartlist<int>::ptr xmlPartSummaryVisitor::getVoices (int staff) const
{
  smartlist<int>::ptr sl = smartlist<int>::create();
  
  map<int, map<int, int> >::const_iterator i =
    fStaffVoicesAndNotesNumber.find (staff);
  
  if (i != fStaffVoicesAndNotesNumber.end()) {
    for (
        map<int, int>::const_iterator v =
          i->second.begin();
        v != i->second.end();
        v++) {
      sl->push_back (v->first);
    } // for
  }
  
  return sl;
}

//________________________________________________________________________
int xmlPartSummaryVisitor::staffVoicesNumber (int staff) const
{
  int result = 0;
  
  map<int, map<int, int> >::const_iterator i =
    fStaffVoicesAndNotesNumber.find (staff);

  if (i != fStaffVoicesAndNotesNumber.end()) {
    result = i->second.size();
  }
  return result;
}

//________________________________________________________________________
int xmlPartSummaryVisitor::getStaffNotesNumber (int staffID) const
{
  int result = 0;
  
  map<int, int>::const_iterator i =
    fStavesNotesCount.find (staffID);
    
  if (i != fStavesNotesCount.end()) {
    result = i->second;
  }
  return result;
}

//________________________________________________________________________
int xmlPartSummaryVisitor::getVoiceMainStaffID (int voiceID) const
{
  smartlist<int>::ptr staves   = getStaves (voiceID);
  int                 staffID  = 0;
  int                 maxNotes = 0;
  
  for (
      vector<int>::const_iterator i =
        staves->begin();
      i != staves->end();
      i++) {
    int n = getStaffVoiceNotesNumber ((*i), voiceID);
    
    if (n > maxNotes) {
      maxNotes = n;
      staffID  = (*i);
    }
    /*
    cout <<
      "*i = " << *i <<
      ", maxnotes = " << maxnotes <<
      ", staffid = " << staffid <<
      std::endl;
    */
  } // for
  
  return staffID;
}

//________________________________________________________________________
int xmlPartSummaryVisitor::getVoiceNotesNumber (int voiceID) const
{
  int result = 0;
  
  map<int, int>::const_iterator i =
    fVoicesNotesCount.find( voiceID );
    
  if (i != fVoicesNotesCount.end()) {
    result = i->second;
  }
  
  return result;
}

//________________________________________________________________________
int xmlPartSummaryVisitor::getStaffVoiceNotesNumber (
  int staffID, int voiceID) const
{
  int result = 0;
  
  map<int, map<int, int> >::const_iterator i =
    fStaffVoicesAndNotesNumber.find (staffID);
  
  if (i != fStaffVoicesAndNotesNumber.end()) {
    map<int, int>::const_iterator v =
      i->second.find (voiceID);
      
    if (v != i->second.end()) {
      result = v->second;
    }
  }
  
  return result;
}

//________________________________________________________________________
void xmlPartSummaryVisitor::visitStart ( S_divisions& elt ) 
{
  fCurrentDivisions = (int)(*elt);
  if (fTranslationSettings->fTrace) {
    if (fCurrentDivisions == 1)
      cerr << "There is 1 division per quater note" << std::endl;
    else
      cerr <<
        "There are " << fCurrentDivisions << 
        " divisions per quater note" << std::endl;
  }
}

//________________________________________________________________________
void xmlPartSummaryVisitor::visitStart ( S_part& elt)
{
  fCurrentDivisions = 0;
  
  fStavesNumber = 1; // default if there are no <staves> element
  
  fStavesNotesCount.clear();
  fVoicesNotesCount.clear();
  fStaffVoicesAndNotesNumber.clear();

  fCurrentVoice = 0;
  fCurrentStaff = 0;
  
//JMI  fCurrentDuration = -8;
  fCurrentStepIsARest = false;
 // JMI fCurrentLpsrDuration = lpsrDuration::create(0, 0, 0);
  
  fLastLyric = 0;
  fLastSyllabic = 0;
  fOnGoingLyrics = false;
}

//________________________________________________________________________
void xmlPartSummaryVisitor::visitStart ( S_staves& elt)
{
  fStavesNumber = int(*elt);
}

void xmlPartSummaryVisitor::visitStart ( S_staff& elt)
{
  fCurrentStaff = int(*elt);
}
    
void xmlPartSummaryVisitor::visitStart ( S_voice& elt )
{
  fCurrentVoice = int(*elt);
}

//________________________________________________________________________
void xmlPartSummaryVisitor::visitEnd ( S_note& elt)
{
  // account for note in staff
  fStavesNotesCount [fCurrentStaff] ++;
  
  // account for note in voice
  fVoicesNotesCount [fCurrentVoice] ++;
  
  // account for voice in staff
  fStaffVoicesAndNotesNumber [fCurrentStaff] [fCurrentVoice]++;

  cout <<
    "--> xmlPartSummaryVisitor::visitEnd ( S_note& elt) :" << std::endl <<
    "  fCurrentStaff = " << fCurrentStaff << std::endl <<
    "  fCurrentVoice = " << fCurrentVoice << std::endl <<
    "  fStavesNotesCount[fCurrentStaff] = " << fStavesNotesCount[fCurrentStaff] << std::endl <<
    "  fVoicesNotesCount[fCurrentVoice] = " << fVoicesNotesCount[fCurrentVoice] << std::endl <<
    "  fStaffVoicesAndNotesNumber[fCurrentStaff][fCurrentVoice] = " <<
    fStaffVoicesAndNotesNumber [fCurrentStaff][fCurrentVoice] << endl;

}

void xmlPartSummaryVisitor::visitStart ( S_rest& elt)
{
  //  cout << "--> xmlpart2lpsrvisitor::visitStart ( S_rest& elt ) " << std::endl;
  fCurrentStepIsARest = true;
}

void xmlPartSummaryVisitor::visitStart ( S_duration& elt )
{
  int duration=(int)(*elt);
//  cout << "=== xmlPartSummaryVisitor::visitStart ( S_duration& elt ), duration = " << duration << std::endl;
/* JMI
  if (fTranslationSettings->fDebug)
    std::cerr << "duration = " << duration << ", " << 
    "fCurrentDivisions*4 = " << fCurrentDivisions*4 << std::endl;
  if (fCurrentDivisions*4 == 0)
    {
    std::cerr << 
      std::endl << 
      "%--> xmlpart2lpsrvisitor::visitEnd, duration = " << duration <<
      ", fCurrentDivisions*4 = " << fCurrentDivisions*4 << std::endl;
    //return;
  }

  S_lpsrDuration noteDuration =
    lpsrDuration::create(duration, fCurrentDivisions*4, fCurrentDotsNumber);
 */
}

//________________________________________________________________________
void xmlPartSummaryVisitor::visitStart ( S_lyric& elt ) { 
  fLastLyric = elt;
  fOnGoingLyrics = true;
}

void xmlPartSummaryVisitor::visitEnd ( S_lyric& elt ) { 
  fOnGoingLyrics = false;
}

void xmlPartSummaryVisitor::visitStart ( S_syllabic& elt ) {
  fLastSyllabic = elt;
/* JMI
  std::string syllabicValue = fLastSyllabic->getValue();

  if (syllabicValue == "begin") {
    fOnGoingLyrics = true;
  }
  else if (syllabicValue == "end") {
    fOnGoingLyrics = true;
  }
  */
}

void xmlPartSummaryVisitor::visitEnd ( S_text& elt ) 
{
  /*
        <lyric number="1">
          <syllabic>single</syllabic>
          <text>1. Sing</text>
          </lyric>
  */

  string      text = elt->getValue();

  cout << "--> text = |" << text << "|" << std::endl;
  
  std::size_t spacefound=text.find(" ");
  
  if (spacefound!=std::string::npos) text = "\""+text+"\"";
  
  std::string lastLyricNumber   = fLastLyric->getAttributeValue ("number");
  std::string lastSyllabicValue = fLastSyllabic->getValue();
  
  /*
  cout <<
    "--> lastLyricNumber = " << lastLyricNumber <<
    ", lastSyllabicValue = " << lastSyllabicValue <<
    ", text = " << text << endl <<
    flush;
  */
      
  // create fStanzas [lastLyricNumber] on first visit



  std::string fLyricsName = "foo";
  std::string fVoiceName = "faa";
  
  if (! fCurrentLyricsStanza) {
    // create lyrics on first visit
    fCurrentLyrics =
      lpsrLyrics::create ("myLyrics", "myVoice");
      
    // create stanza on first visit
    fCurrentLyricsStanza =
      lpsrStanza::create (fLyricsName, fVoiceName);
 //   fStanzas[lastLyricNumber] = std::list<std::list<std::string> >();
  }
    
  // create stanza chunk
  S_lpsrStanzaChunk
    chunk =
      lpsrStanzaChunk::create (lpsrStanzaChunk::kWordChunk, text);

  if (lastSyllabicValue == "single" || lastSyllabicValue == "begin") {
    // add stanza chunk to current lyrics
    fCurrentLyricsStanza -> addChunkToStanza (chunk);
          
 //   fStanzas[lastLyricNumber].push_back(std::list<std::string>());
 //   fStanzas[lastLyricNumber].back().push_back(text);
  }
  else if (lastSyllabicValue == "middle" || lastSyllabicValue == "end") {
    // add chunk to current stanza
    fCurrentLyricsStanza -> addChunkToStanza (chunk);
 //   fStanzas[lastLyricNumber].back().push_back(text);
  }
  else {
    stringstream s;
    std::string  result;
  
    s << "--> text value " << text << " unknown";
    s >> result;
    lpsrMusicXMLError(result);
  }
}


} // namespace
