/*

  MusicXML Library
  Copyright (C) 2006  Grame

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

#ifndef __types__
#define __types__

#include "exports.h"
#include "xml.h"
#include "visitor.h"

namespace MusicXML2 
{

/*!
\addtogroup MusicXML
@{
*/

template <int elt> class musicxml : public xmlelement
{ 
    protected:	
		musicxml() { fType = elt; }

	public:
		static SMARTP<musicxml<elt> > new_musicxml()	
			{ musicxml<elt>* o = new musicxml<elt>; assert(o!=0); return o; }
		static SMARTP<musicxml<elt> > new_musicxml(const std::vector<Sxmlelement>& elts)	
			{ musicxml<elt>* o = new musicxml<elt>(elts); assert(o!=0); return o; }

        virtual void acceptIn(basevisitor& v) {
			if (visitor<SMARTP<musicxml<elt> > >* p = dynamic_cast<visitor<SMARTP<musicxml<elt> > >*>(&v)) {
				SMARTP<musicxml<elt> > sptr = this;
				p->visitStart(sptr);
			}
			else xmlelement::acceptIn(v);
		}

        virtual void acceptOut(basevisitor& v) {
			if (visitor<SMARTP<musicxml<elt> > >* p = dynamic_cast<visitor<SMARTP<musicxml<elt> > >*>(&v)) {
				SMARTP<musicxml<elt> > sptr = this;
				p->visitEnd(sptr);
			}
			else xmlelement::acceptOut(v);
		}
};

/*! @} */

}

#endif
