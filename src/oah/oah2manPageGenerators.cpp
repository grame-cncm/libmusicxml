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

#include "oah2manPageGenerators.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "version.h"

#include "oahOah.h"

using namespace std;

namespace MusicXML2
{

//________________________________________________________________________
oah2manPageGenerator::oah2manPageGenerator (
  const S_oahHandler handler,
  ostream&           manPageOutputStream)
    : oahVisitor (
        handler),
      fManPageOutputStream (
        manPageOutputStream)
{}

oah2manPageGenerator::~oah2manPageGenerator ()
{}


} // namespace