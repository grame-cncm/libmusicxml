/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "versions.h"

namespace MusicXML2
{
//______________________________________________________________________________
float   versions::libVersion()        { return 3.20f; }
const char* versions::libVersionStr()     { return "3.20"; }

float   versions::xml2guidoVersion()    { return 3.1f; }
const char* versions::xml2guidoVersionStr()   { return "3.1"; }

float   versions::xml2lilypondVersion()   { return 0.93f; }
const char* versions::xml2lilypondVersionStr()    { return "0.93"; }

float   versions::xml2brailleVersion()   { return 0.03f; }
const char* versions::xml2brailleVersionStr()    { return "0.03"; }

float   versions::xml2bmmlVersion()   { return 0.01f; }
const char* versions::xml2bmmlVersionStr()    { return "0.01"; }

float   versions::xml2meiVersion()   { return 0.01f; }
const char* versions::xml2meiVersionStr()    { return "0.01"; }

float   versions::bmml2lilypondVersion()   { return 0.01f; }
const char* versions::bmml2lilypondVersionStr()    { return "0.01"; }

float   versions::mei2lilypondVersion()   { return 0.01f; }
const char* versions::mei2lilypondVersionStr()    { return "0.01"; }

float   versions::bmml2bmmlVersion()   { return 0.01f; }
const char* versions::bmml2bmmlVersionStr()    { return "0.01"; }

float   versions::mei2meiVersion()   { return 0.01f; }
const char* versions::mei2meiVersionStr()    { return "0.01"; }
}

