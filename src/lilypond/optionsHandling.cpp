/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <sstream>

#include <climits>      /* INT_MIN, INT_MAX */
#include <iomanip>      // setw, setprecision, ...

#include <regex>

//#include "version.h"
#include "utilities.h"

#include "messagesHandling.h"

#include "optionsHandling.h"


using namespace std;

namespace MusicXML2 
{

#define TRACE_OPTIONS 0

const int K_OPTIONS_ELEMENTS_INDENTER_OFFSET = 3;
  // indent a bit more for readability

const int K_FIELD_WIDTH = 40;


}
