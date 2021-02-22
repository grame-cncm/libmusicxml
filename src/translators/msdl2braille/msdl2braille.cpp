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
#include "msr2bsrOah.h"
#include "bsrOah.h"

#include "waeExceptions.h"
#include "waeMessagesHandling.h"

#include "msdlScanner.h"

#include "msdl2brlInsiderHandler.h"
#include "msdl2brlRegularHandler.h"

#include "msr2bsrInterface.h"
#include "bsr2bsrFinalizerInterface.h"
#include "bsr2brailleTranslatorInterface.h"

#include "msdl2braille.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
xmlErr msdlStream2brailleWithHandler (
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

  S_msrScore theMsrScore;

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
  catch (msgMsdlToMsrInternalException& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }
  catch (std::exception& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalMsdl2brlInsiderOahGroup->getQuitAfterPass2a ()) {
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

  if (gGlobalMsdl2brlInsiderOahGroup->getQuitAfterPass2b ()) {
    err <<
      endl <<
      "Quitting after pass 1b as requested" <<
      endl;

    return kNoErr;
  }

  // the first BSR score
  S_bsrScore firstBsrScore;

  {
    // create the first BSR from the MSR (pass 2a)
    // ------------------------------------------------------

    try {
      firstBsrScore =
        convertMsrScoreToBsrScore (
          theMsrScore,
          gGlobalMsrOahGroup,
          gGlobalBsrOahGroup,
          "Pass 2a",
          "Create a first BSR from the MSR");
    }
    catch (msgMsrScoreToBsrScoreException& e) {
      displayException (e, gOutputStream);
      return kInvalidFile;
    }
    catch (std::exception& e) {
      displayException (e, gOutputStream);
      return kInvalidFile;
    }

    // display the first BSR score if requested
    // ------------------------------------------------------

    if (gGlobalBsrOahGroup->getDisplayBsr ()) {
      displayBsrFirstScore_OptionalPass (
        firstBsrScore,
        gGlobalMsrOahGroup,
        gGlobalBsrOahGroup);
    }

    if (gGlobalBsrOahGroup->getDisplayBsrShort ()) {
      displayBsrFirstScoreShort_OptionalPass (
        firstBsrScore,
        gGlobalMsrOahGroup,
        gGlobalBsrOahGroup);
    }

    // should we return now?
    // ------------------------------------------------------

    if (gGlobalMsr2bsrOahGroup->getQuitAfterPass3a ()) {
      err <<
        endl <<
        "Quitting after pass 3a as requested" <<
        endl;

      return kNoErr;
    }
  }

  // the finalized BSR score
  S_bsrScore finalizedBsrScore;

  {
    // create the finalized BSR from the first BSR (pass 2b)
    // ------------------------------------------------------

    try {
      finalizedBsrScore =
        convertBsrFirstScoreToFinalizedBsrScore (
          firstBsrScore,
          gGlobalBsrOahGroup,
          "Pass 2b",
          "Create the finalized BSR from the first BSR");
    }
    catch (msgBsrScoreToFinalizedBsrScoreException& e) {
      displayException (e, gOutputStream);
      return kInvalidFile;
    }
    catch (std::exception& e) {
      displayException (e, gOutputStream);
      return kInvalidFile;
    }

    // display the finalized BSR score if requested
    // ------------------------------------------------------

    if (gGlobalBsrOahGroup->getDisplayBsr ()) {
      displayFinalizedBsrScore_OptionalPass (
        finalizedBsrScore,
        gGlobalMsrOahGroup,
        gGlobalBsrOahGroup);
    }

    if (gGlobalBsrOahGroup->getDisplayBsrShort ()) {
      displayFinalizedBsrScoreShort_OptionalPass (
        finalizedBsrScore,
        gGlobalMsrOahGroup,
        gGlobalBsrOahGroup);
    }

    // should we return now?
    // ------------------------------------------------------

    if (gGlobalMsr2bsrOahGroup->getQuitAfterPass3b ()) {
      err <<
        endl <<
        "Quitting after pass 3b as requested" <<
        endl;

      return kNoErr;
    }
  }

  {
    // generate Braille music text from the BSR (pass 3)
    // ------------------------------------------------------

    string
      outputFileName =
        handler->
          fetchOutputFileNameFromTheOptions ();

#ifdef TRACING_IS_ENABLED
      if (getTraceOah ()) {
        err <<
          "msdlStream2brailleWithHandler() outputFileName = \"" <<
          outputFileName <<
          "\"" <<
          endl;
      }
#endif

    if (! outputFileName.size ()) {
#ifdef TRACING_IS_ENABLED
      if (getTraceOah ()) {
        err <<
          "msdlStream2brailleWithHandler() output goes to standard output" <<
          endl;
      }
#endif

      // convert the BSR to braille
      try {
        convertBsrScoreToBraille (
          finalizedBsrScore,
          gGlobalBsrOahGroup,
          "Pass 3",
          "Convert the finalized BSR into braille",
          out);
      }
      catch (msgLpsrScoreToLilypondException& e) {
      displayException (e, gOutputStream);
        return kInvalidFile;
      }
      catch (std::exception& e) {
      displayException (e, gOutputStream);
        return kInvalidFile;
      }
    }

    else {
#ifdef TRACING_IS_ENABLED
      if (getTraceOah ()) {
        err <<
          "msdlStream2brailleWithHandler() output goes to file \"" <<
          outputFileName <<
          "\"" <<
          endl;
      }
#endif

      // open output file
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTracePasses ()) {
        err <<
          "Opening file \"" << outputFileName << "\" for writing" <<
          endl;
      }
#endif

      ofstream
        brailleCodeFileOutputStream (
          outputFileName,
          ofstream::out);

      if (! brailleCodeFileOutputStream.is_open ()) {
        stringstream s;

        s <<
          "Could not open Braille music output file \"" <<
          outputFileName <<
          "\" for writing, quitting";

        string message = s.str ();

        err <<
          message <<
          endl;

        throw msgBsrScoreToBrailleException (message);
      }

      // convert the finalized BSR to braille (pass 4)
      try {
        convertBsrScoreToBraille (
          finalizedBsrScore,
          gGlobalBsrOahGroup,
          "Pass 4",
          "Convert the finalized BSR into braille",
          brailleCodeFileOutputStream);
      }
      catch (msgLpsrScoreToLilypondException& e) {
        displayException (e, gOutputStream);
        return kInvalidFile;
      }
      catch (std::exception& e) {
        displayException (e, gOutputStream);
        return kInvalidFile;
      }

      // close output file
#ifdef TRACE_OAH
      if (gTraceOah->fTracePasses) {
        gLogStream <<
          endl <<
          "Closing file \"" << outputFileName << "\"" <<
          endl;
      }
#endif

      brailleCodeFileOutputStream.close ();
    }
  }

	return kNoErr;
}

//_______________________________________________________________________________
xmlErr msdlStream2brailleWithOptionsVector (
  istream&             inputStream,
  const optionsVector& options,
  std::ostream&        out,
  std::ostream&        err)
{
  // the fake executable name
  // ------------------------------------------------------

  string fakeExecutableName = "msdl2brl";

  // reset the global indenter
  // ------------------------------------------------------

  gIndenter.resetToZero ();

  // create the global log indented output stream
  // ------------------------------------------------------

  createTheGlobalIndentedOstreams (out, err);

	// print the options vector
#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
      displayOptionsVector (options, err);
  }
#endif

  // are there 'insider' and/or 'regular' options present?
  // ------------------------------------------------------

  bool insiderOptions = false;
  bool regularOptions = false;

	for (unsigned int i = 1; i < options.size (); ++i) {
	  string optionName = options [i].first;

    string optionNameWithoutDash = optionName.substr (1);

		if (optionNameWithoutDash == K_INSIDER_OPTION_NAME) {
		  insiderOptions = true;
		}
		if (optionNameWithoutDash == K_REGULAR_OPTION_NAME) {
		  regularOptions = true;
		}
	} // for

#ifdef TRACING_IS_ENABLED
  if (getTraceOah ()) {
    gLogStream <<
      "msdlStream2brailleWithOptionsVector()" <<
      ", insiderOptions: " << booleanAsString (insiderOptions) <<
      ", regularOptions: " << booleanAsString (regularOptions) <<
      endl;
  }
#endif

  if (insiderOptions && regularOptions) {
    stringstream s;

    s <<
      "options '-insider' and '-regular' cannot be used together";

    oahError (s.str ());
  }

  // here, at most one of insiderOptions and regularOptions is true

  // create an msdl2brl insider OAH handler
  // ------------------------------------------------------

  // create an insider msdl2brl OAH handler
  S_msdl2brlInsiderHandler
    insiderOahHandler =
      msdl2brlInsiderHandler::create (
        fakeExecutableName,
        fakeExecutableName + " insider OAH handler with options vector");

  // the OAH handler to be used, a regular handler is the default
  // ------------------------------------------------------

  S_oahHandler handler;

  if (insiderOptions) {
    // use the insider msdl2brl OAH handler
    handler = insiderOahHandler;
  }
  else {
    // create a regular msdl2brl OAH handler
    handler =
      msdl2brlRegularHandler::create (
        fakeExecutableName,
        fakeExecutableName + " regular OAH handler with options vector",
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

  msdlStream2brailleWithHandler (
    inputStream,
    handler,
    out,
    err);

  return kNoErr;
}

//_______________________________________________________________________________
EXP xmlErr msdlFile2brailleWithOptionsVector (
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

    throw msgBsrScoreToBrailleException (message);
  }

  return
    msdlStream2brailleWithOptionsVector (
      inputStream, options, out, err);
}

xmlErr msdlFile2brailleWithHandler (
  string        fileName,
  S_oahHandler  handler,
  std::ostream& out,
  std::ostream& err)
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

    throw msgMsdlToBrailleException (message);
  }

  return
    msdlStream2brailleWithHandler (
      inputStream, handler, out, err);
}

//_______________________________________________________________________________
EXP xmlErr msdlString2brailleWithOptionsVector (
  const char *         buffer,
  const optionsVector& options,
  std::ostream&        out,
  std::ostream&        err)
{
  stringstream
    inputStream (
      buffer,
      ios_base::in);

	// call msdlStream2brailleWithHandler() even if buffer is empty,
	// to handle the help options if any
  return
    msdlStream2brailleWithOptionsVector (
      inputStream, options, out, err);
}

xmlErr msdlString2brailleWithHandler (
  const char *  buffer,
  S_oahHandler  handler,
  std::ostream& out,
  std::ostream& err)
{
  stringstream
    inputStream (
      buffer,
      ios_base::in);

	// call msdlStream2brailleWithHandler() even if buffer is empty,
	// to handle the help options if any
  return
    msdlStream2brailleWithHandler (
      inputStream, handler, out, err);
}


}
