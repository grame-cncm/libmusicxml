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

#include "partsummaryvisitor.h"

using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
void partsummaryvisitor::visitStart ( S_part& elt)
{
  fStavesCount = 1;
  fStavesNotesCount.clear();
  fVoicesNotesCount.clear();
  fStaffVoices.clear();

  fVoice = 0;
  fStaff = 0;
}

//________________________________________________________________________
void partsummaryvisitor::visitStart ( S_staves& elt)
{
  fStavesCount = int(*elt);
}

//________________________________________________________________________
void partsummaryvisitor::visitEnd ( S_note& elt)
{
  fStavesNotesCount[fStaff]++;
  fVoicesNotesCount[fVoice]++;
  fStaffVoices[fStaff][fVoice]++;
  /*
  cout << 
    "fStaff = " << fStaff << 
    ", fVoice = " << fVoice <<
    ", fStavesNotesCount[fStaff] = " << fStavesNotesCount[fStaff] << std::endl <<
    ", fVoicesNotesCount[fVoice] = " << fVoicesNotesCount[fVoice] <<
    ", fStaffVoices[fStaff][fVoice] = " << fStaffVoices[fStaff][fVoice] << endl;
  */
}


  /*
      
  // create fStanzas[lastLyricNumber] on first visit
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
  */
  

//________________________________________________________________________
smartlist<int>::ptr partsummaryvisitor::getStaves() const
{
  smartlist<int>::ptr sl = smartlist<int>::create();
  for ( map<int, int>::const_iterator i = fStavesNotesCount.begin(); i != fStavesNotesCount.end(); i++) {
    sl->push_back (i->first);
  }
  return sl;
}

//________________________________________________________________________
smartlist<int>::ptr partsummaryvisitor::getStaves (int voice) const
{
  smartlist<int>::ptr sl = smartlist<int>::create();
  for ( map<int, map<int, int> >::const_iterator i = fStaffVoices.begin(); i != fStaffVoices.end(); i++) {
    map<int, int>::const_iterator l = i->second.find( voice );
    if (l != i->second.end())
      sl->push_back (i->first);
  }
  return sl;
}

//________________________________________________________________________
smartlist<int>::ptr partsummaryvisitor::getVoices () const
{
  smartlist<int>::ptr sl = smartlist<int>::create();
  for ( map<int, int>::const_iterator i = fVoicesNotesCount.begin(); i != fVoicesNotesCount.end(); i++) {
    /*
    cout <<
      "i->first = " << i->first <<
      std::endl;
    */
    sl->push_back (i->first);
  }
  return sl;
}

//________________________________________________________________________
smartlist<int>::ptr partsummaryvisitor::getVoices (int staff) const
{
  smartlist<int>::ptr sl = smartlist<int>::create();
  map<int, map<int, int> >::const_iterator i = fStaffVoices.find( staff );
  if (i != fStaffVoices.end()) {
    for ( map<int, int>::const_iterator v = i->second.begin(); v != i->second.end(); v++) {
      sl->push_back (v->first);
    }
  }
  return sl;
}

//________________________________________________________________________
int partsummaryvisitor::countVoices (int staff) const
{
  int count = 0;
  map<int, map<int, int> >::const_iterator i = fStaffVoices.find( staff );
  if (i != fStaffVoices.end()) {
    count = i->second.size();
  }
  return count;
}

//________________________________________________________________________
int partsummaryvisitor::getStaffNotesCount (int id) const
{
  int count = 0;
  map<int, int>::const_iterator i = fStavesNotesCount.find( id );
  if (i != fStavesNotesCount.end()) {
    count = i->second;
  }
  return count;
}

//________________________________________________________________________
int partsummaryvisitor::getMainStaff (int voiceid) const
{
  smartlist<int>::ptr v = getStaves (voiceid);
  int staffid = 0;
  int maxnotes = 0;
  for (vector<int>::const_iterator i = v->begin(); i != v->end(); i++) {
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
  }
  return staffid;
}

//________________________________________________________________________
int partsummaryvisitor::getVoiceNotesCount (int voiceid) const
{
  int count = 0;
  map<int, int>::const_iterator i = fVoicesNotesCount.find( voiceid );
  if (i != fVoicesNotesCount.end()) {
    count = i->second;
  }
  return count;
}

//________________________________________________________________________
int partsummaryvisitor::getVoiceNotesCount (int staffid, int voiceid) const
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
