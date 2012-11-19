/*
  Copyright © Grame 2003,2007

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


#include "keyvisitor.h"

using namespace std;

namespace MusicXML2 {


//________________________________________________________________________
ostream& operator<< (ostream& os, const keyvisitor& elt)
{
    elt.print(os);
    return os;
}

//________________________________________________________________________
// transpose element suppport
//________________________________________________________________________
void keyvisitor::reset ()
{
	fFifths = fCancel = 0;
	fMode = "";
}

void keyvisitor::visitStart ( S_key& elt )		{ reset(); }
void keyvisitor::visitStart ( S_cancel& elt )	{ fCancel = (int)(*elt); }
void keyvisitor::visitStart ( S_fifths& elt )	{ fFifths = (int)(*elt); }
void keyvisitor::visitStart ( S_mode& elt )		{ fMode = elt->getValue(); }

//________________________________________________________________________
void keyvisitor::print (ostream& out) const
{
	out << fFifths;
	if (fMode.size())	out << " mode: " << fMode;
	if (fCancel)		out << " cancel: " << fCancel;
}

}
