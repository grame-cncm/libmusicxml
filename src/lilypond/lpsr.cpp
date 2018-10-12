/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iostream>
#include <list>
#include <algorithm>
#include <iomanip>      // setw, setprecision, ...
#include <climits>      // INT_MIN, INT_MAX

#include "version.h"

#include "lpsr.h"

#include "setTraceOptionsIfDesired.h"
#ifdef TRACE_OPTIONS
  #include "traceOptions.h"
#endif

#include "lpsrOptions.h"
#include "lilypondOptions.h"

#include "xml2lyOptionsHandling.h"


using namespace std;

namespace MusicXML2 
{

//______________________________________________________________________________
void initializeLPSR ()
{
  initializeLPSRBasicTypes ();
}


}
