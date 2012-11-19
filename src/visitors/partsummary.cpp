/*

  MusicXML Library
  Copyright (C) 2003,2008  Grame

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

  Grame Research Laboratory, 9 rue du Garet, 69001 Lyon - France
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
void partsummary::visitStart ( S_part& elt)
{
	fStavesCount = 1;
	fStaves.clear();
	fVoices.clear();
	fStaffVoices.clear();
}

//________________________________________________________________________
void partsummary::visitStart ( S_staves& elt)
{
	fStavesCount = int(*elt);
}

//________________________________________________________________________
void partsummary::visitEnd ( S_note& elt)
{
	notevisitor::visitEnd (elt);
	fStaves[notevisitor::getStaff()]++;
	fVoices[notevisitor::getVoice()]++;
	fStaffVoices[notevisitor::getStaff()][notevisitor::getVoice()]++;
}

//________________________________________________________________________
smartlist<int>::ptr partsummary::getStaves() const
{
	smartlist<int>::ptr sl = smartlist<int>::create();
	for ( map<int, int>::const_iterator i = fStaves.begin(); i != fStaves.end(); i++) {
		sl->push_back (i->first);
	}
	return sl;
}

//________________________________________________________________________
smartlist<int>::ptr partsummary::getStaves (int voice) const
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
smartlist<int>::ptr partsummary::getVoices () const
{
	smartlist<int>::ptr sl = smartlist<int>::create();
	for ( map<int, int>::const_iterator i = fVoices.begin(); i != fVoices.end(); i++) {
		sl->push_back (i->first);
	}
	return sl;
}

//________________________________________________________________________
smartlist<int>::ptr partsummary::getVoices (int staff) const
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
int partsummary::countVoices (int staff) const
{
	int count = 0;
	map<int, map<int, int> >::const_iterator i = fStaffVoices.find( staff );
	if (i != fStaffVoices.end()) {
		count = i->second.size();
	}
	return count;
}

//________________________________________________________________________
int partsummary::getStaffNotes (int id) const
{
	int count = 0;
	map<int, int>::const_iterator i = fStaves.find( id );
	if (i != fStaves.end()) {
		count = i->second;
	}
	return count;
}

//________________________________________________________________________
int partsummary::getMainStaff (int voiceid) const
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
int partsummary::getVoiceNotes (int voiceid) const
{
	int count = 0;
	map<int, int>::const_iterator i = fVoices.find( voiceid );
	if (i != fVoices.end()) {
		count = i->second;
	}
	return count;
}

//________________________________________________________________________
int partsummary::getVoiceNotes (int staffid, int voiceid) const
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
