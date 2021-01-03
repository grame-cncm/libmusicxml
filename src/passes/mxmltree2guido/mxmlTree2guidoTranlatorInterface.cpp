/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#include <fstream>

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "versions.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "xml2gmnInsiderOahHandler.h"

#include "msr.h"

#include "mxmlTree.h"

#include "xml2guidovisitor.h"


using namespace std;

namespace MusicXML2
{
//_______________________________________________________________________________
void generateGuidoFromMxmlTree (
  Sxmlelement mxmlTree,
  string      outputFileName,
  ostream&    cerr,
  string      passNumber)
{
  // sanity check
  msgAssert (
    mxmlTree != 0,
    "generateGuidoFromMxmlTree(): mxmlTree is null");

  // start the clock
  clock_t startClock = clock ();

  // convert the mxmlTree intto Guido data
  xml2guidovisitor v (
    gGlobalXml2gmnInsiderOahGroup->
      getGenerateComments (),
    gGlobalXml2gmnInsiderOahGroup->
      getGenerateStem (),
    gGlobalXml2gmnInsiderOahGroup->
      getGenerateBars (),
    0); // partNum, i.e. all parts

  Sguidoelement
    guidoData = v.convert (mxmlTree);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    cerr <<
      "xmlFile2gmn() outputFileName = \"" <<
      outputFileName <<
      "\"" <<
      endl;
  }
#endif

  if (! outputFileName.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      cerr <<
        "xmlFile2gmn() output goes to standard output" <<
        endl;
    }
#endif

    // write the Guido data to the output file stream
    gOutputStream <<
      guidoData <<
      endl;
  }

  else {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      cerr <<
        "xmlFile2gmn() output goes to file \"" <<
        outputFileName <<
        "\"" <<
        endl;
    }
#endif

    // open output file
    ofstream outputFileStream;

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTracePasses ()) {
      cerr <<
        "Opening file '" << outputFileName << "' for writing" <<
        endl;
    }
#endif

    ofstream
      guidoFileOutputStream (
        outputFileName.c_str(),
        ofstream::out);

    if (! guidoFileOutputStream.is_open ()) {
      stringstream s;

      s <<
        "Could not open Guido output file \"" <<
        outputFileName <<
        "\" for writing, quitting";

      string message = s.str ();

      cerr <<
        message <<
        endl;

      throw msrScoreToGuidoException (message);
    }

    // write the Guido data to the output file stream
    guidoFileOutputStream <<
      guidoData <<
      endl;

    // close output file
#ifdef TRACE_OAH
    if (gTraceOah->fTracePasses) {
      gLogOstream <<
        endl <<
        "Closing file \"" << outputFileName << "\"" <<
        endl;
    }
#endif

    guidoFileOutputStream.close ();

    // register time spent
    clock_t endClock = clock ();

    timing::gGlobalTiming.appendTimingItem (
      passNumber,
      "Write the new mxmlTree as Guido data",
      timingItem::kMandatory,
      startClock,
      endClock);
  }
}


}