/*

  MusicXML Library
  Copyright (C) 2008  Grame

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

#ifndef __smartlist__
#define __smartlist__

#include <vector>
#include "smartpointer.h"

namespace MusicXML2 
{

/*
	smartlist are provided only to exchange container (vector, list,...) with clients 
	while avoiding the runtime library issue with windows visual c++
*/
template <typename T, typename L=std::vector<T> >
class smartlist  : public smartable, public L {
	protected:
				 smartlist() {}
		virtual ~smartlist() {}
	public:
		typedef SMARTP<smartlist<T> >	ptr;
		static ptr create() { smartlist<T> * o = new smartlist<T>; assert(o!=0); return o; }
};

}

#endif
