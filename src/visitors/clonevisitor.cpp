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

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <iostream>
#include "clonevisitor.h"

using namespace std;

namespace MusicXML2
{

//______________________________________________________________________________
void clonevisitor::copyAttributes (const Sxmlelement& src, Sxmlelement& dst )
{
	vector<Sxmlattribute> attr = src->attributes();
	vector<Sxmlattribute>::const_iterator iter;
	for (iter=attr.begin(); iter != attr.end(); iter++) {
		Sxmlattribute attrcopy = xmlattribute::create();
		attrcopy->setName( (*iter)->getName());
		attrcopy->setValue( (*iter)->getValue());
		dst->add( attrcopy );
	}
}

//______________________________________________________________________________
Sxmlelement clonevisitor::copy (const Sxmlelement& src)
{
	Sxmlelement copy = xmlelement::create();
	if (copy) {
		copy->setName( src->getName());
		copy->setValue( src->getValue());
		copyAttributes (src, copy);
	}
	return copy;
}

//______________________________________________________________________________
void clonevisitor::visitStart ( Sxmlelement& elt )
{
	if (!fClone) return;
	Sxmlelement copy = xmlelement::create();
	copy->setName( elt->getName());
	copy->setValue( elt->getValue());
	copyAttributes (elt, copy);
	fLastCopy = copy;
	if (fStack.empty())
		fStack.push(copy);
	else fStack.top()->push(copy);
	if (!elt->empty()) fStack.push(copy);
}

//______________________________________________________________________________
void clonevisitor::visitEnd ( Sxmlelement& elt )
{
	if (!fClone) return;
	if (!elt->empty()) fStack.pop();
}

}
