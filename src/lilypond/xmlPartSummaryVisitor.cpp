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

  fVoicesNumber = 0;
}

xmlPartSummaryVisitor::~xmlPartSummaryVisitor () {}

//________________________________________________________________________
int xmlPartSummaryVisitor::getStavesNumber () const
  { return fStavesNumber; }

//________________________________________________________________________
int xmlPartSummaryVisitor::getTotalVoicesNumber () const
  { return fVoicesNotesNumbersMap.size(); }

//________________________________________________________________________
int xmlPartSummaryVisitor::getPartVoicesNumber (string partID) const
{
/*
    map<string, map<int, int> >
                      fPartVoicesLyricsNumbersMap;
                      // fPartVoicesLyricsNumbersMap ["partP1"][2] == 4
                      // means there are 4 lyrics in part P1 / voice 2
*/
  int result = 0;
  
  map<string, map<int, int> >::const_iterator i =
    fPartVoicesLyricsNumbersMap.find (partID);
  
  if (i != fPartVoicesLyricsNumbersMap.end())
    result = (*i).second.size();

  return result;
}

//________________________________________________________________________
vector<int> xmlPartSummaryVisitor::getPartVoicesIDs (string partID) const
{
/*
    map<string, map<int, int> >
                      fPartVoicesLyricsNumbersMap;
                      // fPartVoicesLyricsNumbersMap ["partP1"][2] == 4
                      // means there are 4 lyrics in part P1 / voice 2
*/
  cout <<
    "--> getPartVoicesIDs, partid = " << partID << endl;
    
  vector<int> result;
  
  map<string, map<int, int> >::const_iterator i =
    fPartVoicesLyricsNumbersMap.find (partID);
  
  if (i != fPartVoicesLyricsNumbersMap.end()) {
    cout <<
      "--> getPartVoicesIDs, found " << partID <<
      " in fPartVoicesLyricsNumbersMap" << endl;
      
    map<int, int>::const_iterator j;

    for (j = (*i).second.begin(); j != (*i).second.end(); i++)
      result.push_back ((*j).first);
  } else {
    cout <<
      "--> getPartVoicesIDs, NOT found " << partID <<
      " in fPartVoicesLyricsNumbersMap" << endl;
  }
  
    return result;
}

//________________________________________________________________________
int xmlPartSummaryVisitor::getPartVoiceLyricsNumber
  (string partID, int voiceID) const
{
/*
    map<string, map<int, int> >
                      fPartVoicesLyricsNumbersMap;
                      // fPartVoicesLyricsNumbersMap ["partP1"][2] == 4
                      // means there are 4 lyrics in part P1 / voice 2
*/
  int result = 0;
  
  map<string, map<int, int> >::const_iterator i =
    fPartVoicesLyricsNumbersMap.find (partID);
  
  if (i != fPartVoicesLyricsNumbersMap.end()) {
    map<int, int>::const_iterator j = (*i).second.find (voiceID);
  
    if (j != (*i).second.end())
      result = (*j).second;
  }
  
  return result;
}

//________________________________________________________________________
list<int> xmlPartSummaryVisitor::getAllStavesIDs() const
{
  list<int> sl;
  for (
    map<int, int>::const_iterator i =
      fStavesNotesNumbersMap.begin();
      i != fStavesNotesNumbersMap.end();
      i++) {
    sl.push_back (i->first);
  } // for
  return sl;
}

//________________________________________________________________________
/*
list<int> xmlPartSummaryVisitor::getVoiceStaves (int voice) const
{
  list<int> sl;
  for (
      map<int, map<int, int> >::const_iterator i =
        fStaffVoicesAndNotesNumbersMap.begin();
      i != fStaffVoicesAndNotesNumbersMap.end();
      i++) {
    map<int, int>::const_iterator l = i->second.find( voice );
    if (l != i->second.end())
      sl.push_back (i->first);
  } // for
  return sl;
}
*/
//________________________________________________________________________
vector<int> xmlPartSummaryVisitor::getAllVoicesIDs () const
{
  vector<int> sl;

//  cout << "--> getVoices () : " << endl;
  for (
      map<int, int>::const_iterator i =
        fVoicesNotesNumbersMap.begin();
      i != fVoicesNotesNumbersMap.end();
      i++) {
    /*
    cout <<
      "i->first = " << i->first <<
      ", i->second = " << i->second <<
      endl;
    */
    sl.push_back (i->first);
  } // for
  
  return sl;
}

//________________________________________________________________________
list<int> xmlPartSummaryVisitor::getStaffVoices (int staff) const
{
  list<int> sl;
  
  map<int, map<int, int> >::const_iterator i =
    fStaffVoicesAndNotesNumbersMap.find (staff);
  
  if (i != fStaffVoicesAndNotesNumbersMap.end()) {
    for (
        map<int, int>::const_iterator v =
          i->second.begin();
        v != i->second.end();
        v++) {
      sl.push_back (v->first);
    } // for
  }
  
  return sl;
}

//________________________________________________________________________
int xmlPartSummaryVisitor::staffVoicesNumber (int staff) const
{
  int result = 0;
  
  map<int, map<int, int> >::const_iterator i =
    fStaffVoicesAndNotesNumbersMap.find (staff);

  if (i != fStaffVoicesAndNotesNumbersMap.end()) {
    result = i->second.size();
  }
  return result;
}

//________________________________________________________________________
int xmlPartSummaryVisitor::getStaffNotesNumber (int staffID) const
{
  int result = 0;
  
  map<int, int>::const_iterator i =
    fStavesNotesNumbersMap.find (staffID);
    
  if (i != fStavesNotesNumbersMap.end()) {
    result = i->second;
  }
  return result;
}

//________________________________________________________________________
int xmlPartSummaryVisitor::getVoiceMainStaffID (int voiceID) const
{
  list<int> voiceStaves; // JMI  = getVoiceStaves (voiceID);
  
  int                 staffID  = 0;
  int                 maxNotes = 0;
  
  for (
      list<int>::const_iterator i =
        voiceStaves.begin();
      i != voiceStaves.end();
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
      endl;
    */
  } // for
  
  return staffID;
}

//________________________________________________________________________
int xmlPartSummaryVisitor::getVoiceNotesNumber (int voiceID) const
{
  int result = 0;
  
  map<int, int>::const_iterator i =
    fVoicesNotesNumbersMap.find( voiceID );
    
  if (i != fVoicesNotesNumbersMap.end()) {
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
    fStaffVoicesAndNotesNumbersMap.find (staffID);
  
  if (i != fStaffVoicesAndNotesNumbersMap.end()) {
    map<int, int>::const_iterator v =
      i->second.find (voiceID);
      
    if (v != i->second.end()) {
      result = v->second;
    }
  }
  
  return result;
}

//________________________________________________________________________
void xmlPartSummaryVisitor::visitStart ( S_part& elt)
{
  fCurrentPartID = elt->getValue();
  
  fStavesNumber = 1; // default if there are no <staves> element

  fStavesNotesNumbersMap.clear();
  fVoicesNotesNumbersMap.clear();
  fPartVoicesLyricsNumbersMap.clear();
  fStaffVoicesAndNotesNumbersMap.clear();

  fCurrentVoice = 0;
  fPartVoicesLyricsNumbersMap [fCurrentPartID].clear ();

  fCurrentStaff = 0;
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
  if (fCurrentVoice > fVoicesNumber)
    fVoicesNumber = fCurrentVoice;
  /*
  cout <<
    "--> S_voice, fCurrentVoice = " << fCurrentVoice <<
    ", fVoicesNumber = " << fVoicesNumber << endl;
  */
}

//________________________________________________________________________
void xmlPartSummaryVisitor::visitStart ( S_lyric& elt ) { 
  int lyricNumber = int(*elt);

  if (
      lyricNumber
        >
      fPartVoicesLyricsNumbersMap [fCurrentPartID][fCurrentVoice])
    fPartVoicesLyricsNumbersMap [fCurrentPartID][fCurrentVoice] =
      lyricNumber;
}

//________________________________________________________________________
void xmlPartSummaryVisitor::visitEnd ( S_note& elt)
{
  // account for note in staff
  fStavesNotesNumbersMap [fCurrentStaff] ++;
  
  // account for note in voice
  fVoicesNotesNumbersMap [fCurrentVoice] ++;
  
  // account for voice in staff
  fStaffVoicesAndNotesNumbersMap [fCurrentStaff] [fCurrentVoice]++;

/*
  cout <<
    "--> xmlPartSummaryVisitor::visitEnd ( S_note& elt) :" << endl <<
    "  fCurrentStaff = " << fCurrentStaff << endl <<
    "  fCurrentVoice = " << fCurrentVoice << endl <<
    "  fStavesNotesNumbersMap [fCurrentStaff] = " << fStavesNotesNumbersMap [fCurrentStaff] << endl <<
    "  fStavesNotesNumbersMap [fCurrentStaff] = " << fStavesNotesNumbersMap [fCurrentStaff] << endl <<
    "  fVoicesLyrisNumbersMap [fCurrentVoice] = " << fVoicesLyrisNumbersMap [fCurrentVoice] << endl <<
    "  fStaffVoicesAndNotesNumbersMap [fCurrentStaff][fCurrentVoice] = " <<
    fStaffVoicesAndNotesNumbersMap  [fCurrentStaff][fCurrentVoice] << endl;
*/
}

//     string       lyricsName =
//                  "_Lyrics" + int2EnglishWord (atoi (it1->first.c_str()));

} // namespace
