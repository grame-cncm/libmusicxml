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

#include "oahOah.h"

#include "msrOah.h"
#include "msr2bsrOah.h"
#include "bsrOah.h"

#include "xml2brlInsiderHandler.h"
#include "xml2brlRegularHandler.h"

#include "musicxml2mxmlTreeInterface.h"
#include "mxmlTree2msrSkeletonBuilderInterface.h"
#include "mxmlTree2msrTranslatorInterface.h"
#include "msr2mxmlTreeInterface.h"
#include "msr2bsrInterface.h"
#include "bsr2bsrFinalizerInterface.h"
#include "bsr2brailleTranslatorInterface.h"

#include "musicxml2braille.h"


using namespace std;

namespace MusicXML2
{
/*
  ENFORCE_TRACE_OAH can be used to issue trace messages
  before gGlobalOahOahGroup->fTrace has been initialized
*/

//#define ENFORCE_TRACE_OAH

//_______________________________________________________________________________
static xmlErr xmlFile2brailleWithHandler (
  SXMLFile&            xmlfile,
  std::ostream&        out,
  std::ostream&        err,
  S_oahHandler         handler)
{
  if (gGlobalMxmlTreeOahGroup->getTraceMusicXMLTree ()) {
    gLogStream <<
      endl <<
      "<!-- ----------------------------------------------------------- -->" <<
      endl <<
      "xmlFile2brailleWithHandler(), xmlfile contains:" <<
      endl << endl;

    ++gIndenter;

    xmlfile->print (gLogStream);
    gLogStream << endl << endl;

    --gIndenter;

    gLogStream <<
      "<!-- ----------------------------------------------------------- -->" <<
      endl << endl;
  }

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

  S_msrScore theMsrScore;

  // create the MSR skeleton from the mxmlTree (pass 2a)
  // ------------------------------------------------------

  try {
    theMsrScore =
      convertMxmlTreeToMsrScoreSkeleton (
        mxmlTree,
        gGlobalMsrOahGroup,
        "Pass 2a",
        "Create an MSR skeleton from the mxmlTree");
  }
  catch (mxmlTreeToMsrException& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }
  catch (std::exception& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalXml2brlInsiderOahGroup->getQuitAfterPass2a ()) {
    err <<
      endl <<
      "Quitting after pass 2a as requested" <<
      endl;

    return kNoErr;
  }

  // populate the MSR skeleton from MusicXML data (pass 2b)
  // ------------------------------------------------------

  try {
    populateMsrSkeletonFromMxmlTree (
      mxmlTree,
      theMsrScore,
        "Pass 2b",
        "Populate the MSR skeleton from MusicXML data");
  }
  catch (mxmlTreeToMsrException& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }
  catch (std::exception& e) {
    displayException (e, gOutputStream);
    return kInvalidFile;
  }

  // should we return now?
  // ------------------------------------------------------

  if (gGlobalXml2brlInsiderOahGroup->getQuitAfterPass2b ()) {
    err <<
      endl <<
      "Quitting after pass 2b as requested" <<
      endl;

    return kNoErr;
  }

  // the first BSR score
  S_bsrScore firstBsrScore;

  {
    // create the first BSR from the MSR (pass 3a)
    // ------------------------------------------------------

    try {
      firstBsrScore =
        convertMsrScoreToBsrScore (
          theMsrScore,
          gGlobalMsrOahGroup,
          gGlobalBsrOahGroup,
          "Pass 3a",
          "Create a first BSR from the MSR");
    }
    catch (msrScoreToBsrScoreException& e) {
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
    // create the finalized BSR from the first BSR (pass 3b)
    // ------------------------------------------------------

    try {
      finalizedBsrScore =
        convertBsrFirstScoreToFinalizedBsrScore (
          firstBsrScore,
          gGlobalBsrOahGroup,
          "Pass 3b",
          "Create the finalized BSR from the first BSR");
    }
    catch (bsrScoreToFinalizedBsrScoreException& e) {
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
    // generate Braille music text from the BSR (pass 4)
    // ------------------------------------------------------

    string
      outputFileName =
        handler->
          fetchOutputFileNameFromTheOptions ();

#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceOah ()) {
        err <<
          "xmlFile2braille() outputFileName = \"" <<
          outputFileName <<
          "\"" <<
          endl;
      }
#endif

    if (! outputFileName.size ()) {
#ifdef TRACING_IS_ENABLED
      if (gGlobalTraceOahGroup->getTraceOah ()) {
        err <<
          "xmlFile2braille() output goes to standard output" <<
          endl;
      }
#endif

      // convert the BSR to braille text
      try {
        convertBsrScoreToBrailleText (
          finalizedBsrScore,
          gGlobalBsrOahGroup,
          "Pass 4",
          "Convert the finalized BSR into braille text",
          out);
      }
      catch (lpsrScoreToLilypondException& e) {
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
      if (gGlobalTraceOahGroup->getTraceOah ()) {
        err <<
          "xmlFile2braille() output goes to file \"" <<
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
          outputFileName.c_str (),
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

        throw bsrScoreToBrailleTextException (message);
      }

      // convert the finalized BSR to braille text
      try {
        convertBsrScoreToBrailleText (
          finalizedBsrScore,
          gGlobalBsrOahGroup,
          "Pass 4",
          "Convert the finalized BSR into braille text",
          brailleCodeFileOutputStream);
      }
      catch (lpsrScoreToLilypondException& e) {
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
static xmlErr xmlFile2brailleWithOptionsVector (
  SXMLFile&            xmlfile,
  const optionsVector& options,
  std::ostream&        out,
  std::ostream&        err)
{
	Sxmlelement mxmlTree;

	if (xmlfile) {
	  mxmlTree = xmlfile->elements ();

    if (mxmlTree) {
      if (mxmlTree->getName () == "score-timewise")
        return kUnsupported;
    }
  }

	else {
#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  err <<
    "xmlFile2braille(), xmlfile is NULL" <<
    endl;
#endif
#endif

    return kInvalidFile;
	}

  // the fake executable name
  // ------------------------------------------------------

  string fakeExecutableName = "xml2brl";

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

    string optionNameWithoutDash = optionName.substr (1);

		if (optionNameWithoutDash == K_INSIDER_OPTION_NAME) {
		  insiderOptions = true;
		}
		if (optionNameWithoutDash == K_REGULAR_OPTION_NAME) {
		  regularOptions = true;
		}
	} // for

#ifdef TRACING_IS_ENABLED
#ifdef ENFORCE_TRACE_OAH
  gLogStream <<
    "xmlFile2braille()" <<
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

  // create an xml2brl insider OAH handler
  // ------------------------------------------------------

  // create an insider xml2brl OAH handler
  S_xml2brlInsiderHandler
    insiderOahHandler =
      xml2brlInsiderHandler::create (
        fakeExecutableName,
        fakeExecutableName + " insider OAH handler with options vector");

  // the OAH handler to be used, a regular handler is the default
  // ------------------------------------------------------

  S_oahHandler handler;

  if (insiderOptions) {
    // use the insider xml2brl OAH handler
    handler = insiderOahHandler;
  }
  else {
    // create a regular xml2brl OAH handler
    handler =
      xml2brlRegularHandler::create (
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
  catch (msrOahException& e) {
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

  xmlFile2brailleWithHandler (
    xmlfile,
    out,
    err,
    handler);

  return kNoErr;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlfile2braille (
  const char *         fileName,
  const optionsVector& options,
  std::ostream&        out,
  std::ostream&        err)
{
  //
	SXMLFile
	  xmlfile =
	    createXMLFileFromFile (
	      fileName,
	      "Pass 1",
	      "Create an mxmlTree reading a MusicXML file");

	if (xmlfile) {
		return
		  xmlFile2brailleWithOptionsVector (
		    xmlfile, options, out, err);
	}

	return kInvalidFile;
}

xmlErr musicxmlFile2brailleWithHandler (
  const char * fileName,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler)
{
	SXMLFile
	  xmlfile =
	    createXMLFileFromFile (
	      fileName,
	      "Pass 1",
	      "Create an mxmlTree reading a MusicXML file");

	if (xmlfile) {
		return
		  xmlFile2brailleWithHandler (
		    xmlfile, out, err, handler);
	}

	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlfd2braille (
  FILE *               fd,
  const optionsVector& options,
  std::ostream&        out,
  std::ostream&        err)
{
	SXMLFile
	  xmlfile =
	    createXMLFileFromFd (
	      fd,
	      "Pass 1",
	      "Create an mxmlTree reading a MusicXML descriptor");

	if (xmlfile) {
		return
		  xmlFile2brailleWithOptionsVector (
		    xmlfile, options, out, err);
	}

	return kInvalidFile;
}

xmlErr musicxmlFd2brailleWithHandler (
  FILE *       fd,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler)
{
	SXMLFile
	  xmlfile =
	    createXMLFileFromFd (
	      fd,
	      "Pass 1",
	      "Create an mxmlTree reading a MusicXML descriptor");

	if (xmlfile) {
		return
		  xmlFile2brailleWithHandler (
		    xmlfile, out, err, handler);
	}

	return kInvalidFile;
}

//_______________________________________________________________________________
EXP xmlErr musicxmlstring2braille (
  const char *         buffer,
  const optionsVector& options,
  std::ostream&        out,
  std::ostream&        err)
{
	SXMLFile
	  xmlfile =
	    createXMLFileFromString (
	      buffer,
	      "Pass 1",
	      "Create an mxmlTree reading a MusicXML buffer");

	// call xmlFile2braille() even if xmlfile is null,
	// to handle the help options if any
  return
    xmlFile2brailleWithOptionsVector (
      xmlfile, options, out, err);

	return kInvalidFile;
}

xmlErr musicxmlString2brailleWithHandler (
  const char * buffer,
  ostream&     out,
  ostream&     err,
  S_oahHandler handler)
{
	SXMLFile
	  xmlfile =
	    createXMLFileFromString (
	      buffer,
	      "Pass 1",
	      "Create an mxmlTree reading a MusicXML buffer");

	// call xmlFile2braille() even if xmlfile is null,
	// to handle the help options if any
  return
    xmlFile2brailleWithHandler (
      xmlfile, out, err, handler);

	return kInvalidFile;
}


}
