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

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <vector>
#include "unrolled_clonevisitor.h"

using namespace std;
namespace MusicXML2
{

//______________________________________________________________________________
void unrolled_clonevisitor::visitStart( S_measure& elt)
{
	start(elt);
	Sxmlattribute attr = lastCopy()->getAttribute("number");
	if (attr) {
		attr->setValue(fMeasureNum++);
	}
}

//______________________________________________________________________________
void unrolled_clonevisitor::visitStart( S_part& elt)
{
	fMeasureNum = 1;
	start(elt);
}

//______________________________________________________________________________
void unrolled_clonevisitor::visitStart( S_key& elt)
{
	if (elt != fCurrentKey) {
		fCurrentKey = elt;
		fKeyCopy = true;
		Sxmlelement xml = elt;
		start (xml);
	}
	else {
		fKeyCopy = false;
		clone(false);
	}
}

//______________________________________________________________________________
void unrolled_clonevisitor::visitStart( S_clef& elt)
{
	if (elt != fCurrentClef) {
		fCurrentClef = elt;
		fClefCopy = true;
		Sxmlelement xml = elt;
		start (xml);
	}
	else {
		fClefCopy = false;
		clone(false);
	}
}

//______________________________________________________________________________
void unrolled_clonevisitor::visitStart( S_time& elt)
{
	if (elt != fCurrentTime) {
		fCurrentTime = elt;
		fTimeCopy = true;
		Sxmlelement xml = elt;
		start (xml);
	}
	else {
		fTimeCopy = false;
		clone(false);
	}
}

//______________________________________________________________________________
void unrolled_clonevisitor::visitEnd  ( S_key& elt)		{ if (fKeyCopy) end( elt ); else clone(true); }
void unrolled_clonevisitor::visitEnd  ( S_clef& elt)	{ if (fClefCopy) end( elt ); else clone(true); }
void unrolled_clonevisitor::visitEnd  ( S_time& elt)	{ if (fTimeCopy) end( elt ); else clone(true); }

//______________________________________________________________________________
void unrolled_clonevisitor::visitStart( S_sound& elt)
{
	start(elt);
	vector<Sxmlattribute> attr = lastCopy()->attributes();
	vector<Sxmlattribute>::iterator iter;
	for (iter=attr.begin(); iter != attr.end();) {
		const string name = (*iter)->getName();
		if ((name == "segno" ) ||
			(name == "forward-repeat" ) ||
			(name == "coda" ) ||
			(name == "fine" ) ||
			(name == "dacapo" ) ||
			(name == "dalsegno" ) ||
			(name == "tocoda" )) {
			iter = attr.erase(iter);
		}
		else iter++;
	}
}

}
