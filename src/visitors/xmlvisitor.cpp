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

#include <iostream>
#include "xmlvisitor.h"

using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
ostream& operator<< (ostream& os, const xmlendl& endl)
{
    endl.print(os);
    return os;
}

//______________________________________________________________________________
void xmlendl::print(std::ostream& os) const { 
	int i = fIndent;
    os << std::endl;
    while (i-- > 0)  os << "    ";
}

//______________________________________________________________________________
void xmlvisitor::visitStart ( S_comment& elt ) 
{
	fOut <<  fendl << "<!--" << elt->getValue() << "-->";
}

//______________________________________________________________________________
void xmlvisitor::visitStart ( S_processing_instruction& elt ) 
{
	fOut <<  fendl << "<?" << elt->getValue() << "?>";
}

//______________________________________________________________________________
void xmlvisitor::visitStart ( Sxmlelement& elt ) 
{
	fOut <<  fendl << "<" << elt->getName();
	// print the element attributes first
	vector<Sxmlattribute>::const_iterator attr; 
	for (attr = elt->attributes().begin(); attr != elt->attributes().end(); attr++)
		fOut << " " << (*attr)->getName() << "=\"" << (*attr)->getValue() << "\"";				
	if (elt->empty()) {
		fOut << "/>";	// element is empty, we can direclty close it
	}
	else {
		fOut << ">";
		if (!elt->getValue().empty())
			fOut << elt->getValue();
		if (elt->size())
			fendl++;
	}
}

//______________________________________________________________________________
void xmlvisitor::visitEnd ( Sxmlelement& elt ) 
{
	if (!elt->empty()) {
		if (elt->size()) {
			fendl--;
			cout << fendl;
		}
		fOut << "</" << elt->getName() << ">";
	}
}

}
