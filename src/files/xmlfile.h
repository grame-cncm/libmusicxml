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

#ifndef __xmlfile__
#define __xmlfile__

#include <ostream>
#include <string>
#include "exports.h"
#include "xml.h"
#include "ctree.h"

namespace MusicXML2 
{

//______________________________________________________________________________
class EXP TXMLDecl {
	std::string fVersion;
	std::string fEncoding;
	int			fStandalone;

	public:
		enum { kundefined=-1, kNo, kYes };
				 TXMLDecl (const std::string version, const std::string encoding, int stdalone=kundefined) 
						: fVersion(version), fEncoding(encoding), fStandalone(stdalone) {}
		virtual ~TXMLDecl() {}
		void print (std::ostream& s);
};

//______________________________________________________________________________
class EXP TDocType {
	std::string fStartElement;
	bool		fPublic;
	std::string fPubLitteral;
	std::string fSysLitteral;

	public:
				 TDocType (const std::string start);
				 TDocType (const std::string start, bool pub, const std::string publit, const std::string syslit) 
						: fStartElement(start), fPublic(pub), fPubLitteral(publit), fSysLitteral(syslit) {}
		virtual ~TDocType() {}
		void print (std::ostream& s);
};

//______________________________________________________________________________
class EXP TXMLFile : public smartable
{ 
	TXMLDecl *			fXMLDecl;
	TDocType *			fDocType;
	Sxmlelement			fXMLTree;
    protected:
				 TXMLFile() : fXMLDecl(0), fDocType(0) {}
		virtual ~TXMLFile() { delete fXMLDecl; delete fDocType; }
	public:
		static SMARTP<TXMLFile> create();

	public:		
		Sxmlelement	elements()				{ return fXMLTree; }
		void		set (Sxmlelement root)	{ fXMLTree = root; }
		void		set (TXMLDecl * dec)	{ fXMLDecl = dec; }
		void		set (TDocType * dt)		{ fDocType = dt; }
		void		print (std::ostream& s);
};
typedef SMARTP<TXMLFile> SXMLFile;


}

#endif
