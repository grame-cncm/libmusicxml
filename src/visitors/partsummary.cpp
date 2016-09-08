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

#include "partsummary.h"

using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
void partsummaryvisitor::visitStart ( S_part& elt)
{
  fStavesCount = 1;
  fStaves.clear();
  fVoices.clear();
  fStaffVoices.clear();

  fLastLyric = 0;
  fLastSyllabic = 0;
  fStanzas.clear();
}

//________________________________________________________________________
void partsummaryvisitor::visitStart ( S_staves& elt)
{
  fStavesCount = int(*elt);
}

//________________________________________________________________________
void partsummaryvisitor::visitEnd ( S_note& elt)
{
  fStaves[fStaff]++;
  fVoices[fVoice]++;
  fStaffVoices[fStaff][fVoice]++;
}

//________________________________________________________________________
void partsummaryvisitor::visitStart ( S_lyric& elt ) { 
  fLastLyric = elt;
}

void partsummaryvisitor::visitStart ( S_syllabic& elt ) {
  fLastSyllabic = elt;
}

void partsummaryvisitor::visitEnd ( S_text& elt ) 
{
  /*
      <note default-x="129.06" default-y="-40.00">
        <pitch>
          <step>E</step>
          <alter>-1</alter>
          <octave>4</octave>
          </pitch>
        <duration>1</duration>
        <voice>1</voice>
        <type>eighth</type>
        <stem>up</stem>
        <beam number="1">begin</beam>
        <lyric number="1">
          <syllabic>single</syllabic>
          <text>1. Sing</text>
          </lyric>
        <lyric number="2">
          <syllabic>single</syllabic>
          <text>2. For</text>
        </lyric>
  */

  string      text = elt->getValue();
  
  std::size_t spacefound=text.find(" ");
  if (spacefound!=std::string::npos) text = "\""+text+"\"";
  
  std::string lastLyricNumber = fLastLyric->getAttributeValue("number");
  std::string lastSyllabicValue = fLastSyllabic->getValue();
  
  /*
  cout <<
    "--> lastLyricNumber = " << lastLyricNumber <<
    ", lastSyllabicValue = " << lastSyllabicValue <<
    ", text = " << text << endl <<
    flush;
  */
      
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
}

//________________________________________________________________________
smartlist<int>::ptr partsummaryvisitor::getStaves() const
{
  smartlist<int>::ptr sl = smartlist<int>::create();
  for ( map<int, int>::const_iterator i = fStaves.begin(); i != fStaves.end(); i++) {
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
  for ( map<int, int>::const_iterator i = fVoices.begin(); i != fVoices.end(); i++) {
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
int partsummaryvisitor::getStaffNotes (int id) const
{
  int count = 0;
  map<int, int>::const_iterator i = fStaves.find( id );
  if (i != fStaves.end()) {
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
    int n = getVoiceNotes (*i, voiceid);
    if (n > maxnotes) {
      maxnotes = n;
      staffid = *i;
    }
  }
  return staffid;
}

//________________________________________________________________________
int partsummaryvisitor::getVoiceNotes (int voiceid) const
{
  int count = 0;
  map<int, int>::const_iterator i = fVoices.find( voiceid );
  if (i != fVoices.end()) {
    count = i->second;
  }
  return count;
}

//________________________________________________________________________
int partsummaryvisitor::getVoiceNotes (int staffid, int voiceid) const
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
