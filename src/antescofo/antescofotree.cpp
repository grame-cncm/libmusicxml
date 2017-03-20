/*
  Copyright (C) 2013 Thomas Coffy

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  thomas.coffy@gmail.com
  http://repmus.ircam.fr/antescofo
*/


#include <sstream>

#include "antescofotree.h"

using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
Santescofoparam antescofoparam::create(string value, bool quote) 
    { antescofoparam * o = new antescofoparam(value, quote); assert(o!=0); return o; }
Santescofoparam antescofoparam::create(long value, bool quote) 
    { antescofoparam * o = new antescofoparam(value, quote); assert(o!=0); return o; }

Santescofoelement antescofoelement::create(string name, string sep)	
		{  antescofoelement * o = new antescofoelement(name, sep); assert(o!=0); return o; }
Santescofonote antescofonote::create(unsigned short voice) {
    antescofonotestatus * status = antescofonotestatus::get(voice);
	antescofonote * o = new antescofonote (voice,"", status->fOctave, status->fDur, ""); 
	assert(o!=0); 
	return o;
}
Santescofonote antescofonote::create(unsigned short voice, string name, char oct, antescofonoteduration& dur, string acc)
		{ antescofonote * o = new antescofonote (voice, name, oct, dur, acc); assert(o!=0); return o; }
Santescofoseq antescofoseq::create()			{ antescofoseq* o = new antescofoseq(); assert(o!=0); return o;}
Santescofochord antescofochord::create()		{ antescofochord* o = new antescofochord(); assert(o!=0); return o;}
Santescofotag antescofotag::create(string name)	{ antescofotag* o = new antescofotag(name); assert(o!=0); return o;}

//______________________________________________________________________________
antescofonotestatus* antescofonotestatus::fInstances[kMaxInstances] = { 0 };
antescofonotestatus* antescofonotestatus::get (unsigned short voice)
{ 
    if (voice < kMaxInstances) {
        if (!fInstances[voice])
            fInstances[voice] = new antescofonotestatus;
        return fInstances[voice];
    }
    return 0;
}

void antescofonotestatus::resetall ()
{ 
    for (int i=0; i<kMaxInstances; i++) {
        if (fInstances[i]) fInstances[i]->reset();
    }
}

void antescofonotestatus::freeall ()
{ 
    for (int i=0; i<kMaxInstances; i++) {
        delete fInstances[i];
		fInstances[i] = 0;
    }
}

//______________________________________________________________________________
void antescofoparam::set (string value, bool quote)
{
	fValue = value;
	fQuote = quote;
}

//______________________________________________________________________________
void antescofoparam::set (long value, bool quote)
{
	stringstream s;
	s << value;
	s >> fValue;
	fQuote = quote;
}

//______________________________________________________________________________
long antescofoelement::add (Santescofoelement& elt)	{ 
	fElements.push_back(elt); 
	return fElements.size()-1;
}
long antescofoelement::add (Santescofoparam& param) { 
	fParams.push_back(param); 
	return fParams.size()-1;
}
long antescofoelement::add (Santescofoparam param) { 
	fParams.push_back(param); 
	return fParams.size()-1;
}

//______________________________________________________________________________
void antescofoelement::print(ostream& os)
{
    os << fName;

    // print the optional parameters section
    if (!fParams.empty()) {
        //os << "<";
        os << " ";
        vector<Santescofoparam>::const_iterator param;
        for (param = fParams.begin(); param != fParams.end(); ) {
            if ((*param)->quote())
				os << "" << (*param)->get() << "";
			else
				os << (*param)->get();
            if (++param != fParams.end())
                os << ", ";
        }
        //os << ">";
        os << " ";
    }

    bool isSeq = dynamic_cast<const antescofoseq *>(this) != 0;
    bool isChord = dynamic_cast<const antescofochord *>(this) != 0;
    bool prevNote = false;
    bool prevSeq = false;
    // print the optional contained elements
    if (!fElements.empty()) {
        os << fStartList;
        vector<Santescofoelement>::const_iterator ielt;
        for (ielt = fElements.begin(); ielt != fElements.end(); ielt++) {
            Santescofoseq seq;
            seq.cast((antescofoelement *)(*ielt));
            Santescofonote note;
            note.cast((antescofoelement *)(*ielt));

            if (isChord) {
                if (note) {
                    os << (prevNote ? ", " : " ");
                    prevNote = true;
                }
                else if (seq) {
                    os << (prevSeq ? ", " : " ");
                    prevSeq = true;
                }
                else os << " ";
            }
            else os << " ";
            os << *ielt;            
        }
       os << fEndList;
    }
	if (isSeq) os << std::endl;
}

//______________________________________________________________________________
ostream& operator<< (ostream& os, const Santescofoelement& elt)
{
    elt->print(os);
    return os;
}

//______________________________________________________________________________
void antescofonote::set (unsigned short voice, string name, char octave, antescofonoteduration& dur, string acc)
{
    antescofonotestatus * status = antescofonotestatus::get(voice);
    stringstream s;
    long dots = dur.fDots;
    
 	fNote = name;
	fAccidental = acc;
	fOctave = octave;
	fDuration = dur;
	
	s << name;
    // octave is ignored in case of rests
	if (name[0] != '_') {
		if (!acc.empty())
			s << acc;
		if (name != "empty") {
			if (!status)
				s << (int)octave;
			else if (status->fOctave != octave) {
				s << (int)octave;
				status->fOctave = octave;
			}
		}
    }
	if (!status || (*status != dur)) {
        if (dur.fNum != 1) {
            s << "*" << (int)dur.fNum;
        }
        s << "/" << (int)dur.fDenom;
        if (status) *status = dur;
    }
    while (dots-- > 0)
        s << ".";
    s >> fName;
}

//______________________________________________________________________________
antescofoelement::antescofoelement(string name, string sep) : fName(name), fSep(sep) {}
antescofoelement::~antescofoelement() {}

//______________________________________________________________________________
antescofoparam::antescofoparam(string value, bool quote) : fValue(value), fQuote(quote) {}
antescofoparam::antescofoparam(long value, bool quote)   { set(value, quote); }
antescofoparam::~antescofoparam () {}

//______________________________________________________________________________
antescofonote::antescofonote(unsigned short voice, string name, char octave, 
                    antescofonoteduration& dur, string acc) 
            : antescofoelement(""), fDuration(1,4) { set(voice, name, octave, dur, acc); }
antescofonote::~antescofonote() {}

//______________________________________________________________________________
antescofoseq::antescofoseq() : antescofoelement("") { fStartList="; "; fEndList=""; }
antescofoseq::~antescofoseq() {}

//______________________________________________________________________________
antescofochord::antescofochord () : antescofoelement("", ", ") { fStartList="; "; fEndList=""; }


antescofochord::~antescofochord() {}

//______________________________________________________________________________
antescofotag::antescofotag(string name) : antescofoelement(""+name) 
	//{ fStartList="("; fEndList=")"; }
	{ fStartList=""; fEndList=""; }
antescofotag::~antescofotag() {}

}
