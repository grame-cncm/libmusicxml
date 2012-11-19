/*

  MusicXML Library
  Copyright © Grame 2007

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

    Grame Research Laboratory, 9, rue du Garet 69001 Lyon - France
    research@grame.fr

*/

#ifndef __unrolled_clonevisitor__
#define __unrolled_clonevisitor__

#include "visitor.h"
#include "clonevisitor.h"
#include "typedefs.h"

namespace MusicXML2 
{

/*!
\addtogroup visitors
@{
*/

//______________________________________________________________________________
/*!
\brief A visitor that creates an "unrolled" version of a score.
*/
class EXP unrolled_clonevisitor : public clonevisitor,
	public visitor<S_measure>,
	public visitor<S_key>,
	public visitor<S_clef>,
	public visitor<S_time>,
	public visitor<S_ending>,
	public visitor<S_repeat>,
	public visitor<S_part>,
	public visitor<S_sound>
{
	S_key	fCurrentKey;	bool fKeyCopy;
	S_clef	fCurrentClef;	bool fClefCopy;
	S_time	fCurrentTime;	bool fTimeCopy;
	long fMeasureNum;
	
	protected:
		virtual void start (Sxmlelement elt)	{ clonevisitor::visitStart(elt); }
		virtual void end (Sxmlelement elt)		{ clonevisitor::visitEnd(elt); }
	public:
				 unrolled_clonevisitor() : fMeasureNum(1) {}
		virtual ~unrolled_clonevisitor() {}

		virtual void visitStart( S_measure& elt);
		virtual void visitStart( S_part& elt);
		virtual void visitStart( S_sound& elt);
		virtual void visitStart( S_key& elt);
		virtual void visitStart( S_clef& elt);
		virtual void visitStart( S_time& elt);

		virtual void visitEnd  ( S_measure& elt)	{ end( elt ); }
		virtual void visitEnd  ( S_part& elt)		{ end( elt ); }
		virtual void visitEnd  ( S_sound& elt)		{ end( elt ); }
		virtual void visitEnd  ( S_key& elt);
		virtual void visitEnd  ( S_clef& elt);
		virtual void visitEnd  ( S_time& elt);

		virtual void visitStart( S_ending& elt)		{}		// don't copy
		virtual void visitStart( S_repeat& elt)		{}		// don't copy
};

/*! @} */

} // namespace MusicXML2


#endif
