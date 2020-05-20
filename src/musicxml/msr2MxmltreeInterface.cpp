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
#include <fstream>      // ofstream, ofstream::open(), ofstream::close()

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "versions.h"

#include "generalOah.h"

#include "setTraceOahIfDesired.h"
#ifdef TRACE_OAH
  #include "traceOah.h"
#endif

#include "mxmlTree.h"

#include "msr2MxmltreeInterface.h"

#include "msr2MxmltreeTranslator.h"

using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
Sxmlelement buildMxmltreeFromMsrScore (
  const S_msrScore mScore,
  S_msrOah         msrOpts,
  indentedOstream& logOstream)
{
  // sanity check
  msrAssert (
    mScore != 0,
    "mScore is null");

  clock_t startClock = clock ();

#ifdef TRACE_OAH
  if (gTraceOah->fTracePasses) {
    string separator =
      "%--------------------------------------------------------------";

    logOstream <<
      endl <<
      separator <<
      endl <<
      gTab <<
      "Optional pass: translating the MSR into an mxmltree" <<
      endl <<
      separator <<
      endl;
  }
#endif

  // create an msr2MxmltreeTranslator
  msr2MxmltreeTranslator
    translator (
      logOstream,
      mScore);

  // build the mxmltree score
  translator.buildMxmltreeFromMsrScore ();

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "Optional pass",
    "translate the MSR to an mxmltree",
    timingItem::kMandatory,
    startClock,
    endClock);

  // get the mxmltree
  Sxmlelement
    mxmlTree =
      translator.getScorePartWiseElement ();

  // return it
  return mxmlTree;
}

//_______________________________________________________________________________
void displayMxmltreeScore ( // unused JMI
  const Sxmlelement mxmlTree,
  S_msrOah          msrOpts,
  indentedOstream&  logOstream)
{
  // sanity check
  msrAssert (
    mxmlTree != 0,
    "mxmlTree is null");

  clock_t startClock = clock ();

  string separator =
    "%--------------------------------------------------------------";

  logOstream <<
    separator <<
    endl <<
    gTab <<
    "Optional pass: displaying the Mxmltree as text" <<
    endl <<
    separator <<
    endl <<
    endl <<
// JMI    mxmlTree <<
    separator <<
    endl <<
    endl;

  clock_t endClock = clock ();

  // register time spent
  timing::gTiming.appendTimingItem (
    "",
    "display the Mxmltree",
    timingItem::kOptional,
    startClock,
    endClock);
}

//_______________________________________________________________________________
void convertMsrScoreToMusicXMLScore_Loop (
  S_msrScore mScore,
  string     outputFileName)
{
  // open output file if need be
  // ------------------------------------------------------

  ofstream outFileStream;

#ifdef TRACE_OAH
  if (gTraceOah->fTracePasses) {
    gLogOstream <<
      "Opening file '" << outputFileName << "' for writing" <<
      endl;
  }
#endif

  outFileStream.open (
    outputFileName.c_str(),
    ofstream::out);

  if (! outFileStream.is_open ()) {
    gLogOstream <<
      "Could not open MusicXML output file \"" <<
      outputFileName <<
      "\" for writing, exiting" <<
      endl;

    exit (9);
  }

  // create an indented output stream for the LilyPond code
  // to be written to outFileStream
  indentedOstream
    musicXMLCodeFileOutputStream (
      outFileStream,
      gIndenter);

  // convert the MSR score to an mxmltree
  Sxmlelement
    mxmltree =
      buildMxmltreeFromMsrScore (
        mScore,
        gMsrOah,
        gLogOstream);

  // create the MusicXML data
	SXMLFile xmlFile = createXMLFile ();
/*
	SXMLFile xmlFile = TXMLFile::create ();

  TXMLDecl * xmlDecl = new TXMLDecl ("1.0", "UTF-8", TXMLDecl::kNo);
	xmlFile->set (xmlDecl);

	TDocType * docType = new TDocType ("score-partwise");
	xmlFile->set (docType);
	*/

	// insert the mxmlTree into it
  xmlFile->set (mxmltree);

	// write the MusicXML data to the output file stream
	xmlFile->print (outFileStream);
	outFileStream << endl;

#ifdef TRACE_OAH
  if (gTraceOah->fTracePasses) {
    gLogOstream <<
      endl <<
      "Closing file '" << outputFileName << "'" <<
      endl;
  }
#endif

  outFileStream.close ();
}


}
