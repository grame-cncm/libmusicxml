/*

  MusicXML Library
  Copyright (C) 2006,2007  Grame

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

#ifndef __clefVisitor__
#define __clefVisitor__

#include "typedefs.h"
#include "visitor.h"

namespace MusicXML2 
{

/*!
\addtogroup visitors
@{
*/

/*!
\brief key element data
*/
class EXP clefvisitor : 
	public visitor<S_clef>,
	public visitor<S_sign>,
	public visitor<S_line>,
	public visitor<S_clef_octave_change>
{
	protected:
			    void reset();
		virtual void visitStart ( S_clef& elt );
		virtual void visitStart ( S_sign& elt );
		virtual void visitStart ( S_line& elt );
		virtual void visitStart ( S_clef_octave_change& elt );

	public:
		enum { kStandardLine, kTrebleStdLine=2, kBassStdLine=4, kCStdLine=3, kTabStdLine=5 };
		enum { kNoNumber = -1 };
		
				 clefvisitor() { reset(); }
		virtual ~clefvisitor() {}

		std::string	fSign;
		int			fLine;
		int			fOctaveChange;
		int			fNumber;
};


/*! @} */

}

#endif
