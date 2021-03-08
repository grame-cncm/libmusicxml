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
#include "msr2lpsrOah.h"
#include "lpsrOah.h"

#include "waeExceptions.h"
#include "waeMessagesHandling.h"

#include "msdlParser.h"

#include "msdl2lyInsiderHandler.h"
#include "msdl2lyRegularHandler.h"

#include "msr2mxmlTreeInterface.h"
#include "msr2lpsrInterface.h"
#include "lpsr2lilypondInterface.h"

#include "msdl2lilypond.h"


using namespace std;

namespace MusicXML2
{

//_______________________________________________________________________________
xmlErr msdlStream2lilypondWithHandler (
  string        inputSourceName,
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
    msdlParser
      parser (
        inputSourceName,
        inputStream);

    parser.parse ();
  }
  catch (msgMxmlTreeToMsrException& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }
  catch (std::exception& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }

return kNoErr; // JMI TEMP

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalMsdl2lyInsiderOahGroup->getQuitAfterPass2a ()) {
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

  if (gGlobalMsdl2lyInsiderOahGroup->getQuitAfterPass2b ()) {
    err <<
      endl <<
      "Quitting after pass 2b as requested" <<
      endl;

    return kNoErr;
  }

  // the LPSR score
  S_lpsrScore theLpsrScore;

  {
    // create the LPSR from the MSR (pass 3)
    // ------------------------------------------------------

    try {
      theLpsrScore =
        convertMsrScoreToLpsrScore (
          theMsrScore,
          gGlobalMsrOahGroup,
          gGlobalLpsrOahGroup,
          "Pass 3",
          "Convert the MSR into an LPSR");
    }
    catch (msgScoreToLpsrScoreException& e) {
      displayException (e, gOutputStream);
      return kInvalidFile;
    }
    catch (std::exception& e) {
      displayException (e, gOutputStream);
      return kInvalidFile;
    }

    // display the LPSR score if requested
    // ------------------------------------------------------

    if (gGlobalLpsrOahGroup->getDisplayLpsr ()) {
      displayLpsrScore_OptionalPass (
        theLpsrScore,
        gGlobalMsrOahGroup,
        gGlobalLpsrOahGroup);
    }

    if (gGlobalLpsrOahGroup->getDisplayLpsrShort ()) {
      displayLpsrScoreShort_OptionalPass (
        theLpsrScore,
        gGlobalMsrOahGroup,
        gGlobalLpsrOahGroup);
    }

    // should we return now?
    // ------------------------------------------------------

    if (gGlobalLpsrOahGroup->getQuitAfterPass3 ()) {
      err <<
        endl <<
        "Quitting after pass 3 as requested" <<
        endl;

      return kNoErr;
    }
  }

  {
    // generate LilyPond code from the LPSR (pass 4)
    // ------------------------------------------------------

    string
      outputFileName =
        handler->
          fetchOutputFileNameFromTheOptions ();

#ifdef TRACING_IS_ENABLED
    if (getTraceOah ()) {
      err <<
        "msdlStream2lilypond() outputFileName = \"" <<
        outputFileName <<
        "\"" <<
        endl;
    }
#endif

    if (! outputFileName.size ()) {
#ifdef TRACING_IS_ENABLED
      if (getTraceOah ()) {
        err <<
          "msdlStream2lilypond() output goes to standard output" <<
          endl;
      }
#endif

      // create an indented output stream for the LilyPond code
      // to be written to outputFileStream
      indentedOstream
        lilypondStandardOutputStream (
          out,
          gIndenter);

      // convert the LPSR score to LilyPond code
      try {
        convertLpsrScoreToLilypondCode (
          theLpsrScore,
          gGlobalMsrOahGroup,
          gGlobalLpsrOahGroup,
          "Pass 4",
          "Convert the LPSR score to LilyPond code",
          lilypondStandardOutputStream);
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
          "msdlStream2lilypond() output goes to file \"" <<
          outputFileName <<
          "\"" <<
          endl;
      }
#endif

      // open output file
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTracePasses ()) {
        err <<
          "Opening file '" << outputFileName << "' for writing" <<
          endl;
      }
#endif

      ofstream
        outputFileStream (
          outputFileName.c_str (),
          ofstream::out);

      if (! outputFileStream.is_open ()) {
        stringstream s;

        s <<
          "Could not open LilyPond output file \"" <<
          outputFileName <<
          "\" for writing, quitting";

        string message = s.str ();

        err <<
          message <<
          endl;

        throw msgLpsrScoreToLilypondException (message);
      }

      // create an indented output stream for the LilyPond code
      // to be written to outputFileStream
      indentedOstream
        lilypondFileOutputStream (
          outputFileStream,
          gIndenter);

      // convert the LPSR score to LilyPond code
      try {
        convertLpsrScoreToLilypondCode (
          theLpsrScore,
          gGlobalMsrOahGroup,
          gGlobalLpsrOahGroup,
          "Pass 4",
          "Convert the LPSR score to LilyPond code",
          lilypondFileOutputStream);
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

      outputFileStream.close ();
    }
  }

	return kNoErr;
}

//_______________________________________________________________________________
 xmlErr msdlStream2lilypondWithOptionsVector (
  string               inputSourceName,
  istream&             inputStream,
  const optionsVector& options,
  std::ostream&        out,
  std::ostream&        err)
{
  // the fake executable name
  // ------------------------------------------------------

  string fakeExecutableName = "msdl2ly";

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
      "msdlStream2lilypond()" <<
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

  // create an msdl2ly insider OAH handler
  // ------------------------------------------------------

  // create an insider msdl2ly OAH handler
  S_msdl2lyInsiderHandler
    insiderOahHandler =
      msdl2lyInsiderHandler::create (
        fakeExecutableName,
        fakeExecutableName + " insider OAH handler with options vector",
        kHandlerUsedThruOptionsVector);

  // the OAH handler to be used, a regular handler is the default
  // ------------------------------------------------------

  S_oahHandler handler;

  if (insiderOptions) {
    // use the insider msdl2ly OAH handler
    handler = insiderOahHandler;
  }
  else {
    // create a regular msdl2ly OAH handler
    handler =
      msdl2lyRegularHandler::create (
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

  msdlStream2lilypondWithHandler (
    inputSourceName,
    inputStream,
    handler,
    out,
    err);

  return kNoErr;
}

//_______________________________________________________________________________
EXP xmlErr msdlfile2lilypondWithOptionsVector (
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

    throw msgMsdlToLilyponException (message);
  }

  return
    msdlStream2lilypondWithOptionsVector (
      fileName, inputStream, options, out, err);
}

xmlErr msdlFile2lilypondWithHandler (
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

    throw msgMsdlToLilyponException (message);
  }

  return
    msdlStream2lilypondWithHandler (
      fileName, inputStream, handler, out, err);
}

//_______________________________________________________________________________
EXP xmlErr msdlstring2lilypondWithOptionsVector (
  const char *         buffer,
  const optionsVector& options,
  std::ostream&        out,
  std::ostream&        err)
{
  stringstream
    inputStream (
      buffer,
      ios_base::in);

	// call msdlStream2lilypondWithOptionsVector() even if buffer is empty,
	// to handle the help options if any
  return
    msdlStream2lilypondWithOptionsVector (
      "buffer", inputStream, options, out, err);

	return kInvalidFile;
}

xmlErr msdlString2lilypondWithHandler (
  const char *  buffer,
  S_oahHandler  handler,
  std::ostream& out,
  std::ostream& err)
{
  stringstream
    inputStream (
      buffer,
      ios_base::in);

	// call msdlStream2lilypondWithOptionsVector() even if buffer is empty,
	// to handle the help options if any
  return
    msdlStream2lilypondWithHandler (
      "buffer", inputStream, handler, out, err);

	return kInvalidFile;
}


}
