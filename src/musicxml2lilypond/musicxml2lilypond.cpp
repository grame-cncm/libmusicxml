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

#include "xml.h"
#include "xmlfile.h"
#include "xmlreader.h"

#include "enableTracingIfDesired.h"
#ifdef TRACING_IS_ENABLED
  #include "traceOah.h"
#endif

#include "msrOah.h"
#include "msr2lpsrOah.h"
#include "lpsrOah.h"

#include "xml2lyInsiderOahHandler.h"
#include "xml2lyRegularOahHandler.h"

#include "musicxml2mxmlTreeInterface.h"
#include "mxmlTree2msrSkeletonBuilderInterface.h"
#include "mxmlTree2msrTranslatorInterface.h"
#include "msr2mxmlTreeInterface.h"
#include "msr2lpsrInterface.h"
#include "lpsr2lilypondInterface.h"

#include "musicxml2lilypond.h"

using namespace std;

namespace MusicXML2
{
/*
  ENFORCE_TRACE_OAH can be used to issue trace messages
  before gGlobalOahOahGroup->fTrace has been initialized
*/
////#define ENFORCE_TRACE_OAH

//_______________________________________________________________________________
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
static void printOptionsVector (const optionsVector& theOptionsVector)
{
	cerr <<
    "The options vector for xmlFile2lilypond contains " <<
    theOptionsVector.size () <<
    " elements: " <<
    endl;

	for (auto option: theOptionsVector) {
	  cerr << "   \"" << option.first << "\" \"" << option.second << "\"" << endl;
	} // for
	cerr << endl;
}
#endif
#endif

//_______________________________________________________________________________
static xmlErr xmlFile2lilypondWithHandler (
  SXMLFile&     xmlfile,
  std::ostream& out,
  std::ostream& err,
  S_oahHandler  handler)
{
  // has quiet mode been requested?
  // ------------------------------------------------------

  if (gGlobalGeneralOahGroup->getQuiet ()) {
    // disable all trace and display options
    handler->
      enforceHandlerQuietness ();
  }

  // get the mxmlTree
  // ------------------------------------------------------

  Sxmlelement
    mxmlTree =
      xmlfile->elements ();

  // the MSR score
  // ------------------------------------------------------

  S_msrScore mScore;

  // create the MSR skeleton from the mxmlTree (pass 2a)
  // ------------------------------------------------------

  try {
    mScore =
      convertMxmlTreeToMsrScoreSkeleton (
        mxmlTree,
        gGlobalMsrOahGroup,
        "Pass 2a");
  }
  catch (mxmlTreeToMsrException& e) {
    return kInvalidFile;
  }
  catch (std::exception& e) {
    return kInvalidFile;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalXml2lyInsiderOahGroup->getQuit2a ()) {
    err <<
      endl <<
      "Quitting after pass 2a as requested" <<
      endl;

    return kNoErr;
  }

  // populate the MSR from MusicXML contents (pass 2b)
  // ------------------------------------------------------

  try {
    populateMsrSkeletonFromMxmlTree (
      mxmlTree,
      mScore,
      "Pass 2b");
  }
  catch (mxmlTreeToMsrException& e) {
    return kInvalidFile;
  }
  catch (std::exception& e) {
    return kInvalidFile;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalXml2lyInsiderOahGroup->getQuit2b ()) {
    err <<
      endl <<
      "Quitting after pass 2b as requested" <<
      endl;

    return kNoErr;
  }

  // the LPSR score
  S_lpsrScore lpScore;

  {
    // create the LPSR from the MSR (pass 3)
    // ------------------------------------------------------

    // start the clock
    clock_t startClock = clock ();
    const string passNumber = "Pass 3";

    try {
      lpScore =
        convertMsrScoreToLpsrScore (
          mScore,
          gGlobalMsrOahGroup,
          gGlobalLpsrOahGroup,
          passNumber);
    }
    catch (msrScoreToLpsrScoreException& e) {
      return kInvalidFile;
    }
    catch (std::exception& e) {
      return kInvalidFile;
    }

    clock_t endClock = clock ();

    // register time spent
    timing::gGlobalTiming.appendTimingItem (
      passNumber,
      "build the LPSR",
      timingItem::kMandatory,
      startClock,
      endClock);

    // display the LPSR score if requested
    // ------------------------------------------------------

    if (gGlobalLpsrOahGroup->getDisplayLpsr ()) {
      displayLpsrScore_OptionalPass (
        lpScore,
        gGlobalMsrOahGroup,
        gGlobalLpsrOahGroup);
    }

    if (gGlobalLpsrOahGroup->getDisplayLpsrShort ()) {
      displayLpsrScoreShort_OptionalPass (
        lpScore,
        gGlobalMsrOahGroup,
        gGlobalLpsrOahGroup);
    }

    // should we return now?
    // ------------------------------------------------------

    if (gGlobalLpsrOahGroup->getQuit3 ()) {
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

    // start the clock
    clock_t startClock = clock ();
    const string passNumber = "Pass 4";

    string
      outputFileName =
        handler->
          fetchOutputFileNameFromTheOptions ();

#ifdef TRACING_IS_ENABLED
    if (gGlobalTraceOahGroup->getTraceOah ()) {
      err <<
        "xmlFile2lilypond() outputFileName = \"" <<
        outputFileName <<
        "\"" <<
        endl;
    }
#endif

    if (! outputFileName.size ()) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceOah ()) {
        err <<
          "xmlFile2lilypond() output goes to standard output" <<
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
          lpScore,
          gGlobalMsrOahGroup,
          gGlobalLpsrOahGroup,
          passNumber,
          lilypondStandardOutputStream);
      }
      catch (lpsrScoreToLilypondException& e) {
        return kInvalidFile;
      }
      catch (std::exception& e) {
        return kInvalidFile;
      }
    }

    else {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceOah ()) {
        err <<
          "xmlFile2lilypond() output goes to file \"" <<
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

        throw lpsrScoreToLilypondException (message);
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
          lpScore,
          gGlobalMsrOahGroup,
          gGlobalLpsrOahGroup,
          passNumber,
          lilypondFileOutputStream);
      }
      catch (lpsrScoreToLilypondException& e) {
        return kInvalidFile;
      }
      catch (std::exception& e) {
        return kInvalidFile;
      }

      // close output file
#ifdef TRACE_OAH
      if (gTraceOah->fTracePasses) {
        gLogOstream <<
          endl <<
          "Closing file \"" << outputFileName << "\"" <<
          endl;
      }
#endif

      outputFileStream.close ();
    }

    // register time spent
    clock_t endClock = clock ();

    timing::gGlobalTiming.appendTimingItem (
      passNumber,
      "generate LilyPond code",
      timingItem::kMandatory,
      startClock,
      endClock);
  }

	return kNoErr;
}

//_______________________________________________________________________________
static xmlErr xmlFile2lilypondWithOptionsVector (
  SXMLFile&            xmlfile,
  const optionsVector& options,
  std::ostream&        out,
  std::ostream&        err)
{
	Sxmlelement st;

	if (xmlfile) {
	  st = xmlfile->elements();

    if (st) {
      if (st->getName() == "score-timewise")
        return kUnsupported;
    }
  }

	else {
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  err <<
    "xmlFile2musicxml(), xmlfile is NULL" <<
    endl;
#endif
#endif

    return kInvalidFile;
	}

  // the fake executable name
  // ------------------------------------------------------

  string fakeExecutableName = "xml2ly";

  // reset the global indenter
  // ------------------------------------------------------

  gIndenter.resetToZero ();

  // create the global log indented output stream
  // ------------------------------------------------------

  createTheGlobalIndentedOstreams (out, err);

	// print the options vector
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
      printOptionsVector (options);
#endif
#endif

  // are there 'insider' and/or 'regular' options present?
  // ------------------------------------------------------

  bool insiderOptions = false;
  bool regularOptions = false;

	for (int i = 1; i < options.size (); i++) {
	  string optionName = options [i].first;

		if (optionName == "-insider") {
		  insiderOptions = true;
		}
		if (optionName == "-regular") {
		  regularOptions = true;
		}
	} // for

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "xmlFile2lilypond()" <<
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

  // create an xml2ly insider OAH handler
  // ------------------------------------------------------

  // create an insider xml2ly OAH handler
  S_xml2lyInsiderOahHandler
    insiderOahHandler =
      xml2lyInsiderOahHandler::create (
        fakeExecutableName,
        fakeExecutableName + " insider OAH handler with options vector");

  // the OAH handler to be used, a regular handler is the default
  // ------------------------------------------------------

  S_oahHandler handler;

  if (insiderOptions) {
    // use the insider xml2ly OAH handler
    handler = insiderOahHandler;
  }
  else {
    // create a regular xml2ly OAH handler
    handler =
      xml2lyRegularOahHandler::create (
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
  catch (msrOahException& e) {
    return kInvalidOption;
  }
  catch (std::exception& e) {
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
      endl << endl;

    gIndenter.resetToZero ();
  }

  // do the job
  // ------------------------------------------------------

  xmlFile2lilypondWithHandler (
    xmlfile,
    out,
    err,
    handler);

  return kNoErr;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlfile2lilypond (
  const char *         fileName,
  const optionsVector& options,
  std::ostream&        out,
  std::ostream&        err)
{
	SXMLFile
	  xmlfile =
	    createXMLFileFromFile (fileName, "Pass 1");

	if (xmlfile) {
		return
		  xmlFile2lilypondWithOptionsVector (
		    xmlfile, options, out, err);
	}

	return kInvalidFile;
}

xmlErr musicxmlFile2lilypondWithHandler (
  const char *         fileName,
  std::ostream&        out,
  std::ostream&        err,
  S_oahHandler         handler)
{
	SXMLFile
	  xmlfile =
	    createXMLFileFromFile (fileName, "Pass 1");

	if (xmlfile) {
		return
		  xmlFile2lilypondWithHandler (
		    xmlfile, out, err, handler);
	}

	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlfd2lilypond (
  FILE *               fd,
  const optionsVector& options,
  std::ostream&        out,
  std::ostream&        err)
{
	SXMLFile
	  xmlfile =
	    createXMLFileFromFd (fd, "Pass 1");

	if (xmlfile) {
		return
		  xmlFile2lilypondWithOptionsVector (
		    xmlfile, options, out, err);
	}

	return kInvalidFile;
}

xmlErr musicxmlFd2lilypondWithHandler (
  FILE *        fd,
  std::ostream& out,
  std::ostream& err,
  S_oahHandler  handler)
{
	SXMLFile
	  xmlfile =
	    createXMLFileFromFd (fd, "Pass 1");

	if (xmlfile) {
		return
		  xmlFile2lilypondWithHandler (
		    xmlfile, out, err, handler);
	}

	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlstring2lilypond (
  const char *         buffer,
  const optionsVector& options,
  std::ostream&        out,
  std::ostream&        err)
{
	SXMLFile
	  xmlfile =
	    createXMLFileFromString (buffer, "Pass 1");

	// call xmlFile2lilypond() even if xmlfile is null,
	// to handle the help options if any
  return
    xmlFile2lilypondWithOptionsVector (
      xmlfile, options, out, err);

	return kInvalidFile;
}

xmlErr musicxmlString2lilypondWithHandler (
  const char *  buffer,
  std::ostream& out,
  std::ostream& err,
  S_oahHandler  handler)
{
	SXMLFile
	  xmlfile =
	    createXMLFileFromString (buffer, "Pass 1");

	// call xmlFile2lilypond() even if xmlfile is null,
	// to handle the help options if any
  return
    xmlFile2lilypondWithHandler (
      xmlfile, out, err, handler);

	return kInvalidFile;
}


}
