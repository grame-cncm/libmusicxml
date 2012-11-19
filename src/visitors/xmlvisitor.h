/*
  Copyright © Grame 2006

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

#ifndef __xmlvisitors__
#define __xmlvisitors__

#include <ostream>

#include "tree_browser.h"
#include "typedefs.h"
#include "visitor.h"
#include "xml.h"

namespace MusicXML2 
{

//______________________________________________________________________________
/*!
\internal
\brief to be used in place of std::endl
	to provide a correct indentation of the xml output.
*/
class xmlendl {
	private:
		int fIndent;
	public:
				 xmlendl() : fIndent(0) {}
		virtual ~xmlendl() {}

		//! increase the indentation
		xmlendl& operator++ (int)  { fIndent++; return *this; }
		//! decrease the indentation
		xmlendl& operator-- (int)  { fIndent--; return *this; }
		//! reset the indentation to none
		void print(std::ostream& os) const;
};
std::ostream& operator<< (std::ostream& os, const xmlendl& eol);

//______________________________________________________________________________
class xmlvisitor : 
	public visitor<S_comment>,
	public visitor<S_processing_instruction>,
	public visitor<Sxmlelement>
{
	std::ostream&	fOut;
	xmlendl			fendl;

    public:
				 xmlvisitor(std::ostream& stream) : fOut(stream) {}
		virtual ~xmlvisitor() {}

		virtual void visitStart ( Sxmlelement& elt);
		virtual void visitEnd   ( Sxmlelement& elt);
		virtual void visitStart ( S_comment& elt);
		virtual void visitStart ( S_processing_instruction& elt);
};

} // namespace MusicXML2


#endif
