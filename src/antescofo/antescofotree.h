/*
  Copyright (C) 2013 Thomas Coffy

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  thomas.coffy@gmail.com
  http://repmus.ircam.fr/antescofo
*/


#ifndef __antescofo__
#define __antescofo__

#include <vector>
#include <string>

#include "exports.h"
#include "smartpointer.h"

namespace MusicXML2 
{

class antescofovisitor;
class antescofoelement;
class antescofoparam;
typedef SMARTP<antescofoelement> 	Santescofoelement;
typedef SMARTP<antescofoparam> 		Santescofoparam;

EXP std::ostream& operator<< (std::ostream& os, const Santescofoelement& elt);

/*!
\addtogroup antescofo
@{
*/

/*!
\brief A antescofotag parameter representation.

	A parameter is represented by its value.
*/
class EXP antescofoparam : public smartable {
	public:
        static SMARTP<antescofoparam> create(std::string value, bool quote=true);
        static SMARTP<antescofoparam> create(long value, bool quote=true);

		//! the parameter value
		void set (std::string value, bool quote=true);
		void set (long value, bool quote=true);
		std::string get () const 						{ return fValue; }
		bool   quote () const 						{ return fQuote; }
        
    protected:
		antescofoparam(std::string value, bool quote);
		antescofoparam(long value, bool quote);
		virtual ~antescofoparam ();
        
    private:
		std::string 	fValue;
		bool	fQuote;
};

/*!
\brief A generic antescofo element representation.

	An element is represented by its name and the
	list of its enclosed elements plus optional parameters.
*/
class EXP antescofoelement : public smartable {
	public:
        static SMARTP<antescofoelement> create(std::string name, std::string sep=" ");
		
		long add (Santescofoelement& elt);
		long add (Santescofoparam& param);
		long add (Santescofoparam param);
		void print (std::ostream& os);

		//! the element name
		void 	setName (std::string name)			{ fName = name; }
		std::string 	getName () const				{ return fName; }
		std::string 	getStart () const				{ return fStartList; }
		std::string 	getEnd () const					{ return fEndList; }
		std::string 	getSep () const					{ return fSep; }
        std::vector<Santescofoelement>& elements()		{ return fElements; }
		const std::vector<Santescofoelement>& elements() const 	{ return fElements; }
        const std::vector<Santescofoparam>& parameters() const 	{ return fParams; }
		
		bool empty () const { return fElements.empty(); }

    protected:
		antescofoelement(std::string name, std::string sep=" ");
		virtual ~antescofoelement();

		std::string	fName;
		//! the contained element start marker (default to empty)
		std::string	fStartList;
		//! the contained element end marker (default to empty)
		std::string	fEndList;
		//! the element separator (default to space)
		std::string	fSep;
		//! list of the enclosed elements
		std::vector<Santescofoelement>	fElements;
		//! list of optional parameters
		std::vector<Santescofoparam>	fParams;		
};

/*!
\brief A antescofo note duration representation.

	A note duration is represented by a numerator 
    (denotes the number of beats), a denominator (denotes the beat value)
     and optional dots.
     Triplets are repesented as 1/3, 1/6, ... quintuplets, septuplets and so on
     are handled analogously.
*/
class EXP antescofonoteduration {
	public:
		antescofonoteduration(long num, long denom, long dots=0) 
            { set (num, denom, dots); }
		virtual ~antescofonoteduration() {}
        
        void set (long num, long denom, long dots=0) 
            { fNum=num; fDenom=denom; fDots=dots; }
        antescofonoteduration& operator= (const antescofonoteduration& dur)	
            { fNum=dur.fNum; fDenom=dur.fDenom; fDots=dur.fDots; return *this; }
        bool operator!= (const antescofonoteduration& dur) const	
            { return (fNum!=dur.fNum) || (fDenom!=dur.fDenom) || (fDots!=dur.fDots); }

        long	fNum;
		long	fDenom;
		long	fDots;
};

/*!
\brief A antescofo note representation.

	A note is represented by its name, optional accidentals,
    duration (in the form of numerator/denominator) and optional dots.
*/
class EXP antescofonote : public antescofoelement {
	public:
        static SMARTP<antescofonote> create(unsigned short voice);
        static SMARTP<antescofonote> create(unsigned short voice, std::string name, char octave,
                                                antescofonoteduration& dur, std::string acc="");
		
		void set (unsigned short voice, std::string name, char octave, antescofonoteduration& dur, std::string acc);
		void setName (const std::string name)			{ fNote = name; } 
		void setOctave (char octave)					{ fOctave = octave; }
		void setDuration (const antescofonoteduration& dur)	{ fDuration = dur; }
		void setAccidental (const std::string acc)		{ fAccidental = acc; }

		const char * 	name() const		{ return fNote.c_str(); }
		const char * 	accidental() const	{ return fAccidental.c_str(); }
		char 			octave() const		{ return fOctave; }
		const antescofonoteduration& duration() const { return fDuration; }

	protected:
		antescofonote(unsigned short voice);
		antescofonote(unsigned short voice, std::string name, char octave, 
                    antescofonoteduration& dur, std::string acc="");
		virtual ~antescofonote();
	
	std::string 	fNote;
	std::string 	fAccidental;
	char 	fOctave;
	antescofonoteduration fDuration;

};
typedef SMARTP<antescofonote> Santescofonote;

/*!
\brief Represents the current status of notes duration and octave.

    Octave and duration may be ommitted for antescofo notes. If so,
    they are infered from preceeding notes (or rest), within the same 
    sequence or chord, or assumed to have standard values.
\n
	The object is defined as a multi-voices singleton: a single
    object is allocated for a specific voice and thus it will
	not operate correctly on a same voice parrallel formatting 
    operations.

\todo handling the current beat value for \e *num duration form.
*/
class EXP antescofonotestatus {
	public:
        enum { kMaxInstances=128 };
        
		static antescofonotestatus* get(unsigned short voice);
		static void resetall();
		static void freeall();

        enum { defoctave=1, defnum=1, defdenom=4 };
        
        void reset()	{ fOctave=defoctave; fDur.set(defnum, defdenom, 0); }
        antescofonotestatus& operator= (const antescofonoteduration& dur)	{ fDur = dur; return *this; }
        bool operator!= (const antescofonoteduration& dur) const		{ return fDur!= dur; }
            
		char				fOctave;
		antescofonoteduration 	fDur;
//		char				fBeat;

	protected:
		antescofonotestatus() :	fOctave(defoctave), fDur(defnum, defdenom, 0) {}
	private:
		static antescofonotestatus * fInstances[kMaxInstances];
};

/*!
\brief The antescofo sequence element
*/
class EXP antescofoseq : public antescofoelement {
	public:
        static SMARTP<antescofoseq> create();
	protected:
		antescofoseq();
		virtual ~antescofoseq();
};
typedef SMARTP<antescofoseq> Santescofoseq;

/*!
\brief The antescofo chord element
*/
class EXP antescofochord : public antescofoelement {
	public:
        static SMARTP<antescofochord> create();
	protected:
		antescofochord ();
		virtual ~antescofochord();
};
typedef SMARTP<antescofochord> Santescofochord;

/*!
\brief A antescofo tag representation.

	A tag is represented by its name and optional parameters.
	A range tag contains enclosed elements.
*/
class EXP antescofotag : public antescofoelement {
	public:
        static SMARTP<antescofotag> create(std::string name);
	protected:
		antescofotag(std::string name);
		virtual ~antescofotag();
};
typedef SMARTP<antescofotag> Santescofotag;
/*! @} */

}

#endif
