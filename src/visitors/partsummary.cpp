/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifdef MSVC
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
    void partsummary::visitStart ( S_print& elt)
    {
        auto sLayout = elt->find(k_staff_layout);
        
        while (sLayout != elt->end() )
        {
            int number = sLayout->getAttributeIntValue("number", 0);
            //if (number> fStaffDistances.size())
            //    fStaffDistances.reserve(number+1);
            int thisDistance = sLayout->getIntValue(k_staff_distance, 0);
            if (fStaffDistances.find(number-1) != fStaffDistances.end()) {
                if (fStaffDistances[number-1] < thisDistance)
                {
                    fStaffDistances[number-1] = thisDistance;
                }
            }else {
                fStaffDistances.insert(std::pair<int, int>(number-1, thisDistance));
            }
            sLayout = elt->find(k_staff_layout, sLayout++);
        }
    }

//________________________________________________________________________
void partsummary::visitEnd ( S_note& elt)
{
	notevisitor::visitEnd (elt);
    if (inChord()) return;
    if (!isGrace() ) {
        std::string fCurrentMeasureNumber = fCurrentMeasure->getAttributeValue("number");
        
        timePositions.addTimePosition(fCurrentMeasureNumber,
                                      getMeasureTime(getVoice()).toDouble(),
                                      *this);
        moveMeasureTime (getDuration(), getVoice());
    }
	fStaves[notevisitor::getStaff()]++;
	fVoices[notevisitor::getVoice()]++;
	fStaffVoices[notevisitor::getStaff()][notevisitor::getVoice()]++;
}

void partsummary::visitStart ( S_measure& elt )
{
    for (auto&& voice: fCurrentVoicedMeasurePosition ) {
        voice.second.set(0, 1);
    }
    
    fCurrentMeasure = elt;
}

void partsummary::visitStart ( S_divisions& elt )
{
    fCurrentDivision = (long)(*elt);
}

//______________________________________________________________________________
void partsummary::visitStart ( S_backup& elt )
{
    int duration = elt->getIntValue(k_duration, 0);
    if (duration) {
        for (auto&& voice: fCurrentVoicedMeasurePosition) {
            // don't back up if a voice is already zeroed
            if (getMeasureTime(voice.first) > rational()) {
                moveMeasureTime(-duration, voice.first);
            }
        }
    }
}

//______________________________________________________________________________
void partsummary::visitStart ( S_forward& elt )
{
    int duration = elt->getIntValue(k_duration, 0);
    int voice = elt->getIntValue(k_voice, kUndefinedVoice);
    moveMeasureTime(duration, voice);
}

void partsummary::visitStart ( S_direction& elt )
{
    if (elt->find(k_octave_shift) == elt->end()) {
        return;
    }
    int offset = elt->getLongValue(k_offset, 0);
    
    int voice = elt->getIntValue(k_voice, -1);
    int staff = elt->getIntValue(k_staff, 1);
    std::string fCurrentMeasureNumber = fCurrentMeasure->getAttributeValue("number");
    
    rational curTime = voice == -1 ? maxStaffTime(staff) : getMeasureTime(voice) ;
    if (offset != 0) {
        rational ratOffset(offset, fCurrentDivision*4);
        curTime += ratOffset;
        curTime.rationalise();
    }
    
    const string& type = elt->find(k_octave_shift)->getAttributeValue("type");
    int size = elt->find(k_octave_shift)->getAttributeIntValue("size", 8);
    switch (size) {
        case 8:        size = 1; break;
        case 15:    size = 2; break;
        case 22:    size = 3; break;
        default:    return;
    }
    if (type != "stop") {
        if (type == "up")
            size = -size;
    }
    else {
        size = 0;
    }
    
    fOctavas[staff][fCurrentMeasureNumber][curTime] = size;
//    cerr<<"PS added octava staff:"<<staff
//    <<" measure:"<<fCurrentMeasureNumber<<" time:"<<curTime<<" "<<curTime.toDouble()
//    <<" Type:"<<size<<endl;
}

void partsummary::moveMeasureTime (long duration, int voice)
{
    rational r(duration, fCurrentDivision*4);
    r.rationalise();
    
    bool found = fCurrentVoicedMeasurePosition.count(voice);
    if (!found) {
        fCurrentVoicedMeasurePosition[voice] = rational();
    }
    fCurrentVoicedMeasurePosition[voice] += r;
    fCurrentVoicedMeasurePosition[voice].rationalise();
}

rational partsummary::getMeasureTime(int voice) {
    bool found = fCurrentVoicedMeasurePosition.count(voice);
    if (!found) {
        fCurrentVoicedMeasurePosition[voice] = rational();
    }
    return fCurrentVoicedMeasurePosition[voice];
}

rational partsummary::maxStaffTime(int staff) {
    rational maxtime = rational();
    for (auto voice: fCurrentVoicedMeasurePosition) {
        if (maxtime < voice.second) {
            maxtime = voice.second;
        }
    }
    return maxtime;
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
    if (fStavesCount == 1) {
        return 1;
    }
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
