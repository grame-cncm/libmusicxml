----------------------------------------------------------------------------

   MusicXML library version 3

----------------------------------------------------------------------------

  This library is free software; you can redistribute it and/or
  modify it under the terms of the Mozilla Public License Version 2.0.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  Grame Research Laboratory, 11, cours de Verdun - Gensoul, 69002 Lyon - France
  research@grame.fr

----------------------------------------------------------------------------

CONTENT
	
	doc		: the library architecture documentation
			  MusicXML format is documented in the dtds or schema
	dtds	: the MusicXML DTDs
	schema	: the MusicXML schema
	samples	: sample code
	
	musicxml library and tools (names dependent on the platform)

For more information, files, see at the MusicXML web site:
	http://www.makemusic.com/musicxml/

----------------------------------------------------------------------------

INSTALL

MacOS
	copy the libmusicxml2.framework to /Library/Frameworks 
Windows
	copy the libmusicxml2.dll along with your application
Linux
	copy the library to /usr/local/lib or to /usr/lib
	binaries are compiled for 64 bits architectures on Ubuntu 11.10
	you should get the src code (from the package of from git) 
	and compile on your platform

----------------------------------------------------------------------------

Platform specific makefiles are included for historic reasons but they are 
not maintained any more. Please use cmake to generate native projects and makefiles.

Note: 
 the library is still named libmusicxml2. It doesn't refer to the version 
 number but to the significant architecture change that has been introduced with 
 the version 2 of the library. The name has been kept for compatibility reasons 
 and because there is no change from architecture viewpoint.
