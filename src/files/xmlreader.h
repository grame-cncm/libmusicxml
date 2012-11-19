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

#ifndef __xmlreader__
#define __xmlreader__

#include <stack>
#include <stdio.h>
#include "exports.h"
#include "xmlfile.h"
#include "reader.h"

namespace MusicXML2 
{

//______________________________________________________________________________
class EXP xmlreader : public reader
{ 
	std::stack<Sxmlelement>	fStack;
	SXMLFile				fFile;

	public:
				 xmlreader() {}
		virtual ~xmlreader() {}
		
		SXMLFile readbuff(const char* file);
		SXMLFile read(const char* file);
		SXMLFile read(FILE* file);

		bool	xmlDecl (const char* version, const char *encoding, int standalone);
		bool	docType (const char* start, bool status, const char *pub, const char *sys);

		void	newComment (const char* comment);
		void	newProcessingInstruction (const char* pi);
		bool	newElement (const char* eltName);
		bool	newAttribute (const char* eltName, const char *val);
		void	setValue (const char* value);
		bool	endElement (const char* eltName);
		void	error (const char* s, int lineno);
};

}

#endif
