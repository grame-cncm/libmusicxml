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

#ifndef __timesignVisitor__
#define __timesignVisitor__

#include <vector>
#include <utility>
#include "typedefs.h"
#include "rational.h"
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
class EXP timesignvisitor : 
	public visitor<S_time>,
	public visitor<S_beats>,
	public visitor<S_beat_type>,
	public visitor<S_senza_misura>
{
	protected:
		std::string fCurrentBeat;

				void reset();
		virtual void visitStart( S_time& elt );
		virtual void visitStart( S_beats& elt );
		virtual void visitStart( S_beat_type& elt );
		virtual void visitStart( S_senza_misura& elt );

	public:
		enum { kNoStaffNumber = -1 };

				 timesignvisitor() { reset(); }
		virtual ~timesignvisitor() {}

		/*!	gives the time sign as a rational
			\param index an index into fTimeSign vector
			\return the time signature as a rational or rational(0,1)
		*/
		rational timesign(unsigned int index);
		
		std::vector<std::pair<std::string,std::string> > fTimeSign;
		std::string fSymbol;
		int		fStaffNumber;
		bool	fSenzaMisura;
};


/*! @} */

}

#endif
