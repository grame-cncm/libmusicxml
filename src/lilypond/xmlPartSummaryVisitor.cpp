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
  : fStavesCount(1)
  { fTranslationSettings = ts; }

xmlPartSummaryVisitor::~xmlPartSummaryVisitor () {}

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
  
  fStavesCount = 1;
  fStavesNotesCount.clear();
  fVoicesNotesCount.clear();
  fStaffVoices.clear();

  fVoice = 0;
  fStaff = 0;
  
//JMI  fCurrentDuration = -8;
  fCurrentStepIsARest = false;
 // JMI fCurrentLpsrDuration = lpsrDuration::create(0, 0, 0);
  
  fLastLyric = 0;
  fLastSyllabic = 0;
  fStanzas.clear();
  fOnGoingLyrics = false;
}

//________________________________________________________________________
void xmlPartSummaryVisitor::visitStart ( S_staves& elt)
{
  fStavesCount = int(*elt);
}

//________________________________________________________________________
void xmlPartSummaryVisitor::visitEnd ( S_note& elt)
{
  // account for note in staff
  fStavesNotesCount [fStaff] ++;
  
  // account for note in voice
  fVoicesNotesCount [fVoice] ++;
  
  // account for voice in staff
  fStaffVoices [fStaff] [fVoice]++;
  /*
  cout << 
    "fStaff = " << fStaff << 
    ", fVoice = " << fVoice <<
    ", fStavesNotesCount[fStaff] = " << fStavesNotesCount[fStaff] << std::endl <<
    ", fVoicesNotesCount[fVoice] = " << fVoicesNotesCount[fVoice] <<
    ", fStaffVoices[fStaff][fVoice] = " << fStaffVoices[fStaff][fVoice] << endl;
  */
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
  if (! fStanzas.count(lastLyricNumber)) {
    fStanzas[lastLyricNumber] = std::list<std::list<std::string> >();
  }
    
  if (lastSyllabicValue == "single") {
    fStanzas[lastLyricNumber].push_back(std::list<std::string>());
    fStanzas[lastLyricNumber].back().push_back(text);
  }
  else if (lastSyllabicValue == "begin") {
    fStanzas[lastLyricNumber].push_back(std::list<std::string>());
    fStanzas[lastLyricNumber].back().push_back(text);
  }
  else if (lastSyllabicValue == "middle") {
    fStanzas[lastLyricNumber].back().push_back(text);
  }
  else if (lastSyllabicValue == "end") {
    fStanzas[lastLyricNumber].back().push_back(text);
  }
}

//________________________________________________________________________
smartlist<int>::ptr xmlPartSummaryVisitor::getStaves() const
{
  smartlist<int>::ptr sl = smartlist<int>::create();
  for ( map<int, int>::const_iterator i = fStavesNotesCount.begin(); i != fStavesNotesCount.end(); i++) {
    sl->push_back (i->first);
  } // for
  return sl;
}

//________________________________________________________________________
smartlist<int>::ptr xmlPartSummaryVisitor::getStaves (int voice) const
{
  smartlist<int>::ptr sl = smartlist<int>::create();
  for (
      map<int, map<int, int> >::const_iterator i = fStaffVoices.begin();
      i != fStaffVoices.end();
      i++) {
    map<int, int>::const_iterator l = i->second.find( voice );
    if (l != i->second.end())
      sl->push_back (i->first);
  } // for
  return sl;
}

//________________________________________________________________________
smartlist<int>::ptr xmlPartSummaryVisitor::getVoices () const
{
  smartlist<int>::ptr sl = smartlist<int>::create();
  
  for (
      map<int, int>::const_iterator i = fVoicesNotesCount.begin();
      i != fVoicesNotesCount.end();
      i++) {
    /*
    cout <<
      "i->first = " << i->first <<
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
  
  map<int, map<int, int> >::const_iterator i = fStaffVoices.find( staff );
  
  if (i != fStaffVoices.end()) {
    for (
        map<int, int>::const_iterator v = i->second.begin();
        v != i->second.end();
        v++) {
      sl->push_back (v->first);
    } // for
  }
  
  return sl;
}

//________________________________________________________________________
int xmlPartSummaryVisitor::countVoices (int staff) const
{
  int count = 0;
  map<int, map<int, int> >::const_iterator i = fStaffVoices.find( staff );
  if (i != fStaffVoices.end()) {
    count = i->second.size();
  }
  return count;
}

//________________________________________________________________________
int xmlPartSummaryVisitor::getStaffNotesCount (int id) const
{
  int count = 0;
  map<int, int>::const_iterator i = fStavesNotesCount.find( id );
  if (i != fStavesNotesCount.end()) {
    count = i->second;
  }
  return count;
}

//________________________________________________________________________
int xmlPartSummaryVisitor::getMainStaff (int voiceid) const
{
  smartlist<int>::ptr v = getStaves (voiceid);
  int                 staffid = 0;
  int                 maxnotes = 0;
  
  for (
      vector<int>::const_iterator i = v->begin();
      i != v->end();
      i++) {
    int n = getVoiceNotesCount (*i, voiceid);
    
    if (n > maxnotes) {
      maxnotes = n;
      staffid = *i;
    }
    /*
    cout <<
      "*i = " << *i <<
      ", maxnotes = " << maxnotes <<
      ", staffid = " << staffid <<
      std::endl;
    */
  } // for
  
  return staffid;
}

//________________________________________________________________________
int xmlPartSummaryVisitor::getVoiceNotesCount (int voiceid) const
{
  int count = 0;
  
  map<int, int>::const_iterator i = fVoicesNotesCount.find( voiceid );
  if (i != fVoicesNotesCount.end()) {
    count = i->second;
  }
  
  return count;
}

//________________________________________________________________________
int xmlPartSummaryVisitor::getVoiceNotesCount (int staffid, int voiceid) const
{
  int count = 0;
  map<int, map<int, int> >::const_iterator i = fStaffVoices.find( staffid );
  
  if (i != fStaffVoices.end()) {
    map<int, int>::const_iterator v = i->second.find( voiceid );
    if (v != i->second.end()) {
      count = v->second;
    }
  }
  
  return count;
}


} // namespace
