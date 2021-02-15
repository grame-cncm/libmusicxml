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
#include <climits>      // INT_MIN, INT_MAX

#include <regex>

#include "waeMessagesHandling.h"

#include "msr2summaryVisitor.h"
*/

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"
#include "generalOah.h"

#include "msrOah.h"
#include "msr2lpsrOah.h"

#include "msdl2msdrTokensListTranslator.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
msdl2msdrTokensListTranslator::msdl2msdrTokensListTranslator ()
{
  // the MSDL tokens list we're building
  fMsdrTokensList =
    msdrTokensList::create ();
}

msdl2msdrTokensListTranslator::~msdl2msdrTokensListTranslator ()
{}

void msdl2msdrTokensListTranslator::buildMsdrTokensList ()
{
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) { // getTraceTokens JMI
    gLogStream <<
      "Build MSDL tokens list" <<
      endl;
  }
#endif

}


} // namespace
