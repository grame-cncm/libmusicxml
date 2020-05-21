/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <iomanip>      // setw, setprecision, ...
#include <cmath>
#include <string>

#include "oah2ManPageGenerators.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "version.h"

#include "oahOah.h"

using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
oah2ManPageGenerator::oah2ManPageGenerator (
  const S_oahHandler handler,
  ostream&           logOstream,
  ostream&           manPageOutputStream)
    : oahVisitor (
        handler),
      fOah2ManPageLogOstream (
        logOstream),
      fOah2ManPageOutputStream (
        manPageOutputStream)
{}

oah2ManPageGenerator::~oah2ManPageGenerator ()
{}


} // namespace
