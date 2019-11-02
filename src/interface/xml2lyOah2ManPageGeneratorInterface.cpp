/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include "messagesHandling.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif


//#include "xml2lyOah.h"
#include "xml2lyManPageOah.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
void generateXml2lyManPageFromOahHandler (
  const S_oahHandler handler,
  S_xml2lyManPageOah manPageOpts,
  indentedOstream&   logOstream,
  ostream&           manPageOutputStream)
{
  // sanity check
  msrAssert (
    handler != 0,
    "handler is null");

  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

#ifdef TRACE_OAH
  if (gTraceOah->fTracePasses) {
    logOstream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      "Optional pass: writing the xml2ly OAH handler as man page data" <<
      endl <<
      separator <<
      endl;
  }
#endif

/* JMI
  // create an xml2lyOah2ManPageGenerator
  xml2lyOah2ManPageGenerator
    generator (
      handler,
      manPageOpts,
      logOstream,
      manPageOutputStream);

  // generate the man page
  generator.generateManPageFromOahHandler ();
*/

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Pass 4",
    "write the xml2ly man page data",
    timingItem::kMandatory,
    startClock,
    endClock);
}


}
