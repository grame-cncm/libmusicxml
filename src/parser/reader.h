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

#ifndef __reader__
#define __reader__

class reader
{ 
	public:
				 reader() {}
		virtual ~reader() {};
		
		virtual bool	xmlDecl (const char* version, const char *encoding, int standalone) = 0;
		virtual bool	docType (const char* start, bool status, const char *pub, const char *sys) = 0;

		virtual void	newComment (const char* comment) {}
		virtual void	newProcessingInstruction (const char* pi) {}
		virtual bool	newElement (const char* eltName) = 0;
		virtual bool	newAttribute (const char* eltName, const char *val) = 0;
		virtual void	setValue (const char* value) = 0;
		virtual bool	endElement (const char* eltName) = 0;
		virtual void	error (const char* s, int lineno) = 0;
};


#endif
