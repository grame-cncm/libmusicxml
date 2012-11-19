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

#ifndef __keyVisitor__
#define __keyVisitor__

#include <ostream>

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
class EXP keyvisitor : 
	public visitor<S_key>,
	public visitor<S_cancel>,
	public visitor<S_fifths>,
	public visitor<S_mode>,
	public visitor<S_key_step>,
	public visitor<S_key_alter>,
	public visitor<S_key_octave>
{
	protected:
				void reset();
		virtual void visitStart ( S_key& elt );
		virtual void visitStart ( S_cancel& elt );
		virtual void visitStart ( S_fifths& elt );
		virtual void visitStart ( S_mode& elt );

		virtual void visitStart ( S_key_step& elt )		{}	// not yet supported
		virtual void visitStart ( S_key_alter& elt )	{}	// not yet supported
		virtual void visitStart ( S_key_octave& elt )	{}	// not yet supported

	public:
				 keyvisitor() { reset(); }
		virtual ~keyvisitor() {}

		virtual void print (std::ostream& out) const;

		int			fFifths;
		int			fCancel;
		std::string	fMode;
};

EXP std::ostream& operator<< (std::ostream& os, const keyvisitor& elt);


/*! @} */

}

#endif
