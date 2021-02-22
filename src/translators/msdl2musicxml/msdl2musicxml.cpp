/*
  MusicXML Library
  Copyright (C) Grame 2006-2013

  This Source Code Form is subject to the terms of the Mozilla Public
  License, v. 2.0. If a copy of the MPL was not distributed with this
  file, You can obtain one at http://mozilla.org/MPL/2.0/.

  Grame Research Laboratory, 11, cours de Verdun Gensoul 69002 Lyon - France
  research@grame.fr
*/

#ifdef MSVC
# pragma warning (disable : 4786)
#endif

#include <iostream>
#include <fstream>      // ofstream, ofstream::open(), ofstream::close()
                        // ifstream, ifstream::open(), ifstream::close()

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "oahOah.h"

#include "msrOah.h"
#include "msr2msrOah.h"
#include "musicxmlOah.h"

#include "waeExceptions.h"
#include "waeMessagesHandling.h"

#include "msdlScanner.h"

#include "msdl2xmlInsiderHandler.h"
#include "msdl2xmlRegularHandler.h"

#include "msr2msrInterface.h"
#include "msr2mxmlTreeInterface.h"
#include "mxmlTree2xmlTranlatorInterface.h"

#include "mxmlTree.h"
#include "msdl2musicxml.h"


using namespace std;

namespace MusicXML2
{
/*
  ENFORCE_TRACE_OAH can be used to issue trace messages
  before gGlobalOahOahGroup->fTrace has been initialized
*/

//#define ENFORCE_TRACE_OAH

//_______________________________________________________________________________
EXP xmlErr msdlStream2musicxmlWithHandler (
  istream&      inputStream,
  S_oahHandler  handler,
  std::ostream& out,
  std::ostream& err)
{
  // has quiet mode been requested?
  // ------------------------------------------------------

  if (gGlobalGeneralOahGroup->getQuiet ()) {
    // disable all trace and display options
    handler->
      enforceHandlerQuietness ();
  }

  // the MSR score
  // ------------------------------------------------------

  S_msrScore firstMsrScore;

  // perform lexical analysis (pass 1a)
  // ------------------------------------------------------

  try {
    msdlScanner scanner (inputStream);

    scanner.scanWholeInputAtOnce (); // TEMP JMI

    /*
    theMsrScore =
      convertMxmlTreeToMsrScoreSkeleton (
        mxmlTree,
        gGlobalMsrOahGroup,
        "Pass 1a",
        "Perform lexical analysis");
        */
  }
  catch (msgMxmlTreeToMsrException& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }
  catch (std::exception& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalMsdl2xmlInsiderOahGroup->getQuitAfterPass2a ()) {
    err <<
      endl <<
      "Quitting after pass 2a as requested" <<
      endl;

    return kNoErr;
  }


  return kNoErr; // JMI TEMP


  // perform syntactical and semantic analysis (pass 1b)
  // ------------------------------------------------------

  try {
  /*
    populateMsrSkeletonFromMxmlTree (
      mxmlTree,
      theMsrScore,
        "Pass 1b",
        "Perform syntactical and semantic analysis");
        */
  }
  catch (msgMsdlToMsrScoreException& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }
  catch (std::exception& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalMsdl2xmlInsiderOahGroup->getQuitAfterPass2b ()) {
    err <<
      endl <<
      "Quitting after pass 2b as requested" <<
      endl;

    return kNoErr;
  }

  // convert the first MSR score into a second MSR (pass 3)
  // ------------------------------------------------------

  S_msrScore secondMsrScore;

  try {
    secondMsrScore =
      convertMsrScoreToMsrScore (
        firstMsrScore,
        gGlobalMsrOahGroup,
        gGlobalMsr2msrOahGroup,
        "Pass 3",
        "Convert the first MSR into a second MSR");
  }
  catch (msgMxmlTreeToMsrException& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }
  catch (std::exception& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }

  // convert the second MSR into a second mxmlTree (pass 4)
  // ------------------------------------------------------

  Sxmlelement secondMxmlTree;

  try {
    secondMxmlTree =
      convertMsrScoreToMxmltree (
        secondMsrScore,
        gGlobalMsrOahGroup,
        "Pass 4",
        "Convert the second MSR into a second mxmlTree",
        timingItem::kMandatory);
  }
  catch (msgMxmlTreeToMsrException& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }
  catch (std::exception& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }

  // generate MusicXML from the second mxmlTree (pass 5)
  // ------------------------------------------------------

  string
    outputFileName =
      handler->
        fetchOutputFileNameFromTheOptions ();

  try {
    generateMusicXMLFromMxmlTree (
      secondMxmlTree,
      outputFileName,
      err,
      "Pass 5",
      "Convert the second mxmlTree into MusicXML text");
  }
  catch (msgMxmlTreeToMsrException& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }
  catch (std::exception& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }

  return kNoErr;
}

//_______________________________________________________________________________
EXP xmlErr msdlStream2musicxmlWithOptionsVector (
  istream&             inputStream,
  const optionsVector& options,
  std::ostream&        out,
  std::ostream&        err)
{
  // the fake executable name
  // ------------------------------------------------------

  string fakeExecutableName = "msdl2xml";

  // reset the global indenter
  // ------------------------------------------------------

  gIndenter.resetToZero ();

  // create the global log indented output stream
  // ------------------------------------------------------

  createTheGlobalIndentedOstreams (out, err);

	// print the options vector
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
      displayOptionsVector (options, err);
#endif
#endif

  // are there 'insider' and/or 'regular' options present?
  // ------------------------------------------------------

  bool insiderOptions = false;
  bool regularOptions = false;

	for (unsigned int i = 1; i < options.size (); ++i) {
	  string optionName = options [i].first;

		if (optionName == K_INSIDER_OPTION_NAME) {
		  insiderOptions = true;
		}
		if (optionName == K_REGULAR_OPTION_NAME) {
		  regularOptions = true;
		}
	} // for

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "msdlStream2musicxml()" <<
    ", insiderOptions: " << booleanAsString (insiderOptions) <<
    ", regularOptions: " << booleanAsString (regularOptions) <<
    endl;
#endif
#endif

  if (insiderOptions && regularOptions) {
    stringstream s;

    s <<
      "options '-insider' and '-regular' cannot be used together";

    oahError (s.str ());
  }

  // here, at most one of insiderOptions and regularOptions is true

  // create an msdl2xml insider OAH handler
  // ------------------------------------------------------

  // create an insider msdl2xml OAH handler
  S_msdl2xmlInsiderHandler
    insiderOahHandler =
      msdl2xmlInsiderHandler::create (
        fakeExecutableName,
        fakeExecutableName + " insider OAH handler with options vector");

  // the OAH handler to be used, a regular handler is the default
  // ------------------------------------------------------

  S_oahHandler handler;

  if (insiderOptions) {
    // use the insider msdl2xml OAH handler
    handler = insiderOahHandler;
  }
  else {
    // create a regular msdl2xml OAH handler
    handler =
      msdl2xmlRegularHandler::create (
        fakeExecutableName,
        fakeExecutableName + " regular OAH handler",
        insiderOahHandler);
  }

  // handle the command line options and arguments
  // ------------------------------------------------------

  try {
    // handle the options from the options vector
    oahElementHelpOnlyKind
      helpOnlyKind =
        handler->
          handleOptionsFromOptionsVector (
            fakeExecutableName,
            options);

    // have help options been used?
    switch (helpOnlyKind) {
      case kElementHelpOnlyYes:
        return kNoErr; // quit now
        break;
      case kElementHelpOnlyNo:
        // go ahead
        break;
    } // switch
  }
  catch (msgOahException& e) {
    displayException (e, gOutputStream);
    return kInvalidOption;
  }
  catch (std::exception& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }

  // check indentation
  if (gIndenter != 0) {
    gLogStream <<
      "### " <<
      fakeExecutableName <<
      " gIndenter value after options ands arguments checking: " <<
      gIndenter.getIndent () <<
      " ###" <<
      endl;

    gIndenter.resetToZero ();
  }

  // do the job
  // ------------------------------------------------------

  msdlStream2musicxmlWithHandler (
    inputStream,
    handler,
    out,
    err);

  return kNoErr;
}

//_______________________________________________________________________________
EXP xmlErr msdlFile2musicxmlWithOptionsVector (
  string               fileName,
  const optionsVector& options,
  std::ostream&        out,
  std::ostream&        err)
{
  // open input file
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePasses ()) {
    err <<
      "Opening file \"" << fileName << "\" for writing" <<
      endl;
  }
#endif

  ifstream
    inputStream (
      fileName.c_str (),
      ifstream::in);

  if (! inputStream.is_open ()) {
    stringstream s;

    s <<
      "Could not open MSDL input file \"" <<
      fileName <<
      "\" for reading, quitting";

    string message = s.str ();

    err <<
      message <<
      endl;

    throw msgMsdlToMusicxmlException (message);
  }

  return
    msdlStream2musicxmlWithOptionsVector (
      inputStream, options, out, err);
}

EXP xmlErr msdlFile2musicxmlWithHandler (
  string       fileName,
  S_oahHandler handler,
  ostream&     out,
  ostream&     err)
{
  // open input file
#ifdef TRACING_IS_ENABLED
  if (gGlobalTraceOahGroup->getTracePasses ()) {
    err <<
      "Opening file \"" << fileName << "\" for writing" <<
      endl;
  }
#endif

  ifstream
    inputStream (
      fileName.c_str (),
      ifstream::in);

  if (! inputStream.is_open ()) {
    stringstream s;

    s <<
      "Could not open MSDL input file \"" <<
      fileName <<
      "\" for reading, quitting";

    string message = s.str ();

    err <<
      message <<
      endl;

    throw msgMsdlToMusicxmlException (message);
  }

  return
    msdlStream2musicxmlWithHandler (
      inputStream, handler, out, err);
}

//_______________________________________________________________________________
EXP xmlErr musicxmlstring2musicxmlWithOptionsVector (
  const char *         buffer,
  const optionsVector& options,
  std::ostream&        out,
  std::ostream&        err)
{
  stringstream
    inputStream (
      buffer,
      ios_base::in);

	// call msdlStream2musicxmlWithOptionsVector() even if buffer is empty,
	// to handle the help options if any
  return
    msdlStream2musicxmlWithOptionsVector (
      inputStream, options, out, err);

	return kInvalidFile;
}

EXP xmlErr msdlString2musicxmlWithHandler (
  const char * buffer,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler)
{
  stringstream
    inputStream (
      buffer,
      ios_base::in);

	// call msdlStream2musicxmlWithOptionsVector() even if buffer is empty,
	// to handle the help options if any
  return
    msdlStream2musicxmlWithHandler (
      inputStream, handler, out, err);

	return kInvalidFile;
}


}
