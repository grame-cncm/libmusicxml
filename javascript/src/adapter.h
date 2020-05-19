/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "libmusicxml.h"

class libMusicXMLAdapter
{
	public:
				 libMusicXMLAdapter() {};
		virtual ~libMusicXMLAdapter() {};

		float			libVersion();
		std::string		libVersionStr();

		float			musicxml2guidoVersion();
		std::string		musicxml2guidoVersionStr();
		float			musicxml2lilypondVersion();
		std::string		musicxml2lilypondVersionStr();
		float			musicxml2brailleVersion();
		std::string		musicxml2brailleVersionStr();

		std::string		string2guido(const std::string& buff, bool generateBars);
		std::string		string2lily(const std::string& buff, const MusicXML2::optionsVector& options);
		std::string		string2braille(const std::string& buff, const MusicXML2::optionsVector& options);

		std::string		xmlStringTranspose(const std::string& buff, int interval);
};
