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
smartlist<int>::ptr xmlPartSummaryVisitor::getAllStaves() const
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
smartlist<int>::ptr xmlPartSummaryVisitor::getVoiceStaves (int voice) const
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
smartlist<int>::ptr xmlPartSummaryVisitor::getStaffVoices (int staff) const
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
  smartlist<int>::ptr voiceStaves = getVoiceStaves (voiceID);
  
  int                 staffID  = 0;
  int                 maxNotes = 0;
  
  for (
      vector<int>::const_iterator i =
        voiceStaves->begin();
      i != voiceStaves->end();
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
void xmlPartSummaryVisitor::visitStart ( S_part& elt)
{
  fStavesNumber = 1; // default if there are no <staves> element
  
  fStavesNotesCount.clear();
  fVoicesNotesCount.clear();
  fStaffVoicesAndNotesNumber.clear();

  fCurrentVoice = 0;
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

//     string       lyricsName =
//                  "_Lyrics" + int2EnglishWord (atoi (it1->first.c_str()));

} // namespace
