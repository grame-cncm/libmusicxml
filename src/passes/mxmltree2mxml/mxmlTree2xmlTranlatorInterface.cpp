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

#include "mxmlTree.h"

#include "msr.h"
#include "mxmlTree2xmlTranlatorInterface.h"

using namespace std;

namespace MusicXML2
{
//_______________________________________________________________________________
EXP void generateMusicXMLFromMxmlTree (
  Sxmlelement mxmlTree,
  string      outputFileName,
  ostream&    cerr,
  string      passNumber,
  string      passDescription)
{
  // sanity check
  msgAssert (
    mxmlTree != 0,
    "generateMusicXMLFromMxmlTree(): mxmlTree is null");

  // start the clock
  clock_t startClock = clock ();

  // create the MusicXML data
  SXMLFile xmlFile = createMxmlFile ();

  // insert the mxmlTree into it
  xmlFile->set (mxmlTree);

#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTraceOah ()) {
    cerr <<
      "xmlFile2musicxml() outputFileName = \"" <<
      outputFileName <<
      "\"" <<
      endl;
  }
#endif

  if (! outputFileName.size ()) {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      cerr <<
        "xmlFile2musicxml() output goes to standard output" <<
        endl;
    }
#endif

    // write the MusicXML data to the output file stream
    xmlFile->print (gOutputStream);
    gOutputStream << endl;
  }

  else {
#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      cerr <<
        "xmlFile2musicxml() output goes to file \"" <<
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
      musicxmlFileOutputStream (
        outputFileName.c_str(),
        ofstream::out);

    if (! musicxmlFileOutputStream.is_open ()) {
      stringstream s;

      s <<
        "Could not open MusicXML output file \"" <<
        outputFileName <<
        "\" for writing, quitting";

      string message = s.str ();

      cerr <<
        message <<
        endl;

      throw msrScoreToMusicXMLException (message);
    }

    // write the MusicXML data to the output file stream
    xmlFile->print (musicxmlFileOutputStream);
    musicxmlFileOutputStream << endl;


    // close output file
#ifdef TRACE_OAH
    if (gTraceOah->fTracePasses) {
      gLogOstream <<
        endl <<
        "Closing file \"" << outputFileName << "\"" <<
        endl;
    }
#endif

    musicxmlFileOutputStream.close ();
  }

  // register time spent
  clock_t endClock = clock ();

  timing::gGlobalTiming.appendTimingItem (
    passNumber,
    passDescription,
    timingItem::kMandatory,
    startClock,
    endClock);
}


}
