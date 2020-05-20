/*
  MusicXML Library
  Copyright (C) Grame 2017

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <string>
#include <emscripten.h>
#include <emscripten/bind.h>

#include "adapter.h"
#include "libmusicxml.h"

using namespace emscripten;
using namespace MusicXML2;

/*
 * C++ class binding.
 * This classes can be used in javascript side.
 */
EMSCRIPTEN_BINDINGS(EngineAdapter) { 
	// Binding C++ class adapter for guidoEngine
	emscripten::class_<libMusicXMLAdapter>("libMusicXMLAdapter")
			.constructor<>()
			.function("libVersion", &libMusicXMLAdapter::libVersion)
			.function("libVersionStr", &libMusicXMLAdapter::libVersionStr)

			.function("musicxml2guidoVersion", &libMusicXMLAdapter::musicxml2guidoVersion)
			.function("musicxml2guidoVersionStr", &libMusicXMLAdapter::musicxml2guidoVersionStr)
			.function("musicxml2lilypondVersion", &libMusicXMLAdapter::musicxml2lilypondVersion)
			.function("musicxml2lilypondVersionStr", &libMusicXMLAdapter::musicxml2lilypondVersionStr)
			.function("musicxml2brailleVersion", &libMusicXMLAdapter::musicxml2brailleVersion)
			.function("musicxml2brailleVersionStr", &libMusicXMLAdapter::musicxml2brailleVersionStr)

			.function("string2guido", 	&libMusicXMLAdapter::string2guido)
			.function("string2lily", 	&libMusicXMLAdapter::string2lily)
			.function("string2braille", &libMusicXMLAdapter::string2braille)

			.function("xmlStringTranspose", select_overload<std::string (const std::string&, int)>(&libMusicXMLAdapter::xmlStringTranspose));

	register_vector<std::pair<std::string, std::string> >	("vector<std::pair<std::string, std::string> >");

}

