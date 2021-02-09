/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

/*
#include <sstream>
#include <climits>      // INT_MIN, INT_MAX
#include <iomanip>      // setw, setprecision, ...

#include "xml_tree_browser.h"

#include "conversions.h"

#include "utilities.h"

#include "msr.h"

// JMI #include "msdrTokensList2msrOah.h"
#include "msrOah.h"
*/

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

#include "musicxmlOah.h"
#include "mxmlTreeOah.h"

#include "msdrTokensList2msrTranslator.h"


using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
msdrTokensList2msrTranslator::msdrTokensList2msrTranslator (
  S_msdrTokensList tokensList)
{}

msdrTokensList2msrTranslator::~msdrTokensList2msrTranslator ()
{}

void msdrTokensList2msrTranslator::createMsrFromMsdl ()
{}


}
