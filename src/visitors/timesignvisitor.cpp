/*
  Copyright � Grame 2003,2007

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

#ifdef VC6
# pragma warning (disable : 4786)
#endif

#include <stdlib.h>
#include "timesignvisitor.h"

using namespace std;

namespace MusicXML2 {

//________________________________________________________________________
// transpose element suppport
//________________________________________________________________________
void timesignvisitor::reset ()
{
	fSenzaMisura = false;
	fCurrentBeat = "";
	fTimeSign.clear();
	fStaffNumber = kNoStaffNumber;
	fSymbol = "";
}

void timesignvisitor::visitStart ( S_time& elt ) {
	reset();
	fStaffNumber = elt->getAttributeIntValue("number", kNoStaffNumber);
	fSymbol = elt->getAttributeValue("symbol");
}

void timesignvisitor::visitStart ( S_beats& elt )			{ fCurrentBeat = elt->getValue(); }
void timesignvisitor::visitStart ( S_beat_type& elt )		{ fTimeSign.push_back(make_pair(fCurrentBeat, elt->getValue())); }
void timesignvisitor::visitStart ( S_senza_misura& elt )	{ fSenzaMisura = true; }


rational timesignvisitor::timesign(unsigned int index)
{
	rational r(0,1);
	if (index < fTimeSign.size()) {
		const pair<string,string>& ts = fTimeSign[index];
		long num = strtol (ts.first.c_str(), 0, 10);
		long denum = strtol (ts.second.c_str(), 0, 10);
		if (num && denum) r.set(num, denum);
	}
	return r;
}

}
